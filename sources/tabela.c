#include "tabela.h"
#include <stdio.h>
#include <stdlib.h>

struct tabela
{
    int indice;
    int frequencia;
    float tfidf;
};

Tabela tabela_criar(int indice){
    Tabela tab = (Tabela)calloc(1, sizeof(struct tabela));
    tab->indice = indice;
    tab->frequencia = 1;
    tab->tfidf = 0;
    return tab;

}
void tabela_add_freq(Tabela t){
    t->frequencia += 1;
}
void tabela_set_tfidf(Tabela t, float idf){
    t->tfidf = idf * t->frequencia;
}

void tabela_imprimir(Tabela t){
    printf("Indice: %d|", t->indice);
    printf("Frequencia %d|", t->frequencia);
    printf("Tf-idf: %.2f\n", t->tfidf);
}

void tabela_salvar(Tabela t, FILE * ftabela){
    fwrite(&t->indice, 1, sizeof(int), ftabela);
    fwrite(&t->frequencia, 1, sizeof(int), ftabela);
    fwrite(&t->tfidf, 1, sizeof(float), ftabela);
}

void tabela_carregar(Tabela t, FILE * ftabela){
    fread(&t->frequencia, 1, sizeof(int), ftabela);
    fread(&t->tfidf, 1, sizeof(float), ftabela);
}

void tabela_destruir(Tabela t){
    free(t);
}
int tabela_compara_idx(int idx, Tabela t){
    if(idx == t->indice) return 1;
    return 0;
}

void somar_tabelas(Tabela t1, Tabela t2){
    t1->tfidf += t2->tfidf;
    t1->frequencia += t2->frequencia;
}

int tabela_get_idx(Tabela t){
    return t->indice;
}

int tabelas_possui_idx(Tabela *t, int idx, int tam){
    for(int i = 0; i < tam; i++){
        if(tabela_compara_idx(idx, t[i])){
            return i;
        }
   }
   return -1;
}

Tabela copiar_tabela(Tabela t2){
    Tabela t1 = tabela_criar(tabela_get_idx(t2));
    tabela_set_frequencia(t1, tabela_get_frequencia(t2));
    t1->tfidf = t2->tfidf;
    return t1;
}

int tabela_get_frequencia(Tabela t){
    return t->frequencia;
}


int compara_tabela(const void *t1, const void *t2){
    Tabela tab1 = *(Tabela *)t1;
    Tabela tab2 = *(Tabela *)t2;
    return -1*(tab1->tfidf - tab2->tfidf);
}

int tabela_set_frequencia(Tabela t, int freq){
    t->frequencia = freq;
}
