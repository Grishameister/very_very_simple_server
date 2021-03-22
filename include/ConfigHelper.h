#ifndef HIGHLOAD_SERVER_CONFIGHELPER_H
#define HIGHLOAD_SERVER_CONFIGHELPER_H

#include <fstream>
#include <map>
#include <filesystem>

static constexpr char HOST[] = "host";
static constexpr char PORT[] = "port";
static constexpr char THREAD_LIMIT[] = "thread_limit";
static constexpr char ROOT[] = "document_root";

class ConfigHelper {
public:
    ConfigHelper(const std::string& path): pairs() {
        std::ifstream file(path);

        if (!file.is_open()) {
            throw std::runtime_error("invalid path: " + path);
        }

        for (std::string line; std::getline(file, line);) {
            auto pos = line.find(' ');
            if (pos == std::string::npos) {
                throw std::runtime_error("invalid config format, need : <key> <value>");
            }
            pairs.emplace(line.substr(0, pos), line.substr(pos + 1, line.find(' ', pos + 1)));
        }

        auto it = pairs.find(HOST);
        if (it == pairs.end()) {
            throw std::runtime_error("host undefined");
        }
        host = it->second;

        it = pairs.find(PORT);
        if (it == pairs.end()) {
            throw std::runtime_error("port undefined");
        }
        // поймаем эксепшн наверху в мейне
        port = std::stoul(it->second);

        it = pairs.find(THREAD_LIMIT);
        if (it == pairs.end()) {
            throw std::runtime_error("thread_limit undefined");
        }
        threads_limit = std::stoul(it->second);

        it = pairs.find(ROOT);
        if (it == pairs.end()) {
            throw std::runtime_error("root undefined");
        }
        std::filesystem::path p = it->second;

        if (!std::filesystem::exists(it->second)) {
            // тут не знаю, пока кидаю exception;
            throw std::runtime_error("root doesn't exists");
        }
        if (!std::filesystem::is_directory(it->second)) {
            throw std::runtime_error("not a directory");
        }
        root = it->second;
    }

    std::string GetHost() const {
        return host;
    }

    std::string GetRoot() const {
        return root;
    };

    uint32_t GetLimit() const {
        return threads_limit;
    }

    uint32_t GetPort() const {
        return port;
    }

private:
    std::map<std::string, std::string> pairs;

    std::string host;
    uint32_t threads_limit;
    uint32_t port;
    std::string root;
};

#endif //HIGHLOAD_SERVER_CONFIGHELPER_H
