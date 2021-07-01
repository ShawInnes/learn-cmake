#include "gtest/gtest.h"
#include "remoteserviceagent.h"

TEST(LibInterfaceTest, Name) {
    EXPECT_STREQ("RemoteServiceAgent", getLibInterfaceName());
}

TEST(LibInterfaceTest, Version) {
    EXPECT_STREQ("1.0.1", getLibInterfaceVersion());
}
