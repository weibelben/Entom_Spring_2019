#pragma once

#include <list>
#include <string>
#include "InstructionPackage.h"

using namespace std;

/*
 * @author Ben Weibel weibelben@gmail.com
 * The instruction parser creates an InstructionFileReader and creates the
 * InstructionPackage, populating the parsers getter methods with
 * the members of the package.
 */
class InstructionParser
{
public:
    InstructionParser();
    InstructionParser(string in_file);

    // getter methods
    string getFileName() const;
    string getInstructionTitle() const;
    list<InputFile> getInputFiles() const;
    list<InputFile> getOutputFiles() const;
    list<Transformation> getTransformations() const;

    void reveal_instructions() const;

private:
    string instructionFile;
    InstructionPackage instructionPackage;
};
