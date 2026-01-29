#include "algorithms.h"
#include <math.h>

// MIN-HEAP IMPLEMENTATION
/* Create min-heap with given capacity*/
MinHeap *createMinHeap(int capacity)
{
    MinHeap *h = (MinHeap *)malloc(sizeof(MinHeap));
    if (!h)
        return NULL;

    h->nodes = (HeapNode *)malloc(capacity * sizeof(HeapNode));
    h->pos = (int *)malloc(1000 * sizeof(int)); // Support up to 1000 cities

    if (!h->nodes || !h->pos)
    {
        free(h->nodes);
        free(h->pos);
        free(h);
        return NULL;
    }

    h->size = 0;
    h->capacity = capacity;

    // Initialize position array
    for (int i = 0; i < 1000; i++)
    {
        h->pos[i] = -1;
    }
    return h;
}

/* Free min-heap memory */
void freeMinHeap(MinHeap *h)
{
    if (h)
    {
        free(h->nodes);
        free(h->pos);
        free(h);
    }
}

/* Swap two heap nodes*/
void swapHeapNodes(HeapNode *a, HeapNode *b)
{
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

/* Min-heapify operation Maintains min-heap property */
void minHeapify(MinHeap *h, int idx)
{
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < h->size && h->nodes[left].fScore < h->nodes[smallest].fScore)
        smallest = left;

    if (right < h->size && h->nodes[right].fScore < h->nodes[smallest].fScore)
        smallest = right;

    if (smallest != idx)
    {
        // Update position array
        h->pos[h->nodes[smallest].cityID] = idx;
        h->pos[h->nodes[idx].cityID] = smallest;

        swapHeapNodes(&h->nodes[smallest], &h->nodes[idx]);
        minHeapify(h, smallest);
    }
}

/* Check if heap is empty */
int isHeapEmpty(MinHeap *h)
{
    return h->size == 0;
}

/* Extract minimum element from heap */
HeapNode extractMin(MinHeap *h)
{
    if (isHeapEmpty(h))
    {
        HeapNode empty = {-1, INF, INF};
        return empty;
    }

    HeapNode root = h->nodes[0];
    HeapNode lastNode = h->nodes[h->size - 1];

    // Move last node to root
    h->nodes[0] = lastNode;
    h->pos[root.cityID] = -1;
    h->pos[lastNode.cityID] = 0;

    h->size--;
    minHeapify(h, 0);

    return root;
}

