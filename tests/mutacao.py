#!/usr/bin/env python3
"""Teste de mutação (evidência complementar à cobertura).

Introduz um defeito por vez em src/maquina.c e verifica se alguma suíte de teste falha.
Mutantes marcados como equivalentes não alteram o comportamento observável (o valor
já é zero por causa dos invariantes) e podem sobreviver. Uso: make mutacao
"""
import os, shutil, subprocess, sys, tempfile

CC = os.environ.get("CC", "cc")
SRC = open("src/maquina.c", encoding="utf-8").read()
SUITES = ["estados", "transicoes", "caminhos"]

# (nome, trecho original, trecho mutado, equivalente?)
MUTANTES = [
 ("M1  guarda de T3: >= vira >", "m->credito >= PRECO_PRODUTO", "m->credito > PRECO_PRODUTO", False),
 ("M2  aceita moeda de 5", "valor == 1 || valor == 2", "valor == 1 || valor == 2 || valor == 5", False),
 ("M3  T6 troco sem descontar o preço", "m->credito - PRECO_PRODUTO", "m->credito", False),
 ("M4  devolver_credito não zera o crédito", "    m->devolvido = m->credito;\n    m->credito = 0;", "    m->devolvido = m->credito;", False),
 ("M5  T2 sobrescreve o crédito", "            m->credito += valor;\n            m->devolvido = 0;\n            return 1;\n        }\n        if (e == EVT_SELECIONAR_PRODUTO)", "            m->credito = valor;\n            m->devolvido = 0;\n            return 1;\n        }\n        if (e == EVT_SELECIONAR_PRODUTO)", False),
 ("M6  T7 não estorna o crédito", "            devolver_credito(m);                                  /* estorno integral */", "            m->credito = 0; m->devolvido = 0;", False),
 ("M7  T9 não devolve o crédito", "        if (e == EVT_FALHA) {                                     /* T9 */\n            devolver_credito(m);", "        if (e == EVT_FALHA) {                                     /* T9 */\n            m->credito = 0; m->devolvido = 0;", False),
 ("M8  T4 rejeitada", "            return 1;                                             /* T4: crédito insuficiente, permanece */", "            return m->estado == ESTADO_DISPENSANDO;", False),
 ("M9  T5 vai para FORA_DE_SERVICO", "            devolver_credito(m);\n            m->estado = ESTADO_OCIOSA;", "            devolver_credito(m);\n            m->estado = ESTADO_FORA_DE_SERVICO;", False),
 ("M10 E4 aceita falha", "    case ESTADO_FORA_DE_SERVICO:\n        if (e == EVT_REPARAR)", "    case ESTADO_FORA_DE_SERVICO:\n        if (e == EVT_FALHA) return 1;\n        if (e == EVT_REPARAR)", False),
 ("M11 E3 aceita cancelar", "    case ESTADO_DISPENSANDO:\n", "    case ESTADO_DISPENSANDO:\n        if (e == EVT_CANCELAR) { devolver_credito(m); m->estado = ESTADO_OCIOSA; return 1; }\n", False),
 ("M12 T10 não zera o crédito (equivalente)", "            m->credito = 0;\n            m->devolvido = 0;\n            m->estado = ESTADO_OCIOSA;\n            return 1;\n        }\n        return 0;\n    }\n    return 0;", "            m->devolvido = 0;\n            m->estado = ESTADO_OCIOSA;\n            return 1;\n        }\n        return 0;\n    }\n    return 0;", True),
 ("M13 T3 não zera devolvido (equivalente)", "        if (e == EVT_SELECIONAR_PRODUTO) {\n            m->devolvido = 0;", "        if (e == EVT_SELECIONAR_PRODUTO) {", True),
 ("M14 aceita qualquer moeda positiva", "    return valor == 1 || valor == 2;", "    return valor > 0;", False),
 ("M15 T6 não zera o crédito", "            m->devolvido = m->credito - PRECO_PRODUTO;            /* troco */\n            m->credito = 0;", "            m->devolvido = m->credito - PRECO_PRODUTO;            /* troco */", False),
 ("M16 T8 vai para COM_CREDITO", "            m->devolvido = 0;\n            m->estado = ESTADO_FORA_DE_SERVICO;\n            return 1;\n        }\n        return 0;\n\n    case ESTADO_COM_CREDITO", "            m->devolvido = 0;\n            m->estado = ESTADO_COM_CREDITO;\n            return 1;\n        }\n        return 0;\n\n    case ESTADO_COM_CREDITO", False),
 ("M17 T5 não devolve o crédito", "        if (e == EVT_CANCELAR) {                                  /* T5 */\n            devolver_credito(m);", "        if (e == EVT_CANCELAR) {                                  /* T5 */\n            m->credito = 0;", False),
]

def detectado(codigo, pasta):
    arq = os.path.join(pasta, "maquina.c")
    open(arq, "w", encoding="utf-8").write(codigo)
    for t in SUITES:
        exe = os.path.join(pasta, "t_" + t)
        c = subprocess.run([CC, "-std=c11", "-Isrc", "-Itests", f"tests/test_{t}.c", arq, "-o", exe], capture_output=True)
        if c.returncode != 0 or subprocess.run([exe], capture_output=True).returncode != 0:
            return True
    return False

def main():
    mortos = sobreviventes_ruins = 0
    with tempfile.TemporaryDirectory() as pasta:
        for nome, a, b, equiv in MUTANTES:
            if a not in SRC:
                sys.exit(f"padrão não encontrado em src/maquina.c: {nome}")
            morto = detectado(SRC.replace(a, b, 1), pasta)
            mortos += morto
            if not morto and not equiv:
                sobreviventes_ruins += 1
            print(("detectado   " if morto else "SOBREVIVEU  ") + nome)
    n = len(MUTANTES); eq = sum(m[3] for m in MUTANTES)
    print(f"\n{mortos} de {n} mutantes detectados ({n - eq} não equivalentes, {eq} equivalentes)")
    sys.exit(1 if sobreviventes_ruins else 0)

main()
