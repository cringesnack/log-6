#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
//при унарных операциях новый граф становится исходным вдальнейшем

//отождествление вершин u и v. если были смежными => у конечной петля
int identifyVertices(Graph* g, int u, int v)
{
    int i, j;
    int oldN = g->n; //первоначальное количество вершин
    int k = 0; //счётчик новых индексов вершин
    int* map;//массив соответствия старых и новых номеров вершин
    Graph result = { 0, NULL };
    //проверка данных
    if (u < 0 || u >= oldN || v < 0 || v >= oldN || u == v)
        return 0;
//Выделяется память под oldN целых чисел 
//Каждый элемент map[i] будет хранить новый индекс вершины i.
    map = (int*)malloc((size_t)oldN * sizeof(int));
    if (map == NULL)
        return 0;

    //вершине v значение -1 (удаляется)
    //Для остальных вершин назначаем новые номера по порядку
    for (i = 0; i < oldN; i++) {
        if (i == v)
            map[i] = -1;
        else
            map[i] = k++;
    }
    //Количество вершин уменьшается на одну, 
    //поэтому создаётся матрица размером (oldN - 1) × (oldN - 1).
    if (!createGraph(&result, oldN - 1)) {
        free(map);
        return 0;
    }

    //построение новой матрицы из старой
    for (i = 0; i < oldN; i++) { //старые строки
        if (i == v) continue; //пропуск удаляемой строки

        for (j = 0; j < oldN; j++) { //старые столбцы
            if (j == v) continue;
            //заполняем ячейку, которая будет соответствовать 
            // объединённой вершине и самой себе (3 источника петли)
            if (i == u && j == u)
                result.a[map[i]][map[j]] =
                    g->a[u][u] || g->a[v][v] || g->a[u][v];
            //рассматриваем объединённую вершину и какую-либо другую вершину j
            //Нужно объединить все связи двух старых вершин с этой вершиной
            else if (i == u)
                result.a[map[i]][map[j]] = g->a[u][j] || g->a[v][j];
            //то же самое только со столбцами
            else if (j == u)
                result.a[map[i]][map[j]] = g->a[i][u] || g->a[i][v];
            //Если ни строка, ни столбец не относятся к объединённой вершине, 
            //связь просто копируется из старой матрицы
            else
                result.a[map[i]][map[j]] = g->a[i][j];
        }
    }
    //замена старой матрицы новой
    free(map);
    freeGraph(g);
    *g = result;
    return 1;
}

//стягивание ребра.стягиваемое не станет петлёй
int contractEdge(Graph* g, int u, int v)
{
    int i, j;
    int oldN = g->n;
    int k = 0;
    int* map;
    Graph result = { 0, NULL };

    if (u < 0 || u >= oldN || v < 0 || v >= oldN || u == v)
        return 0;
    //проверка наличия ребра между выбранными вершинами
    if (!g->a[u][v]) {
        printf("Operation is impossible: there is no edge between these vertices.\n");
        return 0;
    }

    map = (int*)malloc((size_t)oldN * sizeof(int));
    if (map == NULL)
        return 0;
    //назначение новых индксов. v удаляется
    for (i = 0; i < oldN; i++) {
        if (i == v)
            map[i] = -1;
        else
            map[i] = k++;
    }

    if (!createGraph(&result, oldN - 1)) {
        free(map);
        return 0;
    }
    
    for (i = 0; i < oldN; i++) {
        if (i == v) continue;

        for (j = 0; j < oldN; j++) {
            if (j == v) continue;

            if (i == u && j == u)
                //нет g->a[u][v], это было стягиваемое
                result.a[map[i]][map[j]] = g->a[u][u] || g->a[v][v];
            else if (i == u)
                result.a[map[i]][map[j]] = g->a[u][j] || g->a[v][j];
            else if (j == u)
                result.a[map[i]][map[j]] = g->a[i][u] || g->a[i][v];
            else
                result.a[map[i]][map[j]] = g->a[i][j];
        }
    }

    free(map);
    freeGraph(g);
    *g = result;
    return 1;
}

//расщепление v. созадание новой вершины с теми же соседями и соениение v с ней
int splitVertex(Graph* g, int v)
{
    int i, j;
    int oldN = g->n;
    Graph result = { 0, NULL };
    //проверка вершины
    if (v < 0 || v >= oldN)
        return 0;
    //новая матрица размером (oldN + 1) × (oldN + 1)
    if (!createGraph(&result, oldN + 1))
        return 0;

    //копирование всех элементов старой матрицы в левую верхнюю часть новой
    for (i = 0; i < oldN; i++)
        for (j = 0; j < oldN; j++)
            result.a[i][j] = g->a[i][j];

    //копирование всех связей v с новой вершиной
    for (i = 0; i < oldN; i++) {
        //новая вершина получает связи исходной вершины по строке и столбцу
        result.a[oldN][i] = g->a[v][i];
        result.a[i][oldN] = g->a[i][v];
    }

    //Если у исходной вершины была петля, она также появится у новой вершины
    result.a[oldN][oldN] = g->a[v][v];

    //добавляется ребро между исходной и новой вершинами
    result.a[v][oldN] = 1;
    result.a[oldN][v] = 1;

    freeGraph(g);
    *g = result;
    return 1;
}