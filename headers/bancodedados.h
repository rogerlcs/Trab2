#ifndef _H_BANCO_DADOS_
#define _H_BANCO_DADOS_
#include "documento.h"
#include "palavra.h"
#include "tabela.h"


typedef struct bancodedados* BancoDeDados;
void calcula_tfidf(BancoDeDados bd);
BancoDeDados bd_criar(char *pasta);
BancoDeDados bd_carregar(char *filebd);
void bd_adicionar_doc(BancoDeDados bd, Documento doc);
void bd_adicionar_palavra(BancoDeDados bd, Palavra p);
void bd_salvar(BancoDeDados bd, char *nomearq);
void bd_organizar_palavras(BancoDeDados bd);
int bd_possui_palavra(BancoDeDados bd, char *palavra);
Palavra bd_obter_palavra_indice(BancoDeDados bd, int idx);
Documento bd_obter_doc_indice(BancoDeDados bd, int idx);
Palavra bd_obter_palavra(BancoDeDados bd, char *palavra);
float calcular_idf(int n1, int d1);
void bd_destruir(BancoDeDados bd);
void le_palavras(Documento doc, BancoDeDados bd);
int bd_get_n_docs(BancoDeDados bd);
void bd_organizar_documentos(BancoDeDados bd);
BancoDeDados bd_copiar_documentos(BancoDeDados bd1, BancoDeDados bd2);
int bd_get_n_palavras(BancoDeDados bd);
char * bd_doc_calc_classe(BancoDeDados bd, BancoDeDados bd2, int k);
void calcular_tfidf_palavra(BancoDeDados bd, Palavra p, int n_docs);
void calcular_tfidf_documento(BancoDeDados bd, Documento d, int n_palavras);



#endif