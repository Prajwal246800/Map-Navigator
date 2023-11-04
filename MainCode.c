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
Graph* CreateGraph(int numPlaces)
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
Graph* createGraph(int numPlaces) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numPlaces = numPlaces;
    graph->adjLists = (Node**)malloc(numPlaces * sizeof(Node*));
    graph->places = (Place*)malloc(numPlaces * sizeof(Place));
    FILE* file = fopen("Sample.txt", "r");
    if (file == NULL) {
        printf("Error opening the file for reading.\n");
        return NULL;
    }
    int vertex;
    for (int i = 0; i < numPlaces; i++) {
        if (fscanf(file, "%d", &vertex) != 1)
        {
            printf("Error: Could not read the place number.\n");
            fclose(file);
            free(graph->places);
            free(graph->adjLists);
            free(graph);
            return NULL;
        }
        if (vertex != i + 1) {
            printf("Error: Place order in the file does not match the expected order.\n");
            fclose(file);
            free(graph->places);
            free(graph->adjLists);
            free(graph);
            return NULL;
        }
        char label[20];
        char name[50];
        // Read and validate the "Name :" label
        if (fscanf(file, "%s", label) != 1 || strcmp(label, "Name") != 0) {
            printf("Error: Expected 'Name' label.\n");
            fclose(file);
            free(graph->places);
            free(graph->adjLists);
            free(graph);
            return NULL;
        }
        // Read the name of the place
        fscanf(file, " : %49[^\n]", name);
        strcpy(graph->places[i].name, name);
        // Read and validate the "Connects :" label
        if (fscanf(file, "%s", label) != 1 || strcmp(label, "Connects") != 0) {
            printf("Error: Expected 'Connects' label.\n");
            fclose(file);
            free(graph->places);
            free(graph->adjLists);
            free(graph);
            return NULL;
        }
        int connectedVertex;
        while (fscanf(file, "%d", &connectedVertex) == 1) {
            Node* newNode = createNode(connectedVertex - 1);
            newNode->next = graph->adjLists[i];
            graph->adjLists[i] = newNode;
        }
    }
    fclose(file);
    return graph;
}

Graph* addPlaceToGraph(Graph* graph , PlaceArray placeArray[])
{
    printf("Entered\n");
    int newNumVertices = graph->numPlaces + 1;
    printf("New Graph Created 1\n");
    Graph* newGraph = (Graph*)malloc(sizeof(Graph));
    newGraph->numPlaces = newNumVertices;
    newGraph->adjLists = (Node**)malloc(newNumVertices * sizeof(Node*));
    printf("New Graph Created 2\n");
    newGraph->places = (Place*)malloc(newNumVertices * sizeof(Place));
    printf("New Graph Created 3\n");
    for (int i = 0; i < graph->numPlaces; i++)
    {
        printf("Asigning Old Graph To New Graph.....\n");
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
            newNode = createNode(newNumVertices - 1);
            newNode->next = newGraph->adjLists[connectChoice];
            newGraph->adjLists[connectChoice] = newNode;
        }
    } while (connectChoice != -1);
    free(graph->places);
    free(graph->adjLists);
    free(graph);
    return newGraph;
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
void DisplayAllPlace(Graph *graph,int numPlaces)
{
    PlaceArray places[MAX_PLACES];
    extractPlaceNames("Sample.txt", places , &numPlaces);
    for (int i = 0; i < numPlaces; i++) {
        printf("%d: %s\n", i + 1, places[i].name);
    }
}

int main()
{
    //Extract All Place Names From The file And Pace It In The Array
    PlaceArray placeArray[MAX_PLACES];
    int numPlaces;
    extractPlaceNames("Sample.txt", placeArray, &numPlaces);
    printf("The Number Of Places Is %d \n", numPlaces);
    Graph *graph = createGraph(numPlaces);
    int choice = 0;
    do
    {
        choice:
        printf("Enter Your Choice : \n\t 1.Add New Place \n\t 2.Add New Road \n\t 3.Delete Existing Place \n\t 4.Delete Existing Road \n\t 5.Display All Places \n\t 6.exit \n");
        scanf("%d", &choice);
        switch(choice)
        {
            case 1:
                graph = addPlaceToGraph(graph , placeArray);
                saveGraphToFile(graph);
                break;
            case 5:
                DisplayAllPlace(graph , numPlaces);
                break;
            case 6:
                exit(0);
            default:
                printf("Invalid Choice Please Enter Valid Choice.\n");
                goto choice;
                break;
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
