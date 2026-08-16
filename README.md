# vStrike ⚡ — Real-Time Action Pong with Tabular Reinforcement Learning

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B17)
[![Raylib](https://img.shields.io/badge/Rendered%20With-Raylib%205.0-red.svg)](https://www.raylib.com/)
[![CMake](https://img.shields.io/badge/Build-CMake-brightgreen.svg)](https://cmake.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

> An action-oriented Pong arena game built in modern C++17 featuring a custom, from-scratch **Tabular Q-Learning Reinforcement Learning AI** that learns rally dynamics, positioning, and trajectory prediction through self-play and empirical reward optimization.

---

## 📌 Key Highlights

- **Zero Black-Box ML Libraries:** Entire Q-Learning agent, Bellman update rule, epsilon-greedy action selection, and state discretization engineered from scratch in pure C++ (`std::unordered_map`).
- **512-State Discrete Representation:** Solved the continuous state-space explosion problem through multi-dimensional spatial partitioning ($4 \times 8 \times 8 \times 2 = 512$).
- **Dense Continuous Reward Shaping:** Formulated dynamic gradient-based feedback to overcome sparse reward traps and stabilize convergence.
- **Empirical Telemetry & Convergence:** Automated per-episode metrics logging (`metrics.csv`) demonstrating quantifiable win-rate jumps from **0% to ~75%** against an aim-bot sparring dummy.

---

## 🧠 Reinforcement Learning Architecture

```
                      ┌───────────────────────────────┐
                      │      Game Environment         │
                      │  (Ball & Paddle Kinematics)   │
                      └───────┬───────────────▲───────┘
                              │               │
                 State (s)    │               │ Action (a)
           Discretized to 512 │               │ [UP, DOWN, STAY]
                              ▼               │
                      ┌───────────────────────┴───────┐
                      │         Q-Brain Agent         │
                      │  - Epsilon-Greedy Policy      │
                      │  - Bellman Q-Table Updates    │
                      │  - Continuous Reward Shaping  │
                      └───────────────────────────────┘
```

### 1. State Space Discretization (512 States)
To avoid the *Curse of Dimensionality* in a continuous 2D coordinate space ($1000 \times 800$), the environment is mapped into a discrete 1D integer state ID:

$$\text{State ID} = (\text{Ball}_X) + (\text{Ball}_Y \times 4) + (\text{Paddle}_Y \times 32) + (\text{Dir}_X \times 256)$$

- **$\text{Ball}_X$:** 4 horizontal spatial buckets
- **$\text{Ball}_Y$:** 8 vertical spatial buckets
- **$\text{Paddle}_Y$:** 8 vertical spatial buckets
- **$\text{Dir}_X$:** 2 binary states (moving left / right)

### 2. Bellman Update Rule
The Q-table updates iteratively using the standard temporal difference formulation:

$$Q(s, a) \leftarrow Q(s, a) + \alpha \left[ R + \gamma \max_{a'} Q(s', a') - Q(s, a) \right]$$

- **Learning Rate ($\alpha$):** `0.1`
- **Discount Factor ($\gamma$):** `0.9`
- **Exploration Rate ($\epsilon$):** Decays by `0.005` per episode ($1.0 \rightarrow 0.005$)

### 3. Continuous Reward Shaping
To eliminate the sparse reward problem (where an agent only receives feedback upon terminal point events), the loss function integrates a frame-by-frame distance penalty:

$$R_{\text{dense}} = - \left( |\text{Paddle}_{\text{center}} - \text{Ball}_Y| \times 0.1 \right)$$
$$R_{\text{terminal}} = +100 \text{ (Hit / Scoring)}, \quad -100 \text{ (Missed Rally)}$$

---

## 📊 Training & Convergence Results

The AI was trained using an automated self-play sparring dummy. Over **450+ episodes**, the agent transitioned from stochastic exploration to high-accuracy exploitation:

![Convergence Analysis](convergence_graph.png)

| Training Phase | Episodes | Epsilon ($\epsilon$) | Win Rate | Average Reward |
|---|---|---|---|---|
| **Early Exploration** | 1 – 50 | $1.00 \rightarrow 0.75$ | **0%** | $-1500 \text{ to } -2400$ |
| **Transition Phase** | 51 – 200 | $0.75 \rightarrow 0.01$ | **~30%** | $+500 \text{ to } +2000$ |
| **Exploitation / Convergence** | 201 – 450+ | $0.005$ | **~75%** | **$+2500 \text{ to } +5350$** |

---

## 🛠️ Tech Stack & Architecture

- **Language:** C++17
- **Rendering & Windowing:** Raylib 5.0
- **Build System:** CMake $\ge$ 3.20 + Ninja
- **Analytics:** Python 3 (Pandas, Matplotlib)
- **Design Patterns:**
  - State Machine Pattern (`GameScreen`, `GameStates`)
  - Component-based Entity Architecture (`Paddle`, `Ball`)
  - Decoupled Physics Engine (`physicsEngine.cpp`)

---

## 🚀 Building and Running

### Prerequisites
- CMake $\ge 3.20$
- Modern C++ compiler (`g++`, `clang++`, or MSVC with C++17 support)

### Build Instructions
```bash
# 1. Clone repository
git clone https://github.com/pixelsmoothie/vStrike.git
cd vStrike

# 2. Configure with CMake
cmake -B build -S .

# 3. Build target
cmake --build build --config Release

# 4. Run executable
./build/PongArena
```

### Visualizing Metrics
```bash
pip install pandas matplotlib
python data_vizualization/bot_perf_metrics.py
```

---

## 🗺️ Roadmap

- [x] Modern C++17 Pong Arena with Raylib 5.0
- [x] Custom Tabular Q-Learning Engine with 512-State Discretization
- [x] Per-Episode Metrics Telemetry & Convergence Visualizer
- [ ] Automated Unit Testing Suite (Bellman Math & AABB Collisions)
- [ ] In-Game Live Real-Time Telemetry Dashboard
- [ ] Networked Multiplayer (Client-Server UDP Architecture with Lag Compensation)

---

## 📄 License
Distributed under the MIT License. See `LICENSE` for details.
