#include "HCurl.h"

HCurl::HCurl() :
    _curl(std::unique_ptr<CURL,std::function<void(CURL*)>>(curl_easy_init(), curl_easy_cleanup)),
    _output(nullptr),
    _input(nullptr)
{
    this->Init();
}

HCurl::HCurl(InternalCurl curl) :
    _curl(std::move(curl))
{
}

void HCurl::Init() {
    this->set_ostream(_output).set_write_cb(HCurl::write_callback);
    this->set_istream(_input).set_read_cb(HCurl::read_callback);
}

HCurl& HCurl::request() {
    _return_code = curl_easy_perform(_curl.get());
    _return_mess = curl_easy_strerror(_return_code);
    return *this;
}

std::pair<CURLcode, HCurl::CURLmess> HCurl::getinfo() const {
    return {_return_code, _return_mess};
}

bool HCurl::is_ok() const {
    return _return_code == CURLE_OK;
}

HCurl& HCurl::set_ostream(std::ostream* output) {
    _output = output;
    this->set(CURLOPT_WRITEDATA, _output);
    return *this;
}

HCurl& HCurl::get(const std::string& url) {
    this->set(CURLOPT_URL, url.c_str());
    this->set(CURLOPT_CUSTOMREQUEST, "GET");
    return *this;
}

HCurl& HCurl::del(const std::string& url) {
    this->set(CURLOPT_URL, url.c_str());
    this->set(CURLOPT_CUSTOMREQUEST, "DELETE");
    return *this;
}

HCurl& HCurl::put(const std::string& url) {
    this->set(CURLOPT_URL, url.c_str());
    this->set(CURLOPT_CUSTOMREQUEST, "PUT");
    return *this;
}

HCurl& HCurl::post(const std::string& url) {
    this->set(CURLOPT_URL, url.c_str());
    this->set(CURLOPT_CUSTOMREQUEST, "POST");
    this->set(CURLOPT_POST, 1);
    return *this;
}

HCurl& HCurl::set_write_cb(WriteCb write_cb) {
    this->set(CURLOPT_WRITEFUNCTION, write_callback);
    return *this;
}

HCurl& HCurl::set_istream(std::istream* input) {
    _input = input;
    this->set(CURLOPT_READDATA, _input);
    return *this;
}

HCurl& HCurl::set_read_cb(ReadCb read_cb) {
    this->set(CURLOPT_READFUNCTION, read_cb);
    return *this;
}

HCurl& HCurl::set_header(CurlHeader header) {
    _header = std::move(header);
    this->set(CURLOPT_HTTPHEADER, _header.get());
    return *this;
}

HCurl& HCurl::reset() {
    curl_easy_reset(_curl.get());
    _header.reset();
    _output = nullptr;
    _input = nullptr;
    _return_mess.clear();
    _return_code = CURLE_OK;
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
    auto istreamPtr = static_cast<std::istream*>(userdata);
    auto num_of_bytes = istreamPtr->read(buffer, size * nitems).gcount();
    return num_of_bytes;
}