#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"
#include "backbones.h"

int n_vertices, n_arestas, vi, vj;
char modo[3], A0[3] = {"A0"}, B0[3] = {"B0"}, C0[3] = {"C0"}, D0[3] = {"D0"},A1[3] = {"A1"}, B1[3] = {"B1"}, C1[3] = {"C1"}, D1[3] = {"D1"}, E1[3] = {"E1"};

/*********************************************************************************************
* verifica_ext()
*
* Arguments: duas strings
*
* Returns: inteiro
*
* Description: Valida a extensao do ficheiro de entrada
**********************************************************************************************/

int verifica_ext(char* nomeficheiroIn, char* ext_correta)
{
    char* extensao;
    extensao = strrchr(nomeficheiroIn, '.');
    if (extensao != NULL)
    {
        if (!(strcmp(extensao,ext_correta)))
            return 1;
    }
    return 0;
}

/*********************************************************************************************
* remove_ext()
*
* Arguments: duas strings
*
* Returns: uma string
*
* Description: Cria string com a extensao do ficheiro de saida
**********************************************************************************************/

char *remove_ext(char *nomeficheiroIn, char *nomeficheiroOut)
{
    char *ext;
    if (nomeficheiroIn == NULL)
        return NULL;

    strcpy (nomeficheiroOut, nomeficheiroIn);
    ext = strrchr (nomeficheiroOut, '.');
    if (ext != NULL)
        *ext = '\0';

    return nomeficheiroOut;
}

/*********************************************************************************************
* OpenFile()
*
* Arguments: duas strings
*
* Returns: ponteiro para ficheiro
*
* Description: Abre ficheiro para leitura ou escrita
**********************************************************************************************/

FILE *OpenFile(char *name, char *mode)
{
    FILE *fp;

    fp = fopen(name, mode);
    if (fp == NULL)
    {
        exit(0);
    }
    return fp;
}

/*********************************************************************************************
* ValidaModo()
*
* Arguments: string
*
* Returns: int
*
* Description: Valida modo no cabecalho do problema
**********************************************************************************************/

