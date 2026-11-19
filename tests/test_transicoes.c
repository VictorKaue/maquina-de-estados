/* Testes de transições: cobertura de T1 a T10 e de eventos inválidos (TT1 a TT12). */
#include "teste.h"
#include "maquina.h"

static void TT1_T1_ociosa_moeda(void)
{
    Maquina m;
    maquina_iniciar(&m);
    EVENTO(m, EVT_INSERIR_MOEDA, 1);
    VERIFICA(m.estado == ESTADO_COM_CREDITO);
    VERIFICA(m.credito == 1);
}

static void TT2_T2_acumula_credito(void)
{
    Maquina m;
    maquina_iniciar(&m);
    EVENTO(m, EVT_INSERIR_MOEDA, 1);
    EVENTO(m, EVT_INSERIR_MOEDA, 2);
    VERIFICA(m.estado == ESTADO_COM_CREDITO);
    VERIFICA(m.credito == 3);
}

static void TT3_T3_credito_suficiente(void)
{
    Maquina m;
    maquina_iniciar(&m);
    EVENTO(m, EVT_INSERIR_MOEDA, 2);
    EVENTO(m, EVT_SELECIONAR_PRODUTO, 0);
    VERIFICA(m.estado == ESTADO_DISPENSANDO);
}

static void TT4_T4_credito_insuficiente(void)
{
    Maquina m;
    maquina_iniciar(&m);
    EVENTO(m, EVT_INSERIR_MOEDA, 1);
    EVENTO(m, EVT_SELECIONAR_PRODUTO, 0);
    VERIFICA(m.estado == ESTADO_COM_CREDITO);
    VERIFICA(m.credito == 1);
}

static void TT5_T5_cancelar(void)
{
    Maquina m;
    maquina_iniciar(&m);
    EVENTO(m, EVT_INSERIR_MOEDA, 2);
    EVENTO(m, EVT_CANCELAR, 0);
    VERIFICA(m.estado == ESTADO_OCIOSA);
    VERIFICA(m.devolvido == 2);
    VERIFICA(m.credito == 0);
}

static void TT6_T6_entrega_com_troco(void)
{
    Maquina m;
    maquina_iniciar(&m);
    EVENTO(m, EVT_INSERIR_MOEDA, 1);
    EVENTO(m, EVT_INSERIR_MOEDA, 2);
    EVENTO(m, EVT_SELECIONAR_PRODUTO, 0);
    EVENTO(m, EVT_ENTREGA_CONCLUIDA, 0);
    VERIFICA(m.estado == ESTADO_OCIOSA);
    VERIFICA(m.devolvido == 1);
    VERIFICA(m.credito == 0);
}

static void TT7_T7_falha_dispensando(void)
{
    Maquina m;
    maquina_iniciar(&m);
    EVENTO(m, EVT_INSERIR_MOEDA, 2);
    EVENTO(m, EVT_SELECIONAR_PRODUTO, 0);
    EVENTO(m, EVT_FALHA, 0);
    VERIFICA(m.estado == ESTADO_FORA_DE_SERVICO);
    VERIFICA(m.devolvido == 2);
    VERIFICA(m.credito == 0);
}

static void TT8_T8_falha_ociosa(void)
{
    Maquina m;
    maquina_iniciar(&m);
    EVENTO(m, EVT_FALHA, 0);
    VERIFICA(m.estado == ESTADO_FORA_DE_SERVICO);
}

static void TT9_T9_falha_com_credito(void)
{
    Maquina m;
    maquina_iniciar(&m);
    EVENTO(m, EVT_INSERIR_MOEDA, 1);
    EVENTO(m, EVT_FALHA, 0);
    VERIFICA(m.estado == ESTADO_FORA_DE_SERVICO);
    VERIFICA(m.devolvido == 1);
    VERIFICA(m.credito == 0);
}

static void TT10_T10_reparar(void)
{
    Maquina m;
    maquina_iniciar(&m);
    EVENTO(m, EVT_FALHA, 0);
    EVENTO(m, EVT_REPARAR, 0);
    VERIFICA(m.estado == ESTADO_OCIOSA);
    VERIFICA(m.credito == 0);
}

