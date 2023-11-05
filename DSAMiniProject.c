#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define MAX_PLACES 100

typedef struct
{
    char name[50];
} Place;
typedef struct
{
    Place places[MAX_PLACES];
    bool adjacencyMatrix[MAX_PLACES][MAX_PLACES];
    int numPlaces;
}Graph;
Graph g;

void addPlace(char name[])
{
    if (g.numPlaces >= MAX_PLACES)
    {
        printf("Maximum limit exceeded, cannot add more places.\n");
        return;
    }
    Place newPlace;
    strcpy(newPlace.name, name);
    g.places[g.numPlaces++] = newPlace;
    FILE *file = fopen("PlaceInfo.txt", "a");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return;
    }
    fprintf(file, "%s\n", name);
    fclose(file);
}

void addRoad(int from, int to)
{
    if (from < 0 || from >= g.numPlaces || to < 0 || to >= g.numPlaces)
    {
        printf("Invalid place index. Please try again.\n");
        return;
    }
    g.adjacencyMatrix[from][to] = true;
    g.adjacencyMatrix[to][from] = true;
}

void deleteRoad(int from, int to)
{
    if (from < 0 || from >= g.numPlaces || to < 0 || to >= g.numPlaces)
    {
        printf("Invalid place index. Please try again.\n");
        return;
    }
    if (!g.adjacencyMatrix[from][to] || !g.adjacencyMatrix[to][from])
    {
        printf("No road exists between the given places. Please try again.\n");
        return;
    }
    g.adjacencyMatrix[from][to] = false;
    g.adjacencyMatrix[to][from] = false;
}

void deletePlace(int index)
{
    if (index < 0 || index >= g.numPlaces)
    {
        printf("Invalid place index. Please try again.\n");
        return;
    }
    // Remove the place
    for (int i = index; i < g.numPlaces - 1; i++)
    {
        g.places[i] = g.places[i+1];
        for (int j = 0; j < g.numPlaces; j++) {
            g.adjacencyMatrix[i][j] = g.adjacencyMatrix[i+1][j];
            g.adjacencyMatrix[j][i] = g.adjacencyMatrix[j][i+1];
        }
    }
    g.numPlaces--;
    // Update file
    FILE *file = fopen("PlaceInfo.txt", "w");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return;
    }
    for (int i = 0; i < g.numPlaces; i++)
    {
        fprintf(file, "%s\n", g.places[i].name);
    }
    fclose(file);
}

void initGraph()
{
    g.numPlaces = 0;
    FILE *file = fopen("PlaceInfo.txt", "r");
    if (file != NULL)
    {
        char line[50];
        while (fgets(line, sizeof(line), file))
        {
            line[strcspn(line, "\n")] = 0;
            addPlace(line);
        }
        fclose(file);
    }
}

void displayPlaces()
{
    printf("\nPlaces in the map:\n");
    for (int i = 0; i < g.numPlaces; i++)
    {
        printf("[%d] - %s\n", i, g.places[i].name);
    }
}

void displayConnections(int index)
{
    printf("Connected places for %s:\n", g.places[index].name);
    for (int i = 0; i < g.numPlaces; i++)
    {
        if (g.adjacencyMatrix[index][i])
        {
            printf("- %s\n", g.places[i].name);
        }
    }
}

void findRoutes(int from, int to, bool visited[], int path[], int pathIndex)
{
    visited[from] = true;
    path[pathIndex] = from;
    pathIndex++;
    if (from == to)
    {
        printf("Route: ");
        for (int i = 0; i < pathIndex; i++)
        {
            printf("%s -> ", g.places[path[i]].name);
        }
        printf("\n");
    }
    else
    {
        for (int i = 0; i < g.numPlaces; i++)
        {
            if (g.adjacencyMatrix[from][i] && !visited[i])
            {
                findRoutes(i, to, visited, path, pathIndex);
            }
        }
    }
    visited[from] = false;
    pathIndex--;
}

void shortestPath(int from, int to, int distance[])
{
    bool visited[g.numPlaces];
    int previous[g.numPlaces];
    for (int i = 0; i < g.numPlaces; i++)
    {
        visited[i] = false;
        distance[i] = INT_MAX;
    }
    distance[from] = 0;
    for (int i = 0; i < g.numPlaces; i++)
    {
        int minDistance = INT_MAX;
        int current = -1;
        for (int j = 0; j < g.numPlaces; j++)
        {
            if (!visited[j] && distance[j] < minDistance)
            {
                minDistance = distance[j];
                current = j;
            }
        }
        if (current == -1 || current == to)
        {
            break;
        }
        visited[current] = true;
        for (int j = 0; j < g.numPlaces; j++)
        {
            if (g.adjacencyMatrix[current][j] && !visited[j])
            {
                int newDistance = distance[current] + 1; // assuming equal distance between all places
                if (newDistance < distance[j]) {
                    distance[j] = newDistance;
                    previous[j] = current;
                }
            }
        }
    }
    if (distance[to] != INT_MAX)
    {
        printf("Shortest path: ");
        printf("%s", g.places[to].name);
        int vertex = to;
        while (vertex != from)
        {
            vertex = previous[vertex];
            printf(" <- %s", g.places[vertex].name);
        }
        printf("\n");
    }
    else
    {
        printf("No path found.\n");
    }
}   

int main()
{
    initGraph();
    int choice;
    char newPlace[50];
    int from, to, index;
    do
    {
        printf("\nMenu:\n");
        printf("1. Add a new place\n");
        printf("2. Add a new road\n");
        printf("3. Delete a road\n");
        printf("4. Delete a place\n");
        printf("5. Display all places\n");
        printf("6. Exit\n");
        printf("7. Display all Connections\n");
        printf("8. Display all Routes\n");
        printf("9. Display Shortest Route\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        bool visited[g.numPlaces];
        int distance[g.numPlaces];
        switch(choice)
        {
            case 1:
                printf("Enter the name of the new place: ");
                scanf(" %[^\n]s", newPlace);
                addPlace(newPlace);
                printf("Place added successfully!\n");
                break;
            case 2:
                displayPlaces();
                printf("Enter the indices of the places to connect (separated by a space): ");
                scanf("%d %d", &from, &to);
                addRoad(from, to);
                printf("Road added successfully!\n");
                break;
            case 3:
                displayPlaces();
                printf("Enter the indices of the places to disconnect (separated by a space): ");
                scanf("%d %d", &from, &to);
                deleteRoad(from, to);
                printf("Road deleted successfully!\n");
                break;
            case 4:
                displayPlaces();
                printf("Enter the index of the place to delete: ");
                scanf("%d", &index);
                deletePlace(index);
                printf("Place deleted successfully!\n");
                break;
            case 5:
                displayPlaces();
                break;
            case 6:
                printf("Exiting...\n");
                break;
            case 7:
                displayPlaces();
                printf("Enter the index of the place to see the connections: ");
                scanf("%d", &index);
                displayConnections(index);
                break;
            case 8:
                displayPlaces();
                printf("Enter the indices of the places to find the route (separated by a space): ");
                scanf("%d %d", &from, &to);
                int path[MAX_PLACES];
                memset(visited, false, sizeof(visited));
                findRoutes(from, to, visited, path, 0);
                break;
            case 9:
                displayPlaces();
                printf("Enter the indices of the places to find the shortest path (separated by a space): ");
                scanf("%d %d", &from, &to);
                shortestPath(from, to, distance);
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }while (choice != 6);
    return 0;
}