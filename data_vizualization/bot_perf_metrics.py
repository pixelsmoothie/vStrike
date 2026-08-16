import pandas as pd
import matplotlib.pyplot as plt

# Load data
df = pd.read_csv(r"D:\iso-space\vStrike\metrics.csv")
df['Rolling_Reward'] = df['Total_Reward'].rolling(50, min_periods=1).mean()
df['Win_Rate'] = df['AI_Won'].rolling(50, min_periods=1).mean()

# Dark theme
plt.style.use('dark_background')
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 7), sharex=True)
fig.suptitle('Q-Learning Training Metrics (512 States)', fontsize=16, fontweight='bold')

# Top: Reward
ax1.plot(df['Episode'], df['Total_Reward'], color='gray', alpha=0.3, label='Raw Reward')
ax1.plot(df['Episode'], df['Rolling_Reward'], color='#58A6FF', linewidth=2, label='Rolling Avg')
ax1.axhline(0, color='white', linewidth=0.5, linestyle='--', alpha=0.3)
ax1.set_ylabel('Total Reward')
ax1.legend()
ax1.grid(alpha=0.15)

# Bottom: Win Rate + Epsilon
ax2.plot(df['Episode'], df['Win_Rate'], color='#3FB950', linewidth=2, label='Win Rate')
ax2.plot(df['Episode'], df['Epsilon'], color='#F85149', linewidth=2, linestyle='--', label='Epsilon')
ax2.axhline(0.5, color='white', linewidth=0.5, linestyle=':', alpha=0.3)
ax2.set_xlabel('Episode')
ax2.set_ylabel('Rate (0 to 1)')
ax2.legend()
ax2.grid(alpha=0.15)

plt.tight_layout()
plt.savefig(r'D:\iso-space\vStrike\convergence_graph.png', dpi=200)
print("Saved convergence_graph.png")
plt.show()