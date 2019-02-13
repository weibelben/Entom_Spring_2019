#pragma once
#include <string>

using namespace std;

class CrashAndBurn
{
public:
    CrashAndBurn(string warning);

    CrashAndBurn() : CrashAndBurn("Oops, something went wrong...\n") {};
};