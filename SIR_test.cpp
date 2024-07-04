#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "SIR.hpp"

#include "SIR_automa.hpp"
#include "SIR_output.hpp"
#include "doctest.h"

namespace epidemic_SIR {

TEST_CASE("Testing SIR model (equations) 1: simulate first 5 days") {
  Population initial_population{997, 3, 0};
  Parameter parameter{0.4, 0.04};

  int days = 5;

  Virus covid{initial_population};

  covid = evolve(covid, parameter, days);

  std::vector<Population> data = covid.get_data();

  CHECK(data[0].s == 997.);
  CHECK(data[0].i == 3.);
  CHECK(data[0].r == 0.);

  CHECK(data[1].s == doctest::Approx(995.80).epsilon(0.01));
  CHECK(data[1].i == doctest::Approx(4.08).epsilon(0.01));
  CHECK(data[1].r == doctest::Approx(0.12).epsilon(0.01));

  CHECK(data[2].s == doctest::Approx(994.18).epsilon(0.01));
  CHECK(data[2].i == doctest::Approx(5.54).epsilon(0.01));
  CHECK(data[2].r == doctest::Approx(0.28).epsilon(0.01));

  CHECK(data[3].s == doctest::Approx(991.98).epsilon(0.01));
  CHECK(data[3].i == doctest::Approx(7.52).epsilon(0.01));
  CHECK(data[3].r == doctest::Approx(0.50).epsilon(0.01));

  CHECK(data[4].s == doctest::Approx(989.00).epsilon(0.01));
  CHECK(data[4].i == doctest::Approx(10.20).epsilon(0.01));
  CHECK(data[4].r == doctest::Approx(0.81).epsilon(0.01));

  CHECK(data[5].s == doctest::Approx(984.96).epsilon(0.01));
  CHECK(data[5].i == doctest::Approx(13.83).epsilon(0.01));
  CHECK(data[5].r == doctest::Approx(1.21).epsilon(0.01));

  auto data_round = round_off(data);

  CHECK(data_round[0].s == 997);
  CHECK(data_round[0].i == 3);
  CHECK(data_round[0].r == 0);

  CHECK(data_round[1].s == 996);
  CHECK(data_round[1].i == 4);
  CHECK(data_round[1].r == 0);

  CHECK(data_round[2].s == 994);
  CHECK(data_round[2].i == 6);
  CHECK(data_round[2].r == 0);

  CHECK(data_round[3].s == 992);
  CHECK(data_round[3].i == 7);
  CHECK(data_round[3].r == 1);

  CHECK(data_round[4].s == 989);
  CHECK(data_round[4].i == 10);
  CHECK(data_round[4].r == 1);

  CHECK(data_round[5].s == 985);
  CHECK(data_round[5].i == 14);
  CHECK(data_round[5].r == 1);
}

TEST_CASE("Testing SIR model (equations) 2: changing initial conditions") {
  Population initial_population{499, 1, 0};
  Parameter parameter{0.21, 0.01};

  int days = 5;

  Virus covid{initial_population};

  covid = evolve(covid, parameter, days);

  auto data = covid.get_data();

  CHECK(data[0].s == 499.);
  CHECK(data[0].i == 1.);
  CHECK(data[0].r == 0.);

  CHECK(data[1].s == doctest::Approx(498.79).epsilon(0.01));
  CHECK(data[1].i == doctest::Approx(1.20).epsilon(0.01));
  CHECK(data[1].r == doctest::Approx(0.01).epsilon(0.01));

  CHECK(data[2].s == doctest::Approx(498.54).epsilon(0.01));
  CHECK(data[2].i == doctest::Approx(1.44).epsilon(0.01));
  CHECK(data[2].r == doctest::Approx(0.02).epsilon(0.01));

  CHECK(data[3].s == doctest::Approx(498.24).epsilon(0.01));
  CHECK(data[3].i == doctest::Approx(1.73).epsilon(0.01));
  CHECK(data[3].r == doctest::Approx(0.04).epsilon(0.01));

  CHECK(data[4].s == doctest::Approx(497.88).epsilon(0.01));
  CHECK(data[4].i == doctest::Approx(2.07).epsilon(0.01));
  CHECK(data[4].r == doctest::Approx(0.05).epsilon(0.01));

  CHECK(data[5].s == doctest::Approx(497.44).epsilon(0.01));
  CHECK(data[5].i == doctest::Approx(2.48).epsilon(0.01));
  CHECK(data[5].r == doctest::Approx(0.07).epsilon(0.01));

  auto data_round = round_off(data);

  CHECK(data_round[0].s == 499);
  CHECK(data_round[0].i == 1);
  CHECK(data_round[0].r == 0);

  CHECK(data_round[1].s == 499);
  CHECK(data_round[1].i == 1);
  CHECK(data_round[1].r == 0);

  CHECK(data_round[2].s == 499);
  CHECK(data_round[2].i == 1);
  CHECK(data_round[2].r == 0);

  CHECK(data_round[3].s == 498);
  CHECK(data_round[3].i == 2);
  CHECK(data_round[3].r == 0);

  CHECK(data_round[4].s == 498);
  CHECK(data_round[4].i == 2);
  CHECK(data_round[4].r == 0);

  CHECK(data_round[5].s == 497);
  CHECK(data_round[5].i == 3);
  CHECK(data_round[5].r == 0);
}

TEST_CASE("Testing SIR model (equations) 3: adding more days") {
  Population initial_population{9997, 3, 0};
  Parameter parameter{0.3654, 0.0954};

  int days = 25;

  epidemic_SIR::Virus covid{initial_population};

  covid = evolve(covid, parameter, days);
  covid = evolve(covid, parameter, days);

  auto data = covid.get_data();

  CHECK(data[25].s == doctest::Approx(8606.69).epsilon(0.01));
  CHECK(data[25].i == doctest::Approx(1005.68).epsilon(0.01));
  CHECK(data[25].r == doctest::Approx(387.62).epsilon(0.01));

  CHECK(data[26].s == doctest::Approx(8290.42).epsilon(0.01));
  CHECK(data[26].i == doctest::Approx(1226.01).epsilon(0.01));
  CHECK(data[26].r == doctest::Approx(483.566).epsilon(0.01));

  CHECK(data[50].s == doctest::Approx(486.56).epsilon(0.01));
  CHECK(data[50].i == doctest::Approx(2079.95).epsilon(0.01));
  CHECK(data[50].r == doctest::Approx(7433.49).epsilon(0.01));

  auto data_round = round_off(data);

  CHECK(data_round[25].s == 8607);
  CHECK(data_round[25].i == 1005);
  CHECK(data_round[25].r == 388);

  CHECK(data_round[26].s == 8290);
  CHECK(data_round[26].i == 1226);
  CHECK(data_round[26].r == 484);

  CHECK(data_round[50].s == 487);
  CHECK(data_round[50].i == 2080);
  CHECK(data_round[50].r == 7433);
}

}  // namespace epidemic_SIR

