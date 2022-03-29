#pragma once
#include <memory>
#include <functional>
#include <ostream>
#include <istream>
#include <curl/curl.h>

class HCurl {
public:
    using CURLmess = std::string;
    using WriteCb = size_t(*)(char*, size_t, size_t, void*);
    using ReadCb = size_t(*)(char*, size_t, size_t, void*);
private:
    std::unique_ptr<CURL, std::function<void(CURL*)>> _curl;
    std::ostream* _output;
    std::istream* _input;
    CURLmess _return_string;
    CURLcode _return_code;
public:
    HCurl();

    template <typename T>
    CURLcode set(CURLoption option, T value) {
        return curl_easy_setopt(_curl.get(), option, value);
    }

    HCurl& request();
    HCurl& set_ostream(std::ostream* output);
    HCurl& set_write_cb(WriteCb write_cb);
    HCurl& set_istream(std::istream* input);
    HCurl& set_read_cb(ReadCb read_cb);
    std::pair<CURLcode, CURLmess> getinfo() const;
private:
    void Init();
    static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
    static size_t read_callback(char *buffer, size_t size, size_t nitems, void *userdata);
};