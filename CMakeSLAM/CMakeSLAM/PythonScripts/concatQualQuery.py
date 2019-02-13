import sys
import collections

import Data_Class

# Error message printer and function exit handler 
def crashAndBurn(message):
        print("Python Script ERROR: <concatQualQuery.py>")
        print(message)
        input("Press any key to continue...")
        exit()

# finds the index of the nth occurrence of sym after the start index in the given string
def find_nth_char(index, str, sym, n):
        if n < 1:
            crashAndBurn("Input of find_nth_char must be a positive integer.")
        if len(str) < index < 0:
            crashAndBurn("Index " + index + " is invalid for string of length " + len(str) + ":\n\t" + str)

        while index < len(str):
            if str[index] == sym:
                if n == 1:
                    return index
                else:
                    n -= 1
            index += 1
        
        crashAndBurn("The " + n + "th index of " + sym + " was not found in " + str)


"""
File Reader opens the fasta and list file, reads each line from them, and returns a 
tuple with the read data
"""
class File_Reader():
    # opens and read the files 
    def read_files(self, fasta_file_name, list_file_name):
        print("\tParsing [" + fasta_file_name + "] using [" + list_file_name + "].")
        
        # open both files for reading:
        fasta_file = None
        list_file = None

        try:
            fasta_file = open(fasta_file_name, "r")    
            list_file = open(list_file_name, "r")

            fasta_content = fasta_file.readlines()
            list_content = list_file.readlines()

            content_tuple = (fasta_content, list_content)
            
        finally:
            if fasta_file is None:
                crashAndBurn("Could not open " + fasta_file_name + " for reading.")
            if list_file is None:
                crashAndBurn("Could not open " + list_file_name + " for reading.") 

        #close both files, return tuple
        fasta_file.close
        list_file.close
        return content_tuple


    # checks input arg count and calls read_files
    # returns the tuple of read data
    def read(self, fasta_file, list_file):
        if len(sys.argv) != 4:
            crashAndBurn("concatQualQuery Usage:\n\tconcatQualQuery.py <input filename> <List of reads> <output file>")
        
        file_info_tuple = File_Reader.read_files(self, fasta_file, list_file)
        return file_info_tuple


