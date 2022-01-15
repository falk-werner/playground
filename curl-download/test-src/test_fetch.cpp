#include "fetch.h"
#include "net/http_server.hpp"

#include <gtest/gtest.h>

#include <chrono>

TEST(fetch, good)
{
    net::http_server server([](std::string const & request){ return "HTTP 200 OK\r\n\r\nfoo"; });
    std::cout << server.get_port() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(30));
}