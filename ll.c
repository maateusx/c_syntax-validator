#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ll.h"

Pilha *cria_pilha()
{
    Pilha *novo = (Pilha *)malloc(sizeof(Pilha));
    novo->topo = NULL;
    return novo;
}

void push(Pilha *plh, char c)
{
    No *novo = cria_parentese(c);
    if(plh->topo == NULL)
    {
        plh->topo = novo;
    }
    else
    {
        novo->prox = plh->topo;
        plh->topo = novo;
    }
}

char pop(Pilha *plh)
{
    No *aux;
    char c;

    if(plh->topo == NULL)
    {
        return 0;
    }
    else
    {
        c = plh->topo->c;
        aux = plh->topo;
        plh->topo = plh->topo->prox;
        free(aux);
        return c;
    }
}

int precedencia(char c)
{
    if(c == ';')
        return 7;
    if(c == '^')
        return 6;
    if(c == '*' || c == '/')
        return 5;
    if(c == '-' || c == '+')
        return 4;
    if(c == '>' || c == '<' || c == '=' || c == '#')
        return 3;
    if(c == '.')
        return 2;
    if(c == '|')
        return 1;
    return 0;
}

No *cria_parentese(char c)
{
    No* novo = (No*)malloc(sizeof(No));
    novo->prox = NULL;
    novo->c = c;
    return novo;
}

void libera_pilha(Pilha *plh)
{
    No *aux;

    if(plh == NULL)
        return;

    while(plh->topo != NULL)
    {
        aux = plh->topo;
        plh->topo = plh->topo->prox;
        free(aux);
    }

    free(plh);
}

int operadores(char c)
{
    return (c == '+' || c == '-' || c == '/' || c == '*' || c == '^' || c == '<' || c == '>' || c == '#' || c == '=' || c == '.' || c == '|');
}

int operandos(char c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'));
}

//1 - erro lexico /2 - erro sintatico
int verifica_expressao(char exp[])
{
    Pilha *plh = cria_pilha();
    int i;


    for(i=0; exp[i]!='\0'; i++) //ERRO LEXICO
    {
        if(!operadores(exp[i]) && !operandos(exp[i]) && exp[i] != '(' && exp[i] != ')' && exp[i] != ';')
        {
            return 1;
        }
    }

    for(i=0; exp[i]!='\0'; i++) //ERRO SINTATICO
    {
        if(exp[i] == '(') // empilha um parentese quando acha
            push(plh, exp[i]); // empilha somente o parenteses, nada mais
        else if(exp[i] == ')' && pop(plh) != '(') // desempilha quando acha o parentese fechando
        {
            return 2;       //caso o ultimo empilhado seja o parenteses abrindo, esta correto
        }
    }

    if(plh->topo != NULL) //ERRO SINTATICO
        return 2;

    libera_pilha(plh);

    for(i=0; exp[i]!='\0'; i++) //ERRO SINTATICO
    {
        if(operadores(exp[0]))
            return 2;
        if(operadores(exp[i]) && operadores(exp[i+1]))
            return 2;
        else if(operandos(exp[i]) && operandos(exp[i+1]))
            return 2;
        if(operadores(exp[i]) && (exp[i+1] == '\0' || exp[i+1] == ';'))
            return 2;
        if(exp[i] != ';' && exp[i+1] == '\0')
            return 2;
        if(exp[0] == ';')
            return 2;
    }
    return 0; //Teste oncluidos, expressao esta correta
}