int ValidaModo(char *modo)
{
    if(!(strcmp(modo,A1)))
    {
        return 1;
    }
    else if(!(strcmp(modo,B1)))
    {
       return 1;
    }
    else if(!(strcmp(modo,C1)))
    {
       return 1;
    }
    else if(!(strcmp(modo,D1)))
    {
       return 1;
    }
    else if(!(strcmp(modo,E1)))
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
* ReadCabecalho()
*
* Arguments: ponteiro para ficheiro
*
* Returns: ponteiro para ficheiro
*
* Description: Retira os dados do cabecalho do problema
**********************************************************************************************/

FILE *ReadCabecalho(FILE * fp)
{
    if(fscanf(fp, "%d", &n_vertices) == EOF )
        return NULL;
    if(fscanf(fp, "%d", &n_arestas)==1);
    if(fscanf(fp,"%s", modo)==1);

    if(!(ValidaModo(modo)))
        return NULL;

    if((strcmp(modo, A0) == 0) || (strcmp(modo, B0) == 0)|| (strcmp(modo, C0) == 0)|| (strcmp(modo, D0) == 0) || (strcmp(modo, B1) == 0) || (strcmp(modo, C1) == 0) || (strcmp(modo, D1) == 0))
    {
        if(fscanf(fp,"%d", &vi)==1);
    }
    if((strcmp(modo, B0) == 0)||(strcmp(modo, B1) == 0) || (strcmp(modo, C1) == 0))
    {
        if(fscanf(fp,"%i", &vj)==1);
    }

    return fp;
}

double modoB0(Grafo *G, int vi, int vj)
{
    int v1,v2;

    if(vi == vj)
        return -1;

    v1 = G->vertices[vi-1]->numeroDeArestas;
    v2 = G->vertices[vj-1]->numeroDeArestas;

    if(v1 < v2)
    {
        Aresta *t;

        for(t = G->vertices[vi-1]->arestas; t != NULL; t = t->next)
        {
            if(t->vertice2 == vj)
                return t->custo;
        }
    }
    else
    {
        Aresta *t;

        for(t = G->vertices[vj-1]->arestas; t != NULL; t = t->next)
        {
            if(t->vertice2 == vi)
            {
                return t->custo;
            }

        }
    }
    return -1;

}

int modoC0(int vi, Grafo *G)
{
    Aresta *temp1, *temp2, *temp3;
    int number = 0;

    temp1 = G->vertices[vi-1]->arestas;
    if(temp1 == NULL)
        return -1;

    while(temp1 != NULL)
    {
        number = temp1->vertice2;
        temp2 = G->vertices[number-1]->arestas;
        temp1 = temp1->next;

        while(temp2 != NULL)
        {

            number = temp2->vertice2;
            temp3 = G->vertices[number-1]->arestas;
            temp2 = temp2->next;

            while(temp3 != NULL)
            {
                if(temp3->vertice2 == vi)
                    return 1;
                temp3 = temp3->next;
            }
        }


    }
    return 0;
}

int modoD0(int vi, Grafo *G)
{
    Aresta *temp1, *temp2, *temp3;
    int number = 0, cnt = 0;

    temp1 = G->vertices[vi-1]->arestas;
    if(temp1 == NULL)
        return -1;

    while(temp1 != NULL)
    {
        number = temp1->vertice2;
        temp2 = G->vertices[number-1]->arestas;
        temp1 = temp1->next;

        while(temp2 != NULL)
        {

            number = temp2->vertice2;
            temp3 = G->vertices[number-1]->arestas;
            temp2 = temp2->next;

            while(temp3 != NULL)
            {
                if(temp3->vertice2 == vi)
                    cnt++;
                temp3 = temp3->next;
            }
        }
    }
    return cnt/2;
}

/*********************************************************************************************
* main()
*
* Arguments: argc, argv
*
* Returns: Sucesso ou erro
*
* Description: Encontra rotas aereas mais baratas
**********************************************************************************************/

int main( int argc, char **argv)
{
    Grafo* G;

    if(argc < 2)
        return 0;

    char *nomeficheiroOut;
    char extIn[] = {".routes"}, extOut[] = {".bbones"};
    FILE *fpIn, *fpOut;

    if(!(verifica_ext(argv[1],extIn)))
    {
        return 0;
    }

    nomeficheiroOut = (char *) malloc (sizeof(char) * (strlen(argv[1]) + 1) );
    nomeficheiroOut = remove_ext(argv[1],nomeficheiroOut);
    strcat(nomeficheiroOut, extOut);

    fpIn = OpenFile(argv[1],"r");
    if(fpIn == NULL)
    {
        return 0;
    }

    fpOut = OpenFile(nomeficheiroOut, "w");
    if(fpOut == NULL)
    {
        return 0;
    }

    int realizouLeitura = 0;

    while(ReadCabecalho(fpIn) != NULL)
    {
        realizouLeitura = 1;

        G = CriaGrafo(n_vertices, n_arestas, fpIn);

        if(!(strcmp(modo,A1)))
        {
            Mst *msT = CriaMST(G);
            int j;

            Kruskal(G, msT, 0, 0);

            quicksort(msT->mst_table, 0, msT->numArestas_mst - 1, lessV);

            fprintf(fpOut,"%d %d %s %d %.2lf\n",n_vertices, n_arestas, modo, msT->numArestas_mst, msT->custo_mst);
            for(j = 0; j < msT->numArestas_mst; j++)
                fprintf(fpOut,"%d %d %.2lf\n", msT->mst_table[j]->vertice1, msT->mst_table[j]->vertice2, msT->mst_table[j]->custo);

            fprintf(fpOut,"\n");
            freeMST(msT);
            freeGrafo(G);
        }
        else if(!(strcmp(modo,B1)))
        {
            if((vi == vj) || vi < 1 || vj < 1 || n_vertices < vi || n_vertices < vj )
            {
                Mst *msT = CriaMST(G);
                int j;

                Kruskal(G, msT, 0, 0);

                quicksort(msT->mst_table, 0, msT->numArestas_mst - 1, lessV);

                fprintf(fpOut,"%d %d %s %d %d %d %.2lf %d\n",n_vertices, n_arestas, modo, vi,vj, msT->numArestas_mst, msT->custo_mst, -1);
                for(j = 0; j < msT->numArestas_mst; j++)
                    fprintf(fpOut,"%d %d %.2lf\n", msT->mst_table[j]->vertice1, msT->mst_table[j]->vertice2, msT->mst_table[j]->custo);

                fprintf(fpOut,"\n");
                freeMST(msT);
                freeGrafo(G);

            }
            else if((G->vertices[vj-1]->numeroDeArestas < 2 || G->vertices[vi-1]->numeroDeArestas < 2) || (modoB0(G, vi, vj) == -1))
            {
                Mst *msT = CriaMST(G);
                int j;

                Kruskal(G, msT, 0, 0);

                quicksort(msT->mst_table, 0, msT->numArestas_mst - 1, lessV);

                fprintf(fpOut,"%d %d %s %d %d %d %.2lf %d\n",n_vertices, n_arestas, modo, vi,vj, msT->numArestas_mst, msT->custo_mst, -1);
                for(j = 0; j < msT->numArestas_mst; j++)
                    fprintf(fpOut,"%d %d %.2lf\n", msT->mst_table[j]->vertice1, msT->mst_table[j]->vertice2, msT->mst_table[j]->custo);

                fprintf(fpOut,"\n");
                freeMST(msT);
                freeGrafo(G);

            }
            else
            {
                Mst *msT = CriaMST(G);
                int j, status = -1;
                ArestasSubs *Substitutas = Kruskal_Substitutas(G, msT, 0, 0);
                Aresta **TabelaSubstitutas = Substitutas->NaoUsadas;

                quicksort(msT->mst_table, 0, msT->numArestas_mst - 1, lessV);

                if(ExisteArestaMST(vi, vj, msT))
                {
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
                        fprintf(fpOut,"%d %d %s %d %d %d %.2lf %d\n",n_vertices, n_arestas, modo, vi,vj, msT->numArestas_mst, msT->custo_mst, 1);
                        for(j = 0; j < msT->numArestas_mst; j++)
                            fprintf(fpOut,"%d %d %.2lf\n", msT->mst_table[j]->vertice1, msT->mst_table[j]->vertice2, msT->mst_table[j]->custo);

                        fprintf(fpOut,"%d %d %.2lf\n", TabelaSubstitutas[status-1]->vertice1, TabelaSubstitutas[status-1]->vertice2, TabelaSubstitutas[status-1]->custo);

                        fprintf(fpOut,"\n");
                    }
                    else
                    {
                        fprintf(fpOut,"%d %d %s %d %d %d %.2lf %d\n",n_vertices, n_arestas, modo, vi,vj, msT->numArestas_mst, msT->custo_mst, status);
                        for(j = 0; j < msT->numArestas_mst; j++)
                            fprintf(fpOut,"%d %d %.2lf\n", msT->mst_table[j]->vertice1, msT->mst_table[j]->vertice2, msT->mst_table[j]->custo);

                        fprintf(fpOut,"\n");
                    }

                    free(mst_tableCopia);
                    freeUniao(U);
                }
                else
                {
                    fprintf(fpOut,"%d %d %s %d %d %d %.2lf %d\n",n_vertices, n_arestas, modo, vi,vj, msT->numArestas_mst, msT->custo_mst, 0);
                    for(j = 0; j < msT->numArestas_mst; j++)
                        fprintf(fpOut,"%d %d %.2lf\n", msT->mst_table[j]->vertice1, msT->mst_table[j]->vertice2, msT->mst_table[j]->custo);

                    fprintf(fpOut,"\n");
                }

                free(Substitutas);
                free(TabelaSubstitutas);
                freeMST(msT);
                freeGrafo(G);
            }
        }
        else if(!(strcmp(modo,C1)))
        {
            if((vi == vj) || vi < 1 || vj < 1 || n_vertices < vi || n_vertices < vj )
            {
                Mst *msT = CriaMST(G);
                int j;

                Kruskal(G, msT, 0, 0);

                quicksort(msT->mst_table, 0, msT->numArestas_mst - 1, lessV);


                fprintf(fpOut,"%d %d %s %d %d %d %.2lf %d\n",n_vertices, n_arestas, modo, vi,vj, msT->numArestas_mst, msT->custo_mst, -1);
                for(j = 0; j < msT->numArestas_mst; j++)
                    fprintf(fpOut,"%d %d %.2lf\n", msT->mst_table[j]->vertice1, msT->mst_table[j]->vertice2, msT->mst_table[j]->custo);

                fprintf(fpOut,"\n");
                freeMST(msT);
                freeGrafo(G);
            }
            else
            {
                Mst *msT = CriaMST(G);
                int j;

                Kruskal(G, msT,0,0);

                quicksort(msT->mst_table, 0, msT->numArestas_mst - 1, lessV);

                if(ExisteArestaMST(vi, vj, msT))
                {
                    Mst *msT2 = CriaMST(G);

                    G->numeroArestas--;


                    Kruskal(G, msT2, vi, vj);

                    quicksort(msT2->mst_table, 0, msT2->numArestas_mst - 1, lessV);


                    fprintf(fpOut,"%d %d %s %d %d %d %.2lf %d %.2lf\n",n_vertices, n_arestas, modo, vi,vj, msT->numArestas_mst, msT->custo_mst, msT2->numArestas_mst, msT2->custo_mst);

                    for(j = 0; j < msT->numArestas_mst; j++)
                        fprintf(fpOut,"%d %d %.2lf\n", msT->mst_table[j]->vertice1, msT->mst_table[j]->vertice2, msT->mst_table[j]->custo);


                    for(j = 0; j < msT2->numArestas_mst; j++)
                        fprintf(fpOut,"%d %d %.2lf\n", msT2->mst_table[j]->vertice1, msT2->mst_table[j]->vertice2, msT2->mst_table[j]->custo);

                    fprintf(fpOut,"\n");

                    freeMST(msT);
                    freeMST(msT2);
                }
                else
                {
                    fprintf(fpOut,"%d %d %s %d %d %d %.2lf %d\n",n_vertices, n_arestas, modo, vi,vj, msT->numArestas_mst, msT->custo_mst, -1);
                    for(j = 0; j < msT->numArestas_mst; j++)
                        fprintf(fpOut,"%d %d %.2lf\n", msT->mst_table[j]->vertice1, msT->mst_table[j]->vertice2, msT->mst_table[j]->custo);

                    fprintf(fpOut,"\n");
                    freeMST(msT);
                }

                freeGrafo(G);
            }

        }
        else if(!(strcmp(modo,D1)))
        {
            if(vi < 1 || n_vertices < vi)
            {
                Mst *msT = CriaMST(G);
                int j;

                Kruskal(G, msT, 0, 0);

                quicksort(msT->mst_table, 0, msT->numArestas_mst - 1, lessV);


                fprintf(fpOut,"%d %d %s %d %d %.2lf %d\n",n_vertices, n_arestas, modo, vi, msT->numArestas_mst, msT->custo_mst, -1);
                for(j = 0; j < msT->numArestas_mst; j++)
                    fprintf(fpOut,"%d %d %.2lf\n", msT->mst_table[j]->vertice1, msT->mst_table[j]->vertice2, msT->mst_table[j]->custo);

                fprintf(fpOut,"\n");
                freeMST(msT);
                freeGrafo(G);

            }
            else if(G->vertices[vi - 1]->numeroDeArestas == 0)
            {
                Mst *msT = CriaMST(G);
                int j;

                Kruskal(G, msT, 0, 0);

                quicksort(msT->mst_table, 0, msT->numArestas_mst - 1, lessV);


                fprintf(fpOut,"%d %d %s %d %d %d %.2lf %d\n",n_vertices, n_arestas, modo, vi,vj, msT->numArestas_mst, msT->custo_mst, -1);
                for(j = 0; j < msT->numArestas_mst; j++)
                    fprintf(fpOut,"%d %d %.2lf\n", msT->mst_table[j]->vertice1, msT->mst_table[j]->vertice2, msT->mst_table[j]->custo);

                fprintf(fpOut,"\n");
                freeMST(msT);
                freeGrafo(G);
            }
            else
            {
                Mst *msT = CriaMST(G);
                int j, vi_subsCnt = 0, vi_mstCnt = 0;

                ArestasSubs *Substitutas = Kruskal_Substitutas(G, msT, 0, 0);
                Aresta **TabelaSubstitutas = Substitutas->NaoUsadas;

                vi_subsCnt = ocurrVi_Subs(TabelaSubstitutas, Substitutas, vi);

                if((Substitutas->nUsadas - vi_subsCnt) < 1)
                {
                    quicksort(msT->mst_table, 0, msT->numArestas_mst - 1, lessV);

                    fprintf(fpOut,"%d %d %s %d %d %.2lf %d\n",n_vertices, n_arestas, modo, vi, msT->numArestas_mst, msT->custo_mst, 0);
                    for(j = 0; j < msT->numArestas_mst; j++)
                        fprintf(fpOut,"%d %d %.2lf\n", msT->mst_table[j]->vertice1, msT->mst_table[j]->vertice2, msT->mst_table[j]->custo);

                    fprintf(fpOut,"\n");
                }
                else
                {
                    quicksort(msT->mst_table, 0, msT->numArestas_mst - 1, lessV);

                    vi_mstCnt = ocurrVi_MST(msT, vi);
                    Aresta **Copia_TabelaMST = DescartaViMST(msT, vi, vi_mstCnt);
                    Aresta **Copia_TabelaSubstitutas = DescartaViSubstitutas(Substitutas, TabelaSubstitutas, vi, vi_subsCnt);

                    Uniao *U = UFinit(G);
                    cwqu(U, msT->numArestas_mst - vi_mstCnt, Copia_TabelaMST);

                    int i = 0;

                    Aresta **Alternativas = (Aresta **) malloc((Substitutas->nUsadas - vi_subsCnt) * sizeof(Aresta*));

                    for(j = 0, i = 0; j < Substitutas->nUsadas - vi_subsCnt; j++)
                    {
                        if(!UFfind(U, Copia_TabelaSubstitutas[j]->vertice1, Copia_TabelaSubstitutas[j]->vertice2))
                        {
                            Alternativas[i] = Copia_TabelaSubstitutas[j];
                            i++;
                            UFunion(U, Copia_TabelaSubstitutas[j]->vertice1, Copia_TabelaSubstitutas[j]->vertice2);
                        }

                    }
                    if(i)
                    {
                        fprintf(fpOut,"%d %d %s %d %d %.2lf %d\n",n_vertices, n_arestas, modo, vi, msT->numArestas_mst, msT->custo_mst, i);
                        for(j = 0; j < msT->numArestas_mst; j++)
                            fprintf(fpOut,"%d %d %.2lf\n", msT->mst_table[j]->vertice1, msT->mst_table[j]->vertice2, msT->mst_table[j]->custo);

                        quicksort(Alternativas, 0, i-1, lessV);

                        for(j = 0; j < i; j++)
                            fprintf(fpOut,"%d %d %.2lf\n",Alternativas[j]->vertice1, Alternativas[j]->vertice2, Alternativas[j]->custo);

                        fprintf(fpOut,"\n");
                    }
                    else
                    {
                        fprintf(fpOut,"%d %d %s %d %d %.2lf %d\n",n_vertices, n_arestas, modo, vi, msT->numArestas_mst, msT->custo_mst, 0);
                        for(j = 0; j < msT->numArestas_mst; j++)
                            fprintf(fpOut,"%d %d %.2lf\n", msT->mst_table[j]->vertice1, msT->mst_table[j]->vertice2, msT->mst_table[j]->custo);

                        fprintf(fpOut,"\n");
                    }

                    free(Copia_TabelaMST);
                    free(Copia_TabelaSubstitutas);
                    free(Alternativas);
                    freeUniao(U);
                }
                free(TabelaSubstitutas);
                free(Substitutas);
                freeMST(msT);
                freeGrafo(G);
            }
        }
        else if(!(strcmp(modo,E1)))
        {
            Mst *msT = CriaMST(G);
            int j;
            ArestasSubs *Substitutas = Kruskal_Substitutas(G, msT, 0, 0);
            Aresta **TabelaSubstitutas = Substitutas->NaoUsadas;

            quicksort(msT->mst_table, 0, msT->numArestas_mst - 1, lessV);

            fprintf(fpOut,"%d %d %s %d %.2lf\n",n_vertices, n_arestas, modo, msT->numArestas_mst, msT->custo_mst);

            for(j = 0; j < msT->numArestas_mst; j++)
            {
                fprintf(fpOut,"%d %d %.2lf", msT->mst_table[j]->vertice1, msT->mst_table[j]->vertice2, msT->mst_table[j]->custo);
                imprimeAlternativa(fpOut,msT,Substitutas, TabelaSubstitutas,G, msT->mst_table[j]->vertice1, msT->mst_table[j]->vertice2);
            }
            fprintf(fpOut,"\n");

            free(TabelaSubstitutas);
            free(Substitutas);
            freeMST(msT);
            freeGrafo(G);
        }
        else if(!(strcmp(modo,A0)))
        {
            if(n_vertices < vi || vi < 1)
            {
                int err = -1;
                fprintf(fpOut,"%d %d %s %d %d\n\n",n_vertices, n_arestas, modo, vi, err);
                //freeGrafo(G);
            }
            else
            {

                fprintf(fpOut,"%d %d %s %d %d\n\n",n_vertices, n_arestas, modo, vi, G->vertices[vi-1]->numeroDeArestas );
                //freeGrafo(G);
            }
        }
        else if(!(strcmp(modo,B0)))
        {
            if(n_vertices < vi || n_vertices < vj || vi < 1 || vj < 1)
            {
                int err = -1;
                fprintf(fpOut,"%d %d %s %d %d %d\n\n",n_vertices, n_arestas, modo, vi,vj, err);
                //freeGrafo(G);
            }
            else
            {
                double B = modoB0(G, vi, vj);
                if(B == -1)
                {
                    int err = -1;
                    fprintf(fpOut,"%d %d %s %d %d %d\n\n",n_vertices, n_arestas, modo, vi,vj, err);
                    //freeGrafo(G);
                }
                else
                {
                    fprintf(fpOut,"%d %d %s %d %d %.2lf\n\n",n_vertices, n_arestas, modo, vi,vj, B);
                    //freeGrafo(G);
                }
            }
        }
        else if(!(strcmp(modo,C0)))
        {

            if(n_vertices < vi || vi < 1)
            {
                int err = -1;
                fprintf(fpOut,"%d %d %s %d %d\n\n",n_vertices, n_arestas, modo, vi, err);
                //freeGrafo(G);
            }
            else
            {
                fprintf(fpOut,"%d %d %s %d %d\n\n",n_vertices, n_arestas, modo, vi, modoC0(vi, G));
                //freeGrafo(G);
            }
        }
        else if(!(strcmp(modo,D0)))
        {
            if(n_vertices < vi || vi < 1)
            {
                int err = -1;
                fprintf(fpOut,"%d %d %s %d %d\n\n",n_vertices, n_arestas, modo, vi, err);
                //freeGrafo(G);
            }
            else
            {
                fprintf(fpOut,"%d %d %s %d %d\n\n",n_vertices, n_arestas, modo, vi, modoD0(vi, G));
                //freeGrafo(G);
            }
        }

    }

    fclose(fpIn);
    fclose(fpOut);

    if(!realizouLeitura)
       remove(nomeficheiroOut);


    free(nomeficheiroOut);

    return 0;
}

