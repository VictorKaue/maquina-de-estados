/* Testes de estados: cobertura de todos os estados (TE1 a TE4). */
#include "teste.h"
#include "maquina.h"

static void TE1_ociosa(void)
{
    Maquina m;
    maquina_iniciar(&m);
    VERIFICA(m.estado == ESTADO_OCIOSA);
    VERIFICA(m.credito == 0);
}

static void TE2_com_credito(void)
{
    Maquina m;
    maquina_iniciar(&m);
    EVENTO(m, EVT_INSERIR_MOEDA, 1);
    VERIFICA(m.estado == ESTADO_COM_CREDITO);
    VERIFICA(m.credito == 1);
}

static void TE3_dispensando(void)
{
    Maquina m;
    maquina_iniciar(&m);
    EVENTO(m, EVT_INSERIR_MOEDA, 2);
    EVENTO(m, EVT_SELECIONAR_PRODUTO, 0);
    VERIFICA(m.estado == ESTADO_DISPENSANDO);
}

static void TE4_fora_de_servico(void)
{
    Maquina m;
    maquina_iniciar(&m);
    EVENTO(m, EVT_FALHA, 0);
    VERIFICA(m.estado == ESTADO_FORA_DE_SERVICO);
    VERIFICA(maquina_processar(&m, EVT_INSERIR_MOEDA, 1) == 0); /* moeda rejeitada */
    VERIFICA(m.estado == ESTADO_FORA_DE_SERVICO);
    VERIFICA(m.credito == 0);
}

int main(void)
{
    EXECUTA(TE1_ociosa);
    EXECUTA(TE2_com_credito);
    EXECUTA(TE3_dispensando);
    EXECUTA(TE4_fora_de_servico);
    return RESULTADO();
}
