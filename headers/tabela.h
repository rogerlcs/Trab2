#ifndef _H_TABELA_
#define _H_TABELA_
#include <stdio.h>

typedef struct tabela* Tabela;

Tabela tabela_criar(int indice);
void tabela_add_freq(Tabela t);
void tabela_salvar(Tabela t, FILE * ftabela);
void tabela_carregar(Tabela t, FILE * ftabela);
void tabela_calc_tfidf(Tabela t, float tfidf);
void tabela_destruir(Tabela t);
int tabela_get_idx(Tabela t);
int tabela_get_frequencia(Tabela t);
int tabela_set_frequencia(Tabela t, int freq);
float tabela_get_tfidf(Tabela t);
int tabela_compara_idx(int idx, Tabela t);
void tabela_imprimir(Tabela t);
int tabela_compara_frequencia(const void *t1, const void *t2);
int tabela_compara_tfidf(const void *t1, const void *t2);
void tabela_set_tfidf(Tabela t, float tfidf);
int tabelas_possui_idx(Tabela *t, int idx, int tam);
Tabela copiar_tabela(Tabela t2);
//int compara_idx(const void * i1, const void *i2)
void somar_tabelas(Tabela t1, Tabela t2);
int compara_tabela(const void *t1, const void *t2);




#endif