#pragma once
#include <memory>
#include <functional>
#include <curl/curl.h>

/**
 * @brief RAII Wrapper class for curl_slist
 * 
 */
class CurlHeader {
private:
    std::unique_ptr<curl_slist, std::function<void(curl_slist*)>> _header;
public:

    /**
     * @brief Construct a new empty Curl Header object. We use curl_slist internally
     * 
     */
    CurlHeader();

    /**
     * @brief Construct a new Curl Header object from Map like object (such as std::map)
     * 
     * @tparam MapLikeObject object type like std::map
     * @param header object
     */
    template <typename MapLikeObject>
    CurlHeader(const MapLikeObject& header) : _header{nullptr} {
        static_assert(std::is_constructible_v<std::string, typename MapLikeObject::key_type>);
        static_assert(std::is_constructible_v<std::string, typename MapLikeObject::mapped_type>);
        for (const auto& [key, value] : header) {
            this->add(key, value);
        }
    }

    /**
     * @brief Add new key-value into header
     * 
     * @param key A string represent key
     * @param value A string represent value
     * @return CurlHeader& A referrence to object on which method is being called for method chaining
     */
    CurlHeader& add(const std::string& key, const std::string& value);

    /**
     * @brief Get raw pointer to curl_slist
     * @warning The caller must not change or manage lifetime of pointer which this method return
     * 
     * @return curl_slist* Raw pointer to curl_slist used inside CurlHeader
     */
    curl_slist* get() const;

    /**
     * @brief Reset all fields which are set before in CurlHeader
     * 
     * @return CurlHeader& A referrence to object on which method is being called for method chaining
     */
    CurlHeader& reset();
};