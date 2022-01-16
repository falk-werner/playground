#include "download.h"
#include "net/http_server.hpp"

#include <gtest/gtest.h>

#include <unistd.h>

#include <sstream>
#include <future>


TEST(download, good)
{
    net::http_server server([](std::string const & request){ return "HTTP/1.0 200 OK\r\n\r\nfoo"; });

    std::stringstream url;
    url << "http://localhost:" << server.get_port();

    download * handle = download_create(url.str().c_str(), "download.txt");
    ASSERT_NE(nullptr, handle);
    int status = download_perform(handle);
    download_release(handle);

    ASSERT_EQ(200, status);
    unlink("download.txt");
}

TEST(download, bad)
{
    net::http_server server([](std::string const & request){ return 
        "HTTP/1.0 200 OK\r\n"
        "Content-Length: 42\r\n"
        "\r\n"
        "foo"; });

    std::stringstream url;
    url << "http://localhost:" << server.get_port();

    download * handle = download_create(url.str().c_str(), "download.txt");
    ASSERT_NE(nullptr, handle);
    int status = download_perform(handle);
    download_release(handle);

    ASSERT_NE(200, status);
    unlink("download.txt");
}

TEST(download, cancel)
{
    download * handle = NULL;
    std::promise<int> p;
    std::future<int> f = p.get_future();

    net::http_server server([&handle, &f](std::string const & request){ 
        download_cancel(handle);
        f.get();
        return ""; 
    });

    std::stringstream url;
    url << "http://localhost:" << server.get_port();

    handle = download_create(url.str().c_str(), "download.txt");
    ASSERT_NE(nullptr, handle);
 
    int status = download_perform(handle);
    download_release(handle);
    p.set_value(42);

    ASSERT_NE(200, status);
    unlink("download.txt");
}

TEST(download, rety_download)
{
    net::http_server server([](std::string const & request){ 
        static int count = 0;

        if (count == 0)
        {
            count++;
            return 
            "HTTP/1.0 200 OK\r\n"
            "Content-Length: 6\r\n"
            "\r\n"
            "foo";
        }
        else
        {
            return 
            "HTTP/1.0 200 OK\r\n"
            "Content-Length: 3\r\n"
            "Content-Range: 3-5/6\r\n"
            "\r\n"
            "bar";
        }
    });

    std::stringstream url;
    url << "http://localhost:" << server.get_port();

    download * handle = download_create(url.str().c_str(), "download.txt");
    ASSERT_NE(nullptr, handle);
    int status = download_perform(handle);
    download_release(handle);

    ASSERT_EQ(200, status);
    unlink("download.txt");
}
