import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.patches import FancyBboxPatch, FancyArrowPatch
from matplotlib.path import Path

plt.rcParams["font.family"] = "Liberation Sans"
AZUL, VERM, CINZA = "#2F5D8C", "#9B3B3B", "#333333"

fig, ax = plt.subplots(figsize=(10, 6.4))
ax.set_xlim(-2.2, 12.8); ax.set_ylim(-5.0, 4.2); ax.axis("off"); ax.set_aspect("equal")

W, H = 2.8, 1.0
nodes = {"E1": (0, 0, 2.8, "E1\nOCIOSA", AZUL, "#EEF3F8"),
         "E2": (5.2, 0, 2.8, "E2\nCOM_CREDITO", AZUL, "#EEF3F8"),
         "E3": (10.8, 0, 2.8, "E3\nDISPENSANDO", AZUL, "#EEF3F8"),
         "E4": (5.2, -3.7, 3.4, "E4\nFORA_DE_SERVICO", VERM, "#F6E9E9")}
for k, (x, y, w, t, ec, fc) in nodes.items():
    ax.add_patch(FancyBboxPatch((x - w/2, y - H/2), w, H, boxstyle="round,pad=0,rounding_size=0.18",
                                fc=fc, ec=ec, lw=1.6, zorder=3))
    ax.text(x, y, t, ha="center", va="center", fontsize=10.5, zorder=4, linespacing=1.15)

def seta(p, q, rad=0.0):
    ax.add_patch(FancyArrowPatch(p, q, arrowstyle="-|>", mutation_scale=11, color=CINZA, lw=1.2,
                                 connectionstyle=f"arc3,rad={rad}", zorder=2, shrinkA=0, shrinkB=0))

def laco(pts):
    path = Path(pts, [Path.MOVETO, Path.CURVE4, Path.CURVE4, Path.CURVE4])
    ax.add_patch(FancyArrowPatch(path=path, arrowstyle="-|>", mutation_scale=11, color=CINZA, lw=1.2, zorder=2))

def rot(x, y, s, ha="center", va="center"):
    ax.text(x, y, s, ha=ha, va=va, fontsize=8.6, color="#111111", linespacing=1.2, zorder=5)

ax.add_patch(plt.Circle((-1.9, 0), 0.12, color=CINZA, zorder=3)); seta((-1.78, 0), (-1.4, 0))

seta((1.4, 0.22), (3.8, 0.22)); rot(2.6, 0.66, "T1: inserir_moeda\n/ credito += valor")
seta((3.8, -0.22), (1.4, -0.22)); rot(2.6, -0.66, "T5: cancelar\n/ devolve credito")
seta((6.6, 0), (9.4, 0)); rot(8.0, 0.46, "T3: selecionar_produto\n[credito >= preco]")
laco([(4.75, 0.5), (4.2, 1.7), (6.2, 1.7), (5.65, 0.5)]); rot(5.2, 1.6, "T2: inserir_moeda / credito += valor", va="bottom")
seta((10.8, 0.5), (0, 0.5), rad=0.5); rot(5.4, 3.45, "T6: entrega_concluida / devolve troco", va="bottom")
laco([(5.7, -0.5), (5.5, -1.75), (7.1, -1.75), (6.5, -0.5)]); rot(7.35, -1.15, "T4: selecionar_produto\n[credito < preco]", ha="left")
seta((4.5, -0.5), (4.5, -3.2)); rot(4.65, -2.4, "T9: falha\n/ devolve credito", ha="left")
seta((-0.6, -0.5), (3.5, -3.95), rad=0.25); rot(-0.2, -2.6, "T8: falha", ha="right")
seta((3.5, -3.45), (0.6, -0.5), rad=-0.25); rot(1.5, -1.6, "T10: reparar", ha="left")
seta((10.8, -0.5), (6.9, -3.7), rad=-0.2); rot(9.95, -2.55, "T7: falha\n/ estorna credito", ha="left")

fig.savefig("estados.png", dpi=220, bbox_inches="tight", facecolor="white")
