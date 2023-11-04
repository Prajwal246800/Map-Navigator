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

void extractPlaceNames(const char* filename, struct PlaceArray placeArray[], int* numPlaces)
{
    FILE* file = fopen("PlaceInformation.txt", "r");
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
    extractPlaceNames("PlaceInformation.txt", places , &numPlaces);
    printf("\n");
    for (int i = 0; i < numPlaces; i++)
    {
        printf("%d: %s\n", i + 1, places[i].name);
    }
    printf("\n");
}

void roadNumHelp(Graph *graph)
{
    printf("Do You know the Place Numbers (y/n) :\n ");
    char choice ='n';
    scanf(" %c",&choice);
    if(choice == 'n')
    {
        printf("Here I Can Help You To Know Place numbers.\n");
        DisplayAllPlace(graph,graph->numPlaces);
    }
}

Node* createNode(int vertex)
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = vertex;
    newNode->next = NULL;
    return newNode;
}

Graph* createGraph(int numVertices, const char* filename)
{
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (!graph)
    {
        printf("Memory allocation error for graph\n");
        return NULL;
    }
    graph->numPlaces = numVertices;
    graph->adjLists = (Node**)malloc(numVertices * sizeof(Node*));
    graph->places = (Place*)malloc(numVertices * sizeof(Place));
    if (!graph->adjLists || !graph->places)
    {
        printf("Memory allocation error for adjLists or places\n");
        free(graph);
        return NULL;
    }
    for (int i = 0; i < numVertices; i++)
    {
        graph->adjLists[i] = NULL;
    }
    FILE* file = fopen(filename, "r");
    if (!file)
    {
        printf("Unable to open file %s\n", filename);
        free(graph->adjLists);
        free(graph->places);
        free(graph);
        return NULL;
    }
    int vertex = 0;
    char line[100];
    while (fgets(line, sizeof(line), file))
    {
        if (line[0] == '\n' || line[0] == '\0')
        {
            continue; // Skip empty lines
        }
        
        if (line[0] >= '0' && line[0] <= '9')
        {
            sscanf(line, "%d", &vertex);
        } else if (strstr(line, "Name"))
        {
            char* name = strchr(line, ':') + 2; // Skip "Name :" and the space
            name[strlen(name) - 1] = '\0'; // Remove the newline character
            strncpy(graph->places[vertex - 1].name, name, sizeof(graph->places[vertex - 1].name));
        }
        else if (strstr(line, "Connects"))
        {
            char* connects = strchr(line, ':') + 2; // Skip "Connects :" and the space
            char* placeName = strtok(connects, " ,");
            while (placeName != NULL)
            {
                for (int i = 0; i < numVertices; i++)
                {
                    if (strcmp(graph->places[i].name, placeName) == 0)
                    {
                        Node* newNode = (Node*)malloc(sizeof(Node));
                        newNode->vertex = i;
                        newNode->next = graph->adjLists[vertex - 1];
                        graph->adjLists[vertex - 1] = newNode;
                        break;
                    }
                }
                placeName = strtok(NULL, " ,");
            }
        }
    }
    fclose(file);
    return graph;
}

Graph* addPlaceToGraph(Graph* graph , PlaceArray placeArray[])
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
    roadNumHelp(graph);
    do
    {
        printf("Connect to an existing place (Enter the place number to connect, or 0 to Submit Adding) : ");
        scanf("%d", &connectChoice);
        connectChoice--;
        if (connectChoice >= 0 && connectChoice < newNumVertices - 1)
        {
            Node* newNode = createNode(connectChoice);
            newNode->next = newGraph->adjLists[newNumVertices - 1];
            newGraph->adjLists[newNumVertices - 1] = newNode;
            newNode = createNode(newNumVertices - 1);
            newNode->next = newGraph->adjLists[connectChoice];
            newGraph->adjLists[connectChoice] = newNode;
        }
    }while (connectChoice != -1);
    free(graph->places);
    free(graph->adjLists);
    free(graph);
    return newGraph;
}

void addNewRoad(Graph* graph, int src, int dest)
{
    Node* newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
    // Since it's an undirected graph, add an edge from dest to src as well
    newNode = createNode(src);
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
}

void saveGraphToFile(Graph* graph)
{
    FILE* file = fopen("PlaceInformation.txt", "w");
    if (file == NULL)
    {
        printf("Error opening file for writing.\n");
        return;
    }
    for (int i = 0; i < graph->numPlaces; i++)
    {
        fprintf(file, "%d\n", i + 1);
        fprintf(file, "Name : %s\n", graph->places[i].name);
        fprintf(file, "Connects : ");
        Node* currentNode = graph->adjLists[i];
        int first = 1;
        while (currentNode)
        {
            if (!first)
            {
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


int main()
{
    //Extract All Place Names From The file And Place It In The Array
    PlaceArray placeArray[MAX_PLACES];
    int numPlaces;
    extractPlaceNames("PlaceInformation.txt", placeArray, &numPlaces);
    // printf("The Number Of Places Is %d \n", numPlaces);
    Graph *graph = createGraph(numPlaces , "PlaceInformation.txt");
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
                numPlaces++;
                break;
            case 2:
                roadNumHelp(graph);
                int src, dest;
                printf("Enter source and destination (1 to %d): ", numPlaces);
                scanf("%d %d", &src, &dest);
                src--;dest--;
                if (src >= 0 && src < numPlaces && dest >= 0 && dest < numPlaces)
                {
                    addNewRoad(graph, src, dest);
                }
                else
                {
                    printf("Invalid input. Please enter valid vertices.\n");
                }
            case 5:
                DisplayAllPlace(graph , numPlaces);
                break;
            case 6:
                continue;
            default:
                printf("Invalid Choice Please Enter Valid Choice.\n");
                goto choice;
                break;
        }
    }while(choice != 6);
    free(graph->places);
    free(graph->adjLists);
    free(graph);
    return 0;
}
