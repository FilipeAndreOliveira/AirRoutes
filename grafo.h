#ifndef GRAFO_H_INCLUDED
#define GRAFO_H_INCLUDED

typedef struct vertice Vertice;
typedef struct aresta Aresta;
typedef struct grafo Grafo;

struct vertice
{
    int numeroDeVertice;

    int numeroDeArestas;
    Aresta *arestas;
};

struct aresta
{
    double custo;
    int vertice1;
    int vertice2;
    Aresta *next;

};

struct grafo
{
    int numeroVertices;
    Vertice **vertices;

    int numeroArestas;

};

Grafo *CriaGrafo(int numeroVertices, int numeroArestas, FILE *fp);
void ReadAresta(Grafo *G, FILE *fp);
Aresta *ListAddAresta(int vertice1, int vertice2, double custo, Grafo *G);
void freeGrafo(Grafo *G);
void freeLista(Aresta* head);


#endif // GRAFO_H_INCLUDED
