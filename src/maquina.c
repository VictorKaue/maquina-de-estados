#include "maquina.h"

void maquina_iniciar(Maquina *m)
{
    m->estado = ESTADO_OCIOSA;
    m->credito = 0;
    m->devolvido = 0;
}

static int moeda_valida(int valor)
{
    return valor == 1 || valor == 2;
}

/* Devolve todo o crédito e zera a conta. */
static void devolver_credito(Maquina *m)
{
    m->devolvido = m->credito;
    m->credito = 0;
}

int maquina_processar(Maquina *m, Evento e, int valor)
{
    switch (m->estado) {

    case ESTADO_OCIOSA:
        if (e == EVT_INSERIR_MOEDA && moeda_valida(valor)) {      /* T1 */
            m->credito += valor;
            m->devolvido = 0;
            m->estado = ESTADO_COM_CREDITO;
            return 1;
        }
        if (e == EVT_FALHA) {                                     /* T8 */
            m->devolvido = 0;
            m->estado = ESTADO_FORA_DE_SERVICO;
            return 1;
        }
        return 0;

    case ESTADO_COM_CREDITO:
        if (e == EVT_INSERIR_MOEDA && moeda_valida(valor)) {      /* T2 */
            m->credito += valor;
            m->devolvido = 0;
            return 1;
        }
        if (e == EVT_SELECIONAR_PRODUTO) {
            m->devolvido = 0;
            if (m->credito >= PRECO_PRODUTO)                      /* T3 */
                m->estado = ESTADO_DISPENSANDO;
            return 1;                                             /* T4: crédito insuficiente, permanece */
        }
        if (e == EVT_CANCELAR) {                                  /* T5 */
            devolver_credito(m);
            m->estado = ESTADO_OCIOSA;
            return 1;
        }
        if (e == EVT_FALHA) {                                     /* T9 */
            devolver_credito(m);
            m->estado = ESTADO_FORA_DE_SERVICO;
            return 1;
        }
        return 0;

    case ESTADO_DISPENSANDO:
        if (e == EVT_ENTREGA_CONCLUIDA) {                         /* T6 */
            m->devolvido = m->credito - PRECO_PRODUTO;            /* troco */
            m->credito = 0;
            m->estado = ESTADO_OCIOSA;
            return 1;
        }
        if (e == EVT_FALHA) {                                     /* T7 */
            devolver_credito(m);                                  /* estorno integral */
            m->estado = ESTADO_FORA_DE_SERVICO;
            return 1;
        }
        return 0;

    case ESTADO_FORA_DE_SERVICO:
        if (e == EVT_REPARAR) {                                   /* T10 */
            m->credito = 0;
            m->devolvido = 0;
            m->estado = ESTADO_OCIOSA;
            return 1;
        }
        return 0;
    }
    return 0;
}
