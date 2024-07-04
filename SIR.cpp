#include "SIR.hpp"

namespace epidemic_SIR {

std::vector<Population> Virus::get_data() const {
  return m_data;
}

void Virus::update_data(Population const& data_to_add) {
  m_data.push_back(data_to_add);
}

Virus evolve(Virus const& virus_to_evolve, Parameter const& parameter, double duration) {
  Virus virus = virus_to_evolve;
  std::vector<Population> data = virus.get_data();

  assert(!data.empty());

  Population population = data.back();
  int const N = population.s + population.i + population.r;

  for (int i = 0; i != duration; ++i) {
    double const delta_i = parameter.beta * population.s / N * population.i;
    double const delta_r = parameter.gamma * population.i;

    assert(delta_i >= 0 && delta_r >= 0);

    population.s -= delta_i;
    population.i += delta_i - delta_r;
    population.r += delta_r;

    assert(population.s >= 0 && population.i >= 0 && population.r >= 0);
    assert(N == std::round(population.s + population.i + population.r));

    virus.update_data(population);
  }

  return virus;
}

}  // namespace epidemic_SIR
