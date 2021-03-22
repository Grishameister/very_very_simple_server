#ifndef HIGHLOAD_SERVER_HTTPREQUEST_H
#define HIGHLOAD_SERVER_HTTPREQUEST_H

#include <string_view>

class HttpRequest {
public:
    explicit HttpRequest(std::string_view data) {
        PrepareRequest(data);
    }

    std::string GetMethod() const {
        return m_method;
    }

    std::string GetUrl() const {
        return m_url;
    }

    bool CheckDoubleDotsSlash() {
        return m_url.find("../") == std::string::npos;
    }

private:
    std::string PrepareUrl(std::string_view url) {
        auto pos = url.rfind('?');
        if (pos != std::string_view::npos) {
            url.remove_suffix(url.size() - pos);
        }

        return DecodeUrl(url);
    }

    std::string DecodeUrl(std::string_view url) {
        std::string result;
        result.reserve(url.size());
        uint32_t ii = url.size(); // знаю, что тут не int возвращается

        for (size_t i = 0; i < url.size(); i++) {
            if (url[i] != '%') {
                if (url[i] == '+') {
                    result += ' ';
                } else {
                    result += url[i];
                }

            } else {
                sscanf(url.substr(i + 1, 2).data(), "%x", &ii);
                result += static_cast<char>(ii);;
                i += 2;
            }
        }
        return result;
    }

    void PrepareRequest(std::string_view data) {
        int i = 0;
        size_t first = 0;
        size_t pos = data.find(' ', first);
        while (i < 2 && pos != std::string_view::npos) {
            if (i == 0) {
                m_method = data.substr(first, pos - first);
            } else if (i == 1) {
                m_url = PrepareUrl(data.substr(first, pos - first));
            }

            first = pos + 1;
            pos = data.find(' ', first);
            i++;
        }

        if (i != 2) {
            m_method = "None";
            m_url = "None";
            return;
        }
    }

    std::string m_method;
    std::string m_url;
};

#endif //HIGHLOAD_SERVER_HTTPREQUEST_H
