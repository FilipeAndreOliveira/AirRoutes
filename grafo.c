#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"

/*********************************************************************************************
* ListAddAresta()
*
* Arguments: dois inteiros, um double, um ponteiro para Grafo
*
* Returns: pointeiro para Aresta
*
* Description: Aloca memoria para uma nova Aresta e adiciona a lista
**********************************************************************************************/
Aresta *ListAddAresta(int vertice1, int vertice2, double custo, Grafo *G)
{
    Aresta *novaAresta = (Aresta *)malloc(sizeof(Aresta));
    novaAresta->vertice1 = vertice1;
    novaAresta->vertice2 = vertice2;
    novaAresta->custo = custo;
    novaAresta->next = G->vertices[vertice1-1]->arestas;
    G->vertices[vertice1-1]->arestas = novaAresta;
    return novaAresta;
}
/*********************************************************************************************
* ReadAresta()
*
* Arguments: um ponteiro para Grafo, um ponteiro para ficheiro
*
* Returns: void
*
* Description: le conteudo da aresta e adiciona ao Grafo
**********************************************************************************************/
void ReadAresta(Grafo * G, FILE *fp)
{
    int vertice1, vertice2;
    double custo;

    if(fscanf(fp, "%i", &vertice1)==1);
    if(fscanf(fp, "%i", &vertice2)==1);
    if(fscanf(fp, "%lf", &custo)==1);

    G->vertices[vertice1-1]->arestas = ListAddAresta(vertice1,vertice2,custo, G);
    G->vertices[vertice1-1]->numeroDeArestas++;

    G->vertices[vertice2-1]->arestas = ListAddAresta(vertice2,vertice1,custo, G);
    G->vertices[vertice2-1]->numeroDeArestas++;
}
/*********************************************************************************************
* CriaGrafo()
*
* Arguments: dois inteiros, um ponteiro para ficheiro
*
* Returns: um ponteiro para Grafo
*
* Description: Inicializa Grafo
**********************************************************************************************/
Grafo *CriaGrafo(int numeroVertices, int numeroArestas, FILE *fp)
{
    int i;
    Grafo *G = (Grafo*) malloc(sizeof(Grafo));

    G->numeroVertices = numeroVertices;
    G->vertices = (Vertice**) malloc(numeroVertices * sizeof(Vertice*));

    for(i = 0; i < numeroVertices; i++)
    {
        Vertice *vertice = malloc(sizeof(vertice));
        vertice->numeroDeVertice = i + 1;
        vertice->numeroDeArestas = 0;
        vertice->arestas = NULL;
        G->vertices[i] = vertice;
    }

    G->numeroArestas = numeroArestas;

    for(i = 0; i < numeroArestas; i++)
    {
        ReadAresta(G, fp);
    }
    return G;
}

/*********************************************************************************************
* freeLista()
*
* Arguments:  ponteiro para Aresta
*
* Returns: void
*
* Description: Liberta lista de Arestas
**********************************************************************************************/
void freeLista(Aresta* head)
{
    Aresta *tmp;
    int cnt = 0;

    while(head != NULL)
    {
        tmp = head;
        head = head->next;
        free(tmp);
        cnt++;
    }
    return;
}
/*********************************************************************************************
* freeGrafo()
*
* Arguments:  ponteiro para Grafo
*
* Returns: void
*
* Description: Liberta Grafo
**********************************************************************************************/
void freeGrafo(Grafo *G)
{

    int i = 0;
    for(i = 0; i < G->numeroVertices; i++)
    {
        freeLista(G->vertices[i]->arestas);
        free(G->vertices[i]);
    }
    free(G->vertices);
    free(G);

    return;
}
