#include <gtest/gtest.h>
int _tmain(int argc, char * argv[])
{
    //testing::AddGlobalTestEnvironment();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}