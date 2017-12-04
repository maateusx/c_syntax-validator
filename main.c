#include <stdio.h>
#include <stdlib.h>
#include "ll.h"

int main()
{
    FILE *arq_entrada, *arq_saida;
    arq_entrada=fopen("entrada.txt","r");
    //arq_saida=fopen("saida.txt","w");

    if (arq_entrada == NULL)   //Verifica se é nulo
    {
        printf("\n\nArquivo nao existe ou nao pode ser aberto\n\n");
        return 0;
    }

    char exp[1001];
    //int n;

    // scanf("%d", &n);

    while(fscanf(arq_entrada, "%s", exp) != EOF)
    {
        switch(verifica_expressao(exp))
        {
        case 0:
            //verifica_conta(exp); //n sei o tipo certo entao botei void;
            imprime_posfixa(exp);
            imprime_prefixa(exp);
            break;
        case 1: //Erro lexico
            imprime_erro(1);
            printf("Erro Lexico!\n");
            break;
        case 2: //Erro sintatico
            imprime_erro(2);
            printf("Erro Sintatico!\n");
            break;
        }
    }

    fclose(arq_entrada);
    //fclose(arq_saida);

    return 0;
}
