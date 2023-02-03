#include <stdio.h>
#include <stdlib.h>
#include <bancodedados.h>
#include <tabela.h>
#include <string.h>
#include <palavra.h>

int main(int argc, char **argv){
    if(argc < 3){
        printf("Falta argumentos\n");
        exit(1);
    }
    
    BancoDeDados bd = bd_carregar("indice.bin");
    Palavra p = NULL;
    Documento d = NULL;
    int tam_tabela = 0;
    int n_loops = 10;
    Tabela *t = NULL;
    Tabela t2 = NULL;
    int n_tabela = 0;
    int idx_tab = 0;
    char linha[250];
    scanf("%[^\n]", linha);
    char *palavra = strtok(linha, " ");
    while(palavra != NULL){
        //printf("Pal: %s\n", palavra);
        p = bd_obter_palavra(bd, palavra);
        if(p != NULL){
            //imprimir_palavra(p);
            if(!n_tabela){
                n_tabela = palavra_get_n_docs(p);
                tam_tabela = n_tabela * 10;
                t = (Tabela *)calloc(tam_tabela, sizeof(Tabela));
                for(int i = 0; i < n_tabela; i++){
                    t[i] = copiar_tabela(palavra_get_tab(p, i));
                }
            }
            else{
                for(int j = 0; j < palavra_get_n_docs(p); j++){
                    t2 = palavra_get_tab(p, j);
                    idx_tab = tabelas_possui_idx(t, tabela_get_idx(t2), n_tabela);
                    if(idx_tab > -1){
                        somar_tabelas(t[idx_tab], t2);
                    }
                    else{
                        if(n_tabela >= tam_tabela){
                            tam_tabela *= 2;
                            t = (Tabela *)realloc(t, tam_tabela * sizeof(Tabela));
                        }
                        t[n_tabela] = copiar_tabela(t2);
                        n_tabela += 1;
                    }
                }
            }
        }   
        palavra = strtok(NULL, " ");
    }
    qsort(t,n_tabela, sizeof(Tabela),compara_tabela);
    if(n_tabela < 10){
        n_loops = n_tabela;
    }
    for(int i = 0; i < n_tabela; i++){
        d = bd_obter_doc_indice(bd, tabela_get_idx(t[i]));
        documento_imprimir(d);
        printf("--------------------\n");
    }
    
    bd_destruir(bd);

    return 0;
}