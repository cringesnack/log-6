#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "graph.h"

int readVertex(const char* message, int n)
{
    int v;
    printf("%s (1-%d): ", message, n);
    if (scanf("%d", &v) != 1)
        return -1;
    return v - 1;
}

void unaryMenu(Graph* g)
{
    int choice, u, v;
    int success = 0;

    printf("\nChoose unary operation:\n");
    printf("1. Identify two vertices\n");
    printf("2. Contract an edge\n");
    printf("3. Split a vertex\n");
    printf("Choice: ");
    if (scanf("%d", &choice) != 1) return;

    if (choice == 1 || choice == 2) {
        u = readVertex("First vertex", g->n);
        v = readVertex("Second vertex", g->n);
        if (choice == 1)
            success = identifyVertices(g, u, v);
        else
            success = contractEdge(g, u, v);
    }
    else if (choice == 3) {
        u = readVertex("Vertex to split", g->n);
        success = splitVertex(g, u);
    }
    else {
        printf("Unknown operation.\n");
        return;
    }

    if (!success) {
        printf("Operation was not performed.\n");
        return;
    }

    printf("\nResulting graph:\n");
    printMatrix(g);
    printAdjacencyLists(g);
}

void binaryMenu(const Graph* g1, const Graph* g2)
{
    Graph result = { 0, NULL };
    int choice;
    int success = 0;

    printf("\nChoose binary operation:\n");
    printf("1. Union\n");
    printf("2. Intersection\n");
    printf("3. Symmetric difference (ring sum)\n");
    printf("Choice: ");
    if (scanf("%d", &choice) != 1) return;

    if (choice == 1)
        success = graphUnion(g1, g2, &result);
    else if (choice == 2)
        success = graphIntersection(g1, g2, &result);
    else if (choice == 3)
        success = graphSymmetricDifference(g1, g2, &result);
    else {
        printf("Unknown operation.\n");
        return;
    }

    if (!success) {
        printf("Not enough memory to create the result graph.\n");
        return;
    }

    printf("\nResulting graph:\n");
    printMatrix(&result);
    printAdjacencyLists(&result);
    freeGraph(&result);
}
