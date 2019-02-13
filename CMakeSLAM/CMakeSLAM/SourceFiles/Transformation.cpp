#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>

#include "../HeaderFiles/Transformation.h"
#include "../HeaderFiles/CrashAndBurn.h"

using namespace std;

const string ConcatQual_Path = "./PythonScripts/concatQualQuery.py";
const string AlignFromList_Path = "./PythonScripts/alignFromList.py";


Transformation::Transformation(FileMapping mapping, string transformation)
{
    file_mapping = mapping;
    input_transformation_string_ = transformation;

    if (input_transformation_string_.empty()) {
        CrashAndBurn("Transformation string empty.\n");
    }

    transformation_name_ = parse_trans_name(input_transformation_string_);
    transformation_args_ = parse_trans_args(input_transformation_string_);
    transformation_type_ = error_trans;
    python_args_ = parse_python_args(transformation_args_);
}

/*
 * Depending on the name of the transformation defined, this method looks
 * at the possible name,enum pairings in transformation mappings and 
 * properly decides what enum to assign.
 */
void Transformation::parse_enum()
{
    for (auto element : transformation_mappings)
    {
        if (transformation_name_ == get<0>(element))
        {
            transformation_type_ = get<1>(element);
        }
    }

    if (transformation_type_ == error_trans) {
        CrashAndBurn("Transformation input unrecognized:\n\t[" 
            + transformation_name_
            + "] did not match any of the defined transformations\n");
    }
}

void Transformation::print() const
{
    cout << "\t" << transformation_name_ << " was called using:\n"
         << "\t\t" << transformation_args_ << "\n";
}

/*
 * Executes the transformation
 */
void Transformation::transform()
{
    switch(transformation_type_)
    {
    case concat_qual_query:
        execute_concat_qual();
        break;
    case align_from_list:
        execute_align_from_list();
        break;
    case test_trans:
        break;
    case error_trans:

    default:
        CrashAndBurn("Transformation input unrecognized:\n\t[" 
            + transformation_name_
            + "] did not match any of the defined transformations\n");
    }
}

/*
 * cuts the input string down to just the name of the transformation
 */
string Transformation::parse_trans_name(string input)
{
    const auto index = input.find('(');
    if (index < 1) {
        CrashAndBurn("Transformation input unrecognized:\n"
            + input
            + "\n\tUsage:\t<transformation>(<input>-><output>)\n");
    }

    return remove_whitespace(input.substr(0, index));
}

/*
 * cuts the input string down to just the name of the transformation
 */
string Transformation::parse_trans_args(string input)
{
    const auto index1 = input.find('(');
    const auto index2 = input.find(')');
    if (index1 < 1) {
        const string error;
        CrashAndBurn(error + "Transformation input unrecognized\n"
            + "\tUsage:\t<transformation>(<input>-><output>)\n");
    }

    return remove_whitespace(input.substr(index1 + 1, index2 - index1 - 1));
}

/*
 * Parses the input and output file args from the specified arguments
 */
tuple<list<string>, list<string>> Transformation::parse_python_args(string arg_str)
{
    list<string> args = list<string>();
    auto input_list = list<string>();
    auto output_list = list<string>();
    const size_t trans_tok = arg_str.find("=>");

    if (trans_tok == string::npos)
    {
        // csv of input files only
        const auto inputs = remove_whitespace(arg_str);
        input_list = parse_csv(inputs);
    }
    else
    {
        // csv of input files
        const auto inputs = remove_whitespace(arg_str.substr(0, trans_tok - 1));
        input_list = parse_csv(inputs);

        // csv of output files
        const auto outputs = remove_whitespace(arg_str.substr(trans_tok + 2, arg_str.size()));
        output_list = parse_csv(outputs);
    }

    return tuple<list<string>, list<string>>(input_list, output_list);
}

/*
 * returns a list of strings separated from the csv
 */
