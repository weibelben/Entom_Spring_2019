###############################################################################
    ##### ExtraHop Coding Challenge Problem ##### 1/16/2019 #####
    # @author Ben Weibel -- (425) 260-3067 -- weibelben@gmail.com
###############################################################################

import sys

# total of 8 indices in the square grid
MAX_INDEX = 7 
MIN_INDEX = 0

# using the procedure defined in the spec, this method returns 1 if the word
# is discovered in the grid, 0 otherwise
def grid_contains_word(grid, word):
    # track indices in the grid
    x = 0
    y = 0

    # flip flag to 1 if the word is found
    exists = 0

    # iterate through each letter
    for row in grid:
        for letter in row:
            if word[0] == letter and not exists:
                exists = find_word_in_grid(grid, word[1:], x, y)            
            x += 1
        x = 0
        y += 1
    
    return exists

# recursively explores the 8 potential knight moves for the next letter
def find_word_in_grid(grid, word, x, y):
    if len(word) == 0:
        return 1

    # flip flag to 1 if the word is found
    exists = 0

    # left 2, up 1
    if x - 2 >= MIN_INDEX and y - 1 >= MIN_INDEX:
        if word[0] == grid[y-1][x-2]:
            exists = find_word_in_grid(grid, word[1:], x - 2, y - 1)

    # left 2, down 1
    if x - 2 >= MIN_INDEX and y + 1 <= MAX_INDEX and not exists:
        if word[0] == grid[y+1][x-2]:
            exists = find_word_in_grid(grid, word[1:], x - 2, y + 1)

    # left 1, up 2
    if x - 1 >= MIN_INDEX and y - 2 >= MIN_INDEX and not exists:
        if word[0] == grid[y-2][x-1]:
            exists = find_word_in_grid(grid, word[1:], x - 1, y - 2)

    # left 1, down 2
    if x - 1 >= MIN_INDEX and y + 2 <= MAX_INDEX and not exists:
        if word[0] == grid[y+2][x-1]:
            exists = find_word_in_grid(grid, word[1:], x - 1, y + 2)

    # right 1, up 2
    if x + 1 <= MAX_INDEX and y - 2 >= MIN_INDEX and not exists:
        if word[0] == grid[y-2][x+1]:
            exists = find_word_in_grid(grid, word[1:], x + 1, y - 2)

    # right 1, down 2
    if x + 1 <= MAX_INDEX and y + 2 <= MAX_INDEX and not exists:
        if word[0] == grid[y+2][x+1]:
            exists = find_word_in_grid(grid, word[1:], x + 1, y + 2)

    # right 2, up 1
    if x + 2 <= MAX_INDEX and y - 1 >= MIN_INDEX and not exists:
        if word[0] == grid[y-1][x+2]:            
            exists = find_word_in_grid(grid, word[1:], x + 2, y - 1)

    # right 2, down 1
    if x + 2 <= MAX_INDEX and y + 1 <= MAX_INDEX and not exists:
        if word[0] == grid[y+1][x+2]:
            exists = find_word_in_grid(grid, word[1:], x + 2, y + 1)   

    return exists


# this method assumes that the grid has uppercase letters only
# it finds the longest word in the list that is found in the grid 
def find_longest_word(word_list, grid):
    longest_word = ""

    for word in word_list:
        # ignore words in list shorter than the current longest word
        if len(word) > len(longest_word):
            if grid_contains_word(grid, str.upper(word)):
                longest_word = word            

    return longest_word


# reads through a file, extracting all words into a map(or dictionary in py)
# tokenizing ignores all commas, newlines, and separators
def tokenize_input_file(file_handle):
    strings = dict()
    string = ""   

    with open(file_handle) as f:
        for line in f:
            # convert to upper case, remove apostrophes
            line = str.upper(line.replace("'", ""))
            
            for char in line:                
                if 'A' <= char <= 'Z':
                    string += char
                else:
                    if string is not None:
                        strings.update({string: string})
                        string = ""

    return strings



# the main method calls find_longest_word, prints the result, waits for input
# it is assumed that the grid is ALWAYS uppercase
if __name__ == '__main__':
    if len(sys.argv) < 2:
        # this is the example given in the problem
        list_of_words = {"algol", "fortran", "simula"}

        example_grid1 = [
            ['Q', 'W', 'E', 'R', 'T', 'N', 'U', 'I'],
            ['O', 'P', 'A', 'A', 'D', 'F', 'G', 'H'],
            ['T', 'K', 'L', 'Z', 'X', 'C', 'V', 'B'], 
            ['N', 'M', 'R', 'W', 'F', 'R', 'T', 'Y'],
            ['U', 'I', 'O', 'P', 'A', 'S', 'D', 'F'], 
            ['G', 'H', 'J', 'O', 'L', 'Z', 'X', 'C'], 
            ['V', 'B', 'N', 'M', 'Q', 'W', 'E', 'R'], 
            ['T', 'Y', 'U', 'I', 'O', 'P', 'A', 'S']]

        longest_word = find_longest_word(list_of_words, example_grid1)
        print(longest_word)
        input("Press enter to continue...")
        
    else:
        # use the file shakespear.txt which was coppied from the given url
        list_of_words = tokenize_input_file(sys.argv[1])

        grid = [
            ['E', 'X', 'T', 'R', 'A', 'H', 'O', 'P'],
            ['N', 'E', 'T', 'W', 'O', 'R', 'K', 'S'],
            ['Q', 'I', 'H', 'A', 'C', 'I', 'Q', 'T'], 
            ['L', 'F', 'U', 'N', 'U', 'R', 'X', 'B'],
            ['B', 'W', 'D', 'I', 'L', 'A', 'T', 'V'], 
            ['O', 'S', 'S', 'Y', 'N', 'A', 'C', 'K'], 
            ['Q', 'W', 'O', 'P', 'M', 'T', 'C', 'P'], 
            ['K', 'I', 'P', 'A', 'C', 'K', 'E', 'T']]

        longest_word = find_longest_word(list_of_words, grid)
        print(longest_word)
        input("Press enter to continue...")