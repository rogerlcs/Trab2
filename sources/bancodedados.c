
#include "bancodedados.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

struct bancodedados
{
    Documento *docs;
    int n_docs;
    int tam_docs;
    Palavra *palavras;
    int n_palavras;
    int tam_palavras;
    char *pasta;
};


BancoDeDados bd_criar(char *pasta){
    BancoDeDados bd = (BancoDeDados)calloc(1, sizeof(struct bancodedados));
    bd->tam_docs = 200;
    bd->docs = (Documento *)calloc(bd->tam_docs, sizeof(Documento));
    bd->n_docs = 0;
    bd->tam_palavras = 5000;
    bd->palavras = (Palavra *)calloc(bd->tam_palavras, sizeof(Palavra));
    bd->n_palavras = 0;
    int i = strlen(pasta);
    while (pasta[i] != '/' && i > -1) {
        i--;
    }
    bd->pasta = (char*)calloc(i + 1, sizeof(char));
    for (int j = 0; j < i; j++) {
        bd->pasta[j] = pasta[j];
    }
    bd->pasta[i] = '\0';
    return bd;
}

BancoDeDados bd_carregar(char *filebd){
    FILE * fbd = fopen(filebd, "rb");

    if(!fbd){
        printf("Erro ao carregar o banco de dados.");
        exit(1);
    }

    int i = 0;
    Documento d = NULL;
    Palavra p = NULL;
    int n_folder = 0;
    int n_docs = 0;
    int n_palavras = 0;
    fread(&n_folder, 1, sizeof(int), fbd);
    char *pasta = (char*)calloc(n_folder, sizeof(char));
    BancoDeDados bd = bd_criar(pasta);
    fread(bd->pasta, n_folder, sizeof(char), fbd);
    fread(&n_docs, 1, sizeof(int), fbd);
    for(i = 0; i < n_docs; i++){
        d = documento_carregar(fbd);
        bd_adicionar_doc(bd, d);
    }
    fread(&n_palavras, 1, sizeof(int), fbd);
    for(i = 0; i < n_palavras; i++){
        p = palavra_carregar(fbd);
        bd_adicionar_palavra(bd, p);
        //imprimir_palavra(p);
    }
    free(pasta);
    fclose(fbd);
    return bd;
}

void le_palavras(Documento doc, BancoDeDados bd){
    FILE * fdoc;
    char *doc_txt = documento_obter_caminho(doc);
    char *caminho = (char *)calloc(strlen(bd->pasta) + strlen(doc_txt) + 1, sizeof(char));
    sprintf(caminho, "%s/%s", bd->pasta, doc_txt);
    fdoc = fopen(caminho, "r");
    if(!fdoc){
        printf("Nao foi possivel abrir o arquivo: %s", doc_txt);
    }
    else{
        char palavra[256];
        Palavra p = NULL;
        int idx = 0;
        while(!feof(fdoc)){
            fscanf(fdoc, "%s", palavra);
            idx = bd_possui_palavra(bd, palavra);
            if(idx == -1){
                p = palavra_criar(palavra);
                bd_adicionar_palavra(bd, p);
            }
            else{
                p = bd_obter_palavra_indice(bd, idx);
            }
            if(p != NULL){
                palavra_add_tabela(p, bd_get_n_docs(bd)-1, 1);
            }
        }
        fclose(fdoc);
    }
}

void bd_adicionar_doc(BancoDeDados bd, Documento doc){
    if(bd->n_docs >= bd->tam_docs){
        bd->tam_docs += 200;
        bd->docs = (Documento*)realloc(bd->docs, bd->tam_docs * sizeof(Documento));
    }
    bd->docs[bd->n_docs] = doc;
    bd->n_docs += 1;
}

int bd_possui_palavra(BancoDeDados bd, char *palavra){
    for(int i = 0; i < bd->n_palavras; i++){
        if(!compara_palavra_string(palavra, &bd->palavras[i])){
            return i;
        }
    }
    return -1;
}

