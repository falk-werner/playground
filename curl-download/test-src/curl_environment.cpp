#include <curl/curl.h>
#include <gtest/gtest.h>

class CurlEnvironment: public ::testing::Environment
{
public:
    void SetUp() override
    {
        curl_global_init(CURL_GLOBAL_ALL);
    }

    void TearDown() override
    {
        curl_global_cleanup();
    }
};

::testing::Environment * g_curl_environment = ::testing::AddGlobalTestEnvironment(new CurlEnvironment());
