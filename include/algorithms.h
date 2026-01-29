#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "graph.h"

//MIN-HEAP DATA STRUCTURES

/**
 * Heap node for priority queue
 * Used in Dijkstra's and A* algorithms
 */
typedef struct HeapNode {
    int cityID;         // City identifier
    int distance;       // Distance from source (g-score)
    int fScore;         // Total score for A* (f = g + h)
} HeapNode;

/**
 * Min-Heap structure
 * Priority queue for efficient pathfinding
 */
typedef struct MinHeap {
    HeapNode* nodes;    // Array of heap nodes
    int size;           // Current number of elements
    int capacity;       // Maximum capacity
    int* pos;           // Position array for decrease-key operation
} MinHeap;

//MIN-HEAP OPERATIONS

/**
 * Create a new min-heap
 * @param capacity: Maximum number of elements
 * @return: Pointer to new heap, or NULL on failure
 */
MinHeap* createMinHeap(int capacity);

/**
 * Free min-heap memory
 * @param h: Pointer to heap
 */
void freeMinHeap(MinHeap* h);

/**
 * Insert a node into the heap
 * @param h: Pointer to heap
 * @param cityID: City identifier
 * @param distance: Distance value
 * @param fScore: F-score for A* (use same as distance for Dijkstra)
 */
void insertHeap(MinHeap* h, int cityID, int distance, int fScore);

/**
 * Extract minimum element from heap
 * @param h: Pointer to heap
 * @return: HeapNode with minimum fScore
 */
HeapNode extractMin(MinHeap* h);

/**
 * Decrease key value for a city in heap
 * @param h: Pointer to heap
 * @param cityID: City whose value to decrease
 * @param newDist: New distance value
 * @param newFScore: New f-score value
 */
void decreaseKey(MinHeap* h, int cityID, int newDist, int newFScore);

/**
 * Check if heap is empty
 * @param h: Pointer to heap
 * @return: 1 if empty, 0 otherwise
 */
int isHeapEmpty(MinHeap* h);

//  PATH RESULT STRUCTURE 
/**
 * Structure to store path result
 * Contains the path and total distance
 */
typedef struct PathResult {
    int* path;              // Array of city IDs in path order
    int pathLength;         // Number of cities in path
    int totalDistance;      // Total distance of path
    int pathCapacity;       // Allocated capacity for path array
} PathResult;

/**
 * Create a new PathResult structure
 * @param capacity: Initial capacity
 * @return: Pointer to PathResult, or NULL on failure
 */
PathResult* createPathResult(int capacity);

/**
 * Free PathResult memory
 * @param pr: Pointer to PathResult
 */
void freePathResult(PathResult* pr);

/**
 * Add a city to the path
 * @param pr: Pointer to PathResult
 * @param cityID: City ID to add
 */
void addToPath(PathResult* pr, int cityID);

/**
 * Reverse the path array
 * Used to convert parent-traversal to source-to-destination order
 * @param pr: Pointer to PathResult
 */
void reversePath(PathResult* pr);

// GRAPH TRAVERSAL ALGORITHMS 
/**
 * Breadth-First Search traversal
 * Explores graph level by level
 * @param g: Pointer to graph
 * @param startCityID: Starting city ID
 */
void BFS(Graph* g, int startCityID);

/**
 * Depth-First Search traversal
 * Explores graph depth-wise using recursion
 * @param g: Pointer to graph
 * @param startCityID: Starting city ID
 */
void DFS(Graph* g, int startCityID);

/**
 * DFS utility function (recursive helper)
 * @param g: Pointer to graph
 * @param cityIndex: Current city index in array
 * @param visited: Array tracking visited cities
 */
void DFSUtil(Graph* g, int cityIndex, int* visited);

// SHORTEST PATH ALGORITHMS 

/**
 * Dijkstra's shortest path algorithm
 * Finds shortest path based on actual distances
 * @param g: Pointer to graph
 * @param sourceCityID: Source city ID
 * @param destCityID: Destination city ID
 * @return: PathResult with shortest path, or NULL on failure
 */
PathResult* dijkstra(Graph* g, int sourceCityID, int destCityID);

/**
 * A* shortest path algorithm
 * Uses heuristic (Euclidean distance) for faster pathfinding
 * @param g: Pointer to graph
 * @param sourceCityID: Source city ID
 * @param destCityID: Destination city ID
 * @return: PathResult with shortest path, or NULL on failure
 */
PathResult* astar(Graph* g, int sourceCityID, int destCityID);

/**
 * Heuristic function for A* algorithm
 * Calculates Euclidean distance between two cities
 * @param g: Pointer to graph
 * @param cityIndex1: First city index
 * @param cityIndex2: Second city index
 * @return: Estimated distance (integer)
 */
int heuristic(Graph* g, int cityIndex1, int cityIndex2);

//  DISPLAY FUNCTIONS

/**
 * Display path information
 * Shows path sequence and total distance
 * @param g: Pointer to graph
 * @param pr: Pointer to PathResult
 */
void displayPath(Graph* g, PathResult* pr);

#endif