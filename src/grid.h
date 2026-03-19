#pragma once
// ============================================================
//  grid.h  —  Core data structures shared across all algorithms
//  Mirrors the JavaScript GridEngine class exactly.
// ============================================================
#include <vector>
#include <climits>
#include <cstdint>

enum class NodeType : uint8_t {
    EMPTY  = 0,
    WALL   = 1,
    START  = 2,
    END    = 3,
    WEIGHT = 4
};

struct Node {
    int      row        = 0;
    int      col        = 0;
    NodeType type       = NodeType::EMPTY;
    int      weight     = 1;
    bool     visited    = false;
    int      dist       = INT_MAX;
    int      g_cost     = INT_MAX;
    int      h_cost     = 0;
    int      f_cost     = INT_MAX;
    int      parent_row = -1;
    int      parent_col = -1;
};

struct Grid {
    int               rows = 0;
    int               cols = 0;
    std::vector<Node> nodes;

    Grid() = default;
    Grid(int r, int c) : rows(r), cols(c), nodes(r * c) {
        for (int i = 0; i < r; i++)
            for (int j = 0; j < c; j++) {
                nodes[i * c + j].row = i;
                nodes[i * c + j].col = j;
            }
    }

    Node&       at(int r, int c)       { return nodes[r * cols + c]; }
    const Node& at(int r, int c) const { return nodes[r * cols + c]; }

    bool inBounds(int r, int c) const {
        return r >= 0 && r < rows && c >= 0 && c < cols;
    }
    bool isWalkable(int r, int c) const {
        return inBounds(r, c) && at(r, c).type != NodeType::WALL;
    }

    void reset() {
        for (auto& n : nodes) {
            n.visited    = false;
            n.dist       = INT_MAX;
            n.g_cost     = INT_MAX;
            n.h_cost     = 0;
            n.f_cost     = INT_MAX;
            n.parent_row = -1;
            n.parent_col = -1;
        }
    }
};

struct PathResult {
    std::vector<std::pair<int,int>> visited_order;
    std::vector<std::pair<int,int>> path;
    bool  found     = false;
    int   cost      = 0;
};

// 4-directional (default)
static const int DR4[] = { -1,  1,  0,  0 };
static const int DC4[] = {  0,  0, -1,  1 };

// 8-directional (diagonal mode, Phase 8)
static const int DR8[] = { -1, -1, -1,  0,  0,  1,  1,  1 };
static const int DC8[] = { -1,  0,  1, -1,  1, -1,  0,  1 };
