// tests/test_menu_base_parsing.cpp
#include "TestContext.hpp"

#include <gtest/gtest.h>

#include "../src/menu_base.hpp"

TEST(MenuBaseTest, ParsePromptWithColorsAndVars) {
    printf("Running ParsePromptWithColorsAndVars() from %s\n", __FILE__);
    TestContext ctx;
    MenuBase base(ctx.get());

    const std::string raw = "^RHello ^P%MN ^SWorld!";
    const std::string out = "|15Hello |07%MN |03World!";

    // test
    std::string result = base.parseMenuPromptString(raw);

    EXPECT_EQ(result, out); // Validate Color Codes converted to Pipe Colors (Later ANSI)
}

/* WIP need to trace down more.
TEST(MenuBaseTest, SetupYesNoMenuInput) {
    printf("Running SetupYesNoMenuInput() from %s\n", __FILE__);
    TestContext ctx;
    MenuBase base(ctx.get());

    const std::string prompt = "Yes/No?";
    std::vector<CodeMapType> dummyMap;    // not used by the helper

    // test
    std::string out = base.setupYesNoMenuInput(prompt, dummyMap);
    printf("out from %s\n",out);

    EXPECT_NE(out.find("%01"), std::string::npos);
    EXPECT_NE(out.find("%02"), std::string::npos);
    EXPECT_EQ(out.substr(0, prompt.size()), prompt);
}*/