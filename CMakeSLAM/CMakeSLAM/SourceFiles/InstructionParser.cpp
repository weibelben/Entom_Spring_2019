#include <iostream>

#include "../HeaderFiles/InstructionParser.h"
#include "../HeaderFiles/InstructionFileReader.h"


using namespace std;

/*
 * @author Ben Weibel weibelben@gmail.com
 * The instruction parser creates an InstructionFileReader and creates the
 * InstructionPackage, populating the parsers getter methods with
 * the members of the package.
 */
InstructionParser::InstructionParser(string in_file)
{
    instructionFile = in_file;
    InstructionFileReader file_reader = InstructionFileReader(instructionFile);
    instructionPackage = file_reader.get_instruction_package();
}

/*
 * Used for testing purposes
 */
InstructionParser::InstructionParser()
{
    instructionFile = "TEST.txt";
}

/*** Getter methods: *********************************************************/
string InstructionParser::getFileName() const
{
    return instructionFile;
}

string InstructionParser::getInstructionTitle() const
{
    return instructionPackage.instructionTitle;
}

list<InputFile> InstructionParser::getInputFiles() const
{
    return instructionPackage.inputFiles;
}

list<InputFile> InstructionParser::getOutputFiles() const
{
    return instructionPackage.outputFiles;
}

list<Transformation> InstructionParser::getTransformations() const
{
    return instructionPackage.transformations;
}
/*****************************************************************************/

/*
 * prints the instructions parsed from the instruction file
 */
void InstructionParser::reveal_instructions() const
{
    cout << "\n[" << getInstructionTitle() << "] contained the following instructions:\n";
    
    cout << "\nInput Files:\n";
    for (auto file : getInputFiles())
    {
        file.print();
    }

    cout << "\nOutput Files:\n";
    for (auto file : getOutputFiles())
    {
        file.print_output_file();
    }

    cout << "\nTransformations:\n";
    for (auto trans : getTransformations())
    {
        trans.print();
    }

    cout << "\n\n";
}
