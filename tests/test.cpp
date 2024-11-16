#include <gtest/gtest.h>
#include "defmt/defmt.hpp"

// A simple test case
TEST(BasicTest, IntegerTest) {
    std::string str{"2147483647 4294967295 9223372036854775807 18446744073709551615"};
    int32_t num1 = 0;
    uint32_t num2 = 0;
    int64_t num3 = 0;
    uint64_t num4 = 0; 
    defmt::deformat("{} {} {} {}", str, num1, num2, num3, num4);
    EXPECT_EQ(num1, 2147483647);
    EXPECT_EQ(num2, 4294967295);
    EXPECT_EQ(num3, 9223372036854775807);
    EXPECT_EQ(num4, 18446744073709551615);
}

TEST(BasicTest, FloatTest) {
    std::string str{"123.4567 92233.72036 18446744073.7095"};
    float num1 = 0;
    double num2 = 0;
    double num3 = 0;
    defmt::deformat("{} {} {}", str, num1, num2, num3);
    EXPECT_FLOAT_EQ(num1, 123.4567);
    EXPECT_NEAR(num2, 92233.72036, 1e-5);
    EXPECT_NEAR(num3, 18446744073.7095, 1e-5);
}

TEST(CombinationTest, MixedTest) {
    std::string str{"2147483647 4294967295 9223372036854775807 18446744073709551615 123.4567 92233.72036 18446744073.7095"};
    int32_t num1 = 0;
    uint32_t num2 = 0;
    int64_t num3 = 0;
    uint64_t num4 = 0; 
    float num5 = 0;
    double num6 = 0;
    double num7 = 0;
    defmt::deformat("{} {} {} {} {} {} {}", str, num1, num2, num3, num4, num5, num6, num7);
    EXPECT_EQ(num1, 2147483647);
    EXPECT_EQ(num2, 4294967295);
    EXPECT_EQ(num3, 9223372036854775807);
    EXPECT_EQ(num4, 18446744073709551615);
    EXPECT_FLOAT_EQ(num5, 123.4567);
    EXPECT_NEAR(num6, 92233.72036, 1e-5);
    EXPECT_NEAR(num7, 18446744073.7095, 1e-5);
}

TEST(CombinationTest, MixedTestWithSpliter) {
    std::string str{"2147483647,4294967295,9223372036854775807hi18446744073709551615,123.4567|92233.72036,18446744073.7095"};
    int32_t num1 = 0;
    uint32_t num2 = 0;
    int64_t num3 = 0;
    uint64_t num4 = 0; 
    float num5 = 0;
    double num6 = 0;
    double num7 = 0;
    defmt::deformat("{},{},{}hi{},{}|{},{}", str, num1, num2, num3, num4, num5, num6, num7);
    EXPECT_EQ(num1, 2147483647);
    EXPECT_EQ(num2, 4294967295);
    EXPECT_EQ(num3, 9223372036854775807);
    EXPECT_EQ(num4, 18446744073709551615);
    EXPECT_FLOAT_EQ(num5, 123.4567);
    EXPECT_NEAR(num6, 92233.72036, 1e-5);
    EXPECT_NEAR(num7, 18446744073.7095, 1e-5);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}