# ifndef SCANPATH_H
# define SCANPATH_H

# include <iostream>
# include <string>
// # include <set>
// # include <tuple>

// struct Point {
//     int x, y, duration, timestamp;
//     bool operator < (const Point & p) const {
//         return std::tie(x, y, duration, timestamp) < std::tie(p.x, p.y, p.duration, p.timestamp);
//     }
// };

bool read_and_output_file (const std::string & file_path);
// int store_point (int x, int y, int duration, int timestamp);


# endif