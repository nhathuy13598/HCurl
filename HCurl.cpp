#include "HCurl.h"

HCurl::HCurl() :
    _curl(std::unique_ptr<CURL,std::function<void(CURL*)>>(curl_easy_init(), curl_easy_cleanup)),
    _output(nullptr)
{
    this->Init();
}

void HCurl::Init() {
    curl_easy_setopt(_curl.get(), CURLOPT_WRITEFUNCTION, HCurl::write_callback);
    curl_easy_setopt(_curl.get(), CURLOPT_WRITEDATA, _output);
}

HCurl& HCurl::request() {
    _return_code = curl_easy_perform(_curl.get());
    _return_string = curl_easy_strerror(_return_code);
    return *this;
}

std::pair<CURLcode, HCurl::CURLmess> HCurl::getinfo() const {
    return {_return_code, _return_string};
}

HCurl& HCurl::set_ostream(std::ostream* output) {
    _output = output;
    return *this;
}

HCurl& HCurl::set_write_cb(WriteCb write_cb) {
    return *this;
}

HCurl& HCurl::set_istream(std::istream* input) {
    _input = input;
    return *this;
}

HCurl& HCurl::set_read_cb(ReadCb read_cb) {
    return *this;
}

size_t HCurl::write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    if (userdata == nullptr) {
        return size * nmemb;
    }
    auto ostreamPtr = static_cast<std::ostream*>(userdata);
    ostreamPtr->write(ptr, nmemb * size);
    return size * nmemb;
}

size_t HCurl::read_callback(char *buffer, size_t size, size_t nitems, void *userdata) {
    if (userdata == nullptr) {
        return 0;
    }
    return 0;
}