#ifndef _H_PALAVRA_
#define _H_PALAVRA_

#include <stdio.h>
#include "tabela.h"

typedef struct palavra* Palavra;

Palavra palavra_criar();
int compara_palavra_string(const void *palavra, const void *p1);
int compara_palavra_tfidf(const void *p1, const void *p2);
int compara_palavras(const void *p1, const void *p2);
void palavra_destruir(Palavra p);
void palavra_calc_tfidf(Palavra p, float idf);
int palavra_get_n_docs(Palavra p);
Tabela palavra_get_tab(Palavra p, int idx);
void palavra_salvar(Palavra p, FILE * fpalavra);
Palavra palavra_carregar(FILE * fpalavra);
void palavra_add_tabela(Palavra p, int idx_doc, int verify);
void imprimir_palavra(Palavra p);


#endif