#include "graph.h"
#include "algorithms.h"
#include "fileio.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Function prototypes
void displayMainMenu();
void handleInsertCity(Graph* g);
void handleDeleteCity(Graph* g);
void handleAddRoad(Graph* g);
void handleRemoveRoad(Graph* g);
void handleFastNavigation(Graph* g);
void handleAnalysisMode(Graph* g);
void handleSearchCity(Graph* g);
void clearScreen();
void pause();
void clearInputBuffer();

// ==================== MAIN FUNCTION ====================

int main() {
    Graph* cityGraph = createGraph(50);
    
    if (!cityGraph) {
        printf("Error: Failed to create graph!\n");
        return 1;
    }
    
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║   CITY NAVIGATION SYSTEM - C BACKEND v1.0        ║\n");
    printf("║   (Use Python GUI for visualization)             ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");
    
    printf("Loading graph from files...\n\n");
    
    if (!loadGraphFromFiles(cityGraph, CITIES_FILE, ROADS_FILE)) {
        printf("⚠️  Warning: Could not load graph from files.\n");
        printf("Starting with empty graph.\n\n");
    }
    
    int choice;
    int running = 1;
    
    while (running) {
        displayMainMenu();
        printf("\nEnter choice: ");
        
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("\n❌ Invalid input! Please enter a number.\n");
            pause();
            continue;
        }
        clearInputBuffer();
        
        switch (choice) {
            case 1:
                handleInsertCity(cityGraph);
                break;
            case 2:
                handleDeleteCity(cityGraph);
                break;
            case 3:
                handleAddRoad(cityGraph);
                break;
            case 4:
                handleRemoveRoad(cityGraph);
                break;
            case 5:
                displayGraph(cityGraph);
                break;
            case 6:
                handleFastNavigation(cityGraph);
                break;
            case 7:
                handleAnalysisMode(cityGraph);
                break;
            case 8:
                handleSearchCity(cityGraph);
                break;
            case 9:
                sortCitiesByName(cityGraph);
                displayGraph(cityGraph);
                break;
            case 10:
                saveGraphToFiles(cityGraph, CITIES_FILE, ROADS_FILE);
                break;
            case 11:
                printf("\n╔══════════════════════════════════════════════════╗\n");
                printf("║          HOW TO USE PYTHON GUI                   ║\n");
                printf("╚══════════════════════════════════════════════════╝\n\n");
                printf("1. Open terminal in project directory\n");
                printf("2. Navigate to data folder: cd data\n");
                printf("3. Run: python ../frontend/city_nav_gui.py\n\n");
                printf("The GUI provides:\n");
                printf("  • Beautiful graph visualization\n");
                printf("  • Interactive pathfinding\n");
                printf("  • Real-time updates\n");
                printf("  • Much better user experience!\n\n");
                break;
            case 12:
                printf("\n✅ Saving graph before exit...\n");
                saveGraphToFiles(cityGraph, CITIES_FILE, ROADS_FILE);
                printf("Goodbye! 👋\n\n");
                freeGraph(cityGraph);
                running = 0;
                break;
            default:
                printf("\n❌ Invalid choice! Please enter 1-12.\n");
        }
        
        if (running) {
            pause();
        }
    }
    
    return 0;
}

// ==================== MENU DISPLAY ====================

void displayMainMenu() {
    clearScreen();
    printf("\n");
    printf("╔════════════════════════════════════════════════╗\n");
    printf("║      CITY NAVIGATION SYSTEM - MAIN MENU        ║\n");
    printf("╠════════════════════════════════════════════════╣\n");
    printf("║  CITY OPERATIONS                               ║\n");
    printf("║  1. ➕ Insert City                             ║\n");
    printf("║  2. 🗑️  Delete City                            ║\n");
    printf("║                                                ║\n");
    printf("║  ROAD OPERATIONS                               ║\n");
    printf("║  3. 🛣️  Add Road                               ║\n");
    printf("║  4. ❌ Remove Road                             ║\n");
    printf("║                                                ║\n");
    printf("║  ALGORITHMS                                    ║\n");
    printf("║  5. 📊 Display Graph (Text View)               ║\n");
    printf("║  6. 🔍 Find Shortest Path (Dijkstra/A*)       ║\n");
    printf("║  7. 🌲 Graph Traversal (BFS/DFS)              ║\n");
    printf("║                                                ║\n");
    printf("║  UTILITIES                                     ║\n");
    printf("║  8. 🔎 Search City by Name                     ║\n");
    printf("║  9. 📝 Sort Cities by Name                     ║\n");
    printf("║ 10. 💾 Save Graph to Files                     ║\n");
    printf("║                                                ║\n");
    printf("║ 11. 🎨 Python GUI Instructions                 ║\n");
    printf("║ 12. 🚪 Exit                                     ║\n");
    printf("╚════════════════════════════════════════════════╝\n");
}

