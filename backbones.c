#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"
#include "backbones.h"

/*********************************************************************************************
* freeUniao()
*
* Arguments:  ponteiro para Uniao
*
* Returns: void
*
* Description: Liberta Uniao
**********************************************************************************************/

void freeUniao(Uniao *U)
{
    free(U->id);
    free(U->sz);
    free(U);
    return;
}

/*********************************************************************************************
* freeMST()
*
* Arguments:  ponteiro para Mst
*
* Returns: void
*
* Description: Liberta Mst
**********************************************************************************************/

void freeMST(Mst *M)
{
    free(M->mst_table);
    free(M->a_table);
    free(M);
}

/*********************************************************************************************
* verticesEfetivos()
*
* Arguments:  ponteiro para Grafo
*
* Returns: inteiro
*
* Description: Conta numero de vertices que pertencem a Mst
**********************************************************************************************/

int verticesEfetivos(Grafo *G)
{
    int i, cnt = 0;
    for(i = 0; i < G->numeroVertices; i++)
    {
        if(G->vertices[i]->numeroDeArestas != 0)
            cnt++;
    }
    return cnt;
}
/*********************************************************************************************
* CriaMST()
*
* Arguments:  ponteiro para Grafo
*
* Returns: ponteiro para Mst
*
* Description: Cria Arvore minima de suporte
**********************************************************************************************/

Mst *CriaMST(Grafo *G)
{
    Mst *msT = (Mst*) malloc(sizeof(Mst));
    msT->numArestas_mst = (verticesEfetivos(G)-1);
    if(msT->numArestas_mst < 0)
        return 0;
    msT->mst_table = (Aresta**) malloc((msT->numArestas_mst) * sizeof(Aresta*));
    msT->custo_mst = 0;

    return msT;
}

/*********************************************************************************************
* lessCost()
*
* Arguments:  dois ponteiros para Aresta
*
* Returns: inteiro
*
* Description: Compara o custo de duas arestas
**********************************************************************************************/

int lessCost(Aresta *a, Aresta *b)
{
    if (a->custo < b->custo)
        return 1;
    else
        return 0;
}

/*********************************************************************************************
* lessV()
*
* Arguments:  dois ponteiros para Aresta
*
* Returns: inteiro
*
* Description: Ordena as arestas com base no valor dos vertices
**********************************************************************************************/

