#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CONSTANTS 
#define MAX_CITY_NAME 50
#define INF 999999

// DATA STRUCTURES
/**
 * Edge node in adjacency list
 * Represents a road from one city to another
 */
typedef struct Edge {
    int destCityID;         // Destination city ID
    int distance;           // Distance in kilometers
    struct Edge* next;      // Pointer to next edge in list
} Edge;

/**
 * City node
 * Contains city information and adjacency list
 */
typedef struct City {
    int cityID;                     // Unique city identifier
    char cityName[MAX_CITY_NAME];   // City name
    int x;                          // X coordinate (for visualization & A* heuristic)
    int y;                          // Y coordinate (for visualization & A* heuristic)
    Edge* adjList;                  // Head of adjacency list
} City;

/**
 * Graph structure
 * Dynamic array-based graph representation
 */
typedef struct Graph {
    City* cities;           // Dynamic array of cities
    int numCities;          // Current number of cities
    int capacity;           // Allocated capacity
} Graph;

// GRAPH OPERATIONS 
/**
 * Create a new graph with initial capacity
 * @param initialCapacity: Starting capacity for cities array
 * @return: Pointer to newly created graph, or NULL on failure
 */
Graph* createGraph(int initialCapacity);

/**
 * Free all memory associated with the graph
 * @param g: Pointer to graph to be freed
 */
void freeGraph(Graph* g);

// CITY OPERATIONS 
/**
 * Add a new city to the graph
 * @param g: Pointer to graph
 * @param cityID: Unique identifier for the city
 * @param cityName: Name of the city
 * @param x: X coordinate
 * @param y: Y coordinate
 * @return: 1 on success, 0 on failure
 */
int addCity(Graph* g, int cityID, const char* cityName, int x, int y);

/**
 * Delete a city and all its connected roads
 * @param g: Pointer to graph
 * @param cityID: ID of city to delete
 * @return: 1 on success, 0 on failure
 */
int deleteCity(Graph* g, int cityID);

/**
 * Find array index of a city given its ID
 * @param g: Pointer to graph
 * @param cityID: ID to search for
 * @return: Array index, or -1 if not found
 */
int findCityIndex(Graph* g, int cityID);

/**
 * Find city ID by name (case-sensitive)
 * @param g: Pointer to graph
 * @param cityName: Name to search for
 * @return: City ID, or -1 if not found
 */
int findCityByName(Graph* g, const char* cityName);

// ROAD OPERATIONS 
/**
 * Add a directed road (edge) between two cities
 * @param g: Pointer to graph
 * @param fromCityID: Source city ID
 * @param toCityID: Destination city ID
 * @param distance: Distance in kilometers
 * @return: 1 on success, 0 on failure
 */
int addRoad(Graph* g, int fromCityID, int toCityID, int distance);

/**
 * Remove a road between two cities
 * @param g: Pointer to graph
 * @param fromCityID: Source city ID
 * @param toCityID: Destination city ID
 * @return: 1 on success, 0 on failure
 */
int removeRoad(Graph* g, int fromCityID, int toCityID);

// ==================== DISPLAY FUNCTIONS ====================

/**
 * Display complete graph with all cities and roads
 * @param g: Pointer to graph
 */
void displayGraph(Graph* g);

/**
 * Display single city information
 * @param g: Pointer to graph
 * @param cityID: ID of city to display
 */
void displayCity(Graph* g, int cityID);

// ==================== UTILITY FUNCTIONS ====================

/**
 * Sort cities alphabetically by name (bubble sort)
 * @param g: Pointer to graph
 */
void sortCitiesByName(Graph* g);

/**
 * Get city ID at given array index
 * @param g: Pointer to graph
 * @param index: Array index
 * @return: City ID, or -1 if invalid index
 */
int getCityID(Graph* g, int index);

/**
 * Get city name at given array index
 * @param g: Pointer to graph
 * @param index: Array index
 * @return: Pointer to city name, or NULL if invalid index
 */
const char* getCityName(Graph* g, int index);

#endif // GRAPH_H