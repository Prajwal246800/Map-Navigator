#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_PLACES 50
#define MAX_NAME_LENGTH 50

typedef struct Place
{
    char name[50];
}Place;
typedef struct PlaceArray
{
    char name[MAX_NAME_LENGTH];
}PlaceArray;
typedef struct Node
{
    int vertex;
    struct Node* next;
}Node;
typedef struct Graph
{
    int numPlaces;
    Node** adjLists;
    Place* places;
}Graph;

Node* createNode(int vertex)
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = vertex;
    newNode->next = NULL;
    return newNode;
}



void addPlaceToGraph(Graph* graph , PlaceArray placeArray[])
{
    int newNumVertices = graph->numPlaces + 1;
    Graph* newGraph = (Graph*)malloc(sizeof(Graph));
    newGraph->numPlaces = newNumVertices;
    newGraph->adjLists = (Node**)malloc(newNumVertices * sizeof(Node*));
    newGraph->places = (Place*)malloc(newNumVertices * sizeof(Place));
    for (int i = 0; i < graph->numPlaces; i++)
    {
        newGraph->places[i] = graph->places[i];
        newGraph->adjLists[i] = graph->adjLists[i];
    }
    printf("Enter Name of the new place:\n");
    scanf("%s", newGraph->places[newNumVertices - 1].name);
    newGraph->adjLists[newNumVertices - 1] = NULL;
    int connectChoice;
    do {
        printf("Connect to an existing place (Enter the place number to connect, or -1 to finish): ");
        scanf("%d", &connectChoice);
        if (connectChoice >= 0 && connectChoice < newNumVertices - 1) {
            Node* newNode = createNode(connectChoice);
            newNode->next = newGraph->adjLists[newNumVertices - 1];
            newGraph->adjLists[newNumVertices - 1] = newNode;

            // Also, add a connection from the existing place to the new place
            newNode = createNode(newNumVertices - 1);
            newNode->next = newGraph->adjLists[connectChoice];
            newGraph->adjLists[connectChoice] = newNode;
        }
    } while (connectChoice != -1);
    // Free memory of the old graph and assign the new graph to the input pointer
    free(graph->places);
    free(graph->adjLists);
    free(graph);
    *graph = *newGraph;
}



Graph* createGraph(int numPlaces)
{
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numPlaces = numPlaces;
    graph->adjLists = (Node**)malloc(numPlaces * sizeof(Node));
    graph->places = (Place*)malloc(numPlaces * sizeof(Place));
    FILE* file = fopen("Sample.txt", "r");
    if (file == NULL)
    {
        printf("Error opening the file for reading.\n");
        return NULL;
    }
    for (int i = 0; i < numPlaces; i++)
    {
        int vertex;
        fscanf(file, "%d", &vertex);
        if (vertex != i + 1)
        {
            printf("Error: Place order in the file does not match the expected order.\n");
            fclose(file);
            free(graph->places);
            free(graph->adjLists);
            free(graph);
            return NULL;
        }
        char temp[20];
        fscanf(file, "%s", temp);
        fscanf(file, "%s", graph->places[i].name);
        fscanf(file, "%s", temp);
        int connectedVertex;
        while (fscanf(file, "%d", &connectedVertex) == 1)
        {
            Node* newNode = createNode(connectedVertex - 1);
            newNode->next = graph->adjLists[i];
            graph->adjLists[i] = newNode;
        }
    }
    fclose(file);
    return graph;
}




void addEdge(Graph* graph, int src, int dest)
{
    Node* newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
    // Since it's an undirected graph, add an edge from dest to src as well
    newNode = createNode(src);
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
}
void saveGraphToFile(Graph* graph) {
    FILE* file = fopen("Sample.txt", "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    for (int i = 0; i < graph->numPlaces; i++) {
        fprintf(file, "%d\n", i + 1);
        fprintf(file, "Name : %s\n", graph->places[i].name);
        fprintf(file, "Connects : ");
        Node* currentNode = graph->adjLists[i];
        int first = 1;
        while (currentNode) {
            if (!first) {
                fprintf(file, " , ");
            }
            fprintf(file, "%s", graph->places[currentNode->vertex].name);
            first = 0;
            currentNode = currentNode->next;
        }
        fprintf(file, "\n");
    }
    fclose(file);
}
void extractPlaceNames(const char* filename, struct PlaceArray placeArray[], int* numPlaces)
{
    FILE* file = fopen("Sample.txt", "r");
    if (file == NULL)
    {
        printf("Error opening the file.\n");
        exit(1);
    }
    char line[100];
    *numPlaces = 0;
    while (fgets(line, sizeof(line), file))
    {
        if (strstr(line, "Name : ") != NULL)
        {
            char* nameStart = strchr(line, ':') + 2;
            if (nameStart[strlen(nameStart) - 1] == '\n')
            {
                nameStart[strlen(nameStart) - 1] = '\0';
            }
            strncpy(placeArray[*numPlaces].name, nameStart, MAX_NAME_LENGTH);
            (*numPlaces)++;
        }
    }
    fclose(file);
}

int main()
{
    //Extract All Place Names From The file And Pace It In The Array
    PlaceArray placeArray[MAX_PLACES];
    int numPlaces;
    extractPlaceNames("Sample.txt", placeArray, &numPlaces);
    // printf("Extracted place names:\n");
    // for (int i = 0; i < numPlaces; i++) {
    //     printf("%d: %s\n", i + 1, placeArray[i].name);
    // }

    Graph *graph = createGraph(numPlaces);

    int choice = 0;
    do
    {
        printf("Enter Your Choice : \n\t 1.Add New Place \n\t 2.Add New Road \n\t 3.Delete Existing Place \n\t 4.Delete Existing Road \n\t 5.Display All Places \n\t 6.exit");
        switch(choice)
        {
            case 1:
                addPlaceToGraph(graph , placeArray);
        }
    }while(choice!=0);



    // int numVertices;
    // printf("Enter the number of places: ");
    // scanf("%d", &numVertices);
    // Graph* graph = createGraph(numVertices);
    // int src, dest;
    // char addEdgeChoice;
    // do
    // {
    //     printf("Add an edge between places (Enter 'y' to add an edge, 'n' to finish): ");
    //     scanf(" %c", &addEdgeChoice);
    //     if (addEdgeChoice == 'y')
    //     {
    //         printf("Enter source and destination (0 to %d): ", numVertices - 1);
    //         scanf("%d %d", &src, &dest);
    //         if (src >= 0 && src < numVertices && dest >= 0 && dest < numVertices)
    //         {
    //             addEdge(graph, src, dest);
    //         }
    //         else
    //         {
    //             printf("Invalid input. Please enter valid vertices.\n");
    //         }
    //     }
    // }while (addEdgeChoice == 'y');
    // saveGraphToFile(graph);
    // free(graph->places);
    // free(graph->adjLists);
    // free(graph);
    
    // printf("Extracted place names:\n");
    // for (int i = 0; i < numPlaces; i++) {
    //     printf("%d: %s\n", i + 1, placeArray[i].name);
    // }
    return 0;
}
