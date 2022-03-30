#pragma once
#include <ostream>
#include <istream>
#include "CurlHeader.h"

/**
 * @brief RAII wrapper class for easy curl interface, must call curl_global_init before
 * 
 */
class HCurl {
public:
    using CURLmess = std::string;
    using WriteCb = size_t(*)(char*, size_t, size_t, void*);
    using ReadCb = size_t(*)(char*, size_t, size_t, void*);
    using InternalCurl = std::unique_ptr<CURL, std::function<void(CURL*)>>;
private:
    std::unique_ptr<CURL, std::function<void(CURL*)>> _curl;
    CurlHeader _header;
    std::ostream* _output;
    std::istream* _input;
    CURLmess _return_mess;
    CURLcode _return_code;
public:
    /**
     * @brief Construct a new empty HCurl object. This constructor
     * will set read/write callback and read/write data
     */
    HCurl();

    /**
     * @brief Construct a new HCurl object from InternalCurl object
     * 
     * @param curl Unique pointer to raw curl std::unique_ptr<CURL, std::function<void(CURL*)>>
     */
    HCurl(InternalCurl curl);

    /**
     * @brief Set option for curl
     * 
     * @tparam T Value to set, you must ensure type of T according to curl library docs
     * @param option Curl option support by libcurl
     * @param value Value to set for option required by libcurl
     * @return CURLcode Return value of curl_easy_setopt internally
     */
    template <typename T>
    CURLcode set(CURLoption option, T value) {
        return curl_easy_setopt(_curl.get(), option, static_cast<T>(value));
    }

    /**
     * @brief Set url and GET method for curl
     * 
     * @param url Url to request
     * @return HCurl& A referrence to object on which method is being called for method chaining
     */
    HCurl& get(const std::string& url);

    /**
     * @brief Set url and DELETE method for curl
     * @warning This method is not tested. Use with caution
     * @param url Url to request
     * @return HCurl& A referrence to object on which method is being called for method chaining
     */
    HCurl& del(const std::string& url);

    /**
     * @brief Set url and PUT method for curl
     * @warning This method is not tested. Use with caution
     * @param url Url to request
     * @return HCurl& A referrence to object on which method is being called for method chaining
     */
    HCurl& put(const std::string& url);

    /**
     * @brief Set url and POST method for curl. Internally, HCurl
     * will set CURLOPT_POST to 1
     * @param url Url to request
     * @return HCurl& A referrence to object on which method is being called for method chaining
     */
    HCurl& post(const std::string& url);

    /**
     * @brief Send the request. This function block the running thread until it finishs.
     * Internally, HCurl will use curl_easy_perform
     * @return HCurl& A referrence to object on which method is being called for method chaining
     */
    HCurl& request();

    /**
     * @brief Set the ostream object. Should use with set_write_cb or
     * HCurl will use write_callback internally or null
     * @param output Pointer to object which inherited form std::ostream
     * @return HCurl& A referrence to object on which method is being called for method chaining
     */
    HCurl& set_ostream(std::ostream* output);

    /**
     * @brief Set the write cb function. Set it or HCurl
     * will use write_callback internally or null
     * @param write_cb A function which has signature size_t(*)(char*, size_t, size_t, void*)
     * @return HCurl& A referrence to object on which method is being called for method chaining
     */
    HCurl& set_write_cb(WriteCb write_cb);

    /**
     * @brief Set the istream object. Should use with set_read_cb or
     * HCurl will use read_callback internally or null
     * @param input Pointer to object which inherited form std::istream
     * @return HCurl& A referrence to object on which method is being called for method chaining
     */
    HCurl& set_istream(std::istream* input);

    /**
     * @brief Set the read cb function. Set it or HCurl
     * will use read_callback internally or null
     * @param read_cb A function which has signature size_t(*)(char*, size_t, size_t, void*)
     * @return HCurl& A referrence to object on which method is being called for method chaining
     */
    HCurl& set_read_cb(ReadCb read_cb);

    /**
     * @brief Set the header object from
     * 
     * @param header A CurlHeader object. HCurl will take the ownership of this object
     * @return HCurl& A referrence to object on which method is being called for method chaining
     */
    HCurl& set_header(CurlHeader header);

    /**
     * @brief Reset HCurl to reuse it. All options and members of this HCurl will be reset
     * @warning This method is not tested. Use with caution
     * @return HCurl& A referrence to object on which method is being called for method chaining
     */
    HCurl& reset();

    /**
     * @brief Get result after perform request. Always call after request method
     * 
     * @return std::pair<CURLcode, CURLmess> first is CURLcode, second is the message representing CURLcode
     */
    std::pair<CURLcode, CURLmess> getinfo() const;

    /**
     * @brief Check whether response code after performing request
     * 
     * @return true if response code is CURLE_OK
     * @return false otherwise
     */
    bool is_ok() const;
private:
    /**
     * @brief Init some option when HCurl object first constructed.
     * This method will set read/write data and read/write callback
     * 
     */
    void Init();

public:
    /**
     * @brief Default write callback function of HCurl
     * 
     * @param ptr Pointer to buffer which contains data
     * @param size Size of each block data
     * @param nmemb Number of block data
     * @param userdata Pointer to user defined data
     * @return size_t Number of bytes (size * nmemb)
     */
    static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);

    /**
     * @brief Default read callback function of HCurl
     * 
     * @param buffer Pointer to buffer which we will write data to
     * @param size Size of each item
     * @param nitems Number of items to write
     * @param userdata Pointer to user defined data
     * @return size_t Number of bytes has written to buffer
     */
    static size_t read_callback(char *buffer, size_t size, size_t nitems, void *userdata);
};