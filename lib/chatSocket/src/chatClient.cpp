#include "chatClient.h"

namespace chatSocket {

    void chatClient::setDefault() {
        inet_pton(AF_INET, this->m_serverName.c_str(), &this->m_socketInfo.sin_addr);
    }

    void chatClient::showMessage(const std::string& t_message) {
        if (this->m_onMessage != nullptr) {
            this->m_onMessage(t_message);
        }
    }

    void chatClient::_connectServer() {
        this->createSocket();

        if (connect(this->m_socketFd, reinterpret_cast<sockaddr*>(&this->m_socketInfo), this->m_addressLen) == -1) {
            throw std::runtime_error("could not connect to server");
        }
    }

    void chatClient::receiveMessage() {
        this->m_thread = std::move(std::thread([=] {
            int len;
            char message_buffer[this->getMessageSize()];

            while ((len = recv(this->m_socketFd, message_buffer, this->getMessageSize(), 0)) > 0) {
                message_buffer[len] = '\0';
                showMessage(message_buffer);
                std::memset(message_buffer, '\0', sizeof(message_buffer));
            }
            this->m_state = STATE::CLOSED;
        }));
    }

    void chatClient::writeMessage() {
        std::string message = "";
        while (this->m_state == STATE::OPEN) {
            std::getline(std::cin, message);
            this->sendMessage(this->m_socketFd, message);
        }
    }

    chatClient::chatClient() : chatNode() {
        setDefault();
    }

    chatClient::chatClient(const int& t_port) : chatNode(t_port) {
        setDefault();
    }

    chatClient::chatClient(const std::string& t_serverName, const int& t_port) : chatNode(t_port), m_serverName(t_serverName) {
        setDefault();
    }

    void chatClient::setOnMessage(const std::function<void(const std::string&)>& t_function) {
        m_onMessage = t_function;
    }

    void chatClient::connectServer() {
        this->_connectServer();
        showMessage("Connection successfully....");
        this->receiveMessage();
        this->writeMessage();
    }

    void chatClient::closeConnection() {
        this->closeSocket();
        if (this->m_thread.joinable()) {
            this->m_thread.join();
        }
    }

    chatClient::~chatClient() {
        this->closeConnection();
    }

}