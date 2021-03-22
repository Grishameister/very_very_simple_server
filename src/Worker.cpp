#include "Worker.h"
#include <unistd.h>
#include <iostream>
#include "HttpRequest.h"
#include "FileReader.h"

std::atomic<int> Worker::counter{0};

namespace {
    void SetContentType(std::string& response, std::string_view url) {
        url.remove_prefix(std::min(url.rfind('.'), url.size()));
        response.append("Content-Type: ");

        if (url == ".html" || url == ".htm") {
            response.append("text/html\r\n");
        } else if (url == ".gif") {
            response.append("image/gif\r\n");
        } else if (url == ".css") {
            response.append("text/css\r\n");
        } else if (url == ".png") {
            response.append("image/png\r\n");
        } else if (url == ".jpeg" || url == ".jpg" || url == ".jpe") {
            response.append("image/jpeg\r\n");
        } else if (url == ".tiff" || url == ".tff") {
            response.append("image/tiff\r\n");
        } else if (url == ".bmp" || url == ".tff") {
            response.append("image/bmp\r\n");
        } else if (url == ".js") {
            response.append("application/javascript\r\n");
        } else if (url == ".swf") {
            response.append("application/x-shockwave-flash\r\n");
        } else {
            response.append("text/plain\r\n");
        }
    }

}


void Worker::work() {
    bool can_exit = false;
    while (!can_exit) {
        std::unique_ptr<Connection> c;
        m_q.WaitPop(c);
        if (c == nullptr) {
            can_exit = true;
            continue;
        }

        char buf[1024];
        ssize_t len = read(c->m_fd, buf, sizeof(buf) - 1);
        if (len == -1 || len == 0) {
            std::cout << errno << " " << counter++ << std::endl;
            continue;
        }

        HttpRequest req(std::string_view (buf, len));

        if ((req.GetMethod() != "GET" && req.GetMethod() != "HEAD") || !req.CheckDoubleDotsSlash()) {
            std::string response = "HTTP/1.1 400 Bad Request\r\nServer: highload_server\r\n\r\n";
            write(c->m_fd, response.data(), response.size());
            continue;
        }

        FileReader reader(m_root + req.GetUrl());

        if (!reader.IsOpen()) {
            std::string response = "HTTP/1.1 404 Not Found\r\nServer: highload_server\r\n\r\n";
            write(c->m_fd, response.data(), response.size());
            continue;
        }


        std::string response = "HTTP/1.1 200 OK\r\nServer: highload_server\r\n";
        if (!reader.IsMap()) {
            FileReader idx_reader(m_root + req.GetUrl() + "/index.html");
            if (!idx_reader.IsInit()) {
                response = "HTTP/1.1 403 Forbidden\r\nServer: highload_server\r\n\r\n";
                write(c->m_fd, response.data(), response.size());
                continue;
            }

            SetContentType(response, req.GetUrl());
            response.append("Content-Length: ").append(std::to_string(idx_reader.GetSize())).append("\r\n");
            response.append("\r\n");

            write(c->m_fd, response.data(), response.size());
            if (req.GetMethod() == "HEAD") {
                continue;
            }

            int total = 0;
            int left = idx_reader.GetSize();
            len = 0;

            while (total < idx_reader.GetSize()) {
                idx_reader.ShiftPtr(len);
                len = write(c->m_fd, idx_reader.GetPtr(), left);
                total += len;
                left -= len;
            }
            continue;
        }

        response.append("Content-Length: ").append(std::to_string(reader.GetSize())).append("\r\n");
        SetContentType(response, req.GetUrl());
        response.append("\r\n");

        write(c->m_fd, response.data(), response.size());
        if (req.GetMethod() == "HEAD") {
            continue;
        }

        int total = 0;
        int left = reader.GetSize();
        len = 0;

        while (total < reader.GetSize()) {
            reader.ShiftPtr(len);
            len = write(c->m_fd, reader.GetPtr(), left);
            total += len;
            left -= len;
        }
    }
}

