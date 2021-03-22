#ifndef HIGHLOAD_SERVER_CONNECTSTRUCTS_H
#define HIGHLOAD_SERVER_CONNECTSTRUCTS_H

#include "event.h"
#include "SafeQueue.h"
#include "unistd.h"

struct Connection {
    int m_fd = -1;
    explicit Connection(int fd) : m_fd(fd) {}

    ~Connection() {
        if (m_fd != -1) {
            ::close(m_fd);
        }
    }
};

struct ConnectionData {
    SafeQueue<std::unique_ptr<Connection>>& m_q;
    struct event* ev;
    int m_fd;

    ConnectionData(int fd, SafeQueue<std::unique_ptr<Connection>>& q): m_fd(fd), m_q(q), ev(nullptr) {}
    virtual ~ConnectionData() {
        if (ev != nullptr) {
            event_free(ev);
        }
    }
};

struct InAcceptConnection: public ConnectionData {
    struct event_base* base;

    InAcceptConnection(int fd, SafeQueue<std::unique_ptr<Connection>>& q): ConnectionData(fd, q), base(nullptr) {}
    ~InAcceptConnection() {
        if (base != nullptr) {
            event_base_free(base);
        }
    }
};

#endif //HIGHLOAD_SERVER_CONNECTSTRUCTS_H
