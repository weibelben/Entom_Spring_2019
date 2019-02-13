

UNKNOWN_BASE = '?'

# Error message printer and function exit handler 
def crashAndBurn(message):
        print("Python Script ERROR: <Data_Class.py>")
        print(message)
        input("Press any key to continue...")
        exit()

'''
The Read Data class tracks the qualities of each read
'''
class Read_Data:
    def __init__(self, read, qualities, scaffolds, 
                    scaffold_locations, num_matches, max_index):
        self.read           = read
        self.qualities      = qualities
        self.scaffolds      = scaffolds
        self.scaffold_loc   = scaffold_locations
        self.num_matches    = num_matches
        self.max_index      = max_index

    # updates the read and qualities lists based on the quality of the input data
    def update_read(self, scaffold, start, end, qual, line_number, file_name):
        if start > end:
            Read_Data.update_inverted_read(self, scaffold, start, end, qual, line_number, file_name)

        if end > len(self.read) or start < 1:    
            crashAndBurn("Line " + str(line_number) + " of " + file_name 
                + "contained a scaffold that did not fit in its read: [" + str(start) 
                + ":" + str(end) + "]\n\tRead length = " + str(len(self.read) + 1))

        index = start - 1
        while index < end:
            if index - start + 1 > len(scaffold) - 1:
                break

            if self.qualities[index] <= qual:
                if self.qualities[index] == 0:
                    self.num_matches += 1

                base = scaffold[index - start + 1]
                if base != 'A' and base != 'T' and base != 'C' and base != 'G':
                    self.read[index] = UNKNOWN_BASE
                    self.qualities[index] = 0
                else:
                    self.read[index] = base
                    self.qualities[index] = qual

            index += 1

    # flips the read and insertes it, switching A<->T and C<->G
    def update_inverted_read(self, scaffold, start, end, qual, line_number, file_name):
        if start > len(self.read) or end < 1:    
            crashAndBurn("Line " + str(line_number) + " of " + file_name 
                + " contained a scaffold that did not fit in its read: [" + str(end) 
                + ":" + str(start) + "]\n\tRead length = " + str(len(self.read) + 1))

        index = end - 1
        while index < start:            
            if index - end + 1 > len(scaffold) - 1:
                break

            if self.qualities[index] <= qual:
                if self.qualities[index] == 0:
                    self.num_matches += 1

                if scaffold[index - end + 1] == 'A':
                    self.read[index] = 'T'
                elif scaffold[index - end + 1] == 'T':
                    self.read[index] = 'A'
                elif scaffold[index - end + 1] == 'G':
                    self.read[index] = 'C'
                elif scaffold[index - end + 1] == 'C':
                    self.read[index] = 'G'
                #else:
                    ########################## DECIDED to ignore inconcistencies in data and move on ##########################
                    #print(scaffold)
                    #crashAndBurn("Base at index " + str(index - end + 1) + "[" + scaffold[index - end + 1] +
                    #    "] of line " + str(line_number + 1) + " was not recognized")

                self.qualities[index] = qual
            index += 1

    # adds a scaffold to the list of scaffold names
    def add_scaffold(self, scaffold_name, scaffold_indices):
        self.scaffolds.append(scaffold_name)
        self.scaffold_loc.append(scaffold_indices)

    # prints most member data
    def print_data(self):
        print(self.read)
        print(self.scaffolds)
        print(self.scaffold_loc)
        print(str(self.num_matches) + " / " + str(self.max_index) + " matches")

'''
The Phy Data class tracks the species and reads of each read
'''
class Phy_Data:
    def __init__(self, read_length, number_of_species, gene_dictionary):
        self.length = read_length
        self.num_species = number_of_species
        self.gene_dict = gene_dictionary

    def add_species(self, species, read):
        self.num_species += 1
        self.gene_dict.update({species : read})

    def print_data(self):
        print("\t" + str(self.length) + "  " + str(self.num_species) + "\n\t" + str(self.gene_dict))