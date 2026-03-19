// ============================================================
//  A* Search Algorithm
//  Optimal AND informed — uses heuristic to explore smarter.
//  Manhattan distance heuristic is admissible for 4-dir grids
//  (never overestimates), guaranteeing optimal path.
//  Time: O(E log V)   Space: O(V)
//
//  Key insight: f(n) = g(n) + h(n)
//    g(n) = actual cost from start to n
//    h(n) = estimated cost from n to end (Manhattan distance)
//    f(n) = total estimated path cost through n
// ============================================================
#include "../grid.h"
#include <queue>
#include <tuple>
#include <cmath>

static inline int manhattan(int r1, int c1, int r2, int c2) {
    return std::abs(r1 - r2) + std::abs(c1 - c2);
}

PathResult run_astar(Grid& grid, int sr, int sc, int er, int ec) {
    PathResult result;
    grid.reset();

    // Min-heap: (f_cost, g_cost, row, col)
    using Entry = std::tuple<int, int, int, int>;
    std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> open;

    grid.at(sr, sc).g_cost = 0;
    grid.at(sr, sc).h_cost = manhattan(sr, sc, er, ec);
    grid.at(sr, sc).f_cost = grid.at(sr, sc).h_cost;
    open.push({grid.at(sr, sc).f_cost, 0, sr, sc});

    while (!open.empty()) {
        auto [f, g, r, c] = open.top(); open.pop();

        if (grid.at(r, c).visited) continue;
        grid.at(r, c).visited = true;
        result.visited_order.push_back({r, c});

        if (r == er && c == ec) {
            result.found = true;
            int cr = er, cc = ec;
            while (cr != -1) {
                result.cost += grid.at(cr, cc).weight;
                result.path.push_back({cr, cc});
                int pr = grid.at(cr, cc).parent_row;
                int pc = grid.at(cr, cc).parent_col;
                cr = pr; cc = pc;
            }
            std::reverse(result.path.begin(), result.path.end());
            return result;
        }

        for (int d = 0; d < 4; d++) {
            int nr = r + DR4[d], nc = c + DC4[d];
            if (!grid.isWalkable(nr, nc) || grid.at(nr, nc).visited) continue;
            int new_g = g + grid.at(nr, nc).weight;
            if (new_g < grid.at(nr, nc).g_cost) {
                grid.at(nr, nc).g_cost     = new_g;
                grid.at(nr, nc).h_cost     = manhattan(nr, nc, er, ec);
                grid.at(nr, nc).f_cost     = new_g + grid.at(nr, nc).h_cost;
                grid.at(nr, nc).parent_row = r;
                grid.at(nr, nc).parent_col = c;
                open.push({grid.at(nr, nc).f_cost, new_g, nr, nc});
            }
        }
    }
    return result;
}
