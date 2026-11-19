# Trabalho Prático: Máquina de Estados

Disciplina: Sistemas Computacionais (trabalho em grupo)

Máquina de venda automática (4 estados) implementada em C, com testes automatizados de estados, transições e caminhos.

## Estrutura

- `src/` - implementação (`maquina.h`, `maquina.c`)
- `tests/` - testes automatizados (`test_estados.c`, `test_transicoes.c`, `test_caminhos.c`), mini-framework `teste.h` e teste de mutação (`mutacao.py`)
- `docs/diagrama/` - diagrama de estados (`estados.png`) e o script que o gera (`diagrama.py`)
- `relatorio/` - relatório em PDF (formato de entrega); o `.docx` editável fica só na máquina local (ignorado pelo git)

## Executar os testes

```bash
make test         # compila e executa os testes
make cobertura    # cobertura de src/maquina.c (gcov)
make mutacao      # teste de mutação (requer python3)
```

## Integrantes

- Victor Kauê
- Gabriel Cardoso
