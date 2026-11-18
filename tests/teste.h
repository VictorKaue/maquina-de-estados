#ifndef TESTE_H
#define TESTE_H

#include <stdio.h>

static int g_total = 0;
static int g_falhas = 0;

#define VERIFICA(cond) do { \
    g_total++; \
    if (!(cond)) { \
        g_falhas++; \
        fprintf(stderr, "FALHA %s:%d: %s\n", __FILE__, __LINE__, #cond); \
    } \
} while (0)

/* Executa um caso de teste e imprime o resultado individual. */
#define EXECUTA(caso) do { \
    int antes_ = g_falhas; \
    caso(); \
    printf("  [%s] %s\n", g_falhas == antes_ ? "OK" : "FALHOU", #caso); \
} while (0)

/* Imprime o resumo do arquivo e devolve o código de saída (0 = tudo certo). */
#define RESULTADO() (printf("%s: %d/%d verificacoes OK\n", __FILE__, g_total - g_falhas, g_total), g_falhas ? 1 : 0)

/* Aplica um evento e exige que tenha sido tratado. */
#define EVENTO(m, e, v) VERIFICA(maquina_processar(&(m), (e), (v)) == 1)

#endif
