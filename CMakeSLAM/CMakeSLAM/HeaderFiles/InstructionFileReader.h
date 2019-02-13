#pragma once

#include "InstructionPackage.h"

using namespace std;

/*
 * @author Ben Weibel weibelben@gmail.com
 * The instructionFileReader opens the instruction file, copying all of the
 * instructions into a list<string>. An instructionPackage is then created
 * based on the data parsed.
 */
class InstructionFileReader
{
    InstructionPackage instructionPackage;
public:
    InstructionFileReader(string file_name);
    InstructionPackage get_instruction_package() const;
};