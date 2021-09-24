#ifndef _CHAT_CLIENT_H_
#define _CHAT_CLIENT_H_

#include <functional>
#include <iostream>
#include <mutex>
#include <thread>

#include "chatNode.h"

namespace chatSocket {

    class chatClient : protected chatNode {
    protected:
        void setDefault();

        // message event function
        std::function<void(const std::string& t_message)> m_onMessage = nullptr;
        void showMessage(const std::string& t_message);

        // server
        std::string m_serverName = "localhost";

        // thread
        std::thread m_thread;
        std::mutex m_mu;

        void _connectServer();
        void receiveMessage();
        void writeMessage();

    public:
        chatClient();
        chatClient(const int& t_port);
        chatClient(const std::string& t_serverName, const int& t_port);

        void setOnMessage(const std::function<void(const std::string&)>& t_function);

        void connectServer();
        void closeConnection();

        virtual ~chatClient();
    };

}

#endif /* _CHAT_CLIENT_H_ */