/* Eventos inválidos são rejeitados e não alteram a máquina. */
static void TT11_eventos_invalidos(void)
{
    Maquina m;

    maquina_iniciar(&m);                                       /* OCIOSA */
    VERIFICA(maquina_processar(&m, EVT_SELECIONAR_PRODUTO, 0) == 0);
    VERIFICA(maquina_processar(&m, EVT_CANCELAR, 0) == 0);
    VERIFICA(maquina_processar(&m, EVT_ENTREGA_CONCLUIDA, 0) == 0);
    VERIFICA(maquina_processar(&m, EVT_REPARAR, 0) == 0);
    VERIFICA(maquina_processar(&m, EVT_INSERIR_MOEDA, 5) == 0); /* moeda não aceita */
    VERIFICA(maquina_processar(&m, EVT_INSERIR_MOEDA, 3) == 0);
    VERIFICA(maquina_processar(&m, EVT_INSERIR_MOEDA, 0) == 0);
    VERIFICA(maquina_processar(&m, EVT_INSERIR_MOEDA, -1) == 0);
    VERIFICA(m.estado == ESTADO_OCIOSA && m.credito == 0 && m.devolvido == 0);

    EVENTO(m, EVT_INSERIR_MOEDA, 1);                           /* COM_CREDITO */
    VERIFICA(maquina_processar(&m, EVT_ENTREGA_CONCLUIDA, 0) == 0);
    VERIFICA(maquina_processar(&m, EVT_REPARAR, 0) == 0);
    VERIFICA(maquina_processar(&m, EVT_INSERIR_MOEDA, 5) == 0);
    VERIFICA(maquina_processar(&m, EVT_INSERIR_MOEDA, 3) == 0);
    VERIFICA(m.estado == ESTADO_COM_CREDITO && m.credito == 1 && m.devolvido == 0);

    EVENTO(m, EVT_INSERIR_MOEDA, 1);                           /* DISPENSANDO */
    EVENTO(m, EVT_SELECIONAR_PRODUTO, 0);
    VERIFICA(maquina_processar(&m, EVT_INSERIR_MOEDA, 1) == 0);
    VERIFICA(maquina_processar(&m, EVT_CANCELAR, 0) == 0);
    VERIFICA(maquina_processar(&m, EVT_SELECIONAR_PRODUTO, 0) == 0);
    VERIFICA(maquina_processar(&m, EVT_REPARAR, 0) == 0);
    VERIFICA(m.estado == ESTADO_DISPENSANDO && m.credito == 2 && m.devolvido == 0);

    EVENTO(m, EVT_FALHA, 0);                                   /* FORA_DE_SERVICO */
    VERIFICA(maquina_processar(&m, EVT_SELECIONAR_PRODUTO, 0) == 0);
    VERIFICA(maquina_processar(&m, EVT_CANCELAR, 0) == 0);
    VERIFICA(maquina_processar(&m, EVT_ENTREGA_CONCLUIDA, 0) == 0);
    VERIFICA(maquina_processar(&m, EVT_FALHA, 0) == 0);
    VERIFICA(maquina_processar(&m, EVT_INSERIR_MOEDA, 1) == 0);
    VERIFICA(m.estado == ESTADO_FORA_DE_SERVICO && m.credito == 0 && m.devolvido == 2);
}

/* Defesa: um valor de estado fora do enum é rejeitado sem alterar a máquina. */
static void TT12_estado_invalido(void)
{
    Maquina m;
    maquina_iniciar(&m);
    m.estado = (Estado)99;
    VERIFICA(maquina_processar(&m, EVT_INSERIR_MOEDA, 1) == 0);
    VERIFICA((int)m.estado == 99);
    VERIFICA(m.credito == 0);
}

int main(void)
{
    EXECUTA(TT1_T1_ociosa_moeda);
    EXECUTA(TT2_T2_acumula_credito);
    EXECUTA(TT3_T3_credito_suficiente);
    EXECUTA(TT4_T4_credito_insuficiente);
    EXECUTA(TT5_T5_cancelar);
    EXECUTA(TT6_T6_entrega_com_troco);
    EXECUTA(TT7_T7_falha_dispensando);
    EXECUTA(TT8_T8_falha_ociosa);
    EXECUTA(TT9_T9_falha_com_credito);
    EXECUTA(TT10_T10_reparar);
    EXECUTA(TT11_eventos_invalidos);
    EXECUTA(TT12_estado_invalido);
    return RESULTADO();
}
