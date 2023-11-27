#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

typedef struct No {
    double dado;
    struct No *proximo;
} No;

typedef struct {
    No *topo;
} Pilha;

void inicializar(Pilha *pilha) {
    pilha->topo = NULL;
}

void empilhar(Pilha *pilha, double valor) {
    No *novoNo = (No *)malloc(sizeof(No));
    if (novoNo == NULL) {
        printf("Erro: Falha na alocação de memória\n");
        exit(EXIT_FAILURE);
    }
    novoNo->dado = valor;
    novoNo->proximo = pilha->topo;
    pilha->topo = novoNo;
}

double desempilhar(Pilha *pilha) {
    if (pilha->topo != NULL) {
        No *temp = pilha->topo;
        double valor = temp->dado;
        pilha->topo = temp->proximo;
        free(temp);
        return valor;
    } else {
        printf("Erro: Pilha vazia\n");
        exit(EXIT_FAILURE);
    }
}

double avaliarExpressaoPosfixada(const char *expressao) {
    Pilha pilha;
    inicializar(&pilha);

    for (int i = 0; expressao[i] != '\0'; ++i) {
        if (isdigit(expressao[i]) || (expressao[i] == '.' && isdigit(expressao[i + 1]))) {
            char *endptr;
            double numero = strtod(&expressao[i], &endptr);
            i = endptr - expressao - 1;
            empilhar(&pilha, numero);
        } else if (expressao[i] == ' ' || expressao[i] == '\t') {
            continue;
        } else {
            double oper2, oper1;

            if (isalpha(expressao[i])) {
                char operador[10] = {0};  // Aumentei o tamanho para acomodar operadores mais longos
                int j = 0;
                while (isalpha(expressao[i]) && j < 9) {
                    operador[j++] = expressao[i++];
                }
                --i;
                if (strcmp(operador, "sen") == 0) {
                    oper1 = desempilhar(&pilha);
                    empilhar(&pilha, sin(oper1));
                } else if (strcmp(operador, "cos") == 0) {
                    oper1 = desempilhar(&pilha);
                    empilhar(&pilha, cos(oper1));
                } else if (strcmp(operador, "tan") == 0) {
                    oper1 = desempilhar(&pilha);
                    empilhar(&pilha, tan(oper1));
                } else if (strcmp(operador, "log") == 0) {
                    oper1 = desempilhar(&pilha);
                    empilhar(&pilha, log10(oper1));
                } else {
                    printf("Operador está errado (Consulte a tabela)\n");
                    exit(EXIT_FAILURE);
                }
            } else {
                switch (expressao[i]) {
                    case '+':
                        oper2 = desempilhar(&pilha);
                        oper1 = desempilhar(&pilha);
                        empilhar(&pilha, oper1 + oper2);
                        break;
                    case '-':
                        oper2 = desempilhar(&pilha);
                        oper1 = desempilhar(&pilha);
                        empilhar(&pilha, oper1 - oper2);
                        break;
                    case '*':
                        oper2 = desempilhar(&pilha);
                        oper1 = desempilhar(&pilha);
                        empilhar(&pilha, oper1 * oper2);
                        break;
                    case '/':
                        oper2 = desempilhar(&pilha);
                        oper1 = desempilhar(&pilha);
                        if (oper2 != 0.0) {
                            empilhar(&pilha, oper1 / oper2);
                        } else {
                            printf("Erro: Divisão por zero\n");
                            exit(EXIT_FAILURE);
                        }
                        break;
                    case '^':
                        oper2 = desempilhar(&pilha);
                        oper1 = desempilhar(&pilha);
                        empilhar(&pilha, pow(oper1, oper2));
                        break;
                    default:
                        printf("Erro: Operador inválido\n");
                        exit(EXIT_FAILURE);
                }
            }
        }

        /* para teste: imprime valores intermediários da pilha para depuração */
        printf("Pilha: ");
        for (No *temp = pilha.topo; temp != NULL; temp = temp->proximo) {
            printf("%f ", temp->dado);
        }
        printf("\n");
    }

    if (pilha.topo != NULL) {
        return pilha.topo->dado;
    } else {
        printf("Pilha vazia\n");
        exit(EXIT_FAILURE);
    }
}

int main() {
    /* para teste: digita a expressão no terminal
    char expressao[100];
    printf("Digite a expressao posfixada: ");
    fgets(expressao, sizeof(expressao), stdin);

    double resultado = avaliarExpressaoPosfixada(expressao);
    printf("Resultado: %.5f\n", resultado);
    */

    // Expressão posfixada diretamente no código
    const char *expressao = "3 4 + 5 tan *";
    double resultado = avaliarExpressaoPosfixada(expressao);
    printf("Resultado: %.5f\n", resultado);

    return 0;
}