Palavra bd_obter_palavra(BancoDeDados bd, char *palavra){
    Palavra *p = bsearch(palavra, bd->palavras, bd->n_palavras, sizeof(Palavra), compara_palavra_string);
    if(p != NULL){
        return *p;
    }
    return NULL;
}

Palavra bd_obter_palavra_indice(BancoDeDados bd, int idx){
    if(idx < 0 || idx >= bd->n_palavras){
        return NULL;
    }
    return bd->palavras[idx];
}

int bd_get_n_docs(BancoDeDados bd){
    return bd->n_docs;
}


void bd_adicionar_palavra(BancoDeDados bd, Palavra p){
    if(bd->n_palavras >= bd->tam_palavras){
        bd->tam_palavras += 200;
        bd->palavras = (Palavra *)realloc(bd->palavras, bd->tam_palavras * sizeof(Palavra));
    }
    bd->palavras[bd->n_palavras] = p;
    bd->n_palavras += 1;
}

int bd_get_n_palavras(BancoDeDados bd){
    return bd->n_palavras;
}

Documento bd_obter_doc_indice(BancoDeDados bd, int idx){
    if(idx < 0 || idx >= bd->n_docs){
        return NULL;
    }
    return bd->docs[idx];
}

void bd_organizar_palavras(BancoDeDados bd){
    qsort(bd->palavras, bd->n_palavras, sizeof(Palavra), compara_palavras);
}

void bd_salvar(BancoDeDados bd, char *nomearq){
    FILE * fbd = fopen(nomearq, "wb");
    int n_docs = bd_get_n_docs(bd);
    int n_palavras = bd_get_n_palavras(bd);
    int i = 0;
    Documento d = NULL;
    Palavra p = NULL;
    int n_folder = strlen(bd->pasta) + 1;
    fwrite(&n_folder, 1, sizeof(int), fbd);
    fwrite(bd->pasta, n_folder, sizeof(char), fbd);
    fwrite(&n_docs, 1, sizeof(int), fbd);
    for(i = 0; i < n_docs; i++){
        d = bd_obter_doc_indice(bd, i);
        documento_salvar(d, fbd);
    }
    fwrite(&n_palavras, 1, sizeof(int), fbd);
    for(i = 0; i < n_palavras; i++){
        p = bd_obter_palavra_indice(bd, i);
        imprimir_palavra(p);
        //printf(" ");
        palavra_salvar(p, fbd);
    }
    fclose(fbd);

}

void bd_destruir(BancoDeDados bd){
    int i = 0;
    for(i = 0; i < bd_get_n_docs(bd); i++){
        documento_destroi(bd_obter_doc_indice(bd, i));
    }
    free(bd->docs);
    for(i = 0; i < bd_get_n_palavras(bd); i++){
        palavra_destruir(bd_obter_palavra_indice(bd, i));
    }
    free(bd->palavras);
    free(bd->pasta);
    free(bd);
}

void calcula_tfidf(BancoDeDados bd){
    int n_palavras = bd_get_n_palavras(bd);
    int freq = 0;
    float idf_p = 0;
    float idf_d = 0;
    Palavra p = NULL;
    Tabela t_pal = NULL;
    Tabela t_doc = NULL;
    Documento d = NULL;
    for(int i = 0; i < n_palavras; i++){
        p = bd_obter_palavra_indice(bd, i);
        idf_p = log((1 + bd_get_n_docs(bd))/(float)(1 +palavra_get_n_docs(p))) + 1;
        palavra_calc_tfidf(p, idf_p);
        for(int j = 0; j < palavra_get_n_docs(p); j++){
            t_pal = palavra_get_tab(p, j);
            d = bd_obter_doc_indice(bd, tabela_get_idx(t_pal));
            freq = tabela_get_frequencia(t_pal);
            for(int k = 0; k < freq; k++){
                documento_add_tabela(d, i, 1);
            }
            idf_d = log((1 + n_palavras)/(float)(1 + documento_get_n_palavras(d))) + 1;
            documento_calc_tfidf(d, idf_d);
        }
    }
}



