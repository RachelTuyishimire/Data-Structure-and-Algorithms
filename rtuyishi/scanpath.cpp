# include "scanpath.h"
# include <fstream>
# include <iostream>

std::set<Point> points;
int unique_id = 1;


// int store_point (int x, int y, int duration, long timestamp) {
//     Point p = {x, y, duration, timestamp};

//     if (points.find(p) == points.end()){
//         points.insert (p);
//         std::cout << "Store point:" << unique_id << " " << x << " " << y << " " << duration << " " << timestamp << std::endl;
//         return unique_id++;
//     }
//     return -1;
// }


bool read_and_output_file (const std::string & file_path) {
    std:: ifstream file (file_path, std::ios::in);
    if (!file.is_open()){
        
    }
    // std::ifstream input_file (file_path);
    // std::ofstream output_file ("output.txt");

    // if (!input_file || !output_file){
    //     std::cerr << "Error opening file" << std::endl;
    //     return false;
    // }


    // int x, y, duration, timestamp;
    // while (input_file >> x >> y >> duration >> timestamp){
    //     if ( x == -1 && y == -1 && duration == -1 && timestamp == -1)
    //     break; 

    // int id = store_point ( x, y, duration, timestamp);
    // if ( id != -1){
    //     output_file << id << " " << x << " " << y << " " << duration << " " << timestamp << std::endl;    
    //     }
    // }

    // output_file.close();
    // input_file.close();
    // return true;
}