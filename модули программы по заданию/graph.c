#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

//% возможности появления петли
#define LOOP_PROBABILITY 20

int createGraph(Graph* g, int n)
{
    int i;

    if (n < 0)
        return 0;

    g->n = 0;
    g->a = NULL;

    if (n == 0)
        return 1;
    //выделение памти под n указателей на строки матрицы
    g->a = (int**)calloc((size_t)n, sizeof(int*));
    if (g->a == NULL)
        return 0;

    for (i = 0; i < n; i++) {
        //для каждой строки выделяется память под n целых чисел.
        g->a[i] = (int*)calloc((size_t)n, sizeof(int));
        if (g->a[i] == NULL) {
            int j;
            for (j = 0; j < i; j++)
                free(g->a[j]);
            free(g->a);
            g->a = NULL;
            return 0;
        }
    }
    //сохранение количества вершин
    g->n = n;
    return 1;
}

void freeGraph(Graph* g)
{
    int i;

    if (g == NULL || g->a == NULL) {
        if (g != NULL)
            g->n = 0;
        return;
    }

    for (i = 0; i < g->n; i++)
        free(g->a[i]);

    free(g->a);
    g->a = NULL;
    g->n = 0;
}

int generateRandomGraph(Graph* g, int n, int edgeProbability)
{
    int i, j;

    if (!createGraph(g, n))
        return 0;

    //рёбра между вершинами, создать один раз и отзеркалить
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            int edge = (rand() % 100 < edgeProbability) ? 1 : 0;
            g->a[i][j] = edge;
            g->a[j][i] = edge;
        }
    }

    //петли генерируются отдельно с заданной вероятностью (если она<100, то на диагонали будет 1)
    for (i = 0; i < n; i++)
        g->a[i][i] = (rand() % 100 < LOOP_PROBABILITY) ? 1 : 0;

    return 1;
}

int degreeOfVertex(const Graph* g, int v)
{
    int j, degree = 0;

    if (g == NULL || v < 0 || v >= g->n)
        return -1;

    for (j = 0; j < g->n; j++) {
        if (j == v)
            //петля добавляет 2 к степени
            degree += 2 * g->a[v][j]; 
        else
            degree += g->a[v][j];
    }

    return degree;
}

void printMatrix(const Graph* g)
{
    int i, j;

    printf("Order: %d\n", g->n);
    printf("    ");
    for (j = 0; j < g->n; j++)
        printf("%3d", j + 1);
    printf("\n");

    for (i = 0; i < g->n; i++) {
        printf("%3d ", i + 1);
        for (j = 0; j < g->n; j++)
            printf("%3d", g->a[i][j]);
        printf("\n");
    }
}

void printAdjacencyLists(const Graph* g)
{
    int i, j;

    printf("\nAdjacency lists:\n");
    for (i = 0; i < g->n; i++) {
        printf("v%d:", i + 1);
        for (j = 0; j < g->n; j++) {
            if (g->a[i][j])
                printf(" v%d", j + 1);
        }
        printf("\n");
    }
}
