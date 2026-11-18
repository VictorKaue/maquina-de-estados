#ifndef MAQUINA_H
#define MAQUINA_H

/* Máquina de venda automática de produto único. Valores em reais inteiros. */

#define PRECO_PRODUTO 2   /* preço fixo do produto (R$ 2,00) */

typedef enum {
    ESTADO_OCIOSA,
    ESTADO_COM_CREDITO,
    ESTADO_DISPENSANDO,
    ESTADO_FORA_DE_SERVICO
} Estado;

typedef enum {
    EVT_INSERIR_MOEDA,       /* valor aceito: 1 ou 2 */
    EVT_SELECIONAR_PRODUTO,
    EVT_CANCELAR,
    EVT_ENTREGA_CONCLUIDA,
    EVT_FALHA,
    EVT_REPARAR
} Evento;

typedef struct {
    Estado estado;
    int credito;     /* crédito acumulado (R$) */
    int devolvido;   /* valor devolvido pelo último evento: cancelamento, troco ou estorno */
} Maquina;

/* Coloca a máquina no estado inicial (OCIOSA, sem crédito). */
void maquina_iniciar(Maquina *m);

/* Processa um evento. Retorna 1 se o evento foi tratado e 0 se foi rejeitado
   (evento inválido para o estado atual ou moeda não aceita); na rejeição,
   a máquina não muda. O parâmetro valor só é usado em EVT_INSERIR_MOEDA. */
int maquina_processar(Maquina *m, Evento e, int valor);

#endif
