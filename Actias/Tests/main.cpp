#include <gtest/gtest.h>
#include <Actias/System/Runtime.h>

int main(int argc, char** argv)
{
    ActiasInit();
    ::testing::FLAGS_gtest_print_utf8 = true;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