list<string> Transformation::parse_csv(string csv)
{
    list<string> separated_list;
    istringstream iss(csv);
    string str;

    while (getline(iss, str, ','))
    {
        separated_list.push_back(str);
    }

    return separated_list;
}

/*
 * This method removes whitespace (space or tab chars) from the beginning and
 * end of str
 */
string Transformation::remove_whitespace(string str)
{
    int start = 0;
    size_t end = str.length() - 1;

    // ascii for space and tab char
    while (str.at(start) == 32 || str.at(start) == 9) { start++; }
    while (str.at(end) == 32 || str.at(end) == 9) { end--; }
    return str.substr(start, end - start + 1);
}

/*
 * executes the concatQualQuery python program
 */
void Transformation::execute_concat_qual()
{
    auto input_list = get<0>(python_args_);
    auto output_list = get<1>(python_args_);

    if (input_list.size() != 2)
    {
        const string error;
        CrashAndBurn(error + "Number of input files for concatQualQuery must be 2:\n\t"
            + input_transformation_string_ + "\n");
    }    

    if (output_list.size() != 1)
    {
        const string error;
        CrashAndBurn(error + "Number of output files for concatQualQuery must be 1:\n\t"
            + input_transformation_string_ + "\n");
    }

    // un-package input list
    auto input_itr = input_list.begin();
    const auto fasta_file = remove_whitespace(*input_itr);
    ++input_itr;
    const auto list_file = remove_whitespace(*input_itr);

    // un-package output file
    const auto output_itr = output_list.begin();
    const auto output_file = remove_whitespace(*output_itr);

    // un-package commands
    auto fasta = file_mapping.get_file(fasta_file);
    auto out = file_mapping.get_file(output_file);

    if (fasta.is_list())
    {
        if (!out.is_list())
        {
            const string err;
            CrashAndBurn(err + "concatQualQuery called with a list of "
                    + "input fastas, but only one output file.\n");
        }

        auto fasta_list = fasta.file_list.files;
        auto out_list = out.file_list.files;

        if (fasta_list.size() != out_list.size())
        {
            const string err;
            CrashAndBurn(err + "concatQualQuery requirement failure:\n\t "
                + "Number of input files must be the same as the number of output files.\n");
        }

        // run a new concatQualQuery.py instance for each input fasta
        auto out_itr = out_list.begin();
        for (const auto& file : fasta_list)
        {
            const list<string> concatQualQueryArgs = list<string>({
                "python",
                ConcatQual_Path,
                file.filePath,
                file_mapping.get_file(list_file).filePath,
                (*out_itr).filePath
            });
            execute_command(concatQualQueryArgs);

            ++out_itr;
        }
    }
    else
    {
        const auto concatQualQueryArgs = list<string>({
            "python",
            ConcatQual_Path,
            file_mapping.get_file(fasta_file).filePath,
            file_mapping.get_file(list_file).filePath,
            file_mapping.get_file(output_file).filePath
            });
        execute_command(concatQualQueryArgs);
    }
}

void Transformation::execute_align_from_list()
{
    auto input_list = get<0>(python_args_);

    if (input_list.size() != 2)
    {
        const string error;
        CrashAndBurn(error + "Number of input files for concatQualQuery must be 2:\n\t"
            + input_transformation_string_ + "\n");
    }

    // un-package input list
    auto input_itr = input_list.begin();
    const auto list_file = remove_whitespace(*input_itr);
    ++input_itr;

    auto alignFromListArgs = list<string>({
        "python",
        AlignFromList_Path,
        file_mapping.get_file(list_file).filePath
        });

    while (input_itr != input_list.end())
    {
        alignFromListArgs.push_back(file_mapping.get_file(
            remove_whitespace(*input_itr)).filePath);
        ++input_itr;
    }

    execute_command(alignFromListArgs);
}

/*
 * system call, run the python program
 */
void Transformation::execute_command(list<string> args)
{
    auto itr = args.begin();
    string command;

    while (itr != args.end())
    {
        command += " " + *itr;
        ++itr;
    }

    cout << command << "\n";
    system(command.c_str());
}
