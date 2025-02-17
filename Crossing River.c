#include <stdio.h>
#include <stdlib.h>

typedef struct Status {
    int *Info;
    int **Adj;
    int NodeNum;
    int *Vis;
} Status, *SGraph;

int Stack[100];
int top = -1;

int CheckStatus(int S) {
    //判断状态S是否合法
    int F = S / 8, W = S / 4 % 2, G = S / 2 % 2, C = S % 2;

    return F == G || W != G && G != C;
} //CheckStatus

int CheckLink(int S1, int S2) {
    //判断状态S1和状态S2是否可以一步转化
    int S[2][4];
    int C = 0;
    int i;

    for (i = 3; i >= 0; i--) { //状态转化为二进制数
        S[0][i] = S1 % 2;
        S[1][i] = S2 % 2;
        S1 /= 2;
        S2 /= 2;
    } //for
    if (S[0][0] == S[1][0]) return 0; //农夫位置不变
    for (i = 1; i < 4; i++) { //统计发生变动的物体数量
        if (S[0][i] != S[1][i]) C++; //发生变动
    } //for

    return C <= 1;
} //CheckLink

SGraph CreateStatus() {
    //创建初始状态图
    SGraph SG = (SGraph)malloc(sizeof(Status));
    int Inf[16];
    int L = 0;
    int i, j;

    for (i = 0; i < 16; i++) { //标记所有有效态
        if (CheckStatus(i)) Inf[L++] = i;
    } //for
    SG->NodeNum = L;
    SG->Info = (int*)malloc(SG->NodeNum * sizeof(int));
    SG->Adj = (int**)malloc(SG->NodeNum * sizeof(int*));
    for (i = 0; i < SG->NodeNum; i++) SG->Adj[i] = (int*)malloc(SG->NodeNum * sizeof(int)); //申请空间
    for (i = 0; i < SG->NodeNum; i++) SG->Info[i] = Inf[i]; //复制节点信息
    for (i = 0; i < SG->NodeNum; i++) { //初始化邻接矩阵
        for (j = 0; j < SG->NodeNum; j++) SG->Adj[i][j] = 0; //初始化为0
    } //for
    for (i = 0; i < SG->NodeNum; i++) { //遍历节点
        for (j = i + 1; j < SG->NodeNum; j++) { //未判断是否连接的其他节点
            if (CheckLink(SG->Info[i], SG->Info[j])) { //两节点间有连接
                SG->Adj[SG->Info[i]][SG->Info[j]] = 1;
                SG->Adj[SG->Info[j]][SG->Info[i]] = 1; 
            } //if
        } //for
    } //for
    SG->Vis = (int*)malloc(sizeof(int) * 16);
    for (i = 0; i < 16; i++) SG->Vis[SG->Info[i]] = 0; //全记为未访问

    return SG;
} //CreateGraph

void DFSFindWay(int S, int E, SGraph SG) {
    //使用DFS寻找状态图SG中从起点S到终点E的所有路径并输出
    int pos;
    int i;

    if (S == E) { //已经抵达终点
        PrintWay();

        return ;
    } //if
    Stack[++top] = S;
    SG->Vis[S] = 1;
    for (i = 0; i < SG->NodeNum; i++) { //找到起点对应的下标
        if (SG->Info[i] == S) { //起点对应
            pos = i;
            break;
        } //if
    } //for
    for (i = 0; i < SG->NodeNum; i++) { //遍历所有节点
        if (SG->Adj[pos][i] && !SG->Vis[SG->Info[i]]) { //未访问的邻接节点
            DFSFindWay(SG->Info[i], E, SG);
        } //if
    } //for
    top--;
    SG->Vis[S] = 0;
} //DFSFindWay