"""
File Parser uses the fasta and list file data and creates a dictionary of the
read names
"""
class File_Parser():
    def __init__(self):
        self.fasta_index = 0
        self.list_index = 0

    ############################ FASTA PARSING METHODS ########################
    # parses the name of the read from the curr index of the fasta lines
    def parse_fasta_read_name(self, fasta_line):
        start = fasta_line.find(')') + 2
        end   = find_nth_char(start, fasta_line, ' ', 1)
        return fasta_line[start:end]

    # parses the scaffold of the current read header
    def parse_scaffold(self, fasta_line):
        start = fasta_line.find(' ') + 1
        end = fasta_line.find('[') - 3
        return fasta_line[start:end]

    # parses the index of the current scaffold in the fasta file
    def parse_scaffold_index(self, fasta_line):
        start = find_nth_char(0,fasta_line, '(', 2)
        end = find_nth_char(start, fasta_line, ')', 1) + 1
        return fasta_line[start:end]
    
    # parses the start index of the scaffold in the current read from its header
    def parse_start_index(self, fasta_line):
        start = find_nth_char(0, fasta_line, '(', 2) + 2
        end = find_nth_char(start, fasta_line, '-', 1) - 1

        start_index_string = fasta_line[start:end]
        try:
            return int(start_index_string)
        except (ValueError, TypeError):
            crashAndBurn("Line " + (self.list_index + 1) + " of " + sys.argv[1] 
                + "\n\tHad trouble parsing the start index of the scaffold in its respective read")        

    # parses the end index of the scaffold in the current read from its header
    def parse_end_index(self, fasta_line):
        start = find_nth_char(0, fasta_line, '(', 2)
        start = find_nth_char(start, fasta_line, '-', 1) + 2
        end = find_nth_char(start, fasta_line, ')', 1) - 1

        end_index_string = fasta_line[start:end]
        try:
            return int(end_index_string)
        except (ValueError, TypeError):
            crashAndBurn("Line " + (self.list_index + 1) + " of " + sys.argv[1] 
                + "\n\tHad trouble parsing the end index of the scaffold in its respective read")

    # parses the quality of the read
    def parse_read_qual(self, fasta_line):
        start = find_nth_char(0, fasta_line, ')', 2) + 3
        end = find_nth_char(start, fasta_line, '.', 1)

        start_index_string = fasta_line[start:end]
        try:
            return int(start_index_string)
        except (ValueError, TypeError):
            crashAndBurn("Line " + (self.list_index + 1) + " of " + sys.argv[1] 
                + "\n\tHad trouble parsing the quality of the scaffold in its respective read")


    ######################### LIST PARSING METHODS ############################
    #parses the name of the read
    def parse_line_read_name(self, list_line):
        if list_line[0] != '>':
            crashAndBurn("Line " + (self.list_index + 1) + " of " + sys.argv[2] 
                + " did not begin with a \'>\' symbol.")

        space_index = list_line.find(' ')
        if (len(list_line) < space_index < 2):
            crashAndBurn("Line " + (self.list_index + 1) + " of " + sys.argv[1] 
                + " did not follow the format\n\t[>Sequence_name num_bases]")

        name = list_line[1:space_index]        
        return name

    # parses the number of bases in the given read
    def parse_num_bases(self, list_line):
        space_index = list_line.find(' ')
        if (len(list_line) < space_index < 2):
            crashAndBurn("Line " + (self.list_index + 1) + " of " + sys.argv[2] 
                + " did not follow the format\n\t[>Sequence_name num_bases]")

        bases_string = list_line[space_index:]
        try:
            return int(bases_string)
        except (ValueError, TypeError):
            crashAndBurn("Line " + (self.list_index + 1) + " of " + sys.argv[2] 
                + " did not have an integer value as its final arg")

    ######################### General Purpose #################################
    # fills in the member var data for each new read
    def fill_in_data(self, fasta_data, data, fasta_line):
        Data_Class.Read_Data.update_read(data,
                                        fasta_data[self.fasta_index + 1],
                                        File_Parser.parse_start_index(self, fasta_line), 
                                        File_Parser.parse_end_index(self, fasta_line),
                                        File_Parser.parse_read_qual(self, fasta_line),
                                        self.fasta_index + 1,
                                        sys.argv[1])
        
        Data_Class.Read_Data.add_scaffold(data, 
                                        File_Parser.parse_scaffold(self, fasta_line),
                                        File_Parser.parse_scaffold_index(self, fasta_line))
        
    # controls the parsing algorithm for each read
    def parse(self, info_tuple):
        Reads = collections.defaultdict(Data_Class.Read_Data)
        discovered_name = File_Parser.parse_fasta_read_name(self, 
                                                    info_tuple[0][self.fasta_index])

        # iterates through the list file, updates all dictionary indices
        while self.list_index < (len(info_tuple[1])):
            expected_name = File_Parser.parse_line_read_name(self, info_tuple[1][self.list_index])
            bases = File_Parser.parse_num_bases(self, info_tuple[1][self.list_index])
            
            # set empty data
            data = Data_Class.Read_Data(read=[Data_Class.UNKNOWN_BASE] * bases, 
                                        scaffolds=[], 
                                        scaffold_locations=[],
                                        qualities=[0] * bases, 
                                        num_matches=0, 
                                        max_index=bases)   
            
            # continue to add scaffolds while the header matches the read name
            while (discovered_name == expected_name):
                File_Parser.fill_in_data(self, 
                                        info_tuple[0], 
                                        data, 
                                        info_tuple[0][self.fasta_index])

                self.fasta_index += 2                  
                if self.fasta_index > len(info_tuple[0]) - 1:
                    break
                discovered_name = File_Parser.parse_fasta_read_name(self, 
                                                    info_tuple[0][self.fasta_index]) 

            #print(expected_name)
            #Data_Class.Read_Data.print_data(data)

            # add Read_Data var to dictionary  
            Reads[expected_name] = data      
            self.list_index  += 1
            
        return Reads
            

'''
Writes the read data to the respective output file
'''
class File_Writer():
    def __init__(self):
        self.output = None

    def write(self, read_dict, output_file):
        try:
            self.output = open(output_file, "w")
        except IOError:
            if self.output is None:
                crashAndBurn("Could not open " + self.output + " for reading.") 

        index = 0 
        for read in read_dict:
            # write read name
            curr = read_dict[read]
            self.output.write(">" + read)
            
            # write scaffolds
            while index < len(curr.scaffolds):
                self.output.write("  " + curr.scaffolds[index])
                self.output.write(" " + curr.scaffold_loc[index])
                index += 1
            index = 0

            # write match data
            if curr.num_matches == 0:
                self.output.write(" read length " + str(curr.max_index) + " NOT FOUND\n")
            else:
                self.output.write(" " + str(curr.num_matches) + " of ") 
                self.output.write(str(curr.max_index) + " genes match: ") 
                percent = float(curr.num_matches) / float(curr.max_index) * 100.0
                self.output.write(str(percent) + "%\n")

            # write the read itself
            for base in curr.read:
                self.output.write(base)

            self.output.write("\n")
            self.output.close


'''
main method of Concat Qual Query
'''
if __name__ == '__main__':
    if len(sys.argv) != 4:
        crashAndBurn("concatQualQuery Usage:\n\tconcatQualQuery.py <input filename> <List of reads> <output file>")

    # copy data from files to memory    
    info_tuple = File_Reader().read(sys.argv[1], sys.argv[2])
    
    # create a dictionary of that data
    try:
        read_dict = File_Parser().parse(info_tuple)
    except:
        crashAndBurn("There was an issue parsing [" + sys.argv[1] + "]")
    
    # write the updated data to the output file
    File_Writer().write(read_dict, sys.argv[3])

    exit()