// ==================== HANDLER FUNCTIONS ====================

void handleInsertCity(Graph* g) {
    int cityID, x, y;
    char cityName[MAX_CITY_NAME];
    
    printf("\n╔══════════════════════════════════════════════════╗\n");
    printf("║              INSERT NEW CITY                     ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");
    
    printf("Enter City ID: ");
    if (scanf("%d", &cityID) != 1) {
        clearInputBuffer();
        printf("❌ Invalid input!\n");
        return;
    }
    clearInputBuffer();
    
    printf("Enter City Name: ");
    fgets(cityName, MAX_CITY_NAME, stdin);
    cityName[strcspn(cityName, "\n")] = 0;
    
    printf("Enter X coordinate (0-640): ");
    if (scanf("%d", &x) != 1) {
        clearInputBuffer();
        printf("❌ Invalid input!\n");
        return;
    }
    
    printf("Enter Y coordinate (0-480): ");
    if (scanf("%d", &y) != 1) {
        clearInputBuffer();
        printf("❌ Invalid input!\n");
        return;
    }
    clearInputBuffer();
    
    if (addCity(g, cityID, cityName, x, y)) {
        char logMsg[128];
        sprintf(logMsg, "City '%s' added (ID: %d)", cityName, cityID);
        logOperation(logMsg);
        printf("\n💡 TIP: Run Python GUI to see the new city visualized!\n");
    }
}

void handleDeleteCity(Graph* g) {
    int cityID;
    
    printf("\n╔══════════════════════════════════════════════════╗\n");
    printf("║              DELETE CITY                         ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");
    
    printf("Enter City ID to delete: ");
    if (scanf("%d", &cityID) != 1) {
        clearInputBuffer();
        printf("❌ Invalid input!\n");
        return;
    }
    clearInputBuffer();
    
    int index = findCityIndex(g, cityID);
    if (index != -1) {
        char cityName[MAX_CITY_NAME];
        strcpy(cityName, g->cities[index].cityName);
        
        if (deleteCity(g, cityID)) {
            char logMsg[128];
            sprintf(logMsg, "City '%s' (ID: %d) deleted", cityName, cityID);
            logOperation(logMsg);
        }
    }
}

void handleAddRoad(Graph* g) {
    int fromID, toID, distance;
    
    printf("\n╔══════════════════════════════════════════════════╗\n");
    printf("║              ADD ROAD                            ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");
    
    printf("Enter From City ID: ");
    if (scanf("%d", &fromID) != 1) {
        clearInputBuffer();
        printf("❌ Invalid input!\n");
        return;
    }
    
    printf("Enter To City ID: ");
    if (scanf("%d", &toID) != 1) {
        clearInputBuffer();
        printf("❌ Invalid input!\n");
        return;
    }
    
    printf("Enter Distance (km): ");
    if (scanf("%d", &distance) != 1) {
        clearInputBuffer();
        printf("❌ Invalid input!\n");
        return;
    }
    clearInputBuffer();
    
    if (addRoad(g, fromID, toID, distance)) {
        char logMsg[128];
        int fromIdx = findCityIndex(g, fromID);
        int toIdx = findCityIndex(g, toID);
        if (fromIdx != -1 && toIdx != -1) {
            sprintf(logMsg, "Road added: %s -> %s (%d km)", 
                    g->cities[fromIdx].cityName,
                    g->cities[toIdx].cityName,
                    distance);
            logOperation(logMsg);
        }
    }
}

