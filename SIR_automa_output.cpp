#include "SIR_automa_output.hpp"

namespace epidemic_SIR_CA {

void Automa_display::draw_grid(World const& world_to_draw) {
  World world = world_to_draw;
  int const side = world.get_side();
  std::vector<Cell> grid = world.get_grid();

  double const cell_side = m_display_side / side;

  int const cells_number = static_cast<int>(grid.size());

  sf::VertexArray cells_grid(sf::Quads, 4 * cells_number);

  int count = 0;

  for (int i = 0; i != side; ++i) {
    for (int j = 0; j != side; ++j) {
      int const x_coord = i * cell_side;
      int const y_coord = j * cell_side;

      cells_grid[count].position = sf::Vector2f(x_coord, y_coord);
      cells_grid[count + 1].position = sf::Vector2f(x_coord + cell_side, y_coord);
      cells_grid[count + 2].position = sf::Vector2f(x_coord + cell_side, y_coord + cell_side);
      cells_grid[count + 3].position = sf::Vector2f(x_coord, y_coord + cell_side);

      int const index = find_index({i, j}, side);

      if (grid[index] == Cell::Susceptible) {
        cells_grid[count].color = sf::Color::Blue;
        cells_grid[count + 1].color = sf::Color::Blue;
        cells_grid[count + 2].color = sf::Color::Blue;
        cells_grid[count + 3].color = sf::Color::Blue;
      }
      if (grid[index] == Cell::Infectious) {
        cells_grid[count].color = sf::Color::Red;
        cells_grid[count + 1].color = sf::Color::Red;
        cells_grid[count + 2].color = sf::Color::Red;
        cells_grid[count + 3].color = sf::Color::Red;
      }
      if (grid[index] == Cell::Recovered) {
        cells_grid[count].color = sf::Color::Green;
        cells_grid[count + 1].color = sf::Color::Green;
        cells_grid[count + 2].color = sf::Color::Green;
        cells_grid[count + 3].color = sf::Color::Green;
      }
      if (grid[index] == Cell::Empty) {
        cells_grid[count].color = sf::Color::Black;
        cells_grid[count + 1].color = sf::Color::Black;
        cells_grid[count + 2].color = sf::Color::Black;
        cells_grid[count + 3].color = sf::Color::Black;
      }

      count = count + 4;
    }
  }

  m_window.draw(cells_grid);
}

int Automa_display::count_digits(int n) {
  int result = 0;

  while (n != 0) {
    n = std::floor(n / 10);
    result++;
  }

  return result;
}

void Automa_display::draw_axes(std::vector<Population> const& data) {
  // linea asse x
  sf::VertexArray x_axis(sf::Lines, 2);

  x_axis[0].position = sf::Vector2f(m_display_side * 0.15 + m_display_side, m_display_side * 0.9);
  x_axis[1].position = sf::Vector2f(m_display_side * 0.85 + m_display_side, m_display_side * 0.9);
  x_axis[0].color = sf::Color::White;
  x_axis[1].color = sf::Color::White;

  m_window.draw(x_axis);

  // linea asse y
  sf::VertexArray y_axis(sf::Lines, 2);

  y_axis[0].position = sf::Vector2f(m_display_side * 0.15 + m_display_side, m_display_side * 0.2);
  y_axis[1].position = sf::Vector2f(m_display_side * 0.15 + m_display_side, m_display_side * 0.9);
  y_axis[0].color = sf::Color::White;
  y_axis[1].color = sf::Color::White;

  m_window.draw(y_axis);

  // tacche asse x
  int const half_distance_notches = m_display_side * 0.7 / 20;

  sf::VertexArray x_axis_notches(sf::Lines, 22);
  for (int i = 0; i != 22; i = i + 2) {
    x_axis_notches[i].position =
        sf::Vector2f(m_display_side * 0.15 + half_distance_notches * i + m_display_side,
                     m_display_side * 0.9 - m_display_side * 0.01);

    if ((i + 1) / 2 % 2 == 0) {
      x_axis_notches[i + 1].position =
          sf::Vector2f(m_display_side * 0.15 + half_distance_notches * i + m_display_side,
                       m_display_side * 0.9 + m_display_side * 0.025);
    } else {
      x_axis_notches[i + 1].position =
          sf::Vector2f(m_display_side * 0.15 + half_distance_notches * i + m_display_side,
                       m_display_side * 0.9 + m_display_side * 0.01);
    }
    x_axis_notches[i].color = sf::Color::White;
    x_axis_notches[i + 1].color = sf::Color::White;
  }

  m_window.draw(x_axis_notches);

  // tacche asse y
  sf::VertexArray y_axis_notches(sf::Lines, 22);

  for (int i = 0; i != 22; i = i + 2) {
    y_axis_notches[i].position =
        sf::Vector2f(m_display_side * 0.15 - m_display_side * 0.01 + m_display_side,
                     m_display_side * 0.2 + i * half_distance_notches);
    y_axis_notches[i + 1].position =
        sf::Vector2f(m_display_side * 0.15 + m_display_side * 0.01 + m_display_side,
                     m_display_side * 0.2 + i * half_distance_notches);
    y_axis_notches[i].color = sf::Color::White;
    y_axis_notches[i + 1].color = sf::Color::White;
  }

  m_window.draw(y_axis_notches);

  // nome asse x
  sf::Text x_axis_name;

  x_axis_name.setFont(m_font);
  x_axis_name.setString("Days");
  x_axis_name.setCharacterSize(m_display_side / 40);
  x_axis_name.setFillColor(sf::Color::White);
  x_axis_name.setPosition(
      sf::Vector2f(m_display_side * 0.9 + m_display_side, m_display_side * 0.9));

  m_window.draw(x_axis_name);

  // nome asse y
  sf::Text y_axis_name;

  y_axis_name.setFont(m_font);
  y_axis_name.setString("Population");
  y_axis_name.setCharacterSize(m_display_side / 40);
  y_axis_name.setFillColor(sf::Color::White);
  y_axis_name.setPosition(
      sf::Vector2f(m_display_side * 0.06 + m_display_side, m_display_side * 0.15));

  m_window.draw(y_axis_name);

  // valori tacche asse x
  int const distance_notches = m_display_side * 0.7 / 10;

  double const days = static_cast<double>(data.size()) - 1;
  double const days_distance_notches = days / 10;

  for (int i = 0; i != 10; ++i) {
    sf::Text x_axis_notch_value;
    x_axis_notch_value.setFont(m_font);

    auto digits_number = count_digits(days_distance_notches * (i + 1));
    std::string days_notch_with_all_digit = std::to_string(days_distance_notches * (i + 1));
    std::string approximated_days_notch{};

    if (days < 1000) {
      for (int j = 0; j < digits_number + 4; ++j) {
        char const digit = days_notch_with_all_digit[j];
        approximated_days_notch += digit;
      }
    } else {
      char digit0 = days_notch_with_all_digit[0];
      approximated_days_notch.push_back(digit0);
      approximated_days_notch += ".";
      char digit1 = days_notch_with_all_digit[1];
      approximated_days_notch.push_back(digit1);
      approximated_days_notch += "*10^" + std::to_string(digits_number - 1);
    }

    x_axis_notch_value.setString(approximated_days_notch);
    x_axis_notch_value.setCharacterSize(m_display_side / 60);
    x_axis_notch_value.setFillColor(sf::Color::White);
    if ((i + 1) % 2 == 0) {
      x_axis_notch_value.setPosition(
          sf::Vector2f(distance_notches * (i + 1) + m_display_side * 0.14 + m_display_side,
                       m_display_side * 0.935));
    } else {
      x_axis_notch_value.setPosition(
          sf::Vector2f(distance_notches * (i + 1) + m_display_side * 0.14 + m_display_side,
                       m_display_side * 0.915));
    }

    m_window.draw(x_axis_notch_value);
  }

  // valori tacche asse y
  int const N = data[0].s + data[0].i + data[0].r;
  double const N_distance_notches = N / 10;

  for (int i = 0; i != 10; ++i) {
    sf::Text y_axis_notch_value;
    y_axis_notch_value.setFont(m_font);

    auto digits_number = count_digits(N_distance_notches * (10 - i));
    std::string population_notch_with_all_digit = std::to_string(N_distance_notches * (10 - i));
    std::string approximated_population_notch{};

    if (N < 1000) {
      for (int j = 0; j < digits_number + 4; ++j) {
        char const digit = population_notch_with_all_digit[j];
        approximated_population_notch += digit;
      }

      y_axis_notch_value.setPosition(sf::Vector2f(m_display_side * 0.08 + m_display_side,
                                                  m_display_side * 0.2 + i * distance_notches));

    } else {
      char digit0 = population_notch_with_all_digit[0];
      approximated_population_notch.push_back(digit0);
      approximated_population_notch += ".";
      char digit1 = population_notch_with_all_digit[1];
      approximated_population_notch.push_back(digit1);
      approximated_population_notch += "*10^" + std::to_string(digits_number - 1);

      y_axis_notch_value.setPosition(sf::Vector2f(m_display_side * 0.06 + m_display_side,
                                                  m_display_side * 0.2 + i * distance_notches));
    }

    y_axis_notch_value.setString(approximated_population_notch);
    y_axis_notch_value.setCharacterSize(m_display_side / 60);
    y_axis_notch_value.setFillColor(sf::Color::White);

    m_window.draw(y_axis_notch_value);
  }
}

void Automa_display::draw_legend(std::vector<Population> const& data) {
  // rettangolo del bordo della legenda
  sf::VertexArray legend(sf::LineStrip, 5);

  legend[0].position = sf::Vector2f(m_display_side * 0.2 + m_display_side, m_display_side * 0.05);
  legend[1].position = sf::Vector2f(m_display_side * 0.8 + m_display_side, m_display_side * 0.05);
  legend[2].position = sf::Vector2f(m_display_side * 0.8 + m_display_side, m_display_side * 0.15);
  legend[3].position = sf::Vector2f(m_display_side * 0.2 + m_display_side, m_display_side * 0.15);
  legend[4].position = sf::Vector2f(m_display_side * 0.2 + m_display_side, m_display_side * 0.05);

  legend[0].color = sf::Color::White;
  legend[1].color = sf::Color::White;
  legend[2].color = sf::Color::White;
  legend[3].color = sf::Color::White;
  legend[4].color = sf::Color::White;

  m_window.draw(legend);

  // colore e numero suscettibili in legenda
  int const size = static_cast<int>(data.size()) - 1;

  sf::Text susceptible;

  susceptible.setFont(m_font);

  std::string s = "S: " + std::to_string(static_cast<int>(data[size].s));

  susceptible.setString(s);
  susceptible.setCharacterSize(m_display_side / 40);
  susceptible.setFillColor(sf::Color::White);
  susceptible.setPosition(
      sf::Vector2f(m_display_side * 0.22 + m_display_side, m_display_side * 0.055));

  m_window.draw(susceptible);

  sf::VertexArray s_color(sf::Lines, 2);

  s_color[0].position =
      sf::Vector2f(m_display_side * 0.33 + m_display_side, m_display_side * 0.066);
  s_color[1].position =
      sf::Vector2f(m_display_side * 0.43 + m_display_side, m_display_side * 0.066);
  s_color[0].color = sf::Color::Blue;
  s_color[1].color = sf::Color::Blue;

  m_window.draw(s_color);

  // colore e numero infetti in legenda
  sf::Text infectious;

  infectious.setFont(m_font);

  std::string i = "I: " + std::to_string(static_cast<int>(data[size].i));

  infectious.setString(i);

  infectious.setCharacterSize(m_display_side / 40);
  infectious.setFillColor(sf::Color::White);
  infectious.setPosition(
      sf::Vector2f(m_display_side * 0.22 + m_display_side, m_display_side * 0.085));

  m_window.draw(infectious);

  sf::VertexArray i_color(sf::Lines, 2);

  i_color[0].position = sf::Vector2f(m_display_side * 0.33 + m_display_side, m_display_side * 0.10);
  i_color[1].position = sf::Vector2f(m_display_side * 0.43 + m_display_side, m_display_side * 0.10);
  i_color[0].color = sf::Color::Red;
  i_color[1].color = sf::Color::Red;

  m_window.draw(i_color);

  // colore e numero rimossi in legenda
  sf::Text recovered;

  recovered.setFont(m_font);

  std::string r = "R: " + std::to_string(static_cast<int>(data[size].r));

  recovered.setString(r);

  recovered.setCharacterSize(m_display_side / 40);
  recovered.setFillColor(sf::Color::White);
  recovered.setPosition(
      sf::Vector2f(m_display_side * 0.22 + m_display_side, m_display_side * 0.115));

  m_window.draw(recovered);

  sf::VertexArray r_color(sf::Lines, 2);

  r_color[0].position =
      sf::Vector2f(m_display_side * 0.33 + m_display_side, m_display_side * 0.134);
  r_color[1].position =
      sf::Vector2f(m_display_side * 0.43 + m_display_side, m_display_side * 0.134);
  r_color[0].color = sf::Color::Green;
  r_color[1].color = sf::Color::Green;

  m_window.draw(r_color);

  // numero giorni in legenda
  sf::Text days;

  days.setFont(m_font);

  std::string d = "Days: " + std::to_string(static_cast<int>(size));

  days.setString(d);

  days.setCharacterSize(m_display_side / 40);
  days.setFillColor(sf::Color::White);
  days.setPosition(sf::Vector2f(m_display_side * 0.5 + m_display_side, m_display_side * 0.055));

  m_window.draw(days);
}

sf::Transform Automa_display::transform() {
  sf::Transformable transformation;
  transformation.setOrigin(0, m_display_side);
  transformation.setScale(1.f, -1.f);
  return transformation.getTransform();
}

void Automa_display::draw_graph(std::vector<Population> const& data) {
  // linea dei suscettibili
  sf::VertexArray points_s(sf::LineStrip, data.size());

  int const data_size = static_cast<int>(data.size());
  int const N = data[0].s + data[0].i + data[0].r;

  double const points_distance = m_display_side * 0.7 / (data_size - 1);

  for (int i = 0; i != data_size; ++i) {
    points_s[i].position =
        sf::Vector2f(i * points_distance + m_display_side * 0.15 + m_display_side,
                     data[i].s / N * m_display_side * 0.7 + m_display_side * 0.1);

    points_s[i].color = sf::Color::Blue;
  }

  m_window.draw(points_s, transform());

  // linea degli infetti
  sf::VertexArray points_i(sf::LineStrip, data.size());

  for (int i = 0; i != data_size; ++i) {
    points_i[i].position =
        sf::Vector2f(i * points_distance + m_display_side * 0.15 + m_display_side,
                     data[i].i / N * m_display_side * 0.7 + m_display_side * 0.1);

    points_i[i].color = sf::Color::Red;
  }

  m_window.draw(points_i, transform());

  // linea dei rimossi
  sf::VertexArray points_r(sf::LineStrip, data.size());

  for (int i = 0; i != data_size; ++i) {
    points_r[i].position =
        sf::Vector2f(i * points_distance + m_display_side * 0.15 + m_display_side,
                     data[i].r / N * m_display_side * 0.7 + m_display_side * 0.1);

    points_r[i].color = sf::Color::Green;
  }

  m_window.draw(points_r, transform());
}

}  // namespace epidemic_SIR_CA
