#include "documento.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct documento
{
    char *caminho;
    char *classe;
    Tabela *tabela;
    int tam_tabela;
    int n_palavras;
};

Documento documento_cria(char *caminho, char *classe){
    Documento d = (Documento)calloc(1, sizeof(struct documento));
    d->caminho = strdup(caminho);
    d->classe = strdup(classe);
    int n_palavras = 0;
    int tam_tabela = 100;
    d->tabela = (Tabela *)calloc(d->tam_tabela, sizeof(Tabela));
    return d;
}

char * documento_obter_caminho(Documento d){
    return d->caminho;
}

char * documento_obter_classe(Documento d){
    return d->classe;
}


int documento_compara_numerop(const void *d1, const void *d2){
    Documento doc1 = *(Documento *)d1;
    Documento doc2 = *(Documento *)d2;
    return -1 * (doc1->n_palavras - doc2->n_palavras);
}

void documento_add_tabela(Documento d, int idx_pal, int verify){
    int achou = -1;
    if(verify){
        achou = tabelas_possui_idx(d->tabela, idx_pal, d->n_palavras);
    }
    if(achou == -1){
        if(d->n_palavras >= d->tam_tabela){
            d->tam_tabela += 200;
            d->tabela = (Tabela *)realloc(d->tabela, d->tam_tabela * sizeof(Tabela));
        }
        d->tabela[d->n_palavras] = tabela_criar(idx_pal);
        d->n_palavras += 1;
    }
    else{
        tabela_add_freq(documento_obter_tabela(d, achou));
    }
}

void documento_imprimir(Documento d){
    printf("Arquivo:%s\n", d->caminho);
    printf("Classe: %s\n", d->classe);
    printf("Quantidade de palavras: %d\n", d->n_palavras);
}

Tabela documento_obter_tabela(Documento d, int idx){
    if(idx < 0 || idx > d->n_palavras){
        return NULL;
    }
    return d->tabela[idx];
}
int documento_get_n_palavras(Documento d){
    return d->n_palavras;
}

void documento_calc_tfidf(Documento d, float idf){
    for(int i =0; i < documento_get_n_palavras(d); i++){
        tabela_calc_tfidf(d->tabela[i], idf);
    }
}

void documento_salvar(Documento d, FILE * fdoc){
    int n = strlen(d->caminho) + 1;
    fwrite(&n, 1, sizeof(int), fdoc);
    fwrite(d->caminho, n, sizeof(char), fdoc);
    int g = strlen(d->classe) + 1;
    fwrite(&g, 1, sizeof(int), fdoc);
    fwrite(d->classe, g, sizeof(char), fdoc);
    fwrite(&d->n_palavras, 1, sizeof(int), fdoc);
    for(int i = 0; i < d->n_palavras; i++){
        tabela_salvar(d->tabela[i], fdoc);
    }
}

Documento documento_carregar(FILE * fdoc){
    int n = 0;
    int n_palavras = 0;
    fread(&n, 1, sizeof(int), fdoc);
    char *caminho = (char *)calloc(n, sizeof(char));
    fread(caminho, n, sizeof(char), fdoc);
    fread(&n, 1, sizeof(int), fdoc);
    char *classe = (char *)calloc(n, sizeof(char));
    fread(classe, n, sizeof(char), fdoc);
    Documento d = documento_cria(caminho, classe);
    fread(&n_palavras, 1, sizeof(int), fdoc);
    for(int i = 0; i < n_palavras; i++){
        fread(&n, 1, sizeof(int), fdoc);
        documento_add_tabela(d, n, 0);
        tabela_carregar(d->tabela[i], fdoc);
    }
    free(caminho);
    free(classe);
    return d;
}

void documento_destroi(Documento d){
    free(d->caminho);
    free(d->classe);
    for(int i = 0; i < d->n_palavras; i++){
        tabela_destruir(d->tabela[i]);
    }
    free(d->tabela);
    free(d);
}
