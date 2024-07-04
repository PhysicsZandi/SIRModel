#ifndef SIR_INPUT_HPP
#define SIR_INPUT_HPP

#include <cassert>
#include <cmath>
#include <iostream>

#include "SIR.hpp"
#include "SIR_automa.hpp"

int input_choice(int inf, int sup);

int input_days();

namespace epidemic_SIR {

Population input_initial_population();

Parameter input_parameters();

}  // namespace epidemic_SIR

namespace epidemic_SIR_CA {

Population input_initial_population(int grid_side);

Parameter input_parameters();

}  // namespace epidemic_SIR_CA

#endif