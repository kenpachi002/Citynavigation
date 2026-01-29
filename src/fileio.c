#include "fileio.h"
#include <time.h>

// TIMESTAMP UTILITY

/**
 * Get current timestamp in formatted string
 */
void getCurrentTimestamp(char *buffer, int size)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, size, "[%Y-%m-%d %H:%M:%S]", t);
}

// LOGGING OPERATIONS

/**
 * Log general operation to logs.txt
 */
void logOperation(const char *operation)
{
    if (!operation)
        return;

    FILE *fp = fopen(LOGS_FILE, "a");
    if (!fp)
    {
        printf("Warning: Could not open log file.\n");
        return;
    }

    char timestamp[64];
    getCurrentTimestamp(timestamp, sizeof(timestamp));

    fprintf(fp, "%s %s\n", timestamp, operation);
    fclose(fp);
}

/**
 * Log path query to logs.txt
 */
void logPathQuery(const char *source, const char *dest, int distance)
{
    if (!source || !dest)
        return;

    FILE *fp = fopen(LOGS_FILE, "a");
    if (!fp)
    {
        printf("Warning: Could not open log file.\n");
        return;
    }

    char timestamp[64];
    getCurrentTimestamp(timestamp, sizeof(timestamp));

    fprintf(fp, "%s Shortest path query: %s -> %s (%d km)\n",
            timestamp, source, dest, distance);
    fclose(fp);
}

//  LOAD GRAPH FROM FILES

/**
 * Load graph from CSV files
 */
int loadGraphFromFiles(Graph *g, const char *citiesFile, const char *roadsFile)
{
    if (!g || !citiesFile || !roadsFile)
    {
        printf("Error: Invalid parameters!\n");
        return 0;
    }

    FILE *fp;
    char line[256];

    // LOAD CITIES
    fp = fopen(citiesFile, "r");
    if (!fp)
    {
        printf("Error: Could not open %s\n", citiesFile);
        return 0;
    }

    // Skip header line
    if (!fgets(line, sizeof(line), fp))
    {
        printf("Error: Empty cities file!\n");
        fclose(fp);
        return 0;
    }

    int cityID, x, y;
    char cityName[MAX_CITY_NAME];
    int citiesLoaded = 0;

    while (fgets(line, sizeof(line), fp))
    {
        // Remove trailing newline
        line[strcspn(line, "\r\n")] = 0;

        // Parse CSV: CityID,CityName,X_Coord,Y_Coord
        if (sscanf(line, "%d,%[^,],%d,%d", &cityID, cityName, &x, &y) == 4)
        {
            if (addCity(g, cityID, cityName, x, y))
            {
                citiesLoaded++;
            }
        }
    }
    fclose(fp);
    printf("✓ Loaded %d cities from %s\n", citiesLoaded, citiesFile);

    // LOAD ROADS
    fp = fopen(roadsFile, "r");
    if (!fp)
    {
        printf("Error: Could not open %s\n", roadsFile);
        return 0;
    }

    // Skip header line
    if (!fgets(line, sizeof(line), fp))
    {
        printf("Error: Empty roads file!\n");
        fclose(fp);
        return 0;
    }

    int fromID, toID, distance;
    int roadsLoaded = 0;

    while (fgets(line, sizeof(line), fp))
    {
        // Remove trailing newline
        line[strcspn(line, "\r\n")] = 0;

        // Parse CSV: FromCityID,ToCityID,Distance
        if (sscanf(line, "%d,%d,%d", &fromID, &toID, &distance) == 3)
        {
            if (addRoad(g, fromID, toID, distance))
            {
                roadsLoaded++;
            }
        }
    }
    fclose(fp);
    printf("✓ Loaded %d roads from %s\n", roadsLoaded, roadsFile);

    logOperation("Graph loaded from files successfully");
    return 1;
}

// SAVE GRAPH TO FILES
/**
 * Save graph to CSV files
 */
int saveGraphToFiles(Graph *g, const char *citiesFile, const char *roadsFile)
{
    if (!g || !citiesFile || !roadsFile)
    {
        printf("Error: Invalid parameters!\n");
        return 0;
    }

    FILE *fp;

    // ========== SAVE CITIES ==========
    fp = fopen(citiesFile, "w");
    if (!fp)
    {
        printf("Error: Could not create %s\n", citiesFile);
        return 0;
    }

    // Write header
    fprintf(fp, "CityID,CityName,X_Coord,Y_Coord\n");

    // Write all cities
    for (int i = 0; i < g->numCities; i++)
    {
        fprintf(fp, "%d,%s,%d,%d\n",
                g->cities[i].cityID,
                g->cities[i].cityName,
                g->cities[i].x,
                g->cities[i].y);
    }
    fclose(fp);
    printf("✓ Saved %d cities to %s\n", g->numCities, citiesFile);

    // ========== SAVE ROADS ==========
    fp = fopen(roadsFile, "w");
    if (!fp)
    {
        printf("Error: Could not create %s\n", roadsFile);
        return 0;
    }

    // Write header
    fprintf(fp, "FromCityID,ToCityID,Distance\n");

    // Write all roads
    int roadCount = 0;
    for (int i = 0; i < g->numCities; i++)
    {
        Edge *edge = g->cities[i].adjList;
        while (edge)
        {
            fprintf(fp, "%d,%d,%d\n",
                    g->cities[i].cityID,
                    edge->destCityID,
                    edge->distance);
            roadCount++;
            edge = edge->next;
        }
    }
    fclose(fp);
    printf("✓ Saved %d roads to %s\n", roadCount, roadsFile);

    logOperation("Graph saved to files successfully");
    return 1;
}