void imprime_posfixa(char exp[])
{

    FILE *arq = fopen("saida.txt","a");

    Pilha *pilha = cria_pilha();
    char pos_fixa[1001];
    int i=0, j=0;

    while(exp[i] != ';' && exp[i+1] != '\0')
    {

        if(!operadores(exp[i]) && exp[i] != '(' && exp[i] != ')')
        {
            pos_fixa[j] = exp[i];
            i++;
            j++;
        }
        else if(exp[i] != '(' && exp[i] != ')')
        {
            if(pilha->topo == NULL || precedencia(exp[i]) > precedencia(pilha->topo->c))
            {
                push(pilha, exp[i]);
                i++;
            }
            else if(precedencia(exp[i]) < precedencia(pilha->topo->c))
            {
                while(pilha->topo != NULL && pilha->topo->c != '(')
                {
                    pos_fixa[j] = pop(pilha);
                    j++;
                }
                push(pilha, exp[i]);
                i++;
            }
            else
            {
                pos_fixa[j] = pop(pilha);
                push(pilha, exp[i]);
                j++;
                i++;
            }
        }
        else if(exp[i] == '(')
        {
            push(pilha, exp[i]);
            i++;
        }
        else
        {
            while(pilha->topo != NULL && pilha->topo->c != '(')
            {
                pos_fixa[j] = pop(pilha);
                j++;
            }
            pop(pilha);
            i++;
        }
    }
    while(pilha->topo != NULL)
    {
        pos_fixa[j] = pop(pilha);
        j++;
    }
    pos_fixa[j] = ';';
    pos_fixa[j+1] = '\0';
    printf("%s\n", pos_fixa);

    i=0;
    while(pos_fixa[i]!='\0') //Grava forma Pos-Fixa no arquivo de saida
    {
        fputc(pos_fixa[i], arq);
        i++;
    }
    fputc('\n', arq);

    libera_pilha(pilha);

    fclose(arq);
}

void imprime_prefixa(char exp[])
{

    FILE *arq = fopen("saida.txt","a");

    Pilha *pilha = cria_pilha();
    char pre_fixa[100];
    int i=0, j=0;

    while(exp[i] != ';' && exp[i+1] != '\0')
    {
        if(!operandos(exp[i]) && exp[i] != '(' && exp[i] != ')')
        {
            pre_fixa[j] = exp[i];
            i++;
            j++;
        }
        else if(exp[i] != '(' && exp[i] != ')')
        {
            if(pilha->topo == NULL || precedencia(exp[i]) < precedencia(pilha->topo->c)) //>
            {
                push(pilha, exp[i]);
                i++;
            }

            else if(precedencia(exp[i]) > precedencia(pilha->topo->c)) //<
            {
                while(pilha->topo != NULL && pilha->topo->c != '(')
                {
                    pre_fixa[j] = pop(pilha);
                    j++;
                }
                push(pilha, exp[i]);
                i++;
            }

            else
            {
                pre_fixa[j] = pop(pilha);
                push(pilha, exp[i]);
                j++;
                i++;
            }

        }
        else if(exp[i] == '(')
        {
            push(pilha, exp[i]);
            i++;
        }
        else
        {
            while(pilha->topo != NULL && pilha->topo->c != '(')
            {
                pre_fixa[j] = pop(pilha);
                j++;
            }
            pop(pilha);
            i++;
        }
    }

    while(pilha->topo != NULL)
    {
        pre_fixa[j] = pop(pilha);
        j++;
    }
    pre_fixa[j] = ';';
    pre_fixa[j+1] = '\0';
    printf("%s\n", pre_fixa);

    i=0;
    while(pre_fixa[i]!='\0') //Grava forma Pos-Fixa no arquivo de saida
    {
        if(pre_fixa[i]=='(')
            i++;
        fputc(pre_fixa[i], arq);
        i++;
    }
    fputc('\n', arq); //pula linha
    fputc('\n', arq);

    libera_pilha(pilha);

    fclose(arq);
}

void imprime_erro(int N)
{
    FILE *arq = fopen("saida.txt","a");

    char Sint[]="Erro Sintatico", Lex[]="Erro Lexico";
    int i=0;


    if(N == 1) //Erro Lexico
    {
        while(Lex[i] != '\0')
        {
            fputc(Lex[i], arq);
            i++;
        }
    }

    if(N == 2) // Erro Sintatico
    {
        while(Sint[i] != '\0')
        {
            fputc(Sint[i], arq);
            i++;
        }
    }
    fputc('\n', arq);
    fputc('\n', arq);

    fclose(arq);
}

/*void verifica_conta(exp[]) //n sei o tipo certo entao botei void;
{
    int i = 0;
    while (exp[i] != ';')
    {

    }
    if (a[i] == '+') {
       i++;
       return verifica_conta() + verifica_conta();
    }
    if (a[i] == '*') {
       i++;
       return verifica_conta() * verifica_conta();
    }
    while ((a[i] >= '0') && (a[i] <= '9'))
       x = 10*x + (a[i++]-'0');
    return x;
}
*/
