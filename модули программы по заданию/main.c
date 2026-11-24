#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graph.h"

int main(void)
{
    Graph g1 = { 0, NULL };
    Graph g2 = { 0, NULL };
    int n1, n2;
    int probability1, probability2;
    int choice;

    srand((unsigned int)time(NULL));

    printf("number of vertices in graph G1: ");
    if (scanf("%d", &n1) != 1 || n1 < 1) {
        printf("Invalid graph order.\n");
        return 1;
    }

    printf("number of vertices in graph G2: ");
    if (scanf("%d", &n2) != 1 || n2 < 1) {
        printf("Invalid graph order.\n");
        return 1;
    }

    printf("edge probability for G1 (0-100): ");
    if (scanf("%d", &probability1) != 1 || probability1 < 0 || probability1 > 100) {
        printf("Invalid probability.\n");
        return 1;
    }

    printf("edge probability for G2 (0-100): ");
    if (scanf("%d", &probability2) != 1 || probability2 < 0 || probability2 > 100) {
        printf("Invalid probability.\n");
        return 1;
    }

    if (!generateRandomGraph(&g1, n1, probability1)) {
        printf("Not enough memory to create graph G1.\n");
        return 1;
    }
    if (!generateRandomGraph(&g2, n2, probability2)) {
        printf("Not enough memory to create graph G2.\n");
        freeGraph(&g1);
        return 1;
    }

    printf("\nGraph G1:\n");
    printMatrix(&g1);
    printAdjacencyLists(&g1);
    printf("\nGraph G2:\n");
    printMatrix(&g2);
    printAdjacencyLists(&g2);

    do {
        printf("\nwha do u want\n");
        printf("1. Unary operation on G1\n");
        printf("2. Binary operation on G1 and G2\n");
        printf("3. Display both graphs\n");
        printf("0. Exit\n");
        printf("??: ");
        if (scanf("%d", &choice) != 1)
            break;

        switch (choice) {
        case 1:
            unaryMenu(&g1);
            break;
        case 2:
            binaryMenu(&g1, &g2);
            break;
        case 3:
            printf("\nGraph G1:\n");
            printMatrix(&g1);
            printAdjacencyLists(&g1);
            printf("\nGraph G2:\n");
            printMatrix(&g2);
            printAdjacencyLists(&g2);
            break;
        case 0:
            printf("! Program finished.\n");
            break;
        default:
            printf("? Unknown menu item.\n");
        }
    } while (choice != 0);

    freeGraph(&g1);
    freeGraph(&g2);
    return 0;
}
