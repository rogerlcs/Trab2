
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
    bd->tam_docs = 100;
    bd->docs = (Documento *)calloc(bd->tam_docs, sizeof(Documento));
    bd->n_docs = 0;
    bd->tam_palavras = 100;
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
    char *pasta = (char*)calloc(n_folder + 1, sizeof(char));
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
    char *caminho = (char *)calloc(strlen(bd->pasta) + strlen(doc_txt) + 2, sizeof(char));
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
    free(caminho);
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


void bd_organizar_documentos(BancoDeDados bd){
    qsort(bd->docs, bd->n_docs, sizeof(Documento), documento_compara_numerop);
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
        //imprimir_palavra(p);
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
    int freq = 0;
    float idf_p = 0;
    float idf_d = 0;
    Palavra p = NULL;
    Tabela t_pal = NULL;
    Tabela t_doc = NULL;
    Documento d = NULL;
    for(int i = 0; i < bd_get_n_palavras(bd); i++){
        p = bd_obter_palavra_indice(bd, i);
        calcular_tfidf_palavra(bd, p, bd_get_n_docs(bd));
        for(int j = 0; j < palavra_get_n_docs(p); j++){
            t_pal = palavra_get_tab(p, j);
            d = bd_obter_doc_indice(bd, tabela_get_idx(t_pal));
            freq = tabela_get_frequencia(t_pal);
            for(int k = 0; k < freq; k++){
                documento_add_tabela(d, i, 1);
            }
            calcular_tfidf_documento(bd, d, bd_get_n_palavras(bd));
        }
    }
}

float calcular_idf(int n1, int d1){
    return log((1 + n1)/(float)(1 + d1) + 1);
}

void calcular_tfidf_palavra(BancoDeDados bd, Palavra p, int n_docs){
    float idf_p = 0;
    idf_p = calcular_idf(n_docs, palavra_get_n_docs(p));
    palavra_calc_tfidf(p, idf_p);
}

void calcular_tfidf_documento(BancoDeDados bd, Documento d, int n_palavras){
    float idf_d = 0;
    idf_d = calcular_idf(n_palavras, documento_get_n_palavras(d));
    documento_calc_tfidf(d, idf_d);
}


char * bd_doc_calc_classe(BancoDeDados bd, BancoDeDados bd2, int k){
    Tabela t = NULL;
    Tabela t2 = NULL;
    Palavra p = NULL;
    Palavra p2 = NULL;
    Palavra p3 = NULL;
    Documento d = NULL;
    int doc_id = 0, i = 0;
    int tam_cos = k;
    Tabela *pcos = (Tabela *)calloc(tam_cos, sizeof(Tabela));
    int n_cos = 0;
    int n_docs = 0;
    float numerador = 0, denominador1 = 0, idf = 0, tfidf1 = 0, tfidf2 = 0, denominador2 = 0, cos = 0;
    for(i = 0; i < bd_get_n_palavras(bd2); i++){
        p = bd_obter_palavra_indice(bd2, i);
        n_docs = palavra_get_n_docs(p);
        for(int j = 0; j < n_docs; j++){
            doc_id = tabela_get_idx(palavra_get_tab(p, j));
            d = bd_obter_doc_indice(bd, doc_id);
            if(d == NULL){
                break;
            }
            numerador = 0;
            denominador1 = 0, denominador2 = 0;
            for(int k = 0; k < documento_get_n_palavras(d); k++){
                t = copiar_tabela(documento_obter_tabela(d, k));
                p2 = bd_obter_palavra_indice(bd, tabela_get_idx(t));
                p3 = bd_obter_palavra(bd2, palavra_get_string(p2));
                if(p3 == NULL){
                    idf = calcular_idf(bd_get_n_docs(bd) +1, palavra_get_n_docs(p2));
                    tfidf2 = 0;
                }
                else{
                    idf = calcular_idf(bd_get_n_docs(bd) +1, palavra_get_n_docs(p2) + 1);
                    t2 = palavra_get_tab(p3, palavra_get_n_docs(p3)-1);
                    tabela_calc_tfidf(t2, idf);
                    tfidf2 = tabela_get_tfidf(t2);
                }
                tabela_calc_tfidf(t, idf);
                tfidf1 = tabela_get_tfidf(t);
                numerador += tfidf1 * tfidf2;
                denominador1 += tfidf1*tfidf1;
                denominador2 += tfidf2*tfidf2;
                tabela_destruir(t);
            }
            cos = numerador / sqrt(denominador1) * sqrt(denominador2);
            if(tabelas_possui_idx(pcos, doc_id, n_cos) == -1){
                if(n_cos >= tam_cos){
                    tam_cos *= 2;
                    pcos = (Tabela *)realloc(pcos, tam_cos * sizeof(Tabela));
                }
                pcos[n_cos] = tabela_criar(doc_id);
                tabela_set_tfidf(pcos[n_cos], cos);
                n_cos += 1;
            }
        }
    }

    qsort(pcos, n_cos, sizeof(Tabela), tabela_compara_tfidf);

    char *classe = NULL;
    char *classe_maior = NULL;
    int cont_classe = 0;
    int cont_classe_maior = 0;
    if(n_cos > k){
        n_cos = k;
    }
    for(i = 0; i < n_cos; i++){
       d = bd_obter_doc_indice(bd, tabela_get_idx(pcos[i]));
       classe = documento_obter_classe(d);
       cont_classe = 0;
       for(int l = 0; l < n_cos; l++){
        d = bd_obter_doc_indice(bd, tabela_get_idx(pcos[l]));
        if(!strcmp(classe, documento_obter_classe(d))){
            cont_classe += 1;
        }
       }
       if(cont_classe > cont_classe_maior){
        cont_classe_maior = cont_classe;
        classe_maior = classe;
       }
    }

    printf("Classe do Texto: %s\n", classe_maior);
}