namespace epidemic_SIR_CA {

TEST_CASE("Testing SIR model (automata) 1: neighbour") {
  World world(5);
  auto grid = world.get_grid();

  int const index_s = find_index({0, 0}, 5);
  int const index_i_1 = find_index({0, 4}, 5);
  grid[index_s] = Cell::Susceptible;
  grid[index_i_1] = Cell::Infectious;

  world.add_grid(grid);

  int const c_1 = neighbours(world, {0, 0}, Cell::Infectious);

  CHECK(c_1 == 1);

  int const index_i_2 = find_index({0, 1}, 5);
  grid[index_i_2] = Cell::Infectious;

  world.add_grid(grid);

  int const c_2 = neighbours(world, {0, 0}, Cell::Infectious);

  CHECK(c_2 == 2);
}

TEST_CASE("Testing SIR model (automata) 2: spread and recover") {
  World world(5);
  auto grid = world.get_grid();

  grid[find_index({2, 2}, 5)] = Cell::Susceptible;
  grid[find_index({3, 2}, 5)] = Cell::Infectious;
  world.add_grid(grid);

  world = evolve_grid(world, {1, 0, 0});
  auto grid_1 = world.get_grid();

  CHECK(grid_1[find_index({2, 2}, 5)] == Cell::Infectious);

  world = evolve_grid(world, {0, 1, 0});
  auto grid_2 = world.get_grid();

  CHECK(grid_2[find_index({2, 2}, 5)] == Cell::Recovered);
}

}  // namespace epidemic_SIR_CA