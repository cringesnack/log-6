#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include "graph.h"
//бинарные операции не изменяют исходные графы

//объединение / логическое ИЛИ
int graphUnion(const Graph* g1, const Graph* g2, Graph* result)
{
    int i, j;
    int n = (g1->n > g2->n) ? g1->n : g2->n; //макс количество вершин из двух графов
    //новая матрица размером n × n
    if (!createGraph(result, n))
        return 0;
    //перебор всех элементов будущей матрицы
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            //берутся значения из двух исходных матриц
            //если строка и столбец существуют в первом графе, 
            //взять значение g1->a[i][j]. иначе 0
            int x = (i < g1->n && j < g1->n) ? g1->a[i][j] : 0;
            int y = (i < g2->n && j < g2->n) ? g2->a[i][j] : 0;
            //лог или
            result->a[i][j] = x || y;
        }
    }

    return 1;
}



//пересечение / лог И
int graphIntersection(const Graph* g1, const Graph* g2, Graph* result)
{
    int i, j;
    int n = (g1->n < g2->n) ? g1->n : g2->n; //мин количество вершин

    if (!createGraph(result, n))
        return 0;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            //лог И
            result->a[i][j] = g1->a[i][j] && g2->a[i][j];

    return 1;
}



//кольцевая сумма / искл ИЛИ
//после формирования матрицы нужно дополнительно удалить изолированные вершины
int graphSymmetricDifference(const Graph* g1, const Graph* g2, Graph* result)
{
    int i, j;
    int n = (g1->n > g2->n) ? g1->n : g2->n; //макс количество вершин двух графов;
    int* keep; //массив вершин, кторые нужно оставить
    int* newIndex; //массив новых номеров оставляемых вершин
    int newN = 0; //количество вершин, которые останутся после удаления изолированных
    Graph temp = { 0, NULL }; //временный граф, в котором сначала формируется 
                              //полная кольцевая сумма
    
    //временная матрица максимального размера
    if (!createGraph(&temp, n))
        return 0;

    //формирование кольцевой суммы
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            int x = (i < g1->n && j < g1->n) ? g1->a[i][j] : 0;
            int y = (i < g2->n && j < g2->n) ? g2->a[i][j] : 0;
            //искл ИЛИ
            temp.a[i][j] = x ^ y;
        }
    }
    //выделение памяти для вспомогательных массивов
    
    //хранит 1, если вершину нужно оставить, и 0, если удалить
    keep = (int*)malloc((size_t)n * sizeof(int));
    //хранит новый номер каждой оставляемой вершины
    newIndex = (int*)malloc((size_t)n * sizeof(int));

    if (keep == NULL || newIndex == NULL) {
        free(keep);
        free(newIndex);
        freeGraph(&temp);
        return 0;
    }
    //пометка вершин, которые имеют хотя бы одну оставшуюся связь
    //и поиск изолированных
    for (i = 0; i < n; i++) { //перебор вершин
        int hasConnection = 0; //пока у вершины нет связей
        //проверка всех элементов соответствующей строки матрицы
        for (j = 0; j < n; j++) {
            //найден хотя бы один ненулевой элемент => у вершины есть связь
            if (temp.a[i][j] != 0) {
                hasConnection = 1;
                break;
            }
        }
        //сохранение результата проверки в массиве keep
        keep[i] = hasConnection;
        //вершина имеет хотя бы одну связь => увеличение количества сохраняемых вершин
        //(вершины с петлями не удаляются)
        if (hasConnection)
            newN++;
    }

    //перенумерация вершин
    {
        int next = 0;
        for (i = 0; i < n; i++) {
            if (keep[i])
                newIndex[i] = next++;
            else
                newIndex[i] = -1;
        }
    }
    //создание итоговой матрицы
    if (!createGraph(result, newN)) {
        free(keep);
        free(newIndex);
        freeGraph(&temp);
        return 0;
    }

    //перенос связей
    for (i = 0; i < n; i++) {
        if (!keep[i]) continue;
        for (j = 0; j < n; j++) {
            if (keep[j])
                result->a[newIndex[i]][newIndex[j]] = temp.a[i][j];
        }
    }

    free(keep);
    free(newIndex);
    freeGraph(&temp);
    return 1;
}
