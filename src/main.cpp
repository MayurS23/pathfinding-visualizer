// ============================================================
//  main.cpp — WebAssembly Entry Point
//  All exported functions use extern "C" to prevent C++ name
//  mangling. EMSCRIPTEN_KEEPALIVE prevents dead-code stripping.
//
//  Memory layout for JS interop:
//    JS allocates grid arrays via _malloc
//    C++ reads them from WASM linear memory
//    Results written to static buffers
//    JS reads results via HEAP32 typed array view
// ============================================================
#include <emscripten.h>
#include <vector>
#include <cstring>
#include <cstdlib>
#include "grid.h"

// Forward declarations
PathResult run_bfs(Grid&, int, int, int, int);
PathResult run_dfs(Grid&, int, int, int, int);
PathResult run_dijkstra(Grid&, int, int, int, int);
PathResult run_astar(Grid&, int, int, int, int);

// ── Persistent module state in WASM linear memory ───────────
static Grid       g_grid;
static PathResult g_result;
static std::vector<int> g_visited_buf;  // [r0,c0, r1,c1, ...]
static std::vector<int> g_path_buf;     // [r0,c0, r1,c1, ...]

extern "C" {

// Initialize the grid from JS-provided flat arrays.
// wallData[i*cols+j] == 1 means cell (i,j) is a wall.
// weightData[i*cols+j] is the movement cost through that cell.
EMSCRIPTEN_KEEPALIVE
void init_grid(int rows, int cols, int* wallData, int* weightData) {
    g_grid = Grid(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int idx = i * cols + j;
            if (wallData[idx] == 1)
                g_grid.at(i, j).type = NodeType::WALL;
            g_grid.at(i, j).weight = weightData ? weightData[idx] : 1;
        }
    }
}

// Run selected algorithm and return 1 if path found, 0 otherwise.
// algo: 0=BFS, 1=DFS, 2=Dijkstra, 3=A*
EMSCRIPTEN_KEEPALIVE
int run_algorithm(int algo, int sr, int sc, int er, int ec) {
    switch (algo) {
        case 0: g_result = run_bfs(g_grid, sr, sc, er, ec);      break;
        case 1: g_result = run_dfs(g_grid, sr, sc, er, ec);      break;
        case 2: g_result = run_dijkstra(g_grid, sr, sc, er, ec); break;
        case 3: g_result = run_astar(g_grid, sr, sc, er, ec);    break;
        default: return -1;
    }

    // Encode results as flat int arrays for efficient JS reads
    g_visited_buf.clear();
    g_visited_buf.reserve(g_result.visited_order.size() * 2);
    for (auto& [r, c] : g_result.visited_order) {
        g_visited_buf.push_back(r);
        g_visited_buf.push_back(c);
    }

    g_path_buf.clear();
    g_path_buf.reserve(g_result.path.size() * 2);
    for (auto& [r, c] : g_result.path) {
        g_path_buf.push_back(r);
        g_path_buf.push_back(c);
    }

    return g_result.found ? 1 : 0;
}

// Accessors — JS reads result data through WASM memory views
EMSCRIPTEN_KEEPALIVE int* get_visited_buffer() { return g_visited_buf.data(); }
EMSCRIPTEN_KEEPALIVE int  get_visited_count()  { return (int)g_visited_buf.size() / 2; }
EMSCRIPTEN_KEEPALIVE int* get_path_buffer()    { return g_path_buf.data(); }
EMSCRIPTEN_KEEPALIVE int  get_path_length()    { return (int)g_path_buf.size() / 2; }
EMSCRIPTEN_KEEPALIVE int  get_path_cost()      { return g_result.cost; }

} // extern "C"

int main() {
    // Module stays alive — JS drives everything via exported functions
    return 0;
}
