#ifndef FILEIO_H
#define FILEIO_H

#include "graph.h"

// FILE PATH CONSTANTS 
#define CITIES_FILE "cities.txt"
#define ROADS_FILE "roads.txt"
#define LOGS_FILE "logs.txt"

// FILE I/O OPERATIONS 

/**
 * Load graph from CSV files
 * Reads cities.txt and roads.txt to populate graph
 * @param g: Pointer to graph
 * @param citiesFile: Path to cities file
 * @param roadsFile: Path to roads file
 * @return: 1 on success, 0 on failure
 */
int loadGraphFromFiles(Graph* g, const char* citiesFile, const char* roadsFile);

/**
 * Save graph to CSV files
 * Writes current graph state to cities.txt and roads.txt
 * @param g: Pointer to graph
 * @param citiesFile: Path to cities file
 * @param roadsFile: Path to roads file
 * @return: 1 on success, 0 on failure
 */
int saveGraphToFiles(Graph* g, const char* citiesFile, const char* roadsFile);

// LOGGING OPERATIONS 
/**
 * Log a general operation to logs.txt
 * Includes timestamp
 * @param operation: Description of operation
 */
void logOperation(const char* operation);

/**
 * Log a path query to logs.txt
 * Records source, destination, and distance
 * @param source: Source city name
 * @param dest: Destination city name
 * @param distance: Total distance
 */
void logPathQuery(const char* source, const char* dest, int distance);

// UTILITY FUNCTIONS 

/**
 * Get current timestamp in formatted string
 * Format: [YYYY-MM-DD HH:MM:SS]
 * @param buffer: Buffer to store timestamp
 * @param size: Size of buffer
 */
void getCurrentTimestamp(char* buffer, int size);

#endif // FILEIO_H