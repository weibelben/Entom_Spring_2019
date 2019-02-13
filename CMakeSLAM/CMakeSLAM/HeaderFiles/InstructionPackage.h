#pragma once

#include <list>
#include <string>

#include "InputFile.h"
#include "Transformation.h"
#include "FileMapping.h"

using namespace std;

class InstructionPackage
{
public:
    InstructionPackage(list<string> raw_instructions);
    InstructionPackage() : InstructionPackage(list<string>()) {};

    void populate_members();

    string instructionTitle;
    list<InputFile> inputFiles;
    list<InputFile> outputFiles;
    list<Transformation> transformations;
    FileMapping fileMapping;
    size_t num_input_files_;

private:
    list<string> instructions;
    list<string>::const_iterator insIterator;
    
    void set_instruction_title();
    void set_input_files();
    void set_output_files();
    void set_transformations();
    string next_line();
    string next_header();

    static string split_identifier(string line);
    static string split_path(string line);
    static string remove_whitespace(string str);
};