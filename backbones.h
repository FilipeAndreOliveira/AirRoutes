#ifndef BACKBONES_H_INCLUDED
#define BACKBONES_H_INCLUDED

typedef struct uniao Uniao;
typedef struct mst Mst;
typedef struct arestasSubs ArestasSubs;

struct uniao
{
    int *id;
    int *sz;
    int raizesij[2];
};

struct mst
{
    Aresta **mst_table;
    Aresta **a_table;
    int numArestas_mst;
    double custo_mst;
};

struct arestasSubs
{
    Aresta **NaoUsadas;
    int nUsadas;
};

int verticesEfetivos(Grafo *G);
Mst *CriaMST(Grafo *G);
int lessCost(Aresta *a, Aresta *b);
int lessV(Aresta *a, Aresta *b);
int partition(Aresta **a, int l, int r, int (*less) (Aresta*, Aresta*));
void quicksort(Aresta **a, int l, int r, int (*less) (Aresta*, Aresta*));
void cwqu(Uniao *U, int numArestas, Aresta **A);
Uniao *UFinit(Grafo *G);
int UFfind(Uniao *U, int p, int q);
int UFunion(Uniao *U, int p, int q);
Aresta** Arestas_Kr(Grafo *G, int vi, int vj);
void Kruskal(Grafo *G, Mst *msT, int vi, int vj);
int ExisteArestaMST(int vi, int vj, Mst *M);
void ListAddSubstitutas(Aresta *head, Aresta *novaAresta);
ArestasSubs* Kruskal_Substitutas(Grafo *G, Mst *msT, int vi, int vj);
Aresta **DescartaArestaMST(Mst *M, int vi, int vj);
int ocurrVi_Subs(Aresta **TabelaSubstitutas, ArestasSubs *Substitutas, int vi);
int ocurrVi_MST(Mst *M, int vi);
Aresta **DescartaViMST(Mst *M, int vi, int vi_mstCnt);
Aresta **DescartaViSubstitutas(ArestasSubs *Substitutas, Aresta **A, int vi, int vi_subsCnt);
void imprimeAlternativa(FILE *fpOut, Mst * msT,ArestasSubs *Substitutas, Aresta **TabelaSubstitutas, Grafo *G, int vi, int vj);
void freeUniao(Uniao *U);
void freeMST(Mst *M);




#endif // BACKBONES_H_INCLUDED