int lessV(Aresta *a, Aresta *b)
{
    if (a->vertice1 == b->vertice1)
    {
        if(a->vertice2 < b->vertice2)
            return 1;
    }
    else if(a->vertice1 < b->vertice1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
    return 0;

}

/*********************************************************************************************
* partition()
*
* Arguments:  ponteiro para vetor de Arestas, dois inteiros, ponteiro para funcao de comparacao
*
* Returns: inteiro
*
* Description: Suporta o quicksort
**********************************************************************************************/

int partition(Aresta **a, int l, int r, int (*less) (Aresta*, Aresta*))
{
    int i = l - 1, j = r;
    Aresta *v = a[r];

    for (;;)
    {
        while (less(a[++i],v));
        while (less(v, a[--j]))
        {
            if (j == l)
            {
                break;
            }
        }
        if (i >= j)
        {
            break;
        }
        Aresta* p;
        p = a[i];
        a[i] = a[j];
        a[j] = p;
    }
    Aresta *q;
    q = a[i];
    a[i] = a[r];
    a[r] = q;

    return i;
}

/*********************************************************************************************
* quicksort()
*
* Arguments:  ponteiro para vetor de Arestas, dois inteiros, ponteiro para funcao de comparacao
*
* Returns: void
*
* Description: O quicksort e quem mais ordena
**********************************************************************************************/

void quicksort(Aresta **a, int l, int r, int (*less) (Aresta*, Aresta*))
{
    int i;

    if (r <= l)
    {
        return;
    }

    i = partition(a, l, r, less);
    quicksort(a, l, i - 1, less);
    quicksort(a, i + 1, r, less);

    return;
}

/*********************************************************************************************
* cwqu()
*
* Arguments:  ponteiro para Uniao, ponteiro para vetor de Arestas, inteiro
*
* Returns: void
*
* Description: Conecta vertices
**********************************************************************************************/

void cwqu(Uniao *U, int numArestas, Aresta **A)
{
    int i, j, p, q, t, x, n;

    for(n = 0; n < numArestas; n++)
    {
        p = A[n]->vertice1;
        q = A[n]->vertice2;

        for (i = p; i != U->id[i]; i = U->id[i]);
        for (j = q; j != U->id[j]; j = U->id[j]);

        if (i == j)
        {
            continue;
        }

        if (U->sz[i] < U->sz[j])
        {
            U->id[i] = j;
            U->sz[j] += U->sz[i];
            t = j;
        }
        else
        {
            U->id[j] = i;
            U->sz[i] += U->sz[j];
            t = i;
        }

        for (i = p; i != U->id[i]; i = x)
        {
            x = U->id[i];
            U->id[i] = t;
        }
        for (j = q; j != U->id[j]; j = x)
        {
            x = U->id[j];
            U->id[j] = t;
        }

    }

    return;
}

/*********************************************************************************************
* UFinit()
*
* Arguments:  ponteiro para Grafo
*
* Returns: ponteiro para Uniao
*
* Description: Inicializa Uniao
**********************************************************************************************/

Uniao *UFinit(Grafo *G)
{
    int i;
    Uniao *newUniao;
    newUniao = (Uniao *) malloc(sizeof(Uniao));
    newUniao->id = (int *) malloc((G->numeroVertices + 1) * sizeof(int));
    newUniao->sz = (int *) malloc((G->numeroVertices + 1) * sizeof(int));

    for(i = 0; i < (G->numeroVertices + 1); i++ )
    {
        newUniao->id[i] = i;
        newUniao->sz[i] = 1;
    }
    newUniao->raizesij[0] = 0;
    newUniao->raizesij[1] = 0;

    return newUniao;
}

/*********************************************************************************************
* UFind()
*
* Arguments:  ponteiro para Uniao, dois inteiros
*
* Returns: inteiro
*
* Description: Verifica a conectividade entre dois vertices
**********************************************************************************************/

int UFfind(Uniao *U, int p, int q)
{
    int i, j;

    for (i = p; i != U->id[i]; i = U->id[i]);
    for (j = q; j != U->id[j]; j = U->id[j]);

    U->raizesij[0] = i;
    U->raizesij[1] = j;

    if (i == j)
    {
        return 1;
    }

    return 0;
}

/*********************************************************************************************
* UFUnion()
*
* Arguments:  ponteiro para Uniao, dois inteiros
*
* Returns: inteiro
*
* Description: Conecta dois vertices
**********************************************************************************************/

int UFunion(Uniao *U, int p, int q)
{
    int i, j, t, x;

    i = U->raizesij[0];
    j = U->raizesij[1];

    if (U->sz[i] < U->sz[j])
    {
        U->id[i] = j;
        U->sz[j] += U->sz[i];
        t = j;
    }
    else
    {
        U->id[j] = i;
        U->sz[i] += U->sz[j];
        t = i;
    }

    for (i = p; i != U->id[i]; i = x)
    {
        x = U->id[i];
        U->id[i] = t;
    }
    for (j = q; j != U->id[j]; j = x)
    {
        x = U->id[j];
        U->id[j] = t;
    }

    return 0;
}

/*********************************************************************************************
* Arestas_Kr()
*
* Arguments:  ponteiro para Grafo, dois inteiros
*
* Returns: ponteiro para vetor de Arestas excluindo a aresta selecionada
*
* Description: Suporta Kruskal
**********************************************************************************************/

Aresta** Arestas_Kr(Grafo *G, int vi, int vj)
{
    int i, j = 0;
    Aresta **a = (Aresta**) malloc(G->numeroArestas * sizeof(Aresta*));
    Aresta *t;

    for(i = 0; i < G->numeroVertices; i++)
    {
        t = G->vertices[i]->arestas;
        if(G->vertices[i]->numeroDeArestas > 0 )
        {
            while(t != NULL)
            {
                if(t->vertice2 > t->vertice1 && !(t->vertice1 ==  vi && t->vertice2 == vj))
                {
                    a[j] = t;
                    j++;
                }
                t = t->next;
            }
        }
    }

    return a;
}

/*********************************************************************************************
* Kruskal()
*
* Arguments:  ponteiro para Grafo, ponteiro para Mst, dois inteiros
*
* Returns: void
*
* Description: Algoritmo Kruskal
**********************************************************************************************/

void Kruskal(Grafo *G, Mst *msT, int vi, int vj)
{
    int i, k;
    Uniao *U;

    if(vi < vj)
    {
        msT->a_table = Arestas_Kr(G,vi, vj);
    }
    else
    {
        msT->a_table = Arestas_Kr(G,vj, vi);
    }

    quicksort(msT->a_table, 0, G->numeroArestas - 1, lessCost);

    U = UFinit(G);
    msT->numArestas_mst = 0;

    for (i = 0, k = 0; i < G->numeroArestas && k < G->numeroVertices; i++)
    {
        if (!UFfind(U, msT->a_table[i]->vertice1, msT->a_table[i]->vertice2))
        {
            UFunion(U, msT->a_table[i]->vertice1, msT->a_table[i]->vertice2);
            msT->mst_table[k++] = msT->a_table[i];
            msT->numArestas_mst++;
            msT->custo_mst += msT->a_table[i]->custo;

        }
    }
    freeUniao(U);
    return;
}

/*********************************************************************************************
* ExisteArestaMST()
*
* Arguments: dois inteiros, ponteiro para Mst
*
* Returns: int
*
* Description: Verifica existencia de uma aresta na Mst
**********************************************************************************************/

int ExisteArestaMST(int vi, int vj, Mst *M)
{
    int j;

    if(vi < vj)
    {
        for(j = 0; j < M->numArestas_mst; j++)
        {
            if(vi == M->mst_table[j]->vertice1 && vj == M->mst_table[j]->vertice2 )
                return 1;
        }
    }
    else
    {
        for(j = 0; j < M->numArestas_mst; j++)
        {
            if(vj == M->mst_table[j]->vertice1 && vi == M->mst_table[j]->vertice2)
                return 1;
        }
    }

    return 0;
}

/*********************************************************************************************
* ListAddSubstitutas()
*
* Arguments: dois ponteiros para Aresta
*
* Returns: void
*
* Description: Adiciona aresta a lista de ArestasSubs
**********************************************************************************************/

void ListAddSubstitutas(Aresta *head, Aresta *novaAresta)
{
    novaAresta->next = head;
    head = novaAresta;
    return;
}

/*********************************************************************************************
* Kruskal_Substitutas()
*
* Arguments: dois inteiros, ponteiro para Grafo, ponteiro para Mst
*
* Returns: ponteiro para ArestasSubs
*
* Description: Algoritmo de Kruskal para estrutura de ArestasSubs
**********************************************************************************************/

ArestasSubs* Kruskal_Substitutas(Grafo *G, Mst *msT, int vi, int vj)
{
    int i, k;
    Uniao *U;
    ArestasSubs *Substitutas = (ArestasSubs*) malloc(sizeof(ArestasSubs));
    Substitutas->nUsadas = G->numeroArestas - msT->numArestas_mst;
    Substitutas->NaoUsadas = (Aresta **) malloc(Substitutas->nUsadas * sizeof(Aresta*));


    if(vi < vj)
    {
        msT->a_table = Arestas_Kr(G,vi, vj);
    }
    else
    {
        msT->a_table = Arestas_Kr(G,vj, vi);
    }

    quicksort(msT->a_table, 0, G->numeroArestas - 1, lessCost);

    U = UFinit(G);
    int n = 0;
    msT->numArestas_mst = 0;
    Substitutas->nUsadas = 0;

    for (i = 0, k = 0; i < G->numeroArestas && k < G->numeroVertices; i++)
    {
        if (!UFfind(U, msT->a_table[i]->vertice1, msT->a_table[i]->vertice2))
        {
            UFunion(U, msT->a_table[i]->vertice1, msT->a_table[i]->vertice2);
            msT->mst_table[k++] = msT->a_table[i];
            msT->numArestas_mst++;
            msT->custo_mst += msT->a_table[i]->custo;
        }
        else
        {
            Substitutas->NaoUsadas[n] = msT->a_table[i];
            n++;
            Substitutas->nUsadas++;
            /*ListAddSubstitutas(Substitutas->NaoUsadas, msT->a_table[i]);
            Substitutas->NaoUsadas = msT->a_table[i];
            */
        }
    }
    freeUniao(U);
    return Substitutas;
}

/*********************************************************************************************
* DescartaArestaMST()
*
* Arguments: dois inteiros, ponteiro para Mst
*
* Returns: ponteiro para vetor de Arestas
*
* Description: Cria uma copia da Mst sem a aresta introduzida
**********************************************************************************************/

Aresta **DescartaArestaMST(Mst *M, int vi, int vj)
{
    int i, j;
    Aresta **A = (Aresta**) malloc((M->numArestas_mst-1) * sizeof(Aresta*));

    if(vi < vj)
    {
        for(i = 0, j = 0; i < M->numArestas_mst - 1; i++, j++)
        {
            if(vi == M->mst_table[j]->vertice1 && vj == M->mst_table[j]->vertice2 )
            {
                j++;
                A[i] = M->mst_table[j];
            }
            else
            {
                A[i] = M->mst_table[j];
            }

        }
    }
    else
    {
        for(i = 0, j = 0; i < M->numArestas_mst - 1; i++, j++)
        {
            if(vj == M->mst_table[j]->vertice1 && vi == M->mst_table[j]->vertice2 )
            {
                j++;
                A[i] = M->mst_table[j];
            }
            else
            {
                A[i] = M->mst_table[j];
            }

        }
    }

    return A;
}

/*********************************************************************************************
* ocurrVi_MST()
*
* Arguments: inteiro, ponteiro para Mst
*
* Returns: inteiro
*
* Description: Conta o numero de arestas que contenham o vertice vi na Mst
**********************************************************************************************/

int ocurrVi_MST(Mst *M, int vi)
{
    int i, cnt = 0;
    for(i = 0; i < M->numArestas_mst; i++)
    {
        if(M->mst_table[i]->vertice1 == vi)
        {
            cnt++;
        }
        else if(M->mst_table[i]->vertice2 == vi)
        {
            cnt++;
        }

    }

    return cnt;
}

/*********************************************************************************************
* ocurrVi_Subs()
*
* Arguments: inteiro, ponteiro para ArestasSubs, ponteiro para vetor de Arestas
*
* Returns: inteiro
*
* Description: Conta o numero de arestas que contenham o vertice vi em ArestasSubs
**********************************************************************************************/

int ocurrVi_Subs(Aresta **TabelaSubstitutas, ArestasSubs *Substitutas, int vi)
{
    int i, cnt = 0;
    for(i = 0; i < Substitutas->nUsadas; i++)
    {
        if(TabelaSubstitutas[i]->vertice1 == vi)
        {
            cnt++;
        }
        else if(TabelaSubstitutas[i]->vertice2 == vi)
        {
            cnt++;
        }
    }

    return cnt;
}

/*********************************************************************************************
* DescartaViMST()
*
* Arguments: dois inteiros, ponteiro para Mst
*
* Returns: ponteiro para vetor de Arestas
*
* Description: Cria copia das arestas em Mst descartando arestas que contenham vi
**********************************************************************************************/

Aresta **DescartaViMST(Mst *M, int vi, int vi_mstCnt)
{
    int i, j;
    Aresta **CopiaMstVi = (Aresta**) malloc((M->numArestas_mst - vi_mstCnt) * sizeof(Aresta*));

    for(i = 0, j = 0; i < M->numArestas_mst - vi_mstCnt; i++, j++)
    {

        while(vi == M->mst_table[j]->vertice1 || vi == M->mst_table[j]->vertice2 )
        {
            j++;
        }

        CopiaMstVi[i] = M->mst_table[j];
    }

    return CopiaMstVi;
}

/*********************************************************************************************
* DescartaViSubstitutas()
*
* Arguments: dois inteiros, ponteiro para ArestasSubs, ponteiro para vetor de Arestas
*
* Returns: ponteiro para vetor de Arestas
*
* Description: Cria copia das arestas em ArestasSubs descartando arestas que contenham vi
**********************************************************************************************/

Aresta **DescartaViSubstitutas(ArestasSubs *Substitutas, Aresta **A, int vi, int vi_subsCnt)
{
    int i, j;

    Aresta **viExcluido = (Aresta**) malloc((Substitutas->nUsadas - vi_subsCnt) * sizeof(Aresta*));

    for(i = 0, j = 0; i < Substitutas->nUsadas - vi_subsCnt; i++, j++)
    {
        while(vi == A[j]->vertice1 || vi == A[j]->vertice2 )
        {
            j++;
        }

        viExcluido[i] = A[j];
    }

    return viExcluido;
}

/*********************************************************************************************
* ImprimeAlternativa()
*
* Arguments: dois inteiros, ponteiro para ficheiros,  ponteiro para ArestasSubs,
*            ponteiro para vetor de Arestas, ponteiro para Mst, ponteiro para Grafo
*
* Returns: void
*
* Description: Encontra e imprime a Aresta que substitui vi - vj na Mst
**********************************************************************************************/

void imprimeAlternativa(FILE *fpOut, Mst * msT,ArestasSubs *Substitutas, Aresta **TabelaSubstitutas, Grafo *G, int vi, int vj)
{
    int j, status = 0;

    Aresta **mst_tableCopia = DescartaArestaMST(msT, vi, vj);

    Uniao *U = UFinit(G);
    cwqu(U, msT->numArestas_mst - 1, mst_tableCopia);


    for(j = 0; j < Substitutas->nUsadas; j++)
    {
        if(!UFfind(U, TabelaSubstitutas[j]->vertice1, TabelaSubstitutas[j]->vertice2))
        {
            status = j + 1;
            break;
        }
    }

    if(status)
    {
        fprintf(fpOut," %d %d %.2lf\n", TabelaSubstitutas[status-1]->vertice1, TabelaSubstitutas[status-1]->vertice2, TabelaSubstitutas[status-1]->custo);
    }
    else
    {
        fprintf(fpOut," %d\n", -1);
    }

    free(mst_tableCopia);
    freeUniao(U);

    return;

}



