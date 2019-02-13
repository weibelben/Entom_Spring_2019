#include "../TestMain.h"

#include <CMakeSLAM/Catch/catch.hpp>
#include <CMakeSLAM/HeaderFiles/FileMapping.h>
#include <CMakeSLAM/HeaderFiles/Transformation.h>

using namespace std;

FileMapping create_file_mapping()
{
    auto file_mapping = FileMapping();
    file_mapping.add_file("file1", InputFile(file_mapping, "file1", "file1.fasta"));
    file_mapping.add_file("file2", InputFile(file_mapping, "file2", "file2.fasta"));

    return file_mapping;
}

TEST_CASE("Transformation_Constructor_Test")
{
    if (!TEST_RUN)
    {
        cout << "To run SLAM_TEST:\n \tPlease change the static boolean on line 7 of CMakeSLAM to true.\n";
        cout << "\tIt is located in CMakeSLAM/SourceFiles/SMakeSLAM.cpp\n";
        exit(1);
    }

    const string test_string = "test(test=>test)";

    auto transformation = Transformation(create_file_mapping(), test_string);

    REQUIRE(transformation.input_transformation_string_ == test_string);
}

TEST_CASE("Transformation_Empty_Transformation_Test")
{
    const string empty_string;

    CHECK_THROWS(Transformation(create_file_mapping(), empty_string));
}

TEST_CASE("Transformation_Parse_Unrecognized_Enum_Test")
{
    const string unrecognized_string = "unknown(aa=>bb)";

    auto transformation = Transformation(create_file_mapping(), unrecognized_string);

    CHECK_THROWS(transformation.parse_enum());
}

TEST_CASE("Transformation_Parse_concatQualQuery_Test")
{
    const string trans_string = "concatQualQuery(aa=>bb)";

    auto transformation = Transformation(create_file_mapping(), trans_string);

    // no error thrown : valid input
    transformation.parse_enum();
}

TEST_CASE("Transformation_Parse_alignFromList_Test")
{
    const string trans_string = "alignFromList(aa=>bb)";

    auto transformation = Transformation(create_file_mapping(), trans_string);

    // no error thrown : valid input
    transformation.parse_enum();
}

TEST_CASE("Transformation_Parse_Args_Test")
{
    const string test_trans = "test(aa=>bb)";

    auto transformation = Transformation(create_file_mapping(), test_trans);

    REQUIRE(transformation.parse_trans_args(test_trans) == "aa=>bb");
}