#include <stdio.h>
#include <stdlib.h>

typedef struct Graph {
    int *Information; //节点信息
    int **AdjMatrix; //邻接矩阵
    int NodeNum; //节点数量
} Graph, *Ggraph; //定义邻接矩阵表示的图

int Check1(int F, int W, int G, int C);
int Check2(int C1, int C2);
Ggraph CreateGraph();

int Check1(int F, int W, int G, int C) {
    //判断状态F、G、W、C是否可行
    return F == G || W != G && C != G;
} //Check1

int Check2(int C1, int C2) {
    //检查两个状态点间是否有连接
    int A1[4], A2[4];
    int C = 0;
    int i;

    for (i = 3; i >= 0; i--) { //倒序填入
        A1[i] = C1 % 2;
        A2[i] = C2 % 2;
        C1 /= 2;
        C2 /= 2;
    } //for
    if (A1[0] == A2[0]) return 0; //农夫在同岸
    for (i = 1; i <= 3; i++) { //检查其他物品
        if (A1[i] != A2[i]) C++; //位置变动
    } //for

    return C <= 1;
} //Check2

Ggraph CreateGraph() {
    //创建农夫过河的状态图
    Ggraph G = (Ggraph)malloc(sizeof(Graph));
    int A[16]; //中转数组
    int f, w, g, c, L = 0;
    int i, j;

    for (i = 0; i < 16; i++) { //遍历所有状态
        f = i / 8;
        w = i / 4 % 2;
        g = i / 2 % 2;
        c = i % 2;
        if (Check1(f, w, g, c)) A[L++] = i; //此状态可行
    } //for
    G->Information = (int*)malloc(sizeof(int) * L);
    G->AdjMatrix = (int**)malloc(sizeof(int*) * L);
    G->NodeNum = L;
    for (i = 0; i < L; i++) G->AdjMatrix[i] = (int*)malloc(4 * sizeof(int)); //申请第二维结构
    for (i = 0; i < L; i++) { //初始化邻接矩阵
        for (j = 0; j < L; j++) G->AdjMatrix[i][j] = 0; //初始化为0
    } //for
    for (i = 0; i < L; i++) G->Information[i] = A[i]; //填充信息数组
    for (i = 0; i < G->NodeNum; i++) { //填充邻接矩阵
        for (j = i + 1; j < G->NodeNum; j++) { //只要填充上半
            if (Check2(i, j)) { //两点间有连接
                G->AdjMatrix[i][j] = 1;
                G->AdjMatrix[j][i] = 1;
            } //if
        } //for
    } //for

    return G;
} //CreateGraph