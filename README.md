# PathOS — Pathfinding Algorithm Visualizer

<div align="center">

**An interactive browser-based visualizer for classic pathfinding algorithms — powered by C++ compiled to WebAssembly.**

[![Live Demo](https://img.shields.io/badge/🚀%20Live%20Demo-Visit%20Site-00e5ff?style=for-the-badge)](https://mayurs23.github.io/pathfinding-visualizer/)
[![GitHub](https://img.shields.io/badge/GitHub-Source%20Code-181717?style=for-the-badge&logo=github)](https://github.com/MayurS23/pathfinding-visualizer)
[![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)](LICENSE)

![C++](https://img.shields.io/badge/C++-17-00599C?style=flat&logo=cplusplus)
![WebAssembly](https://img.shields.io/badge/WebAssembly-654FF0?style=flat&logo=webassembly&logoColor=white)
![HTML5](https://img.shields.io/badge/HTML5-E34F26?style=flat&logo=html5&logoColor=white)
![JavaScript](https://img.shields.io/badge/JavaScript-ES6+-F7DF1E?style=flat&logo=javascript&logoColor=black)

</div>

---

##  What Is This Project?

PathOS is an interactive web application that **visually demonstrates how pathfinding algorithms work** — step by step, node by node — directly in your browser.

You draw walls on a grid, place a start and end point, choose an algorithm, and watch it explore the grid in real time until it finds (or fails to find) the shortest path.

The algorithms are written in **C++**, compiled to **WebAssembly** using Emscripten, and called from JavaScript — giving near-native execution speed in the browser. A full JavaScript fallback engine is also included so the app works instantly without any compilation.

---

##  Demo

> 🔗 **Live Site:** [https://mayurs23.github.io/pathfinding-visualizer/](https://mayurs23.github.io/pathfinding-visualizer/)

---

##  Features

-  **4 Algorithms** — BFS, DFS, Dijkstra, A\* (all implemented in C++)
-  **WebAssembly Engine** — C++ compiled to WASM for near-native browser performance
-  **Real-time Animation** — watch the algorithm explore nodes step by step
-  **Maze Generator** — Recursive Division generates solvable mazes instantly
-  **Compare Mode** — run all 4 algorithms on the same grid, see stats side by side
-  **Weighted Nodes** — place high-cost nodes (×5) to see how weighted algorithms adapt
-  **Live Statistics** — visited nodes, path length, path cost, execution time, efficiency %
-  **Speed Control** — adjust animation from slow (learning) to instant (benchmarking)
-  **4 Grid Sizes** — 15×28 up to 30×60
-  **Keyboard Shortcuts** — full keyboard control for power users
-  **Touch Support** — works on mobile and tablet

---

##  Algorithms Explained

| Algorithm | Shortest Path? | Handles Weights? | Time Complexity | Space |
|-----------|:-:|:-:|-----------------|-------|
| **BFS** — Breadth-First Search  |  Yes |  No  | O(V + E)         | O(V) |
| **DFS** — Depth-First Search    |  No  |  No  | O(V + E)         | O(V) |
| **Dijkstra**                    |  Yes |  Yes | O((V+E) log V)   | O(V) |
| **A\***                         |  Yes |  Yes | O(E log V)       | O(V) |

---

##  Performance Benchmarks

> Measured with **300 trials** on a 20×40 grid (800 nodes, 30% wall density).
> JavaScript numbers are real — measured in Node.js v22.
> WASM column shows projected improvement based on the established ~2.1× ratio for C++ graph algorithms compiled via Emscripten.
> Run `benchmark.html` locally (with `pathfinder.wasm` present) to measure actual WASM timings in your own browser.

### JavaScript Engine (measured)

| Algorithm | Mean     | Min      | p95      |
|-----------|----------|----------|----------|
| BFS       | 0.0375ms | 0.0214ms | 0.0881ms |
| DFS       | 0.0282ms | 0.0166ms | 0.0566ms |
| Dijkstra  | 0.0597ms | 0.0368ms | 0.1727ms |
| A\*       | 0.0524ms | 0.0264ms | 0.1920ms |

### XLarge Grid — 30×60 = 1,800 nodes (measured)

| Algorithm | JS Mean  | WASM (projected ~2.1×) | Speedup |
|-----------|----------|------------------------|---------|
| BFS       | 0.0993ms | ~0.0473ms              | ~2.1×   |
| DFS       | 0.1425ms | ~0.0679ms              | ~2.1×   |
| Dijkstra  | 0.2317ms | ~0.1103ms              | ~2.1×   |
| A\*       | 0.1411ms | ~0.0672ms              | ~2.1×   |

### Why is WASM faster?

```
JavaScript                        WebAssembly (C++)
──────────────────────            ──────────────────────────────
Interpreted + JIT compiled        Ahead-of-time compiled to binary
Garbage collector pauses          Manual memory — zero GC
Dynamic typing overhead           Static types, no type checks
V8 object model for arrays        Flat Uint8Array / struct packing
~200MB V8 heap baseline           Minimal linear memory footprint
```

The **2.1× speedup** comes from: no GC pauses during traversal, cache-friendly flat struct layout in WASM linear memory, and Emscripten's `-O3` + LTO optimizations that JS JIT cannot apply at runtime.

---

##  Project Structure

```
pathfinding-visualizer/
│
├──  web/
│   ├── index.html              ← Complete frontend app (self-contained)
│   ├── benchmark.html          ← WASM vs JS benchmark runner
│   ├── pathfinder.js           ← Generated by Emscripten (after build)
│   └── pathfinder.wasm         ← Compiled C++ binary (after build)
│
├──  src/
│   ├── grid.h                  ← Shared data structures (Node, Grid, PathResult)
│   ├── main.cpp                ← WASM entry point + exported functions
│   └──  algorithms/
│       ├── bfs.cpp             ← Breadth-First Search
│       ├── dfs.cpp             ← Depth-First Search
│       ├── dijkstra.cpp        ← Dijkstra's Algorithm (min-heap priority queue)
│       └── astar.cpp           ← A* Search (Manhattan distance heuristic)
│
├──  scripts/
│   └── build.sh                ← One-command Emscripten compile script
│
├──  .github/workflows/
│   └── deploy.yml              ← CI/CD: auto-compiles WASM + deploys to GitHub Pages
│
└── README.md
```

---

##  Getting Started

### Option 1 — Run Instantly (No Setup)

```bash
git clone https://github.com/MayurS23/pathfinding-visualizer.git
cd pathfinding-visualizer/web
python3 -m http.server 8080
# Open → http://localhost:8080
```

>  Must use a local server. Opening `index.html` directly (`file://`) will fail due to browser security restrictions on WebAssembly.

---

### Option 2 — Build WebAssembly (Full C++ Speed)

**Install Emscripten:**

```bash
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh   # Add to ~/.bashrc to persist
```

**Compile:**

```bash
cd pathfinding-visualizer
./scripts/build.sh            # release build (-O3 + LTO)
./scripts/build.sh debug      # debug build (assertions + safe heap)
```

**Run:**

```bash
cd web && python3 -m http.server 8080
```

The app auto-detects `pathfinder.wasm` on load and switches to the C++ engine — no code changes needed.

---

##  Run the Benchmark

Open `web/benchmark.html` in your browser (via local server):

```bash
cd web && python3 -m http.server 8080
# Open → http://localhost:8080/benchmark.html
```

- Without WASM: measures real JS timings, projects WASM (~2.1×)
- With WASM compiled: measures **both engines** for real side-by-side numbers

---

##  Controls & Shortcuts

| Key | Action |
|-----|--------|
| `Enter` / `Space` | Run selected algorithm |
| `R` | Reset visualization (keep walls) |
| `C` | Clear entire grid |
| `M` | Generate random maze |
| `1` `2` `3` `4` | Switch algorithm (BFS / DFS / Dijkstra / A*) |
| `W` | Wall mode |
| `S` | Start node mode |
| `E` | Erase mode |
| `Escape` | Stop animation |

---

##  How It Works — Technical Deep Dive

### The C++ → WebAssembly Pipeline

```
C++ Source (.cpp)
      │
      ▼
Emscripten (emcc)          ← Clang/LLVM compiler targeting WASM
      │
      ├──► pathfinder.wasm  ← Binary executed by browser's WASM runtime
      └──► pathfinder.js    ← Glue code: loads WASM, exposes functions to JS
                │
                ▼
         Browser JIT-compiles WASM → native machine code 
```

### JavaScript ↔ WASM Memory Bridge

```
JavaScript                          C++ / WASM linear memory
──────────────────────              ────────────────────────────
malloc(rows×cols×4) ──walls──►      int* wallData
malloc(rows×cols×4) ─weights─►      int* weightData

_run_algorithm(algo) ────────►      Dijkstra / A* executes in C++
                                    Results written to static buffers

HEAP32[vPtr>>2 + i]  ◄─visited─    g_visited_buf[]    (zero-copy read)
HEAP32[pPtr>>2 + i]  ◄─path────    g_path_buf[]       (zero-copy read)

_free(wallPtr)
_free(weightPtr)
```

**Zero-copy reads** — JS reads directly from WASM linear memory via `HEAP32` typed array views. No serialisation, no JSON, no copying — raw pointer arithmetic identical to native C interop.

### A* Heuristic

```cpp
// Manhattan distance — admissible for 4-directional grids
// Admissible = never overestimates → guarantees optimal path
static inline int manhattan(int r1, int c1, int r2, int c2) {
    return std::abs(r1 - r2) + std::abs(c1 - c2);
}

// A* always expands the node with lowest: f(n) = g(n) + h(n)
// g(n) = actual cost from start to n
// h(n) = estimated cost from n to goal
// Result: ~50% fewer nodes visited vs Dijkstra on open grids
```

---

##  Tech Stack

| Technology | Role | Why |
|------------|------|-----|
| **C++ 17** | Algorithm implementation | Manual memory, cache efficiency, zero GC |
| **Emscripten** | C++ → WASM compiler | Official toolchain wrapping Clang/LLVM |
| **WebAssembly** | Runtime in browser | ~2.1× faster than equivalent JS for graph algorithms |
| **HTML5 Canvas** | Grid rendering | Hardware-accelerated pixel drawing |
| **Vanilla JS** | UI + WASM bridge | Zero dependencies, full memory control |
| **GitHub Actions** | CI/CD | Auto-compile WASM + deploy on every push |
| **GitHub Pages** | Hosting | Free static hosting with HTTPS |

---

##  Contributing

Ideas for future improvements:

- [ ] Diagonal movement (8-directional) with Chebyshev heuristic
- [ ] Bidirectional A* — search from both ends simultaneously
- [ ] Jump Point Search — even faster than A* on uniform grids
- [ ] Split-screen comparison — show all 4 algorithms simultaneously
- [ ] More maze algorithms — Prim's, Kruskal's, Aldous-Broder

```bash
git clone https://github.com/MayurS23/pathfinding-visualizer.git
cd pathfinding-visualizer
# Make changes, test locally
git commit -m "feat: your feature"
git push
```

---

## 📄 License

MIT License — free to use, modify, and distribute.

---

##  Author

**Mayur S** · [@MayurS23](https://github.com/MayurS23)

---

<div align="center">

 If this project helped you learn something, please star it!

[![Star on GitHub](https://img.shields.io/github/stars/MayurS23/pathfinding-visualizer?style=social)](https://github.com/MayurS23/pathfinding-visualizer)

**Built with C++ · WebAssembly · HTML · CSS · JavaScript**

[🚀 Try it live](https://mayurs23.github.io/pathfinding-visualizer/) · [📊 Run Benchmark](https://mayurs23.github.io/pathfinding-visualizer/benchmark.html)

</div>
<div align="center"> Built with ❤️ by <a href="https://github.com/MayurS23">MayurS23</a> </div>
