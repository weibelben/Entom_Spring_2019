#include <iostream>
#include <list>

#include <CMakeSLAM/Catch/catch.hpp>
#include <CMakeSLAM/HeaderFiles/InstructionPackage.h>
#include "CMakeSLAM/HeaderFiles/CMakeSLAM.h"

using namespace std;

// const strings used for testing purposes
const string title = "Example title";
const string transformation = "\ttest(test=>test)";

list<string> get_instructions()
{
    list<string> instructions;

    instructions.emplace_back(title);
    instructions.emplace_back("Input Files:\n");
    instructions.emplace_back("\tfile1 = file1.fasta");
    instructions.emplace_back("Output Files:\n");
    instructions.emplace_back("\tfile2 = file2.fasta");
    instructions.emplace_back("Transformations:\n");
    instructions.emplace_back(transformation);

    return instructions;
}

InstructionPackage get_package()
{
    auto package = InstructionPackage(get_instructions());
    return package;
}

TEST_CASE("InstructionPackage_Empty_Constructor_Test")
{
    if (!TEST_RUN)
    {
        cout << "To run SLAM_TEST:\n \tPlease change the static boolean on line 7 of CMakeSLAM to true.\n";
        cout << "\tIt is located in CMakeSLAM/SourceFiles/SMakeSLAM.cpp\n";
        exit(1);
    }

    auto package = InstructionPackage();

    CHECK_THROWS(package.populate_members());
}

TEST_CASE("InstructionPackage_Valid_Instructions_Test")
{
    auto package = get_package();

    REQUIRE(package.fileMapping.empty());
}

TEST_CASE("InstructionPackage_Populate_Title_Test")
{
    auto package = get_package();
    package.populate_members();

    REQUIRE(package.instructionTitle == title);
    REQUIRE(!package.fileMapping.empty());
}

TEST_CASE("InstructionPackage_Populate_Input_Files_Test")
{
    auto package = get_package();
    package.populate_members();

    REQUIRE(package.fileMapping.get_file("file1").fileName == "file1");
    REQUIRE(package.fileMapping.get_file("file1").filePath == "file1.fasta");
}

TEST_CASE("InstructionPackage_Populate_Output_Files_Test")
{
    auto package = get_package();
    package.populate_members();

    REQUIRE(package.fileMapping.get_file("file2").fileName == "file2");
    REQUIRE(package.fileMapping.get_file("file2").filePath == "file2.fasta");
}

TEST_CASE("InstructionPackage_Populate_Trnsormations_Test")
{
    auto package = get_package();
    package.populate_members();

    // input string of first transformation in transformation list == "test"
    REQUIRE((*package.transformations.begin()).input_transformation_string_ == transformation);
}

TEST_CASE("InstructionPackage_No_Title_Test")
{
    auto instructions = get_instructions();
    instructions.remove(title);
    auto package = InstructionPackage(instructions);
    package.populate_members();

    REQUIRE(package.instructionTitle == "Auto-Generated Instruction Title");
    REQUIRE(package.fileMapping.get_file("file1").fileName == "file1");
}

TEST_CASE("InstructionPackage_No_InputFile_Header_Test")
{
    auto instructions = get_instructions();
    instructions.remove("Input Files:\n");
    auto package = InstructionPackage(instructions);

    CHECK_THROWS(package.populate_members());
}

TEST_CASE("InstructionPackage_No_OutputFile_Header_Test")
{
    auto instructions = get_instructions();
    instructions.remove("Output Files:\n");
    auto package = InstructionPackage(instructions);

    CHECK_THROWS(package.populate_members());
}

TEST_CASE("InstructionPackage_No_Transformation_Header_Test")
{
    auto instructions = get_instructions();
    instructions.remove("Transformations:\n");
    auto package = InstructionPackage(instructions);

    CHECK_THROWS(package.populate_members());
}

TEST_CASE("InstructionPackage_No_InputFiles_Test")
{
    auto instructions = get_instructions();
    instructions.remove("\tfile1 = file1.fasta");
    auto package = InstructionPackage(instructions);

    CHECK_THROWS(package.populate_members());
}

TEST_CASE("InstructionPackage_No_OutputFiles_Test")
{
    auto instructions = get_instructions();
    instructions.remove("\tfile2 = file2.fasta");
    auto package = InstructionPackage(instructions);

    CHECK_THROWS(package.populate_members());
}

TEST_CASE("InstructionPackage_No_Transformations_Test")
{
    auto instructions = get_instructions();
    instructions.remove(transformation);
    auto package = InstructionPackage(instructions);

    CHECK_THROWS(package.populate_members());
}

TEST_CASE("InstructionPackage_Repeat_File_Id_Diff_Handle_Test")
{
    auto instructions = get_instructions();
    replace(instructions.begin(), instructions.end(), "\tfile2 = file2.fasta", "\tfile1 = file2.fasta");
    auto package = InstructionPackage(instructions);

    CHECK_THROWS(package.populate_members());
}

TEST_CASE("InstructionPackage_Repeat_File_Id_Same_Handle_Test")
{
    auto instructions = get_instructions();
    replace(instructions.begin(), instructions.end(), "\tfile2 = file2.fasta", "\tfile1 = file2.fasta");
    replace(instructions.begin(), instructions.end(), "\tfile1 = file2.fasta", "\tfile1 = file1.fasta");

    list<string>::const_iterator itr = instructions.begin();
    for (int i = 0; i < 5; i++) { ++itr; }
    instructions.insert(itr, "\tfile3 = file3.fasta");

    auto package = InstructionPackage(instructions);

    package.populate_members();
    REQUIRE(package.fileMapping.get_file("file1").fileName == "file1");
}

TEST_CASE("InstructionPackage_Unknown_Transformation_Test")
{
    auto instructions = get_instructions();
    replace(instructions.begin(), instructions.end(), "\ttest(test=>test)", "\tWrng(test=>test)");
    auto package = InstructionPackage(instructions);

    CHECK_THROWS(package.populate_members());
}