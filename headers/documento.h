#ifndef _DOCUMENTO_H
#define _DOCUMENTO_H
#include <stdio.h>
#include "tabela.h"

typedef struct documento* Documento;
Documento documento_cria(char *caminho, char *classe);
char * documento_obter_caminho(Documento d);
void documento_add_tabela(Documento d, int idx_pal, int verify);
Tabela documento_obter_tabela(Documento d, int idx);
int documento_get_n_palavras(Documento d);
void documento_calc_tfidf(Documento d, float idf);
void documento_salvar(Documento d, FILE * fdoc);
void documento_imprimir(Documento d);
Documento documento_carregar(FILE * fdoc);
void documento_destroi(Documento d);



#endif
