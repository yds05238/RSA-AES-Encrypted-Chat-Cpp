#ifndef _CHAT_SERVER_H_
#define _CHAT_SERVER_H_

#include <algorithm>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

#include "chatNode.h"

namespace chatSocket {

    class chatServer : protected chatNode {
    protected:
        void setDefault();

        // message event function
        std::function<void(const std::string& t_message)> m_onMessage = nullptr;
        void showMessage(const std::string& t_message);

        // thread
        std::vector<std::thread> v_thread;
        void joinThread();
        std::mutex m_mu;

        // client
        int m_nodeSize = 0;
        std::vector<chatNode*> v_client;
        void closeClient();

        // server
        int m_connectionSize = 128;
        void createServer();
        void bindServer();
        void listenServer();
        void handleRequest();

        // Message
        void sendMessageClients(const std::string& t_message);
        void sendMessageAll(const std::string& t_message, const int& t_socket);

    public:
        chatServer();
        chatServer(const int& t_port);
        chatServer(const int& t_port, const int& t_connectionSize);

        void setOnMessage(const std::function<void(const std::string&)>& t_function);

        void runServer();
        void closeServer();

        virtual ~chatServer();
    };

}

#endif /* _CHAT_SERVER_H_ */