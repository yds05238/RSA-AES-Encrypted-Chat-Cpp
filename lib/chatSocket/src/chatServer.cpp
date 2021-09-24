#include "chatServer.h"

namespace chatSocket {

    void chatServer::setDefault() {
        this->m_socketInfo.sin_addr.s_addr = htonl(INADDR_ANY);
    }

    void chatServer::showMessage(const std::string& t_message) {
        if (this->m_onMessage != nullptr) {
            std::lock_guard<std::mutex> locker(this->m_mu);
            this->m_onMessage(t_message);
        }
    }

    chatServer::chatServer() : chatNode() {
        setDefault();
    }

    chatServer::chatServer(const int& t_port) : chatNode(t_port) {
        setDefault();
    }

    chatServer::chatServer(const int& t_port, const int& t_connectionSize) : chatNode(t_port), m_connectionSize(t_connectionSize) {
        setDefault();
    }

    void chatServer::setOnMessage(const std::function<void(const std::string&)>& t_function) {
        m_onMessage = t_function;
    }

    void chatServer::runServer() {
        this->createServer();
        this->bindServer();
        this->listenServer();
        this->handleRequest();
    }

    void chatServer::createServer() {
        const int opt = 1;
        this->createSocket();
        if (setsockopt(this->m_socketFd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt))) {
            throw std::runtime_error("setsockopt");
        }
    }

    void chatServer::bindServer() {
        if (bind(this->m_socketFd, reinterpret_cast<sockaddr*>(&this->m_socketInfo), this->m_addressLen) == -1) {
            throw std::runtime_error("Could not bind socket");
        }
    }

    void chatServer::listenServer() {
        if (listen(this->m_socketFd, this->m_connectionSize) == -1) {
            throw std::runtime_error("Could not open socket for listening");
        }
    }

    void chatServer::handleRequest() {
        showMessage("chatServer is running...");
        showMessage(std::string(inet_ntoa(this->m_socketInfo.sin_addr)) + ":" + std::to_string(ntohs(this->m_socketInfo.sin_port)));

        while (1) {
            struct sockaddr_in temp_addr;
            socklen_t temp_len = sizeof(temp_addr);
            int temp_socket = accept(this->m_socketFd, reinterpret_cast<sockaddr*>(&temp_addr), &temp_len);

            // check connection limit
            if (this->m_nodeSize + 1 > this->m_connectionSize) {
                sendMessage(temp_socket, "Server is full.");
                close(temp_socket);
                continue;
            }
            this->m_nodeSize += 1;

            // create client
            chatNode* new_client = new chatNode();
            new_client->setSocketInfo(temp_addr);
            new_client->setSocketFd(temp_socket);
            new_client->setState(STATE::OPEN);

            std::string client_ip(inet_ntoa(temp_addr.sin_addr));
            std::string client_port(std::to_string(ntohs(temp_addr.sin_port)));

            // create thread
            this->v_thread.push_back(std::thread([=] {
                int len;
                char message_buffer[this->getMessageSize()];
                while ((len = recv(new_client->getSocketFd(), message_buffer, this->getMessageSize(), 0)) > 0) {
                    message_buffer[len] = '\0';
                    std::string message(client_ip + ":" + client_port + "> " + message_buffer);
                    this->sendMessageAll(message, new_client->getSocketFd());
                    std::memset(message_buffer, '\0', sizeof(message_buffer));
                }
                // connection closed.
                this->v_client.erase(std::remove(this->v_client.begin(), this->v_client.end(), new_client));
                this->showMessage(client_ip + ":" + client_port + " disconnected.");
            }));

            this->showMessage(client_ip + ":" + client_port + " connected.");
            this->v_client.push_back(new_client);
        }
    }

    void chatServer::sendMessageClients(const std::string& t_message) {
        for (auto& client : v_client) {
            sendMessage(client->getSocketFd(), t_message);
        }
    }

    void chatServer::sendMessageAll(const std::string& t_message, const int& t_socket) {
        for (auto& client : v_client) {
            if (client->getSocketFd() != t_socket) {
                sendMessage(client->getSocketFd(), t_message);
            }
        }
    }

    void chatServer::closeClient() {
        for (auto& client : v_client) {
            delete client;
        }
    }

    void chatServer::joinThread() {
        for (auto& thread : v_thread) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }

    void chatServer::closeServer() {
        sendMessageClients("Server closed.");
        this->closeClient();
        this->closeSocket();
        this->joinThread();
        this->v_client.clear();
        this->v_thread.clear();
    }

    chatServer::~chatServer() {
        this->closeServer();
    }

}