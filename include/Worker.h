#ifndef HIGHLOAD_SERVER_WORKER_H
#define HIGHLOAD_SERVER_WORKER_H

#include <atomic>
#include "ConnectStructs.h"
#include "SafeQueue.h"

class Worker {
public:
    Worker(SafeQueue<std::unique_ptr<Connection>>& q, const std::string& root) : m_q(q), m_root(root) {}

    void work();

private:
    SafeQueue<std::unique_ptr<Connection>>& m_q;
    static std::atomic<int> counter;
    std::string m_root;
};

#endif //HIGHLOAD_SERVER_WORKER_H
