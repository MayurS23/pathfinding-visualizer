// ============================================================
//  BFS — Breadth-First Search
//  Guarantees SHORTEST PATH on unweighted grids.
//  Every edge has weight 1 → BFS = optimal solution.
//  Time: O(V + E)   Space: O(V)
// ============================================================
#include "../grid.h"
#include <queue>

PathResult run_bfs(Grid& grid, int sr, int sc, int er, int ec) {
    PathResult result;
    grid.reset();

    std::queue<std::pair<int,int>> q;
    q.push({sr, sc});
    grid.at(sr, sc).visited = true;
    grid.at(sr, sc).dist    = 0;

    while (!q.empty()) {
        auto [r, c] = q.front(); q.pop();
        result.visited_order.push_back({r, c});

        if (r == er && c == ec) {
            result.found = true;
            // Trace parents back to reconstruct shortest path
            int cr = er, cc = ec;
            while (cr != -1) {
                result.path.push_back({cr, cc});
                result.cost++;
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
            grid.at(nr, nc).visited    = true;
            grid.at(nr, nc).dist       = grid.at(r, c).dist + 1;
            grid.at(nr, nc).parent_row = r;
            grid.at(nr, nc).parent_col = c;
            q.push({nr, nc});
        }
    }
    return result; // no path found
}
