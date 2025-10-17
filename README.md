Diego Delgado, 008320631

All parts of this project belong to me. I had troubles running my code on the blue server and asked ChatGPT a question 
about it and realized in doing so that obviously my code won't update on the server if I only update it on clion and not
in blue. 

I implemented a scanner class that uses a txt input file to create a tokens output file that contains specialized tokens.
The constructor initializes the scanner object with inputPath in inputPath_. Tokenize reads words from the file into a 
a vector that will be used in the overloaded tokenize. The overloaded tokenize reads and writes the 'words' to an output
file. readWord reads the next token from the input stream, creating the tokens.

When testing my code I used the test prompts that were given. When using g++ -std=c++20 -Wall *.cpp -o huffman_part1 
followed by ./huffman_part1 input_output/for_the_love_of_a_man.txt it worked well. It also worked well when using the 
bash compile_and_test.bash for_the_love_of_a_man.
