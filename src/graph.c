#include "graph.h"

// GRAPH INITIALIZATION 

/**
 * Create graph with initial capacity
 * Allocates memory for graph and city array
 */
Graph* createGraph(int initialCapacity) {
    Graph* g = (Graph*)malloc(sizeof(Graph));
    if (!g) {
        printf("Error: Memory allocation failed for graph!\n");
        return NULL;
    }
    
    g->cities = (City*)malloc(initialCapacity * sizeof(City));
    if (!g->cities) {
        printf("Error: Memory allocation failed for cities array!\n");
        free(g);
        return NULL;
    }
    
    g->numCities = 0;
    g->capacity = initialCapacity;
    
    // Initialize cities - set adjacency lists to NULL
    for (int i = 0; i < initialCapacity; i++) {
        g->cities[i].adjList = NULL;
    }
    
    return g;
}

/**
 * Free all graph memory
 * Frees adjacency lists, cities array, and graph structure
 */
void freeGraph(Graph* g) {
    if (!g) return;
    
    // Free all adjacency lists
    for (int i = 0; i < g->numCities; i++) {
        Edge* current = g->cities[i].adjList;
        while (current) {
            Edge* temp = current;
            current = current->next;
            free(temp);
        }
    }
    
    free(g->cities);
    free(g);
}

// ==================== SEARCH OPERATIONS ====================

/**
 * Find city index by ID
 * Linear search through cities array
 */
int findCityIndex(Graph* g, int cityID) {
    if (!g) return -1;
    
    for (int i = 0; i < g->numCities; i++) {
        if (g->cities[i].cityID == cityID) {
            return i;
        }
    }
    return -1;
}

/**
 * Find city by name
 * Case-sensitive string comparison
 */
int findCityByName(Graph* g, const char* cityName) {
    if (!g || !cityName) return -1;
    
    for (int i = 0; i < g->numCities; i++) {
        if (strcmp(g->cities[i].cityName, cityName) == 0) {
            return g->cities[i].cityID;
        }
    }
    return -1;
}

// ==================== CITY OPERATIONS ====================

/**
 * Add city to graph
 * Resizes array if needed
 */
int addCity(Graph* g, int cityID, const char* cityName, int x, int y) {
    if (!g || !cityName) {
        printf("Error: Invalid parameters!\n");
        return 0;
    }
    
    // Check if city already exists
    if (findCityIndex(g, cityID) != -1) {
        printf("Error: City with ID %d already exists!\n", cityID);
        return 0;
    }
    
    // Resize if needed (double capacity)
    if (g->numCities >= g->capacity) {
        g->capacity *= 2;
        City* temp = (City*)realloc(g->cities, g->capacity * sizeof(City));
        if (!temp) {
            printf("Error: Memory reallocation failed!\n");
            return 0;
        }
        g->cities = temp;
        
        // Initialize new slots
        for (int i = g->numCities; i < g->capacity; i++) {
            g->cities[i].adjList = NULL;
        }
    }
    
    // Add new city
    g->cities[g->numCities].cityID = cityID;
    strncpy(g->cities[g->numCities].cityName, cityName, MAX_CITY_NAME - 1);
    g->cities[g->numCities].cityName[MAX_CITY_NAME - 1] = '\0';
    g->cities[g->numCities].x = x;
    g->cities[g->numCities].y = y;
    g->cities[g->numCities].adjList = NULL;
    
    g->numCities++;
    printf("✓ City '%s' (ID: %d) added successfully!\n", cityName, cityID);
    return 1;
}

/**
 * Delete city from graph
 * Removes all edges pointing to/from this city
 */
int deleteCity(Graph* g, int cityID) {
    if (!g) {
        printf("Error: Invalid graph!\n");
        return 0;
    }
    
    int index = findCityIndex(g, cityID);
    if (index == -1) {
        printf("Error: City with ID %d not found!\n", cityID);
        return 0;
    }
    
    // Free adjacency list of this city
    Edge* current = g->cities[index].adjList;
    while (current) {
        Edge* temp = current;
        current = current->next;
        free(temp);
    }
    
    // Remove all edges pointing TO this city from other cities
    for (int i = 0; i < g->numCities; i++) {
        if (i == index) continue;
        
        Edge* prev = NULL;
        Edge* curr = g->cities[i].adjList;
        
        while (curr) {
            if (curr->destCityID == cityID) {
                if (prev) {
                    prev->next = curr->next;
                } else {
                    g->cities[i].adjList = curr->next;
                }
                Edge* temp = curr;
                curr = curr->next;
                free(temp);
            } else {
                prev = curr;
                curr = curr->next;
            }
        }
    }
    
    // Shift remaining cities left to fill gap
    for (int i = index; i < g->numCities - 1; i++) {
        g->cities[i] = g->cities[i + 1];
    }
    g->numCities--;
    
    printf("✓ City deleted successfully!\n");
    return 1;
}

// ==================== ROAD OPERATIONS ====================

/**
 * Add directed road (edge) between two cities
 * Updates existing edge if already present
 */
