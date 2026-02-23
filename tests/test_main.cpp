#include <gtest/gtest.h>

// Need to add Global excluded from xrm_server.cpp
#include "../src/model-sys/structures.hpp"

std::string GLOBAL_BBS_PATH;
std::string GLOBAL_DATA_PATH;
std::string GLOBAL_MENU_PATH;
std::string GLOBAL_MENU_PROMPT_PATH;
std::string GLOBAL_TEXTFILE_PATH;
std::string GLOBAL_SCRIPT_PATH;
std::string GLOBAL_LOG_PATH;
std::string CORE_DATABASE;

std::string BUILD_INFO = "Oblivion/2 XRM-Server Rev.3 build [00.03.01] Alpha Preview";

TEST(MathTests, Addition)
{
    printf("Running main() from %s\n", __FILE__);
    std::string test = "hello world";
    EXPECT_EQ(test, "hello world");
}