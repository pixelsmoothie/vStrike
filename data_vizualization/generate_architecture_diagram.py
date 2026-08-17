"""
Pixel-perfect PNG diagram generator matching user exact layout and right-angle orthogonal wiring.
Exports directly to assets/screen_flow.png and assets/rl_loop.png.
"""

import matplotlib.pyplot as plt
import matplotlib.patches as patches

# Exact dark-theme palette
BG_COLOR = '#141414'
CARD_BG = '#1A1A1A'
BORDER_GRAY = '#4A4A4A'
BORDER_PURPLE = '#A371F7'
TEXT_WHITE = '#FFFFFF'
TEXT_GRAY = '#8B949E'
ARROW_COLOR = '#8B949E'
LABEL_BG = '#22272E'

def draw_card(ax, x, y, w, h, line1, line2="", border=BORDER_GRAY, is_purple=False):
    """Draws a crisp dark-theme card with precise 2-line centered text."""
    box = patches.FancyBboxPatch(
        (x - w/2, y - h/2), w, h,
        boxstyle="square,pad=0.0",
        facecolor=CARD_BG,
        edgecolor=BORDER_PURPLE if is_purple else border,
        linewidth=1.8 if is_purple else 1.2
    )
    ax.add_patch(box)
    if line2:
        ax.text(x, y + 0.12, line1, ha='center', va='center', color=TEXT_WHITE, fontsize=9.5, fontweight='bold')
        ax.text(x, y - 0.14, line2, ha='center', va='center', color=TEXT_WHITE, fontsize=8.5)
    else:
        ax.text(x, y, line1, ha='center', va='center', color=TEXT_WHITE, fontsize=9.5, fontweight='bold')

def draw_badge(ax, x, y, text):
    """Draws a crisp label badge along an arrow."""
    ax.text(
        x, y, text, ha='center', va='center', fontsize=7.5, color=TEXT_GRAY,
        bbox=dict(boxstyle='square,pad=0.3', facecolor=LABEL_BG, edgecolor=BORDER_GRAY, linewidth=0.8)
    )

def draw_ortho_arrow(ax, points, label="", label_pos=None, dashed=False):
    """Draws strict right-angle orthogonal wire paths with arrowheads."""
    xs, ys = zip(*points)
    linestyle = '--' if dashed else '-'
    ax.plot(xs, ys, color=ARROW_COLOR, linewidth=1.2, linestyle=linestyle)
    
    # Draw arrow head on last segment
    p_prev, p_last = points[-2], points[-1]
    dx = p_last[0] - p_prev[0]
    dy = p_last[1] - p_prev[1]
    
    ax.annotate(
        '', xy=p_last, xytext=(p_last[0] - 0.01*dx, p_last[1] - 0.01*dy),
        arrowprops=dict(arrowstyle="-|>", color=ARROW_COLOR, mutation_scale=10, linewidth=1.2)
    )
    
    if label and label_pos:
        draw_badge(ax, label_pos[0], label_pos[1], label)

# ==============================================================================
# DIAGRAM 1: SCREEN FLOW
# ==============================================================================
def render_screen_flow():
    fig, ax = plt.subplots(figsize=(8.5, 8.5), dpi=300)
    fig.patch.set_facecolor(BG_COLOR)
    ax.set_facecolor(BG_COLOR)
    ax.axis('off')
    ax.set_xlim(0, 10)
    ax.set_ylim(0, 10)

    # Cards
    draw_card(ax, 5.0, 9.2, 4.4, 0.7, "1. App Launch & Window Init", "(main.cpp)")
    draw_card(ax, 5.0, 7.8, 4.4, 0.7, "2. Main Menu Screen", "(MenuScreen)")
    
    draw_card(ax, 2.3, 5.8, 3.6, 0.7, "Settings Screen", "(SettingsScreen)")
    draw_card(ax, 6.8, 5.8, 4.4, 0.7, "3. Mode Selection Carousel", "(ModeSelection)")
    
    draw_card(ax, 4.8, 3.8, 3.4, 0.7, "LocalView", "(1v1 PvP Match)")
    draw_card(ax, 8.2, 3.8, 3.4, 0.7, "AIView", "(Q-Learning Bot Match)")
    
    draw_card(ax, 6.5, 1.8, 4.6, 0.7, "4. In-Game Pause & Exit Overlay", "(SettingsOV)")

    # Right-Angle Connectors
    draw_ortho_arrow(ax, [(5.0, 8.85), (5.0, 8.15)])
    
    # Menu to Settings
    draw_ortho_arrow(ax, [(3.8, 7.45), (3.8, 6.6), (2.3, 6.6), (2.3, 6.15)], label="Click 'SETTINGS'", label_pos=(2.5, 7.0))
    # Menu to ModeSelect
    draw_ortho_arrow(ax, [(6.2, 7.45), (6.2, 6.6), (6.8, 6.6), (6.8, 6.15)], label="Click 'START'", label_pos=(6.7, 7.0))
    
    # ModeSelect to LocalView & AIView
    draw_ortho_arrow(ax, [(5.8, 5.45), (5.8, 4.8), (4.8, 4.8), (4.8, 4.15)], label="Select 'LOCAL 1v1'", label_pos=(4.9, 4.9))
    draw_ortho_arrow(ax, [(7.8, 5.45), (7.8, 4.8), (8.2, 4.8), (8.2, 4.15)], label="Select 'R-BOT'", label_pos=(8.2, 4.9))
    
    # Game Views to Pause
    draw_ortho_arrow(ax, [(4.8, 3.45), (4.8, 2.6), (5.8, 2.6), (5.8, 2.15)], label="Press SPACE", label_pos=(4.8, 2.9))
    draw_ortho_arrow(ax, [(8.2, 3.45), (8.2, 2.6), (7.2, 2.6), (7.2, 2.15)], label="Press SPACE", label_pos=(8.2, 2.9))

    plt.tight_layout()
    plt.savefig('d:/iso-space/vstrike/assets/screen_flow.png', bbox_inches='tight', dpi=300, facecolor=fig.get_facecolor(), edgecolor='none')
    plt.close()
    print("[OK] assets/screen_flow.png saved")

