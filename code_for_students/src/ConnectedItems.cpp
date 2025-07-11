#include "ConnectedItems.h"

/*
 * Author: rtuyishi
 * Date: April 24, 2025
 *
 * Summary:
 * This program reads a file of location pairs with a connection flag, builds an undirected graph, 
 * and groups connected locations using Depth-First Search (DFS).
 *
 * Each group gets a number: 1 for the largest group, 2 for the next largest, and so on. 
 * Ties are broken by the smallest x and y coordinates in the group.
 * 
 * The output lists all directly connected pairs from the input, along with their group number,
 * sorted by group and coordinates.
 *
 * Input:
 * - A file with lines like: (x1, y1, x2, y2, flag)
 * - Only lines where flag == 1 are processed
 * - Example: (2284, 510, 2284, 515, 1) means a direct connection between two points
 *
 * Output:
 * - A file with lines like: (x1, y1, x2, y2, group_number)
 * - Sorted by group, then by coordinates
 *
 * Approach:
 * 1. Parse the input and build a graph using valid edges
 * 2. Use DFS to find connected groups of locations
 * 3. Sort groups by size (desc), then by lowest coordinate
 * 4. Sort each group’s edges and write to the output with group numbers
 *
 * Performance:
 * - Time: O(V + E log E), where V = locations, E = edges
 * - Space: O(V + E)
 */




struct Point {
	int x, y;
	
	// Initializing point with default coordinates (0,0)
	Point() : x(0), y(0) {}
	// Creating point with specified coordinates
	Point(int _x, int _y) : x(_x), y(_y) {}
	
	// Comparing points based on x coordinate first, then y
	bool operator<(const Point& other) const {
		if (x != other.x) return x < other.x;
		return y < other.y;
	}
	
	// Checking if two points have identical coordinates
	bool operator==(const Point& other) const {
		return x == other.x && y == other.y;
	}
};


// Representing a connection between two points with a group identifier
struct Connection {
	Point p1, p2;
	int groupId;
	
	// Initializing with default group ID
	Connection() : groupId(0) {}

	// Creating a connection by ensuring points are ordered (smaller point first)
	Connection(const Point& a, const Point& b, int id = 0) : groupId(id) {
	   
		// Ordering points to maintain consistency
		if (a < b) {
			p1 = a;
			p2 = b;
		} else {
			p1 = b;
			p2 = a;
		}
	}
	
	// Defining comparison operator for sorting connections:
	bool operator<(const Connection& other) const {
		if (groupId != other.groupId) return groupId < other.groupId;
		if (p1.x != other.p1.x) return p1.x < other.p1.x;
		if (p1.y != other.p1.y) return p1.y < other.p1.y;
		if (p2.x != other.p2.x) return p2.x < other.p2.x;
		return p2.y < other.p2.y;
	}
};

// Union-Find data structure for efficient connected components
class DisjointSet {
private:

	std::vector<int> parent;
	std::vector<int> rank;
	std::vector<int> size;
	
public:
	// Initializing disjoint set with n elements
	DisjointSet(int n) {
		parent.resize(n);
		rank.resize(n, 0);
		size.resize(n, 1);
		// Making each element its own parent initially
		for (int i = 0; i < n; i++) {
			parent[i] = i;
		}
	}
	
	// Finding the root of element x with path compression
	int find(int x) {
		if (parent[x] != x) {
			parent[x] = find(parent[x]); 
		}
		return parent[x];
	}
	
	// Merging two sets containing elements x and y
	void unionSets(int x, int y) {
		int rootX = find(x);
		int rootY = find(y);
		
		if (rootX == rootY) return;
		
		// Implementing union by rank for better performance
		if (rank[rootX] < rank[rootY]) {
			parent[rootX] = rootY;
			size[rootY] += size[rootX];
		} else {
			parent[rootY] = rootX;
			size[rootX] += size[rootY];
			// Increasing rank when merging trees of equal height
			if (rank[rootX] == rank[rootY]) {
				rank[rootX]++;
			}
		}
	}
	// Getting size of the set containing element x
	int getSize(int x) {
		return size[find(x)];
	}
};

