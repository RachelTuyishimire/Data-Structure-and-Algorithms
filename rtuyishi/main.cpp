# include "scanpath.h"


int main () {
    std::string file_path = "input.txt";
    if (read_and_output_file(file_path)){
        std::cout << "File read and output successfully." << std::endl;
    }
    else {
        std::cerr << "Failed to process the file" << std::endl;
    }
    return 0;
    }