# ==============================================================================
# DIAGRAM 2: Q-LEARNING LOOP
# ==============================================================================
def render_rl_loop():
    fig, ax = plt.subplots(figsize=(9.0, 7.8), dpi=300)
    fig.patch.set_facecolor(BG_COLOR)
    ax.set_facecolor(BG_COLOR)
    ax.axis('off')
    ax.set_xlim(0, 10)
    ax.set_ylim(0, 9.0)

    # 1. Top Card: Reward Function
    draw_card(ax, 4.2, 8.0, 4.8, 0.8, "4. Continuous Reward Function", "(Distance Penalty -dist * 0.1 + Hit/Miss +-100)")
    
    # 2. Row 2 Left: Discretizer
    draw_card(ax, 2.9, 6.0, 4.6, 0.8, "1. Spatial Discretizer", "(Maps continuous coordinates into 512-State Grid)")
    
    # 3. Row 2 Right: Bellman Equation (Purple Border)
    draw_card(ax, 7.5, 6.0, 4.4, 0.8, "5. Bellman Equation Update & Q-Table Memory", "Q(s, a) += a [ R + g max Q(s', a') - Q(s, a) ]", is_purple=True)
    
    # 4. Row 3 Center: Policy
    draw_card(ax, 5.3, 3.9, 4.4, 0.8, "2. e-Greedy Decision Policy", "(Explore Random 1.0 -> Exploit Max Q 0.005)")
    
    # 5. Row 4 Bottom: Execute Action
    draw_card(ax, 4.2, 1.8, 4.6, 0.8, "3. Execute Action {UP, DOWN, STAY}", "(Paddle moves towards ball: y += speed * dt)")

    # Right-Angle Connectors
    # Discretizer -> Policy
    draw_ortho_arrow(ax, [(2.9, 5.6), (2.9, 4.6), (4.5, 4.6), (4.5, 4.3)], label="State ID (s)", label_pos=(3.0, 4.85))
    
    # Policy -> Action
    draw_ortho_arrow(ax, [(5.3, 3.5), (5.3, 2.7), (4.2, 2.7), (4.2, 2.2)], label="Chosen Action (a)", label_pos=(5.3, 2.85))
    
    # Action -> Reward (Far left loop around)
    draw_ortho_arrow(ax, [(1.9, 1.8), (0.7, 1.8), (0.7, 7.6), (3.4, 7.6)], label="Resulting Position", label_pos=(0.7, 2.3))
    
    # Reward -> Bellman Memory
    draw_ortho_arrow(ax, [(5.5, 7.6), (7.5, 7.6), (7.5, 6.4)], label="Reward Signal (R)", label_pos=(7.5, 7.2))
    
    # Bellman Memory -> Policy (Dashed Feedback Loop)
    draw_ortho_arrow(ax, [(7.5, 5.6), (7.5, 4.5), (6.5, 4.5)], label="Feeds Updated Q-Values for next frame", label_pos=(7.5, 5.0), dashed=True)

    plt.tight_layout()
    plt.savefig('d:/iso-space/vstrike/assets/rl_loop.png', bbox_inches='tight', dpi=300, facecolor=fig.get_facecolor(), edgecolor='none')
    plt.close()
    print("[OK] assets/rl_loop.png saved")

if __name__ == '__main__':
    render_screen_flow()
    render_rl_loop()
