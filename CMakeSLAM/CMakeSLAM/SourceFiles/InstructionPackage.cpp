#include <list>
#include <string>

#include "../HeaderFiles/InstructionPackage.h"
#include "../HeaderFiles/CrashAndBurn.h"


using namespace std;

InstructionPackage::InstructionPackage(list<string> raw_instructions)
{
    num_input_files_ = 0;
    fileMapping = FileMapping();
    instructions = raw_instructions;
    insIterator = instructions.begin();
}

void InstructionPackage::populate_members()
{
    if (instructions.empty())
    {
        CrashAndBurn("Instruction package empty.\n");
    }

    set_instruction_title();
    set_input_files();
    set_output_files();
    set_transformations();
}

/*
 * Parsed Instruction Title Format:
 * <Title of ins file>
 * as the first line with content.
 */
void InstructionPackage::set_instruction_title()
{
    string ins_line = next_header();

    if (ins_line.find("Input Files:") != string::npos)
    {
        instructionTitle = "Auto-Generated Instruction Title";
        --insIterator;
    }
    else
    {
        instructionTitle = ins_line;
    }
}

/*
 * Parsed Input File Format:
 * Input Files:
 *      fileA = path/to/file/fileA.fasta
 */
void InstructionPackage::set_input_files()
{
    string ins_line = next_header();
    
    if (ins_line.find("Input Files:") != 0)
    {
        const string error;
        CrashAndBurn(error
            + "The instruction following the instruction title did not "
            + "specify input files.\nUsage:\nInput Files:\n\tfileN = <path>\n");
    }
    
    // at this point, the iterator contains the line "Input Files:"
    auto arg = next_line();
    while (!arg.empty() && arg.find("Output Files:") == string::npos)
    {
        const string identifier = split_identifier(arg);
        const string file_path = split_path(arg);

        inputFiles.emplace_back(fileMapping, identifier, file_path);
        fileMapping.add_file(identifier, InputFile(fileMapping, identifier, file_path));

        if (insIterator == instructions.end())
        {
            CrashAndBurn("There are no instructions following the input files\n");
        }
        arg = next_line();
    }

    // no input files were parsed
    if (fileMapping.empty())
    {
        CrashAndBurn("No input files were specified.\n");
    }

    num_input_files_ = fileMapping.num_mappings;
}
/*
 * Parsed Output File Format:
 * Output Files:
 *      fileA = path/to/file/fileA.fasta
 */
void InstructionPackage::set_output_files()
{
    string ins_line = next_header();
    
    if (ins_line.find("Output Files:") != 0)
    {
        const string error;
        CrashAndBurn(error
            + "The instruction following the input files did not specify output files.\n"
            + "Usage:\nOutput Files:\n\tfileN = <path>\n");
    }

    // at this point, the iterator contains the line "Output Files:"
    auto arg = next_line();
    while (!arg.empty() && arg.find("Transformations:") == string::npos)
    {
        const string identifier = split_identifier(arg);
        const string file_path = split_path(arg);

        outputFiles.emplace_back(fileMapping, identifier, file_path);
        fileMapping.add_file(identifier, InputFile(fileMapping, identifier, file_path));

        if (insIterator == instructions.end())
        {
            CrashAndBurn("No instructions following the output files\n");
        }
        arg = next_line();
    }

    // no input files were parsed
    if (fileMapping.num_mappings == num_input_files_)
    {
        // No output files were added to the file_mapping
        CrashAndBurn("No output files were found.\n");
    }
}

/*
 * Parsed Transformation Format:
 * Transformations:
 *      <transformation>(fileA.fasta -> fileB.out)
 */
void InstructionPackage::set_transformations()
{
    string ins_line = next_header();
    
    if (ins_line.empty() || ins_line.find("Transformations:") != 0)
    {
        const string error;
        CrashAndBurn(error
            + "The instruction following the output files did not specify transformations.\n"
            + "Usage:\nTransformations:\n\t<transformation>(fileA.ext => fileB.ext)\n");
    }

    // at this point, the iterator contains the line "Transformations:"    
    auto arg = next_line();
    while (!arg.empty())
    {
        auto transformation = Transformation(fileMapping, arg);
        transformation.parse_enum();
        transformations.push_back(transformation);

        arg = next_line();
    }

    // no transformations were parsed
    if (transformations.empty())
    {
        CrashAndBurn("No transformations were found.\n");
    }
}

/*
 * Parses the file identifier from the input or output file line:
 *      fileA = path/to/file/fileA.fasta
 */
string InstructionPackage::split_identifier(string line)
{
    const size_t index = line.find('=');

    if (index == string::npos)
    {
        CrashAndBurn("Input File Format:\n\tidentifier=path/to/file.ext\n");
    }

    return remove_whitespace(line.substr(0, line.find('=')));
}

/*
 * Parses the file path from the input or output file line:
 *      fileA = path/to/file/fileA.fasta
 */
string InstructionPackage::split_path(string line)
{
    const size_t index = line.find('=');
    return remove_whitespace(line.substr(index + 1, line.length() - index));
}

/*
 * This method removes whitespace (space or tab chars) from the beginning and end of str
 */
string InstructionPackage::remove_whitespace(string str)
{
    int start = 0;
    size_t end = str.length() - 1;

    // ascii for space and tab char
    while (str.at(start) == 32 || str.at(start) == 9) { start++; }
    while (str.at(end) == 32 || str.at(end) == 9) { end--; }
    return str.substr(start, end - start + 1);
}

/*
 * Increments the instruction pointer until a line is found with content.
 * Parameters:  instruction is the method that calls the continuation.
 *              argument is the line that was first parsed before continuation call.
 * Returns the first line with content.
 */
string InstructionPackage::next_line()
{
    if (insIterator == instructions.end())
    {
        return string();
    }

    string argument = *insIterator;
    while (argument.empty() || argument.find_first_of('#') == 0)
    {
        // empty line or comment
        ++insIterator;
        if (insIterator == instructions.end())
        {
            return string();
        }

        argument = *insIterator;
    }

    if (argument.substr(0, 1) != "\t")
    {
        return string();
    }

    ++insIterator;
    return argument;
}

string InstructionPackage::next_header()
{
    string argument = *insIterator;

    while (argument.empty() || argument.find_first_of('#') == 0)
    {
        // empty line or comment
        ++insIterator;
        if (insIterator == instructions.end())
        {
            return string();
        }

        argument = *insIterator;       
    }

    ++insIterator;
    return argument;
}


