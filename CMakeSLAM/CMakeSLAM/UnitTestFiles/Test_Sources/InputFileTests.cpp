
#include <CMakeSLAM/Catch/catch.hpp>
#include <CMakeSLAM/HeaderFiles/InputFile.h>
#include "CMakeSLAM/HeaderFiles/CMakeSLAM.h"

#include <iostream>

using namespace std;

const string file_name = "file_name";
const string file_path = "file_path";
const auto file_mapping = FileMapping();

FileMapping get_mapping()
{
    auto mapping = FileMapping();
    mapping.add_file("file1", InputFile(mapping, "file1", "file1.ext"));
    mapping.add_file("file2", InputFile(mapping, "file2", "file2.ext"));

    auto file_list = list<InputFile>();
    file_list.emplace_back(mapping, "file3", "file3.ext");
    file_list.emplace_back(mapping, "file4", "file4.ext");

    mapping.add_file("file_list", InputFile("file_list", FileList("file_list", file_list)));

    return mapping;
}

/*
 * get mapping tests
 */

TEST_CASE("Test_get_mapping")
{
    if (!TEST_RUN)
    {
        cout << "To run SLAM_TEST:\n \tPlease change the static boolean on line 7 of CMakeSLAM to true.\n";
        cout << "\tIt is located in CMakeSLAM/SourceFiles/SMakeSLAM.cpp\n";
        exit(1);
    }

    auto mapping = get_mapping();
    REQUIRE(mapping.get_file("file1").fileName == "file1");
    REQUIRE(mapping.get_file("file1").filePath == "file1.ext");
    REQUIRE(mapping.get_file("file2").fileName == "file2");
    REQUIRE(mapping.get_file("file2").filePath == "file2.ext");
}

TEST_CASE("Get_Mapping_File_List_Is_List")
{
    auto mapping = get_mapping();
    REQUIRE(mapping.get_file("file_list").is_list());
}

TEST_CASE("Get_Mapping_File_List")
{
    auto mapping = get_mapping();
    auto file_list = mapping.get_file("file_list").file_list.files;

    auto itr = file_list.begin();
    auto file1 = *itr;
    ++itr;
    auto file2 = *itr;

    REQUIRE(file1.fileName == "file3");
    REQUIRE(file2.fileName == "file4");
}

/*
 * Input file using file list constructor tests
 */

TEST_CASE("InputFile_Outfile_Constructor_List_Test")
{
    auto inputFile = InputFile(get_mapping(), file_name, "[>file_list].out");

    REQUIRE(inputFile.is_list());
}

TEST_CASE("InputFile_Outfile_Constructor_List_Init_Test")
{
    auto inputFile = InputFile(get_mapping(), file_name, "[>file_list].out");

    REQUIRE(inputFile.fileName == file_name);
}

TEST_CASE("InputFile_Outfile_Constructor_List_Path_Test")
{
    const auto inputFile = InputFile(get_mapping(), file_name, "[>file_list].out");
    auto file_list = inputFile.file_list.files;

    auto itr = file_list.begin();
    auto file1 = *itr;
    ++itr;
    auto file2 = *itr;

    REQUIRE(file1.filePath == "file3.out");
    REQUIRE(file2.filePath == "file4.out");
}

TEST_CASE("InputFile_Outfile_Constructor_List_Name_Test")
{
    const auto inputFile = InputFile(get_mapping(), file_name, "[>file_list].out");
    auto file_list = inputFile.file_list.files;

    auto itr = file_list.begin();
    auto file1 = *itr;
    ++itr;
    auto file2 = *itr;

    REQUIRE(file1.fileName == "file_list0");
    REQUIRE(file2.fileName == "file_list1");
}

/*
 * Singular input file constructor
 */

TEST_CASE("InputFile_Outfile_Constructor_FilePath_Test")
{
    auto inputFile = InputFile(get_mapping(), file_name, "[>file1].out");

    REQUIRE(inputFile.filePath == "file1.out");
}

TEST_CASE("InputFile_Outfile_Constructor_FileName_Test")
{
    auto inputFile = InputFile(get_mapping(), file_name, "[>file1].out");

    REQUIRE(inputFile.fileName == file_name);
}

TEST_CASE("InputFile_Outfile_Bad_Constructor_FileName_Test")
{
    CHECK_THROWS(InputFile(get_mapping(), file_name, "[>fileX].out"));
}

TEST_CASE("InputFile_Constructor_FileName_Test")
{
    auto inputFile = InputFile(file_mapping, file_name, file_path);

    REQUIRE(inputFile.fileName == file_name);
}

TEST_CASE("InputFile_Constructor_FilePath_Test")
{
    auto inputFile = InputFile(file_mapping, file_name, file_path);

    REQUIRE(inputFile.filePath == file_path);
}

TEST_CASE("InputFile_Constructor_No_File_Ext_Test")
{
    CHECK_THROWS(InputFile(file_mapping, file_name, "[>file1]"));
}

TEST_CASE("InputFile_Constructor_Bad_File_Ext_Test")
{
    CHECK_THROWS(InputFile(file_mapping, file_name, "[>file1]ext"));
    CHECK_THROWS(InputFile(file_mapping, file_name, "[>file1]e.xt"));
    CHECK_THROWS(InputFile(file_mapping, file_name, "[>file1.]ext"));
}

/*
 * File list tests
 */

TEST_CASE("FileList_Constructor_listName_Test")
{
    auto name_list = list<string>();
    name_list.emplace_back(file_name);
    name_list.emplace_back(file_path);

    auto inputFiles = FileList(file_mapping, file_name, name_list);

    REQUIRE(inputFiles.listName == file_name);
}

TEST_CASE("FileList_Constructor_InputFile_Name_Test")
{
    // initialize file list
    auto name_list = list<string>();
    name_list.emplace_back(file_name);
    name_list.emplace_back(file_path);
    auto inputFiles = FileList(file_mapping, file_name, name_list);

    // declare input files
    auto itr = inputFiles.files.begin();
    auto file1 = *itr;
    ++itr;
    auto file2 = *itr;

    REQUIRE(file1.fileName == file_name + to_string(0));
    REQUIRE(file2.fileName == file_name + to_string(1));
}