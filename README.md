# Trabalho Prático: Máquina de Estados

Disciplina: Sistemas Computacionais (trabalho em grupo)

Máquina de venda automática (4 estados) implementada em C, com testes automatizados de estados, transições e caminhos.

## Estrutura

- `src/` - implementação (`maquina.h`, `maquina.c`)
- `tests/` - testes automatizados (`test_estados.c`, `test_transicoes.c`, `test_caminhos.c`) e mini-framework `teste.h`
- `docs/diagrama/` - diagrama de estados (`estados.png`) e o script que o gera (`diagrama.py`)
- `relatorio/` - relatório em .docx (editável) e PDF (formato de entrega)

## Executar os testes

```bash
make test         # compila e executa os testes
make cobertura    # cobertura de src/maquina.c (gcov)
```

## Integrantes

- Victor Kauê
- Gabriel Cardoso
