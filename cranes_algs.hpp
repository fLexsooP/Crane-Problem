///////////////////////////////////////////////////////////////////////////////
// cranes_algs.hpp
//
// Algorithms that solve the crane unloading problem.
//
// All of the TODO sections for this project reside in this file.
//
// This file builds on crane_types.hpp, so you should familiarize yourself
// with that file before working on this file.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <math.h>

#include <cassert>

#include "cranes_types.hpp"

namespace cranes {

// Solve the crane unloading problem for the given grid, using an exhaustive
// optimization algorithm.
//
// This algorithm is expected to run in exponential time, so the grid's
// width+height must be small enough to fit in a 64-bit int; this is enforced
// with an assertion.
//
// The grid must be non-empty.
path crane_unloading_exhaustive(const grid& setting) {
    // grid must be non-empty.
    assert(setting.rows() > 0);
    assert(setting.columns() > 0);

    // Compute maximum path length, and check that it is legal.
    const size_t max_steps = setting.rows() + setting.columns() - 2;
    assert(max_steps < 64);

    // TODO: implement the exhaustive search algorithm, then delete this
    // comment.
    path best_path(setting);
    for (size_t steps = 0; steps <= max_steps; steps++) {
        size_t pow_set_size = pow(2, steps);
        for (size_t counter = 0; counter < pow_set_size; counter++) {
            path current_path(setting);
            bool is_valid_path = true;
            for (size_t k = 0; k < steps; k++) {
                step_direction direction;

                // int bit = counter & (1 << k);
                int bit = (counter >> k) & 1;
                // std::cout << "\ncounter = " << counter << "   k = " << k;

                if (bit == STEP_DIRECTION_EAST) {
                    // std::cout << "\nmove east";
                    direction = STEP_DIRECTION_EAST;
                } else {
                    // std::cout << "\nmove south";
                    direction = STEP_DIRECTION_SOUTH;
                }
                if (current_path.is_step_valid(direction)) {
                    current_path.add_step(direction);
                    // std::cout << " ----- valid move";
                } else {
                    is_valid_path = false;
                    // std::cout << " ============== not valid move\n";
                    break;
                }
            }
            // std::cout << "current crane: " << current_path.total_cranes() << "   best crane: " << best_path.total_cranes() << "\n";
            if (is_valid_path) {
                if (current_path.total_cranes() > best_path.total_cranes()) {
                    best_path = current_path;
                }
            }
        }
    }
    return best_path;
}

// Solve the crane unloading problem for the given grid, using a dynamic
// programming algorithm.
//
// The grid must be non-empty.
// path crane_unloading_dyn_prog(const grid& setting) {
path crane_unloading_dyn_prog(const grid& setting) {
    // grid must be non-empty.
    assert(setting.rows() > 0);
    assert(setting.columns() > 0);
    // TODO: implement the dynamic programming algorithm, then delete this
    // comment.

    // crane_grid: the grid of max crane number for corresponding position
    std::vector<std::vector<int>> crane_grid(setting.rows(), std::vector<int>(setting.columns(), 0));

    std::cout << '\n';

    for (size_t r = 0; r < setting.rows(); r++) {
        for (size_t c = 0; c < setting.columns(); c++) {
            if (r == 0 && c == 0) {
                continue;
            }
            // G[i][j] = None if G[i][j]==X
            if (setting.get(r, c) == CELL_BUILDING) {
                // std::cout << "i'm here\n";
                crane_grid[r][c] = -1;
                continue;
            }

            // from_above = None if i=0 or G[i-1][j]==X; or G[i-1][j] + [↓] otherwise
            int from_above = 0;
            if (r == 0 || setting.get(r - 1, c) == CELL_BUILDING) {
                from_above = -1;
            } else {
                int crane = setting.get(r, c) == CELL_CRANE ? 1 : 0;
                from_above += crane_grid[r - 1][c] + crane;
            }

            // from_left = None if j=0 or G[i][j-1]==X; or G[i][j-1] + [→] otherwise
            int from_left = 0;
            if (c == 0 || setting.get(r, c - 1) == CELL_BUILDING) {
                from_left = -1;
            } else {
                int crane = setting.get(r, c) == CELL_CRANE ? 1 : 0;
                from_left += crane_grid[r][c - 1] + crane;
            }
            if (from_above == -1 && from_left == -1) {
                crane_grid[r][c] = -1;
            } else if (from_above == -1) {
                crane_grid[r][c] = from_left;
            } else if (from_left == -1) {
                crane_grid[r][c] = from_above;
            } else {
                crane_grid[r][c] = from_above > from_left ? from_above : from_left;
            }
        }
    }

    // print input grid
    setting.print();
    std::cout << '\n';

    // print crane grid
    for (auto&& row : crane_grid) {
        for (auto&& column : row) {
            std::cout << column << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";

    // for (size_t i = 0; i < crane_grid.size(); i++)
    // {
    //     for (size_t j = 0; j < crane_grid[0].size(); j++)
    //     {
    //         std::cout << crane_grid[i][j] << " ";
    //     }
    //     std::cout << "\n";
    // }

    // trace back from end point
    int i = crane_grid.size() - 1;
    int j = crane_grid[0].size() - 1;
    int r = 0, c = 0, max = -1;
    if (crane_grid[i][j] == -1) {
        for (size_t k = 0; k < crane_grid.size(); k++) {
            for (size_t l = 0; l < crane_grid[0].size(); l++) {
                if (crane_grid[k][l] > max) {
                    max = crane_grid[k][l];
                    r = k;
                    c = l;
                }
            }
        }
        i = r;
        j = c;
    }
    std::vector<step_direction> step_trace_back;
    while (i >= 0 && j >= 0) {
        if (i == 0 && j == 0) {
            break;
        }
        int left = -1;
        int top = -1;
        if (j > 0) {
            left = crane_grid[i][j - 1];
        }
        if (i > 0) {
            top = crane_grid[i - 1][j];
        }
        if (left > top) {
            step_trace_back.push_back(STEP_DIRECTION_EAST);
            j--;
        } else {
            step_trace_back.push_back(STEP_DIRECTION_SOUTH);
            i--;
        }
    }
    std::reverse(step_trace_back.begin(), step_trace_back.end());
    path best(setting);
    for (auto&& step : step_trace_back) {
        best.add_step(step);
    }
    // for (auto &&step : step_trace_back)
    // {
    //     std::cout << step;
    // }

    std::cout << "\n";
    best.print();

    std::cout << "\n\n";
    std::cout << "=============================================\n";
    std::cout << "\n\n";
    return best;
}

}  // namespace cranes
