//
// Created by Dellvin on 06.03.2020.
//
#include <gtest/gtest.h>

extern "C++"{
    #include "../server/server.h"
}

TEST(test, Test1) {
    Server s(2000);
    EXPECT_EQ(true, true);
}

TEST(test, Test2) {
    Session s(43,nullptr);
    EXPECT_EQ(true, true);
}

TEST(test, Test3) {

}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}