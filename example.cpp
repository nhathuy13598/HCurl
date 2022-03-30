#include <iostream>
#include <sstream>
#include <curl/curl.h>
#include "HCurl.h"

int main(void)
{
    std::stringstream input;
    std::stringstream output;
    HCurl curl;

    curl.post("https://www.google.com");
    curl.set_ostream(&output).set_write_cb(HCurl::write_callback);
    curl.set_istream(&input).set_read_cb(HCurl::read_callback);
    curl.request();
    auto result = curl.getinfo();
    std::cout << result.first << std::endl;
    std::cout << result.second << std::endl;
    std::cout << "Content: " << output.str() << std::endl;
    return 0;
}