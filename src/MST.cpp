#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
struct ListNode {
    int destination;
    int weight;
    struct ListNode* next;
};
struct AdjacencyList {
    struct ListNode* head;
};
struct Graph {
    int Vertices;
    struct AdjacencyList* list;
};
struct ListNode* newAdjNode(int destination, int weight)
{
    struct ListNode* node = (struct ListNode*)malloc(sizeof(struct ListNode));
    node->destination = destination;
    node->next = NULL;
    node->weight = weight;
    return node;
}
struct Graph* newParentGraph(int Ver)
{
    struct Graph* graphNode = (struct Graph*)malloc(sizeof(struct Graph));
    graphNode->Vertices = Ver;
    graphNode->list = (struct AdjacencyList*)malloc(Ver * sizeof(struct AdjacencyList));
    for (int j = 0; j < Ver; ++j)
        graphNode->list[j].head = NULL;

    return graphNode;
}
void createAnEgde(struct Graph *graph, int s, int d, int w)
{
    struct ListNode* node = newAdjNode(d, w);
    node->next = graph->list[s].head;
    graph->list[s].head = node;
    node = newAdjNode(s, w);
    node->next = graph->list[d].head;
    graph->list[d].head = node;
}
struct HeapNode {
    int vertice;
    int value;
};

struct Heap {
    int length;
    int capacity;
    int* position;
    struct HeapNode** adjList;
};

struct HeapNode* newHNode(int v, int value)
{
    struct HeapNode* node = (struct HeapNode*)malloc(sizeof(struct HeapNode));
    node->vertice = v;
    node->value = value;
    return node;
}

struct Heap* createHeap(int capacity)
{
    struct Heap* Heap;
    Heap= (struct Heap*)malloc(sizeof(struct Heap));
    Heap->position = (int*)malloc(capacity * sizeof(int));
    Heap->length = 0;
    Heap->capacity = capacity;
    Heap->adjList = (struct HeapNode**)malloc(capacity * sizeof(struct HeapNode*));
    return Heap;
}
void changeNodes(struct HeapNode** node1, struct HeapNode** node2)
{
    struct HeapNode* tmp = *node1;
    *node1 = *node2;
    *node2 = tmp;
}
void StartHeap(struct Heap* Heap, int keyValue)
{
    int leastValue, leftValue, rightValue;
    leastValue = keyValue;
    leftValue = 2 * keyValue + 1;
    rightValue = 2 * keyValue + 2;

    if (Heap->adjList[rightValue]->value < Heap->adjList[leastValue]->value && rightValue < Heap->length)
        leastValue = rightValue;

    if (Heap->adjList[leftValue]->value < Heap->adjList[leastValue]->value && leftValue < Heap->length)
        leastValue = leftValue;


    if (leastValue != keyValue) {
        HeapNode* leastValueNode = Heap->adjList[leastValue];
        HeapNode* keyValueNode = Heap->adjList[keyValue];

        Heap->position[leastValueNode->vertice] = keyValue;
        Heap->position[keyValueNode->vertice] = leastValue;
        changeNodes(&Heap->adjList[leastValue], &Heap->adjList[keyValue]);
        StartHeap(Heap, leastValue);
    }
}
int isEmpty(struct Heap* Heap)
{
    return Heap->length == 0;
}
void printGraph(struct Graph* graph)
{
    int v;
    for (v = 0; v < graph->Vertices; ++v)
    {
        struct ListNode* pCrawl = graph->list[v].head;
        printf("\n Adjacency adjList of vertex %d\n head ", v);
        while (pCrawl)
        {
            printf("--weight(%d)--> %d",pCrawl->weight, pCrawl->destination);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
}
struct HeapNode* extractMin(struct Heap* Heap)
{
    if (isEmpty(Heap))
        return NULL;
    struct HeapNode* root = Heap->adjList[0];
    struct HeapNode* lastNode = Heap->adjList[Heap->length - 1];
    Heap->adjList[0] = lastNode;
    Heap->position[root->vertice] = Heap->length - 1;
    Heap->position[lastNode->vertice] = 0;
    --Heap->length;
    StartHeap(Heap, 0);
    return root;
}

void decreasevalue(struct Heap* Heap, int v, int value)
{
    int i = Heap->position[v];
    Heap->adjList[i]->value = value;
    while (i && Heap->adjList[i]->value < Heap->adjList[(i - 1) / 2]->value) {
        Heap->position[Heap->adjList[i]->vertice] = (i - 1) / 2;
        Heap->position[Heap->adjList[(i - 1) / 2]->vertice] = i;
        changeNodes(&Heap->adjList[i], &Heap->adjList[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}
bool isInHeap(struct Heap* Heap, int v)
{
    if (Heap->position[v] < Heap->length)
        return true;
    return false;
}
void PrintingList(int list[], int total,int weightValue[])
{
    for (int i = 1; i < total; ++i)
        printf("Src:%d --------(Weight:%d)------- Dest:%d\n", list[i],weightValue[i], i);
}

// The main function that constructs Minimum Spanning Tree (MST)
// using Prim's algorithm
void MST(struct Graph* graph)
{
    int V = graph->Vertices; // Get the number of vertices in graph
    int parent[V]; // adjList to store constructed MST
    int value[V]; // value values used to pick minimum weight edge in cut

    // Heap represents set E
    struct Heap* Heap = createHeap(V);

    for (int vertice = 1; vertice < V; ++vertice) {
        parent[vertice] = -1;
        value[vertice] = INT_MAX;
        Heap->adjList[vertice] = newHNode(vertice, value[vertice]);
        Heap->position[vertice] = vertice;
    }
    value[0] = 0;
    Heap->adjList[0] = newHNode(0, value[0]);
    Heap->position[0] = 0;
    Heap->length = V;


    while (!isEmpty(Heap)) {

        struct HeapNode* HeapNode = extractMin(Heap);
        int u = HeapNode->vertice; // Store the extracted vertex number

        struct ListNode* pCrawl = graph->list[u].head;
        while (pCrawl != NULL) {
            int v = pCrawl->destination;
            if (isInHeap(Heap, v) && pCrawl->weight < value[v]) {
                value[v] = pCrawl->weight;
                parent[v] = u;
                decreasevalue(Heap, v, value[v]);
            }
            pCrawl = pCrawl->next;
        }
    }

    int aa=0,i=0;
    for(aa=0;aa<V;aa++)
    {
        i=i+value[aa];
    }
    PrintingList(parent, V,value);
    printf("\nMinimum Cost Is :%d",i);
}
int main()
{
    // Let us create the graph given in above fugure
    int array[400];
    struct Graph* graph = newParentGraph(20);
    static const char filename[] = "in1.txt";
    int i=0,j=0;
    FILE *file = fopen ( filename, "r" );
    if ( file != NULL )
    {
        char line [ 128 ]; /* or other suitable maximum line length */
        while ( fgets ( line, sizeof line, file ) != NULL ) /* read a line */
        {
            array[i++]=atoi(line);
        }
        fclose ( file );
    }
    else
    {
        perror ( filename ); /* why didn't the file open? */
    }
    for(j=0;j<400;j++)
    {
        int src = j/20;
        int dest = j%20;
        createAnEgde(graph, src, dest, array[j]);
    }
    printGraph(graph);
    printf("\nMinimum Spanning Tree At Vertex1:\n\n");
    MST(graph);
    return 0;
}