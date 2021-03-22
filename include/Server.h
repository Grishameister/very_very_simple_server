#ifndef HIGHLOAD_SERVER_SERVER_H
#define HIGHLOAD_SERVER_SERVER_H

#include <cstdio>
#include <string>
#include <unistd.h>
#include <thread>

#include "SafeQueue.h"
#include "ServerSocket.h"
#include "ConnectStructs.h"


class Server {
public:
    Server(std::size_t max_threads, const std::string& host, uint32_t port, const std::string& root) :
            m_sock_(host, port), m_q(), m_max_threads(max_threads), m_root(root), workers() {
    }

    SafeQueue<std::unique_ptr<Connection>>& GetQueueRef() {
        return m_q;
    }

    void run();
private:
    void RunThreads();
    void StopThreads();
    std::size_t m_max_threads;
    std::string m_root;
    ServerSocket m_sock_;
    SafeQueue<std::unique_ptr<Connection>> m_q;
    std::vector<std::thread> workers;
};

#endif //HIGHLOAD_SERVER_SERVER_H
