#ifndef SIR_AUTOMA_OUTPUT_HPP
#define SIR_AUTOMA_OUTPUT_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>

#include "SIR_automa.hpp"

namespace epidemic_SIR_CA {

class Automa_display {
  sf::RenderWindow& m_window;
  sf::Font m_font;
  std::vector<Population> m_data;
  int m_display_side;

 public:
  Automa_display(sf::RenderWindow& window, int display_side)
      : m_window{window}, m_display_side{display_side} {
    if (!m_font.loadFromFile("Roboto.ttf")) {
      throw std::runtime_error{"cannot load font"};
    }
  }

  void draw_grid(World const& world_to_draw);

  int count_digits(int n);

  void draw_axes(std::vector<Population> const& data);

  void draw_legend(std::vector<Population> const& data_to_print);

  sf::Transform transform();

  void draw_graph(std::vector<Population> const& data);
};

}  // namespace epidemic_SIR_CA

#endif