void ConnectedItems::getConnectedItems(char* inputFilePath, char* outputFilePath) {
	// Opening input file stream
	FILE* inFileStream = fopen(inputFilePath, "r");
	if (!inFileStream) {
		char message[1024];
		sprintf(message, "Cannot open input file for reading: %s", inputFilePath);
		throw std::ios_base::failure(message);
	}

	// Opening output file stream
	FILE* outFileStream = fopen(outputFilePath, "w");
	if (!outFileStream) {
		char message[1024];
		sprintf(message, "Cannot open output file for writing: %s", outputFilePath);
		throw std::ios_base::failure(message);
	}

	// Logging start of processing
	LogManager::writePrintfToLog(LogManager::Level::Status,
			"ConnectedItems::getConnectedItems",
			"Starting to process file %s", inputFilePath);

	// Reading connections and mapping points to indices
	std::vector<Connection> allConnections;
	std::map<Point, int> pointToIndex;
	std::vector<Point> points;
	
	int x1, y1, x2, y2, flag;
	char line[100];
	
	// Processing each line from input file
	while (fgets(line, sizeof(line), inFileStream)) {
		if (sscanf(line, "(%d, %d, %d, %d, %d)", &x1, &y1, &x2, &y2, &flag) == 5) {
			if (flag == 1) {
				Point p1(x1, y1);
				Point p2(x2, y2);
				
				// Adding new points to tracking structures
				if (pointToIndex.find(p1) == pointToIndex.end()) {
					pointToIndex[p1] = points.size();
					points.push_back(p1);
				}
				
				if (pointToIndex.find(p2) == pointToIndex.end()) {
					pointToIndex[p2] = points.size();
					points.push_back(p2);
				}
				
				// Storing valid connection
				allConnections.push_back(Connection(p1, p2));
			}
		}
	}
	
	// Building connected components using Union-Find
	int numPoints = points.size();
	DisjointSet ds(numPoints);
	
	// Connecting points using Union-Find
	for (const Connection& conn : allConnections) {
		int idx1 = pointToIndex[conn.p1];
		int idx2 = pointToIndex[conn.p2];
		ds.unionSets(idx1, idx2);
	}
	
	// Grouping connections by component
	std::map<int, std::vector<Connection>> componentConnections;
	std::map<int, int> componentSize;
	std::map<int, Point> componentMinPoint;
	
	// Initializing component tracking
	for (int i = 0; i < numPoints; i++) {
		int root = ds.find(i);
		if (componentMinPoint.find(root) == componentMinPoint.end()) {
			componentMinPoint[root] = Point(INT_MAX, INT_MAX);
		}
		if (points[i] < componentMinPoint[root]) {
			componentMinPoint[root] = points[i];
		}
		componentSize[root] = ds.getSize(root);
	}
	
	// Organizing connections by component
	for (const Connection& conn : allConnections) {
		int idx1 = pointToIndex[conn.p1];
		int root = ds.find(idx1);
		componentConnections[root].push_back(conn);
	}
	
	// Preparing components for sorting
	std::vector<std::pair<int, int>> components;
	for (const auto& entry : componentSize) {
		components.push_back({entry.first, entry.second});
	}
	
	// Sorting components by size and minimum point
	std::sort(components.begin(), components.end(), [&](const auto& a, const auto& b) {
		if (a.second != b.second) return a.second > b.second;
		return componentMinPoint[a.first] < componentMinPoint[b.first];
	});
	
	// Writing output header
	fprintf(outFileStream, "Format: (x1, y1, x2, y2, group number)\n");
	
	// Writing sorted connections to output
	for (size_t i = 0; i < components.size(); i++) {
		int root = components[i].first;
		int groupId = i + 1;
		
		// Sorting connections within component
		std::vector<Connection> sortedConnections = componentConnections[root];
		for (auto& conn : sortedConnections) {
			conn.groupId = groupId;
		}
		
		std::sort(sortedConnections.begin(), sortedConnections.end());
		
		// Writing connection data
		for (const Connection& conn : sortedConnections) {
			fprintf(outFileStream, "(%d, %d, %d, %d, %d)\n", 
				   conn.p1.x, conn.p1.y, conn.p2.x, conn.p2.y, conn.groupId);
		}
	}
	
	// Closing file streams
	fclose(inFileStream);
	fclose(outFileStream);

	// Logging completion
	LogManager::writePrintfToLog(LogManager::Level::Status,
			"ConnectedItems::getConnectedItems",
			"Finished processing file %s", inputFilePath);
}