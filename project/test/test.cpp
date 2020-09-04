//
// Created by Dellvin on 06.03.2020.
//
#include <gtest/gtest.h>

extern "C++"{
    #include "../server/server.h"
}

TEST(test, Test1) {
    Server s(1);
    EXPECT_EQ(s.start(), 3);
}

TEST(test, Test2) {
    Server s(2000);
    EXPECT_NE(s.getLobby(), nullptr);
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}