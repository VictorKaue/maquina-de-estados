/* Testes de caminhos: os 7 caminhos independentes C1 a C7 (TC1 a TC7). */
#include "teste.h"
#include "maquina.h"

static void TC1_compra_simples(void)           /* E1 -T1-> E2 -T3-> E3 -T6-> E1 */
{
    Maquina m;
    maquina_iniciar(&m);
    EVENTO(m, EVT_INSERIR_MOEDA, 2);
    EVENTO(m, EVT_SELECIONAR_PRODUTO, 0);
    EVENTO(m, EVT_ENTREGA_CONCLUIDA, 0);
    VERIFICA(m.estado == ESTADO_OCIOSA);
    VERIFICA(m.devolvido == 0);
}

static void TC2_cancelamento(void)             /* E1 -T1-> E2 -T5-> E1 */
{
    Maquina m;
    maquina_iniciar(&m);
    EVENTO(m, EVT_INSERIR_MOEDA, 1);
    EVENTO(m, EVT_CANCELAR, 0);
    VERIFICA(m.estado == ESTADO_OCIOSA);
    VERIFICA(m.devolvido == 1);
}

static void TC3_varias_moedas_com_troco(void)  /* E1 -T1-> E2 -T2-> E2 -T3-> E3 -T6-> E1 */
{
    Maquina m;
    maquina_iniciar(&m);
    EVENTO(m, EVT_INSERIR_MOEDA, 1);
    EVENTO(m, EVT_INSERIR_MOEDA, 2);
    EVENTO(m, EVT_SELECIONAR_PRODUTO, 0);
    EVENTO(m, EVT_ENTREGA_CONCLUIDA, 0);
    VERIFICA(m.estado == ESTADO_OCIOSA);
    VERIFICA(m.devolvido == 1);
}

static void TC4_insuficiente_e_cancela(void)   /* E1 -T1-> E2 -T4-> E2 -T5-> E1 */
{
    Maquina m;
    maquina_iniciar(&m);
    EVENTO(m, EVT_INSERIR_MOEDA, 1);
    EVENTO(m, EVT_SELECIONAR_PRODUTO, 0);
    VERIFICA(m.estado == ESTADO_COM_CREDITO);
    EVENTO(m, EVT_CANCELAR, 0);
    VERIFICA(m.estado == ESTADO_OCIOSA);
    VERIFICA(m.devolvido == 1);
}

static void TC5_falha_em_repouso(void)         /* E1 -T8-> E4 -T10-> E1 */
{
    Maquina m;
    maquina_iniciar(&m);
    EVENTO(m, EVT_FALHA, 0);
    EVENTO(m, EVT_REPARAR, 0);
    VERIFICA(m.estado == ESTADO_OCIOSA);
}

static void TC6_falha_com_credito(void)        /* E1 -T1-> E2 -T9-> E4 -T10-> E1 */
{
    Maquina m;
    maquina_iniciar(&m);
    EVENTO(m, EVT_INSERIR_MOEDA, 1);
    EVENTO(m, EVT_FALHA, 0);
    VERIFICA(m.devolvido == 1);
    EVENTO(m, EVT_REPARAR, 0);
    VERIFICA(m.estado == ESTADO_OCIOSA);
}

static void TC7_falha_na_entrega(void)         /* E1 -T1-> E2 -T3-> E3 -T7-> E4 -T10-> E1 */
{
    Maquina m;
    maquina_iniciar(&m);
    EVENTO(m, EVT_INSERIR_MOEDA, 2);
    EVENTO(m, EVT_SELECIONAR_PRODUTO, 0);
    EVENTO(m, EVT_FALHA, 0);
    VERIFICA(m.devolvido == 2);
    EVENTO(m, EVT_REPARAR, 0);
    VERIFICA(m.estado == ESTADO_OCIOSA);
    VERIFICA(m.credito == 0);
}

int main(void)
{
    EXECUTA(TC1_compra_simples);
    EXECUTA(TC2_cancelamento);
    EXECUTA(TC3_varias_moedas_com_troco);
    EXECUTA(TC4_insuficiente_e_cancela);
    EXECUTA(TC5_falha_em_repouso);
    EXECUTA(TC6_falha_com_credito);
    EXECUTA(TC7_falha_na_entrega);
    return RESULTADO();
}
