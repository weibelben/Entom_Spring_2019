#pragma once

#include <string>
#include <list>

#include "FileMapping.h"

using namespace std;

class Transformation
{
public:
    /*************************************************************************/
    enum Trans_enum {error_trans, test_trans, concat_qual_query, align_from_list};
    /*************************************************************************/    

    string input_transformation_string_;

    Transformation(FileMapping mapping, string transformation);
    void parse_enum();
    static string parse_trans_args(string input);
    void print() const;
    void transform();

private:
    list<tuple<string, Trans_enum>> transformation_mappings { 
        {"test", test_trans},
        {"concatQualQuery", concat_qual_query},
        {"alignFromList", align_from_list},
        {"ERROR", error_trans}
    };
        
    string transformation_name_;
    string transformation_args_;

    FileMapping file_mapping;
    Trans_enum transformation_type_;
    tuple<list<string>, list<string>> python_args_;
       
    static string parse_trans_name(string input);
    static string remove_whitespace(string str);
    static tuple<list<string>, list<string>> parse_python_args(string arg_str);
    static list<string> parse_csv(string csv);

    void execute_concat_qual();
    void execute_align_from_list();
    static void execute_command(list<string> args);
};