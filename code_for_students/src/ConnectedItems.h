#ifndef CONNECTEDITEMS_H
#define CONNECTEDITEMS_H

#include <iostream>
#include <fstream>
#include <limits.h>
#include <math.h>
#include <unistd.h>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <string.h>
#include "./util/GetMemUsage.h"
#include "./util/LogManager.h"

class ConnectedItems {
public:
/**
	 *
	 *
	 * @param inputFilePath Path of the input file.
	 * @param outputFilePath Path of the output file.
	 * @param topN Number of
	 *
	 * If the input file cannot be read throw an error of type ios_base::failure
	 * If the output file cannot be generated, then throw an error of type ios_base::failure
	 */
	 
    static void getConnectedItems(char* inputFilePath, char* outputFilePath);
};

#endif /* CONNECTEDITEMS_H */