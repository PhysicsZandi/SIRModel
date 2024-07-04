#include "SIR_automa.hpp"

namespace epidemic_SIR_CA {

int World::get_side() const {
  return m_side;
}

std::vector<Cell> World::get_grid() const {
  return m_grid;
}

std::vector<Population> World::get_data() const {
  return m_data;
}

void World::add_grid(std::vector<Cell> const& grid_to_add) {
  m_grid = grid_to_add;
}

void World::add_data(Population const& data_to_add) {
  m_data.push_back(data_to_add);
}

int find_index(Point const& point, int side) {
  // trova l'indice nel vettore grid, date le coordinate row e column
  int const i = (point.row + side) % side;
  int const j = (point.column + side) % side;
  assert(i >= 0 && i < side);
  assert(j >= 0 && j < side);
  int const index = i * side + j;
  assert(index >= 0 && index < side * side);
  return index;
}

int random_int_generator(int inf, int sup) {
  // genera numeri casuali interi da inf a sup
  std::default_random_engine engine{std::random_device{}()};
  std::uniform_int_distribution<int> uniform_distribution{inf, sup};
  return uniform_distribution(engine);
}

bool probability(double probability) {
  // in base a probability, ritorna una variabile booleana true o false
  std::default_random_engine engine{std::random_device{}()};
  std::uniform_real_distribution<double> real_distribution{0, 1};
  double random_number = real_distribution(engine);
  return (random_number <= probability);
}

World update_data(World const& world_to_update_data) {
  // aggiorna il vettore data
  World world = world_to_update_data;
  std::vector<Cell> grid = world.get_grid();
  std::vector<Population> data = world.get_data();
  int const side = world.get_side();

  // conta quanti suscettibili, infetti o rimossi ci sono sulla griglia
  double susceptible_count = 0;
  double infectious_count = 0;
  double recovered_count = 0;

  for (int row = 0; row != side; ++row) {
    for (int column = 0; column != side; ++column) {
      Point const point{row, column};
      int const index = find_index(point, side);

      if (grid[index] == Cell::Susceptible) {
        susceptible_count++;
      }
      if (grid[index] == Cell::Infectious) {
        infectious_count++;
      }
      if (grid[index] == Cell::Recovered) {
        recovered_count++;
      }
    }
  }

  // aggiorna il vettore, inserendo alla fine i contatori
  world.add_data({susceptible_count, infectious_count, recovered_count});
  return world;
}

World generate(World const& world_to_generate, int number_of_people, Cell const& cell_type) {
  // genera in posti casuali le persone del tipo cell_type
  World world = world_to_generate;
  std::vector<Cell> grid = world.get_grid();
  int const side = world.get_side();

  for (int i = 0; i != number_of_people; i++) {
    // genera le coordinate
    Point point{};
    point.row = random_int_generator(0, side - 1);
    point.column = random_int_generator(0, side - 1);

    int index = find_index(point, side);

    // sostituisce solamente se vuota, altrimenti ritenta
    if (grid[index] == Cell::Empty) {
      grid[index] = cell_type;
    } else {
      --i;
    }
  }

  // aggiorna la griglia
  world.add_grid(grid);
  return world;
}

Point find_direction(Point const& point_to_move) {
  // trova la direzione di movimento, generata casualmente
  int random_direction_x = random_int_generator(-1, 1);
  int random_direction_y = random_int_generator(-1, 1);
  Point point = point_to_move;

  point.row = point.row + random_direction_x;
  point.column = point.column + random_direction_y;

  return point;
}

World move(World const& world_to_move, double travel_probability) {
  World world = world_to_move;
  std::vector<Cell> grid = world.get_grid();
  int const side = world.get_side();

  for (int row = 0; row != side; ++row) {
    for (int column = 0; column != side; ++column) {
      Point const point{row, column};
      int const index = find_index(point, side);

      if (grid[index] != Cell::Empty && (probability(travel_probability) == true)) {
        Point const new_point = find_direction(point);
        int const new_index = find_index(new_point, side);

        if (grid[new_index] == Cell::Empty) {
          grid[new_index] = grid[index];
          grid[index] = Cell::Empty;
        }
      }
    }
  }

  // aggiorna la griglia
  world.add_grid(grid);
  return world;
}

int neighbours(World const& world_to_count, Point const& point, Cell const& cell_type) {
  // conta quanti vicini sono del tipo cell_type
  World world = world_to_count;
  std::vector<Cell> grid = world.get_grid();
  int const side = world.get_side();
  int count = 0;
  int const index = find_index(point, side);

  // evita di contare la persona stessa
  if (grid[index] == cell_type) {
    --count;
  }

  for (int i = -1; i != 2; i++) {
    for (int j = -1; j != 2; j++) {
      int const new_index = find_index({point.row + i, point.column + j}, side);
      if (grid[new_index] == cell_type) {
        ++count;
      }
    }
  }

  return count;
}

World evolve_grid(World const& world_to_evolve, Parameter const& parameter) {
  World world = world_to_evolve;
  std::vector<Cell> grid = world.get_grid();
  int const side = world.get_side();

  std::vector<Cell> next_grid = grid;

  for (int row = 0; row != side; row++) {
    for (int column = 0; column != side; column++) {
      Point const point{row, column};
      int const index = find_index(point, side);

      // conta quanti infetti vicini e trasforma s in i, in base alla probabilità
      if (grid[index] == Cell::Susceptible) {
        int const c = neighbours(world, point, Cell::Infectious);
        if ((probability(parameter.beta * c) == true) && c > 0) {
          next_grid[index] = Cell::Infectious;
        }
      }

      // trasforma i in r, in base alla probabilità
      if (grid[index] == Cell::Infectious && (probability(parameter.gamma) == true)) {
        next_grid[index] = Cell::Recovered;
      }
    }
  }

  // aggiorna la griglia
  world.add_grid(next_grid);
  return world;
}

World evolve(World const& world_to_evolve, Parameter const& parameter_to_evolve) {
  // evolve la griglia, muovendo le persone e facendo progredire l'epidemia
  World world = world_to_evolve;

  // muove le persone
  world = move(world, parameter_to_evolve.alpha);
  // progredisce l'epidemia
  world = evolve_grid(world, parameter_to_evolve);

  // aggiorna il vettore data
  world = update_data(world);
  return world;
}

}  // namespace epidemic_SIR_CA
