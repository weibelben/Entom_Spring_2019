import sys
import collections

import Data_Class

# Error message printer and function exit handler 
def crashAndBurn(message):
        print("Python Script ERROR: <concatQualQuery.py>")
        print(message)
        input("Press any key to continue...")
        exit()

"""
File Reader opens the fasta and list file, reads each line from them, and returns a 
tuple with the read data
"""
class File_Reader():

    def parse_gene_name(self, out_file_name):
        start_index = out_file_name.rfind("\\") + 1
        ext_index = out_file_name.find(".", 1)

        if ext_index <= start_index:
            crashAndBurn(out_file_name + " does not have a proper file extention.")

        return out_file_name[start_index : ext_index]

    def parse_read_name(self, read):
        start_index = str(read).find('>') + 1
        end_index = str(read).find(' ')

        if (start_index >= end_index):
            crashAndBurn(read + " is an invalid entry in the list file.")

        return str(read)[start_index : end_index]

    def parse_read_len(self, read):
        start_index = str(read).find(' ') + 1

        if (start_index < 0):
            crashAndBurn(read + " is an invalid entry in the list file.")

        try:
            val = int((read)[start_index : len(read)])
        except:
            crashAndBurn(read + " did not have an integer length")
        return val

    # opens and read the list file, returning a dictionary:
    #   [read_name : tuple<read_length, num_files>] 
    def read_list_file(self, list_file_name):
        print("\tReading list file: [" + list_file_name + "].")

        list_file = None
        read_dict = dict()

        try:
            list_file = open(list_file_name, "r")
            read_list = list_file.readlines()

            for read in read_list:
                read_name = self.parse_read_name(read)
                gene_data = Data_Class.Phy_Data(self.parse_read_len(read), 0, dict())

                read_dict.update({read_name : gene_data})
            
        finally:
            if list_file is None:
                crashAndBurn("Could not open " + list_file_name + " for reading.")

        list_file.close

        return read_dict


    def read_out_file(self, list_dictionary, out_file_name):
        print("\tParsing [" + out_file_name + "].")
        out_file = None

        try:
            out_file = open(out_file_name, "r")
            lines = out_file.readlines()

            line_index = 0
            while line_index < len(lines):
                species_name = self.parse_gene_name(out_file_name)
                read_name = self.parse_read_name(lines[line_index])
                line_index += 1

                read = lines[line_index]
                line_index += 1

                Data_Class.Phy_Data.add_species(list_dictionary[read_name], species_name, read)
        finally:
            if out_file is None:
                crashAndBurn("Could not open " + out_file + " for reading.")

        out_file.close

        return list_dictionary


'''
Writes the read data to the respective output file
'''
class File_Writer():
    def __init__(self):
        self.output = None

    def write(self, phy_dict):
        try:
            for phy in phy_dict:
                self.output = open(phy + ".phy", "w")                
                self.output.write(str(phy_dict[phy].num_species) + " " + str(phy_dict[phy].length) + "\n\n")

                for read in phy_dict[phy].gene_dict:
                    self.output.write(read + "\n\t" + phy_dict[phy].gene_dict[read])

        finally:
            if self.output is None:
                crashAndBurn("Could not open " + self.output + " for reading.") 
           
        self.output.close


'''
main method of Align From List
Usage:
    python alignFromList <list_file_name> <input_file_name>*
'''
if __name__ == '__main__':
    # copy data from files to memory 
    if(len(sys.argv) < 3):
        crashAndBurn("Too few arguments.\nUsage:\n\talignFromList <list_file_name> <input_file_name>*\n")

    file_reader = File_Reader()
    list_dictionary = file_reader.read_list_file(sys.argv[1])

    arg_count = 2
    while arg_count < len(sys.argv):
        # fill in each read's dictionary
        list_dictionary = file_reader.read_out_file(list_dictionary, sys.argv[arg_count])
        arg_count += 1

    file_writer = File_Writer()
    file_writer.write(list_dictionary)

    input("DONE!")
    exit()