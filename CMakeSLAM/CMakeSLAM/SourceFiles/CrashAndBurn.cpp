
#include "../HeaderFiles/CMakeSLAM.h"
#include "../HeaderFiles/CrashAndBurn.h"

#include <stdlib.h>
#include <iostream>

/*
 * If this is not a test run, when this constructor prints the warning message
 * and exits the program
 * Otherwise it simply throws an exception
 */
CrashAndBurn::CrashAndBurn(const string warning)
{
    if (!TEST_RUN)
    {
        cout << "\nERROR:\n";
        cout << warning;
        exit(0);
    }

    throw exception();
}
