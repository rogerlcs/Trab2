#include <stdio.h>
#include <stdlib.h>
#include <bancodedados.h>
#include <tabela.h>
#include <string.h>
#include <palavra.h>
#include <math.h>
#define CLASSES 20

typedef struct
{
    char *classe;
    int freq;
}PClasse;

void pclasse_add(PClasse *pclass, int *tam, char *classe){
    int possui = 0;
    for(int i = 0; i < *tam; i++){
        if(!strcmp(classe, pclass[i].classe)){
            pclass[i].freq += 1;
            possui = 1;
            break;
        }
    }  

    if(!possui){
        PClasse p;
        p.classe = classe;
        //printf("\nClasse:%s\n", classe);
        pclass[*tam] = p;
        *tam += 1; 
    }
}

void pclasse_imprimir(PClasse *pclass, int tam){
    for(int i = 0; i < tam; i++){
        printf("Classe: %s\n", pclass[i].classe);
        printf("Frequencia: %d\n", pclass[i].freq);
    }
}

int pclasse_comparar(const void * pc1, const void *pc2){
    PClasse p1 = *(PClasse *)pc1;
    PClasse p2 = *(PClasse *)pc2;

    return -1 * (p1.freq - p2.freq);
}

void buscar_noticias(BancoDeDados bd);
void classificar_noticia(BancoDeDados bd, int k);
void relatorio_palavras(BancoDeDados bd);
void relatorio_documentos(BancoDeDados bd);


int main(int argc, char **argv){
    if(argc < 3){
        printf("Falta argumentos\n");
        exit(1);
    }
    BancoDeDados bd = bd_carregar(argv[1]);
    int op = 0;
    printf("Escolha a opcao:\n");
    printf("1 - Buscar noticias\n");
    printf("2 - Classificar noticia\n");
    printf("3 - Relatorio de palavra\n");
    printf("4 - Relatorio de documentos\n");
    scanf("%d", &op);
    scanf("%*c");

    switch (op)
    {
    case 1:
        buscar_noticias(bd);
        break;
    case 2:
        classificar_noticia(bd, atoi(argv[2]));
        break;
    case 3:
        relatorio_palavras(bd);
        break;
    case 4:
        relatorio_documentos(bd);
        break;
    
    default:
        break;
    }
    
    bd_destruir(bd);
    

    return 0;
}

void buscar_noticias(BancoDeDados bd){
    Palavra p = NULL;
    Documento d = NULL;
    int tam_tabela = 0;
    Tabela *t = NULL;
    Tabela t2 = NULL;
    int n_tabela = 0;
    int idx_tab = 0;
    char linha[250];
    printf("Digite uma frase:");
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
    for(int i = 0; i < n_tabela; i++){
        d = bd_obter_doc_indice(bd, tabela_get_idx(t[i]));
        if(i < 10){
            documento_imprimir(d);
        }
        tabela_destruir(t[i]);
        printf("--------------------\n");
    }
    free(t);
}

void classificar_noticia(BancoDeDados bd, int k){
    char palavra[250] = "\0";
    char lixo = ' ';
    Palavra p = NULL;
    Palavra p2 = NULL;
    BancoDeDados bd2 = bd_criar("");
    printf("Digite um texto terminado em . :\n");
    while(lixo != '.' && scanf("%[a-zA-Z0-9]", palavra) == 1){
        scanf("%*c", lixo);
        p = bd_obter_palavra(bd, palavra);
        if(p != NULL){
            p2 = palavra_copiar(p);
            palavra_add_tabela(p2, -1, 1);
            if(bd_obter_palavra(bd2, palavra_get_string(p2)) == NULL){
                bd_adicionar_palavra(bd2 ,p2);
            }
            }
    }
    bd_organizar_palavras(bd2);
    char *classe = bd_doc_calc_classe(bd, bd2, k);
    printf("Classe do texto: %s\n", classe);
    bd_destruir(bd2);
}

void relatorio_palavras(BancoDeDados bd){
    char palavra[250];
    int i = 0;
    printf("Digite a palavra: ");
    scanf("%s", palavra);
    Palavra p = bd_obter_palavra(bd, palavra);
    Documento d = NULL;
    Tabela t = NULL;
    int n_classes = 0;
    if(p != NULL){
        printf("%s:\nNumero de documentos: %d\n", palavra, palavra_get_n_docs(p));
        palavra_organizar_tabela(p, tabela_compara_frequencia);
        PClasse *classesFreq = (PClasse *)calloc(CLASSES, sizeof(PClasse));
        while(i < palavra_get_n_docs(p)){
            d = bd_obter_doc_indice(bd, tabela_get_idx(palavra_get_tab(p, i)));
            if(i < 10){
                if(i == 0){
                    printf("Documentos em que mais aparace:\n");
                }
                documento_imprimir(d);
                printf("-----------------------------\n");
            }
            pclasse_add(classesFreq, &n_classes, documento_obter_classe(d));
            i++;
        }
        printf("Frequencia por classe:\n");
        qsort(classesFreq, n_classes, sizeof(PClasse), pclasse_comparar);
        pclasse_imprimir(classesFreq, n_classes);
        free(classesFreq);
    }
}

void relatorio_documentos(BancoDeDados bd){
    bd_organizar_documentos(bd);
    int i = 0;
    printf("Os 10 maiores documentos:\n");
    Documento d = NULL;
    int n_loops = 0;
    if(bd_get_n_docs(bd) >= 10){
        n_loops = 10;
    }
    else n_loops = bd_get_n_docs(bd);
    for(i = 0; i < n_loops; i++){
        d = bd_obter_doc_indice(bd, i);
        documento_imprimir(d);
        printf("-----------------------------\n");
    }
    printf("Os 10 menores documentos:\n");
    for(i = bd_get_n_docs(bd)-1; i > bd_get_n_docs(bd)-n_loops; i--){
        d = bd_obter_doc_indice(bd, i);
        documento_imprimir(d);
        printf("-----------------------------\n");
    }
}