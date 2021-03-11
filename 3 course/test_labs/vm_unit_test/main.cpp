#include "tst_testMemory.h"
#include "tst_testCPU.h"
#include "tst_testCommand.h"
#include "tst_testCommand_Math.h"

#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
