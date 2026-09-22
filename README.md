# vStrike

A cyberpunk-themed battle pong game built from scratch in C++17 and Raylib, compiled for both desktop and the web (WebAssembly).

**Play in your browser:** [pixelsmoothie.github.io/vStrike](https://pixelsmoothie.github.io/vStrike/)

---

## What is this?

Standard Pong gets boring fast, so I wanted to turn it into a combat game:
- **Health bars instead of points:** You don't just score — you deplete your opponent's HP with every missed volley.
- **Rally multipliers:** The longer the rally goes on, the faster and heavier the ball gets.
- **Three ways to play:** Hop into a local 1v1 couch match, connect across your Wi-Fi for LAN multiplayer, or fight a self-learning bot.

<p align="center">
  <img src="assets/screen_flow.png" width="460" alt="Screen Flow" />
</p>

---

## The AI: Teaching a Bot to Play Pong from Scratch

I wanted the bot to actually learn the game instead of following hardcoded `paddle.y = ball.y` tracking rules. But I also didn't want to drag in heavy frameworks like PyTorch or TensorFlow just for a 2D game.

Everything is written in pure C++ using **Tabular Q-learning** stored in a hash map (`std::unordered_map`).

<p align="center">
  <img src="assets/rl_loop.png" width="520" alt="Q-Learning Loop" />
</p>

### Shrinking the World (512 States)
An 800×1000 canvas has nearly a million coordinate combinations — way too huge for a basic lookup table. To keep it lightweight and fast, I bucketed the coordinates into a grid:
- **Ball X:** 4 horizontal zones
- **Ball Y:** 8 vertical zones
- **Paddle Y:** 8 vertical zones
- **Ball Direction:** 2 states (heading left or right)

```
State ID = (Ball_X) + (Ball_Y * 4) + (Paddle_Y * 32) + (Dir_X * 256)
```
This condenses the entire playing field down to **512 discrete states**.

### How It Learns
1. **The Bellman Update:** Every step, the bot updates its confidence for each action:
   $$Q(s, a) \leftarrow Q(s, a) + \alpha \left[ R + \gamma \max_{a'} Q(s', a') - Q(s, a) \right]$$
   *(Learning rate $\alpha = 0.1$, Discount factor $\gamma = 0.9$)*
2. **Reward shaping:** If you only give the bot points when it wins or loses, it takes forever to figure out what it did right. To speed it up, it gets a tiny negative penalty every frame based on how far its paddle center is from the ball:
   $$R_{\text{frame}} = - (|\text{Paddle}_{\text{center}} - \text{Ball}_Y| \times 0.1)$$
   Successful hits award $+100$, and conceding a goal gives $-100$.
3. **Exploration decay:** Starts out trying random moves ($\epsilon = 1.0$) and gradually settles into its learned strategy ($\epsilon \rightarrow 0.005$).

### The Result

Around 200 episodes in, the bot stops flailing and starts tracking rallies cleanly. By episode 400+, it holds consistent defense and punishes tricky angles.

<p align="center">
  <img src="convergence_graph.png" width="600" alt="Convergence Graph" />
</p>

---

## Tech Stack

- **Game Engine & Graphics:** C++17, Raylib 5.0
- **Web Export:** Emscripten (WebAssembly + WebGL)
- **Networking:** ENet (UDP, authoritative host @ 60 Hz)
- **Fonts & Visuals:** IBM Plex Mono, custom chamfered shaders

---

## Getting Started

### Prerequisites
- CMake 3.20+
- A modern C++17 compiler (GCC, Clang, or MSVC)

### Build & Run (Desktop)
```bash
git clone https://github.com/pixelsmoothie/vStrike.git
cd vStrike

cmake -B build -S .
cmake --build build --config Release

# Run
./build/PongArena
```

### Build for Web (WebAssembly)
Make sure you have the [Emscripten SDK](https://emscripten.org/) installed and active:
```bash
emcmake cmake -B build-web -DPLATFORM=Web
cmake --build build-web

# Serve locally
python -m http.server 8080 --directory build-web
```

---

## Playing LAN Multiplayer

You can play against a friend on the same local Wi-Fi:

1. **Player 1 (Host):** Go to **Multiplayer** → press `H` to host.
2. Check your local IP (`ipconfig` on Windows, `ip a` on Linux) — for example `192.168.1.15`.
3. **Player 2 (Client):** Go to **Multiplayer** → enter Player 1's IP → press `J` to connect.

The host runs the simulation and replicates paddle and ball state to the client at 60 Hz over UDP.

---

## Stress Testing the Netcode

To verify packet reliability under burst traffic, there's a standalone test harness in `tests/net_stress_test.cpp`:

```bash
g++ -std=c++17 -DENET_IPV4_ONLY tests/net_stress_test.cpp networking/enet_impl.cpp -o stress_test -lws2_32
./stress_test
```
This pumps 500+ packets in bursts and prints round-trip time (RTT), drop rates, and bandwidth stats.

---

## License

MIT License. Feel free to fork, experiment, or build on it!
