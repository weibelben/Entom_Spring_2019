#include "../HeaderFiles/InstructionFileReader.h"
#include "../HeaderFiles/CrashAndBurn.h"

#include <fstream>
#include <list>
#include <string>

using namespace std;

/*
 * @author Ben Weibel weibelben@gmail.com
 * The instructionFileReader opens the instruction file, copying all of the
 * instructions into a list<string>. An instructionPackage is then created
 * based on the data parsed.
 */
InstructionFileReader::InstructionFileReader(const string file_name)
{
    try
    {
        ifstream stream;
        stream.open(file_name);
        list<string> instructions;

        for (std::string line; getline(stream, line); )
        {
            instructions.push_back(line);
        }

        instructionPackage = InstructionPackage(instructions);
        instructionPackage.populate_members();
        stream.close();
    }
    catch (exception& e)
    {
        CrashAndBurn("Issues occured reading " + file_name + ".\n");
    }
}

/*
 * returns the instructionPackage that the file reader creates
 */
InstructionPackage InstructionFileReader::get_instruction_package() const
{
    return instructionPackage;
}
