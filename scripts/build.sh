#!/usr/bin/env bash
# ============================================================
#  build.sh — Compile C++ → WebAssembly using Emscripten
#  Usage:
#    ./scripts/build.sh           (release build)
#    ./scripts/build.sh debug     (debug build with assertions)
# ============================================================
set -euo pipefail

MODE="${1:-release}"
ROOT="$(cd "$(dirname "$0")/.." && pwd)"

echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "  PathOS Build System"
echo "  Mode: $MODE"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

# ── Locate Emscripten ──────────────────────────────────────
EMSDK_PATHS=(
  "$HOME/emsdk/emsdk_env.sh"
  "/opt/emsdk/emsdk_env.sh"
  "/usr/local/emsdk/emsdk_env.sh"
)
for p in "${EMSDK_PATHS[@]}"; do
  [ -f "$p" ] && source "$p" > /dev/null && break
done

if ! command -v emcc &>/dev/null; then
  echo "ERROR: emcc not found."
  echo "Install Emscripten: https://emscripten.org/docs/getting_started/downloads.html"
  exit 1
fi
echo "✓ emcc $(emcc --version | head -1 | awk '{print $3}')"

# ── Source files ───────────────────────────────────────────
SOURCES=(
  "$ROOT/src/main.cpp"
  "$ROOT/src/algorithms/bfs.cpp"
  "$ROOT/src/algorithms/dfs.cpp"
  "$ROOT/src/algorithms/dijkstra.cpp"
  "$ROOT/src/algorithms/astar.cpp"
)

# ── Exported C functions ───────────────────────────────────
EXPORTS='[
  "_malloc","_free","_main",
  "_init_grid","_run_algorithm",
  "_get_visited_buffer","_get_visited_count",
  "_get_path_buffer","_get_path_length","_get_path_cost"
]'

# ── Compiler flags ─────────────────────────────────────────
COMMON_FLAGS=(
  -std=c++17
  -s WASM=1
  -s MODULARIZE=1
  -s EXPORT_NAME=PathfinderModule
  -s EXPORTED_FUNCTIONS="$EXPORTS"
  -s EXPORTED_RUNTIME_METHODS='["cwrap","ccall","getValue","setValue","HEAP32"]'
  -s ALLOW_MEMORY_GROWTH=1
  -s INITIAL_MEMORY=33554432
  -s NO_EXIT_RUNTIME=1
  -s ENVIRONMENT=web
  -s FILESYSTEM=0
)

if [ "$MODE" = "debug" ]; then
  OPT=(-O0 -g -s ASSERTIONS=2 -s SAFE_HEAP=1 -s STACK_OVERFLOW_CHECK=1)
  echo "  Flags: debug (-O0 + assertions)"
else
  OPT=(-O3 -flto)
  echo "  Flags: release (-O3 + LTO)"
fi

# ── Compile ────────────────────────────────────────────────
echo ""
echo "Compiling..."
emcc "${SOURCES[@]}" \
  "${COMMON_FLAGS[@]}" \
  "${OPT[@]}" \
  -o "$ROOT/web/pathfinder.js"

# ── Post-build stats ───────────────────────────────────────
JS_SIZE=$(du -sh "$ROOT/web/pathfinder.js" 2>/dev/null | cut -f1)
WASM_SIZE=$(du -sh "$ROOT/web/pathfinder.wasm" 2>/dev/null | cut -f1)

echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "  ✅  Build successful!"
echo "      pathfinder.js   →  $JS_SIZE"
echo "      pathfinder.wasm →  $WASM_SIZE"
echo ""
echo "  Run locally:"
echo "      cd web && python3 -m http.server 8080"
echo "      → http://localhost:8080"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
