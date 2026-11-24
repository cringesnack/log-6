#ifndef GRAPH_H
#define GRAPH_H

typedef struct {
    int n;       //число вершин
    int** a;     //указатель на динамически выделяемую матрицу смежности
} Graph;

//остальные
int createGraph(Graph* g, int n);
void freeGraph(Graph* g);
int generateRandomGraph(Graph* g, int n, int edgeProbability);
int degreeOfVertex(const Graph* g, int v);
void printMatrix(const Graph* g);
void printAdjacencyLists(const Graph* g);

//унарные
int identifyVertices(Graph* g, int u, int v);
int contractEdge(Graph* g, int u, int v);
int splitVertex(Graph* g, int v);

//бинарные
int graphUnion(const Graph* g1, const Graph* g2, Graph* result);
int graphIntersection(const Graph* g1, const Graph* g2, Graph* result);
int graphSymmetricDifference(const Graph* g1, const Graph* g2, Graph* result);

//меню и вывод
int readVertex(const char* message, int n);
void unaryMenu(Graph* g);
void binaryMenu(const Graph* g1, const Graph* g2);

#endif
