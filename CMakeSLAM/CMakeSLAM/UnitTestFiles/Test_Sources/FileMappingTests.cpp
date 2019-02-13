
#include <CMakeSLAM/Catch/catch.hpp>
#include <CMakeSLAM/HeaderFiles/FileMapping.h>
#include <CMakeSLAM/HeaderFiles/CMakeSLAM.h>
#include <iostream>

using namespace std;

const string id = "identifier";
const string path = "path/to/id";

TEST_CASE("FileMapping_Constructor_Test")
{
    if (!TEST_RUN)
    {
        cout << "To run SLAM_TEST:\n \tPlease change the static boolean on line 7 of CMakeSLAM.cpp to true.\n";
        cout << "\tIt is located in CMakeSLAM/SourceFiles/SMakeSLAM.cpp\n";
        exit(1);
    }

    const auto file_mapping = FileMapping();

    REQUIRE(file_mapping.empty());
}

TEST_CASE("FileMapping_Add_Valid_File_Test")
{
    auto file_mapping = FileMapping();
    const auto mock_input_file = InputFile(file_mapping, id, path);

    file_mapping.add_file(id, mock_input_file);

    REQUIRE(!file_mapping.empty());
}

TEST_CASE("FileMapping_Add_Duplicate_File_Test")
{
    auto file_mapping = FileMapping();
    const auto mock_input_file_1 = InputFile(file_mapping, id, path);
    const auto mock_input_file_2 = InputFile(file_mapping, id, "different/path");

    file_mapping.add_file(id, mock_input_file_1);
    CHECK_THROWS(file_mapping.add_file(id, mock_input_file_2));
}

TEST_CASE("FileMapping_Get_Valid_File_Test")
{
    auto file_mapping = FileMapping();
    const auto mock_input_file = InputFile(file_mapping, id, path);

    file_mapping.add_file(id, mock_input_file);
    const string identifier = file_mapping.get_file(id).fileName;
    const string file_path = file_mapping.get_file(id).filePath;

    REQUIRE(identifier == id);
    REQUIRE(file_path == path);
}

TEST_CASE("FileMapping_Get_Invalid_File_Test")
{
    auto file_mapping = FileMapping();
    const auto mock_input_file = InputFile(file_mapping, id, path);

    const string non_existent_id = "unknown";

    file_mapping.add_file(id, mock_input_file);
    CHECK_THROWS(file_mapping.get_file(non_existent_id).fileName);
    CHECK_THROWS(file_mapping.get_file(non_existent_id).filePath);
}