int addRoad(Graph* g, int fromCityID, int toCityID, int distance) {
    if (!g) {
        printf("Error: Invalid graph!\n");
        return 0;
    }
    
    int fromIndex = findCityIndex(g, fromCityID);
    int toIndex = findCityIndex(g, toCityID);
    
    if (fromIndex == -1 || toIndex == -1) {
        printf("Error: One or both cities not found!\n");
        return 0;
    }
    
    if (distance <= 0) {
        printf("Error: Distance must be positive!\n");
        return 0;
    }
    
    // Check if road already exists
    Edge* current = g->cities[fromIndex].adjList;
    while (current) {
        if (current->destCityID == toCityID) {
            printf("Road already exists! Updating distance from %d to %d km.\n", 
                   current->distance, distance);
            current->distance = distance;
            return 1;
        }
        current = current->next;
    }
    
    // Create new edge
    Edge* newEdge = (Edge*)malloc(sizeof(Edge));
    if (!newEdge) {
        printf("Error: Memory allocation failed for edge!\n");
        return 0;
    }
    
    newEdge->destCityID = toCityID;
    newEdge->distance = distance;
    newEdge->next = g->cities[fromIndex].adjList;
    g->cities[fromIndex].adjList = newEdge;
    
    printf("✓ Road added: %s → %s (%d km)\n", 
           g->cities[fromIndex].cityName, 
           g->cities[toIndex].cityName, 
           distance);
    return 1;
}

/**
 * Remove road between two cities
 */
int removeRoad(Graph* g, int fromCityID, int toCityID) {
    if (!g) {
        printf("Error: Invalid graph!\n");
        return 0;
    }
    
    int fromIndex = findCityIndex(g, fromCityID);
    
    if (fromIndex == -1) {
        printf("Error: Source city not found!\n");
        return 0;
    }
    
    Edge* prev = NULL;
    Edge* current = g->cities[fromIndex].adjList;
    
    while (current) {
        if (current->destCityID == toCityID) {
            if (prev) {
                prev->next = current->next;
            } else {
                g->cities[fromIndex].adjList = current->next;
            }
            free(current);
            printf("✓ Road removed successfully!\n");
            return 1;
        }
        prev = current;
        current = current->next;
    }
    
    printf("Error: Road not found!\n");
    return 0;
}

// ==================== DISPLAY FUNCTIONS ====================

/**
 * Display entire graph
 * Shows all cities and their connections
 */
void displayGraph(Graph* g) {
    if (!g) {
        printf("Error: Invalid graph!\n");
        return;
    }
    
    printf("\n╔══════════════════════════════════════════════════╗\n");
    printf("║              CITY NETWORK GRAPH                  ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");
    printf("Total Cities: %d\n\n", g->numCities);
    
    if (g->numCities == 0) {
        printf("  (No cities in the graph)\n");
        printf("════════════════════════════════════════════════════\n");
        return;
    }
    
    for (int i = 0; i < g->numCities; i++) {
        printf("┌─ City: %s (ID: %d)\n", 
               g->cities[i].cityName, 
               g->cities[i].cityID);
        printf("│  Coordinates: (%d, %d)\n",
               g->cities[i].x,
               g->cities[i].y);
        
        Edge* edge = g->cities[i].adjList;
        if (!edge) {
            printf("└─ Roads: None\n");
        } else {
            printf("└─ Roads:\n");
            while (edge) {
                int destIndex = findCityIndex(g, edge->destCityID);
                if (destIndex != -1) {
                    printf("   → %s (%d km)\n", 
                           g->cities[destIndex].cityName, 
                           edge->distance);
                }
                edge = edge->next;
            }
        }
        printf("\n");
    }
    printf("════════════════════════════════════════════════════\n");
}

/**
 * Display single city information
 */
void displayCity(Graph* g, int cityID) {
    if (!g) {
        printf("Error: Invalid graph!\n");
        return;
    }
    
    int index = findCityIndex(g, cityID);
    if (index == -1) {
        printf("Error: City not found!\n");
        return;
    }
    
    printf("\n╔══════════════════════════════════════════════════╗\n");
    printf("║              CITY INFORMATION                    ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");
    printf("City: %s (ID: %d)\n", g->cities[index].cityName, g->cities[index].cityID);
    printf("Coordinates: (%d, %d)\n", g->cities[index].x, g->cities[index].y);
    printf("\nOutgoing Roads:\n");
    
    Edge* edge = g->cities[index].adjList;
    if (!edge) {
        printf("  (No outgoing roads)\n");
    } else {
        while (edge) {
            int destIndex = findCityIndex(g, edge->destCityID);
            if (destIndex != -1) {
                printf("  → %s (%d km)\n", 
                       g->cities[destIndex].cityName, 
                       edge->distance);
            }
            edge = edge->next;
        }
    }
    printf("════════════════════════════════════════════════════\n");
}

// ==================== UTILITY FUNCTIONS ====================

/**
 * Sort cities by name (bubble sort)
 * Simple but sufficient for small datasets
 */
void sortCitiesByName(Graph* g) {
    if (!g || g->numCities <= 1) {
        printf("No sorting needed.\n");
        return;
    }
    
    for (int i = 0; i < g->numCities - 1; i++) {
        for (int j = 0; j < g->numCities - i - 1; j++) {
            if (strcmp(g->cities[j].cityName, g->cities[j + 1].cityName) > 0) {
                City temp = g->cities[j];
                g->cities[j] = g->cities[j + 1];
                g->cities[j + 1] = temp;
            }
        }
    }
    printf("✓ Cities sorted by name.\n");
}

/**
 * Get city ID at array index
 */
int getCityID(Graph* g, int index) {
    if (!g || index < 0 || index >= g->numCities)
        return -1;
    return g->cities[index].cityID;
}

/**
 * Get city name at array index
 */
const char* getCityName(Graph* g, int index) {
    if (!g || index < 0 || index >= g->numCities)
        return NULL;
    return g->cities[index].cityName;
}