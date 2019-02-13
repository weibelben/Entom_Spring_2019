#include <iostream>
#include <fstream>

#include "../HeaderFiles/CMakeSLAM.h"
#include "../HeaderFiles/CrashAndBurn.h"
#include "../HeaderFiles/InstructionParser.h"

using namespace std;

/*
 * @author Ben Weibel --- weibelben@gmail.com --- (425) 260-3067
 * This is the main method of the SLAM interface. From main an InstructionParser
 * is created using the input argument provided by the user.
 */
int main(int argc, char* argv[])
{
    if (TEST_RUN) { cout << "-------------------- Test Run ----------------------"; }

    if (argc != 2) { CrashAndBurn("Usage: ./SLAM.out <instructionFile>\n"); }

    auto parser = InstructionParser(argv[1]);    

    
    for (auto transformation : parser.getTransformations())
    {
        transformation.transform();
    }

    /***** Reveal Instructions? **********************************************/
    cout << "Would you like to see all of the parsed instructions? (Y/N)\n";
    char response;
    cin.get(response);

    if (toupper(response) == 'Y')
    {
        parser.reveal_instructions();
    }
    /*************************************************************************/

    return 0;
}