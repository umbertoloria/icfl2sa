import sys
from random import choice

genome_alphabet=["A","G","T","C"]
from string import ascii_uppercase



if __name__=="__main__":
    if len(sys.argv) < 2:
        print("Specificare la taglia con un paramentro es. python3 python_string_generator N")
        raise(Exception)
    
    num_of_char=int(sys.argv[1])

    with open("./input_file.txt","w") as in_file:
        in_file.write(''.join(choice(genome_alphabet) for _ in range(num_of_char))) 