/* Decrease key value for a city */
void decreaseKey(MinHeap *h, int cityID, int newDist, int newFScore)
{
    int i = h->pos[cityID];
    if (i == -1)
        return; // City not in heap

    h->nodes[i].distance = newDist;
    h->nodes[i].fScore = newFScore;

    // Bubble up if necessary
    while (i > 0 && h->nodes[i].fScore < h->nodes[(i - 1) / 2].fScore)
    {
        h->pos[h->nodes[i].cityID] = (i - 1) / 2;
        h->pos[h->nodes[(i - 1) / 2].cityID] = i;

        swapHeapNodes(&h->nodes[i], &h->nodes[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

/* Insert node into heap */
void insertHeap(MinHeap *h, int cityID, int distance, int fScore)
{
    if (h->size >= h->capacity)
        return;

    h->size++;
    int i = h->size - 1;

    h->nodes[i].cityID = cityID;
    h->nodes[i].distance = distance;
    h->nodes[i].fScore = fScore;
    h->pos[cityID] = i;

    // Bubble up
    while (i > 0 && h->nodes[i].fScore < h->nodes[(i - 1) / 2].fScore)
    {
        h->pos[h->nodes[i].cityID] = (i - 1) / 2;
        h->pos[h->nodes[(i - 1) / 2].cityID] = i;

        swapHeapNodes(&h->nodes[i], &h->nodes[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

// PATH RESULT
/* Create PathResult structure */
PathResult *createPathResult(int capacity)
{
    PathResult *pr = (PathResult *)malloc(sizeof(PathResult));
    if (!pr)
        return NULL;

    pr->path = (int *)malloc(capacity * sizeof(int));
    if (!pr->path)
    {
        free(pr);
        return NULL;
    }

    pr->pathLength = 0;
    pr->totalDistance = 0;
    pr->pathCapacity = capacity;
    return pr;
}

/*Free PathResult memory */
void freePathResult(PathResult *pr)
{
    if (pr)
    {
        free(pr->path);
        free(pr);
    }
}

/* Add city to path */
void addToPath(PathResult *pr, int cityID)
{
    if (pr->pathLength >= pr->pathCapacity)
    {
        // Resize if needed
        pr->pathCapacity *= 2;
        pr->path = (int *)realloc(pr->path, pr->pathCapacity * sizeof(int));
        if (!pr->path)
            return;
    }
    pr->path[pr->pathLength++] = cityID;
}

/* Reverse path array */
void reversePath(PathResult *pr)
{
    for (int i = 0; i < pr->pathLength / 2; i++)
    {
        int temp = pr->path[i];
        pr->path[i] = pr->path[pr->pathLength - 1 - i];
        pr->path[pr->pathLength - 1 - i] = temp;
    }
}

// BFS TRAVERSAL
/* Breadth-First Search traversal */
void BFS(Graph *g, int startCityID)
{
    if (!g)
    {
        printf("Error: Invalid graph!\n");
        return;
    }

    int startIndex = findCityIndex(g, startCityID);
    if (startIndex == -1)
    {
        printf("Error: Start city not found!\n");
        return;
    }

    int *visited = (int *)calloc(g->numCities, sizeof(int));
    int *queue = (int *)malloc(g->numCities * sizeof(int));

    if (!visited || !queue)
    {
        free(visited);
        free(queue);
        printf("Error: Memory allocation failed!\n");
        return;
    }

    int front = 0, rear = 0;

    visited[startIndex] = 1;
    queue[rear++] = startIndex;

    printf("\n╔══════════════════════════════════════════════════╗\n");
    printf("║         BFS TRAVERSAL                            ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");
    printf("Starting from: %s\n\n", g->cities[startIndex].cityName);
    printf("Order: ");

    while (front < rear)
    {
        int current = queue[front++];
        printf("%s", g->cities[current].cityName);

        Edge *edge = g->cities[current].adjList;
        while (edge)
        {
            int destIndex = findCityIndex(g, edge->destCityID);
            if (destIndex != -1 && !visited[destIndex])
            {
                visited[destIndex] = 1;
                queue[rear++] = destIndex;
            }
            edge = edge->next;
        }

        if (front < rear)
            printf(" → ");
    }
    printf("\n════════════════════════════════════════════════════\n");

    free(visited);
    free(queue);
}

// DFS TRAVERSAL
/* DFS utility function (recursive) */
void DFSUtil(Graph *g, int cityIndex, int *visited)
{
    visited[cityIndex] = 1;
    printf("%s", g->cities[cityIndex].cityName);

    Edge *edge = g->cities[cityIndex].adjList;
    int hasUnvisited = 0;

    // Check if there are unvisited neighbors
    Edge *temp = edge;
    while (temp)
    {
        int destIndex = findCityIndex(g, temp->destCityID);
        if (destIndex != -1 && !visited[destIndex])
        {
            hasUnvisited = 1;
            break;
        }
        temp = temp->next;
    }

    if (hasUnvisited)
        printf(" → ");

    // Visit unvisited neighbors
    while (edge)
    {
        int destIndex = findCityIndex(g, edge->destCityID);
        if (destIndex != -1 && !visited[destIndex])
        {
            DFSUtil(g, destIndex, visited);
        }
        edge = edge->next;
    }
}

/* Depth-First Search traversal */
void DFS(Graph *g, int startCityID)
{
    if (!g)
    {
        printf("Error: Invalid graph!\n");
        return;
    }

    int startIndex = findCityIndex(g, startCityID);
    if (startIndex == -1)
    {
        printf("Error: Start city not found!\n");
        return;
    }

    int *visited = (int *)calloc(g->numCities, sizeof(int));
    if (!visited)
    {
        printf("Error: Memory allocation failed!\n");
        return;
    }

    printf("\n╔══════════════════════════════════════════════════╗\n");
    printf("║         DFS TRAVERSAL                            ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");
    printf("Starting from: %s\n\n", g->cities[startIndex].cityName);
    printf("Order: ");

    DFSUtil(g, startIndex, visited);

    printf("\n════════════════════════════════════════════════════\n");

    free(visited);
}

// DIJKSTRA'S ALGORITHM
/*Dijkstra's shortest path algorithm */
PathResult *dijkstra(Graph *g, int sourceCityID, int destCityID)
{
    if (!g)
    {
        printf("Error: Invalid graph!\n");
        return NULL;
    }

    int srcIndex = findCityIndex(g, sourceCityID);
    int destIndex = findCityIndex(g, destCityID);

    if (srcIndex == -1 || destIndex == -1)
    {
        printf("Error: Source or destination city not found!\n");
        return NULL;
    }

    int *dist = (int *)malloc(g->numCities * sizeof(int));
    int *parent = (int *)malloc(g->numCities * sizeof(int));

    if (!dist || !parent)
    {
        free(dist);
        free(parent);
        printf("Error: Memory allocation failed!\n");
        return NULL;
    }

    // Initialize distances and parents
    for (int i = 0; i < g->numCities; i++)
    {
        dist[i] = INF;
        parent[i] = -1;
    }
    dist[srcIndex] = 0;

    MinHeap *h = createMinHeap(g->numCities);
    if (!h)
    {
        free(dist);
        free(parent);
        return NULL;
    }

    // Insert all cities into heap
    for (int i = 0; i < g->numCities; i++)
    {
        insertHeap(h, g->cities[i].cityID, dist[i], dist[i]);
    }

    // Dijkstra's algorithm
    while (!isHeapEmpty(h))
    {
        HeapNode minNode = extractMin(h);
        int u = findCityIndex(g, minNode.cityID);

        if (u == destIndex)
            break;

        Edge *edge = g->cities[u].adjList;
        while (edge)
        {
            int v = findCityIndex(g, edge->destCityID);

            if (dist[u] != INF && dist[u] + edge->distance < dist[v])
            {
                dist[v] = dist[u] + edge->distance;
                parent[v] = u;
                decreaseKey(h, edge->destCityID, dist[v], dist[v]);
            }
            edge = edge->next;
        }
    }

    freeMinHeap(h);

    // Build path result
    PathResult *result = createPathResult(g->numCities);
    if (!result)
    {
        free(dist);
        free(parent);
        return NULL;
    }

    if (dist[destIndex] == INF)
    {
        printf("No path exists between these cities!\n");
        free(dist);
        free(parent);
        return result;
    }

    result->totalDistance = dist[destIndex];

    // Trace path back from destination to source
    int current = destIndex;
    while (current != -1)
    {
        addToPath(result, g->cities[current].cityID);
        current = parent[current];
    }
    reversePath(result);

    free(dist);
    free(parent);

    return result;
}

// A* ALGORITHM
/* Heuristic function - Euclidean distance */
int heuristic(Graph *g, int cityIndex1, int cityIndex2)
{
    int dx = g->cities[cityIndex1].x - g->cities[cityIndex2].x;
    int dy = g->cities[cityIndex1].y - g->cities[cityIndex2].y;
    return (int)sqrt(dx * dx + dy * dy);
}

/* A* shortest path algorithm */
PathResult *astar(Graph *g, int sourceCityID, int destCityID)
{
    if (!g)
    {
        printf("Error: Invalid graph!\n");
        return NULL;
    }

    int srcIndex = findCityIndex(g, sourceCityID);
    int destIndex = findCityIndex(g, destCityID);

    if (srcIndex == -1 || destIndex == -1)
    {
        printf("Error: Source or destination city not found!\n");
        return NULL;
    }

    int *gScore = (int *)malloc(g->numCities * sizeof(int));
    int *fScore = (int *)malloc(g->numCities * sizeof(int));
    int *parent = (int *)malloc(g->numCities * sizeof(int));

    if (!gScore || !fScore || !parent)
    {
        free(gScore);
        free(fScore);
        free(parent);
        printf("Error: Memory allocation failed!\n");
        return NULL;
    }

    // Initialize scores
    for (int i = 0; i < g->numCities; i++)
    {
        gScore[i] = INF;
        fScore[i] = INF;
        parent[i] = -1;
    }

    gScore[srcIndex] = 0;
    fScore[srcIndex] = heuristic(g, srcIndex, destIndex);

    MinHeap *h = createMinHeap(g->numCities);
    if (!h)
    {
        free(gScore);
        free(fScore);
        free(parent);
        return NULL;
    }

    insertHeap(h, g->cities[srcIndex].cityID, gScore[srcIndex], fScore[srcIndex]);

    // A* algorithm
    while (!isHeapEmpty(h))
    {
        HeapNode minNode = extractMin(h);
        int u = findCityIndex(g, minNode.cityID);

        if (u == destIndex)
            break; // Reached destination

        Edge *edge = g->cities[u].adjList;
        while (edge)
        {
            int v = findCityIndex(g, edge->destCityID);
            int tentative_gScore = gScore[u] + edge->distance;

            if (tentative_gScore < gScore[v])
            {
                parent[v] = u;
                gScore[v] = tentative_gScore;
                fScore[v] = gScore[v] + heuristic(g, v, destIndex);

                if (h->pos[edge->destCityID] == -1)
                {
                    insertHeap(h, edge->destCityID, gScore[v], fScore[v]);
                }
                else
                {
                    decreaseKey(h, edge->destCityID, gScore[v], fScore[v]);
                }
            }
            edge = edge->next;
        }
    }

    freeMinHeap(h);

    // Build path result
    PathResult *result = createPathResult(g->numCities);
    if (!result)
    {
        free(gScore);
        free(fScore);
        free(parent);
        return NULL;
    }

    if (gScore[destIndex] == INF)
    {
        printf("No path exists between these cities!\n");
        free(gScore);
        free(fScore);
        free(parent);
        return result;
    }

    result->totalDistance = gScore[destIndex];

    // Trace path
    int current = destIndex;
    while (current != -1)
    {
        addToPath(result, g->cities[current].cityID);
        current = parent[current];
    }
    reversePath(result);

    free(gScore);
    free(fScore);
    free(parent);

    return result;
}

// DISPLAY PATH
/* Display path result */
void displayPath(Graph *g, PathResult *pr)
{
    if (!pr || pr->pathLength == 0)
    {
        printf("No path to display.\n");
        return;
    }

    printf("\n╔══════════════════════════════════════════════════╗\n");
    printf("║           SHORTEST PATH FOUND                    ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");
    printf("Total Distance: %d km\n", pr->totalDistance);
    printf("Number of Cities: %d\n\n", pr->pathLength);
    printf("Path: ");

    for (int i = 0; i < pr->pathLength; i++)
    {
        int index = findCityIndex(g, pr->path[i]);
        if (index != -1)
        {
            printf("%s", g->cities[index].cityName);
            if (i < pr->pathLength - 1)
                printf(" → ");
        }
    }
    printf("\n════════════════════════════════════════════════════\n");
}