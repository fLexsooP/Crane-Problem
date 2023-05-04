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
                std::cout << "\ncounter = " << counter << "   k = " << k;

                if (bit == STEP_DIRECTION_EAST) {
                    std::cout << "\nmove east";
                    direction = STEP_DIRECTION_EAST;
                } else {
                    std::cout << "\nmove south";
                    direction = STEP_DIRECTION_SOUTH;
                }
                if (current_path.is_step_valid(direction)) {
                    current_path.add_step(direction);
                    std::cout << " ----- valid move";
                } else {
                    is_valid_path = false;
                    std::cout << " ============== not valid move\n";
                    break;
                }
            }
            std::cout << "current crane: " << current_path.total_cranes() << "   best crane: " << best_path.total_cranes() << "\n";
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
    // TODO: implement the dynamic programming algorithm, then delete this
    // comment.
    assert(setting.columns() > 0);
    path p(setting);
    return p;
}

}  // namespace cranes
