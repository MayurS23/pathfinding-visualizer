// ============================================================
//  Dijkstra's Algorithm
//  Guarantees SHORTEST PATH on weighted grids.
//  Uses a min-heap priority queue for O(log V) extraction.
//  Time: O((V + E) log V)   Space: O(V)
// ============================================================
#include "../grid.h"
#include <queue>
#include <tuple>

PathResult run_dijkstra(Grid& grid, int sr, int sc, int er, int ec) {
    PathResult result;
    grid.reset();

    // Min-heap entries: (distance, row, col)
    using Entry = std::tuple<int, int, int>;
    std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> pq;

    grid.at(sr, sc).dist = 0;
    pq.push({0, sr, sc});

    while (!pq.empty()) {
        auto [dist, r, c] = pq.top(); pq.pop();

        // Skip stale heap entries (lazy deletion pattern)
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
            int new_dist = dist + grid.at(nr, nc).weight;
            if (new_dist < grid.at(nr, nc).dist) {
                grid.at(nr, nc).dist       = new_dist;
                grid.at(nr, nc).parent_row = r;
                grid.at(nr, nc).parent_col = c;
                pq.push({new_dist, nr, nc});
            }
        }
    }
    return result;
}
