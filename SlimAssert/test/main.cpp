
// Either use <cassert> + lower case "assert(x > y);" or
// use "../SlimAssert.h" and upper case "ASSERT(x > y);"

// Be sure that for a Release build the define NDEBUG exists.

#include <cstdio>
//#include <cassert>
#include "../SlimAssert.h"

int main(void)
{
    int x = 3;
    int y = 5;

    ASSERT(x > y);
//    assert(x > y);

    return 0;
}
