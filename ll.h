struct no
{
    char c;
    struct no* prox;
};
typedef struct no No;

struct pilha
{
    No* topo;
};
typedef struct pilha Pilha;

Pilha* cria_pilha();
void push(Pilha *plh, char c);
char pop(Pilha *plh);
int precedencia(char c);
No* cria_parentese(char c);
void imprime_posfixa(char exp[]);
int operandos(char c);
int operadores(char c);
int verifica_expressao(char exp[]);
void libera_pilha(Pilha *plh);
void imprime_erro(int N);
void verifica_conta(char exp[]); //n sei o tipo certo entao botei void;