void handleRemoveRoad(Graph* g) {
    int fromID, toID;
    
    printf("\n╔══════════════════════════════════════════════════╗\n");
    printf("║              REMOVE ROAD                         ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");
    
    printf("Enter From City ID: ");
    if (scanf("%d", &fromID) != 1) {
        clearInputBuffer();
        printf("❌ Invalid input!\n");
        return;
    }
    
    printf("Enter To City ID: ");
    if (scanf("%d", &toID) != 1) {
        clearInputBuffer();
        printf("❌ Invalid input!\n");
        return;
    }
    clearInputBuffer();
    
    if (removeRoad(g, fromID, toID)) {
        char logMsg[128];
        sprintf(logMsg, "Road removed: %d -> %d", fromID, toID);
        logOperation(logMsg);
    }
}

void handleFastNavigation(Graph* g) {
    int sourceID, destID, algorithm;
    
    printf("\n╔══════════════════════════════════════════════════╗\n");
    printf("║           FAST NAVIGATION MODE                   ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");
    printf("1. 🔍 Dijkstra's Algorithm (Guaranteed shortest)\n");
    printf("2. ⭐ A* Algorithm (Faster with heuristic)\n");
    printf("\nEnter choice: ");
    
    if (scanf("%d", &algorithm) != 1) {
        clearInputBuffer();
        printf("❌ Invalid input!\n");
        return;
    }
    
    printf("\nEnter Source City ID: ");
    if (scanf("%d", &sourceID) != 1) {
        clearInputBuffer();
        printf("❌ Invalid input!\n");
        return;
    }
    
    printf("Enter Destination City ID: ");
    if (scanf("%d", &destID) != 1) {
        clearInputBuffer();
        printf("❌ Invalid input!\n");
        return;
    }
    clearInputBuffer();
    
    PathResult* result = NULL;
    
    if (algorithm == 1) {
        printf("\n🔄 Running Dijkstra's Algorithm...\n");
        result = dijkstra(g, sourceID, destID);
    } else if (algorithm == 2) {
        printf("\n🔄 Running A* Algorithm...\n");
        result = astar(g, sourceID, destID);
    } else {
        printf("\n❌ Invalid algorithm choice!\n");
        return;
    }
    
    if (result && result->pathLength > 0) {
        displayPath(g, result);
        
        int srcIdx = findCityIndex(g, sourceID);
        int destIdx = findCityIndex(g, destID);
        if (srcIdx != -1 && destIdx != -1) {
            logPathQuery(g->cities[srcIdx].cityName, 
                         g->cities[destIdx].cityName, 
                         result->totalDistance);
        }
        
        printf("\n💡 TIP: Run Python GUI to see this path visualized!\n");
        printf("   The path will be highlighted in green.\n");
    } else {
        printf("\n❌ No path found or invalid cities!\n");
    }
    
    freePathResult(result);
}

void handleAnalysisMode(Graph* g) {
    int choice, cityID;
    
    printf("\n╔══════════════════════════════════════════════════╗\n");
    printf("║              ANALYSIS MODE                       ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");
    printf("1. 🌊 BFS Traversal (Breadth-First)\n");
    printf("2. 🌲 DFS Traversal (Depth-First)\n");
    printf("\nEnter choice: ");
    
    if (scanf("%d", &choice) != 1) {
        clearInputBuffer();
        printf("❌ Invalid input!\n");
        return;
    }
    
    printf("\nEnter Start City ID: ");
    if (scanf("%d", &cityID) != 1) {
        clearInputBuffer();
        printf("❌ Invalid input!\n");
        return;
    }
    clearInputBuffer();
    
    if (choice == 1) {
        BFS(g, cityID);
        logOperation("BFS traversal performed");
    } else if (choice == 2) {
        DFS(g, cityID);
        logOperation("DFS traversal performed");
    } else {
        printf("\n❌ Invalid choice!\n");
    }
}

void handleSearchCity(Graph* g) {
    char cityName[MAX_CITY_NAME];
    
    printf("\n╔══════════════════════════════════════════════════╗\n");
    printf("║           SEARCH CITY BY NAME                    ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");
    
    printf("Enter City Name: ");
    fgets(cityName, MAX_CITY_NAME, stdin);
    cityName[strcspn(cityName, "\n")] = 0;
    
    int cityID = findCityByName(g, cityName);
    
    if (cityID != -1) {
        printf("\n✅ City found!\n");
        displayCity(g, cityID);
    } else {
        printf("\n❌ City '%s' not found.\n", cityName);
    }
}

// ==================== UTILITY FUNCTIONS ====================

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause() {
    printf("\nPress Enter to continue...");
    getchar();
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}