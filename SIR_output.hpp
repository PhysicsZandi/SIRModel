#ifndef SIR_OUTPUT_HPP
#define SIR_OUTPUT_HPP

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>

#include "SIR.hpp"

namespace epidemic_SIR {

void print(std::vector<Population> const& data_to_print, Parameter const& parameter);

std::vector<Population> round_off(std::vector<Population> const& data_to_round_off);

void print_round_off(std::vector<Population> const& data_to_print, Parameter const& parameter);

class Graph_display {
  sf::RenderWindow& m_window;
  sf::Font m_font;
  int m_display_side;

 public:
  Graph_display(sf::RenderWindow& window, int display_side)
      : m_window{window}, m_display_side{display_side} {
    if (m_font.loadFromFile("Roboto.ttf") == false) {
      throw std::runtime_error{"cannot load font"};
    }
  }

  int count_digits(int n);

  void draw_axes(std::vector<Population> const& data);

  void draw_legend(std::vector<Population> const& data_to_print);

  sf::Transform transform();

  void draw_susceptible(std::vector<Population> const& data);

  void draw_infectious(std::vector<Population> const& data);

  void draw_recovered(std::vector<Population> const& data);
};

}  // namespace epidemic_SIR

#endif
