#ifdef TEST
#include "unity.h"

extern int lin_pid_fcn(int argc, char * argv[]);

void Test_lin_pid_fcn(void)
{
    char * argv[] = { "lin_pid", "0xFF" };
    int result = lin_pid_fcn(2, argv);
    TEST_ASSERT_EQUAL_INT(EXIT_FAILURE, result);
}


#endif