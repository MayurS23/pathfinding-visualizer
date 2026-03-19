// ============================================================
//  DFS — Depth-First Search
//  Does NOT guarantee shortest path. Explores deep paths first.
//  Useful for maze traversal and contrast with BFS.
//  Time: O(V + E)   Space: O(V) — call stack depth
// ============================================================
#include "../grid.h"
#include <stack>

PathResult run_dfs(Grid& grid, int sr, int sc, int er, int ec) {
    PathResult result;
    grid.reset();

    // Iterative DFS — avoids stack overflow on large grids
    std::stack<std::pair<int,int>> stk;
    stk.push({sr, sc});
    grid.at(sr, sc).visited = true;

    while (!stk.empty()) {
        auto [r, c] = stk.top(); stk.pop();
        result.visited_order.push_back({r, c});

        if (r == er && c == ec) {
            result.found = true;
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

        // Push in reverse to maintain natural exploration order
        for (int d = 3; d >= 0; d--) {
            int nr = r + DR4[d], nc = c + DC4[d];
            if (!grid.isWalkable(nr, nc) || grid.at(nr, nc).visited) continue;
            grid.at(nr, nc).visited    = true;
            grid.at(nr, nc).parent_row = r;
            grid.at(nr, nc).parent_col = c;
            stk.push({nr, nc});
        }
    }
    return result;
}
