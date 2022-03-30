#include "CurlHeader.h"
CurlHeader::CurlHeader() :
    _header(std::unique_ptr<curl_slist, std::function<void(curl_slist*)>>(nullptr, curl_slist_free_all))
{
}

CurlHeader& CurlHeader::add(const std::string& key, const std::string& value) {
    auto headerPtr = curl_slist_append(_header.get(), std::string{key + ": " + value}.c_str());
    _header.release();
    _header = std::unique_ptr<curl_slist, std::function<void(curl_slist*)>>(headerPtr, curl_slist_free_all);
    return *this;
}

curl_slist* CurlHeader::get() const {
    return _header.get();
}

CurlHeader& CurlHeader::reset() {
    _header.reset();
    return *this;
}