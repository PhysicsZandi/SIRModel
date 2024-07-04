#include "SIR.hpp"
#include "SIR_automa.hpp"
#include "SIR_automa_output.hpp"
#include "SIR_input.hpp"
#include "SIR_output.hpp"

int main() {
  try {
    std::cout << "Hi, which epidemiological model?" << '\n'
              << "1. SIR model with differential equations" << '\n'
              << "2. SIR model with cellular automata" << '\n';

    auto model_choice = input_choice(1, 2);

    switch (model_choice) {
      case 1: {
        auto initial_population = epidemic_SIR::input_initial_population();
        auto parameter = epidemic_SIR::input_parameters();
        epidemic_SIR::Virus epidemic{initial_population};

        int simulate_more_days_choice{};

        do {
          auto days = input_days();
          epidemic = epidemic_SIR::evolve(epidemic, parameter, days);
          auto data = epidemic.get_data();

          int output_choice{};

          do {
            std::cout << "Which output?" << '\n'
                      << "1. Table with values (standard output)" << '\n'
                      << "2. Table with round-off values (standard output)" << '\n'
                      << "3. Dinamic or static graph" << '\n'
                      << "0. Exit" << '\n';

            output_choice = input_choice(0, 4);

            switch (output_choice) {
              case 0: {
              } break;

              case 1: {
                epidemic_SIR::print(data, parameter);
              } break;

              case 2: {
                epidemic_SIR::print_round_off(data, parameter);
              } break;

              case 3: {
                auto const display_side = 600;

                std::cout << "Which graph?" << '\n'
                          << "1. Graph with S" << '\n'
                          << "2. Graph with I" << '\n'
                          << "3. Graph with R" << '\n'
                          << "4. Graph with S, I" << '\n'
                          << "5. Graph with I, R" << '\n'
                          << "6. Graph with S, R" << '\n'
                          << "7. Graph with S, I, R" << '\n';

                auto graph_choice = input_choice(1, 7);

                std::cout << "Which kind of graph?" << '\n'
                          << "1. Dinamic graph (evolving from day zero)" << '\n'
                          << "2. Static graph (simulate instantly all days)" << '\n';

                auto kind_of_graph_choice = input_choice(1, 2);

                switch (kind_of_graph_choice) {
                  case 1: {
                    std::cout << "Which speed?" << '\n'
                              << "1. 1 framerate" << '\n'
                              << "2. 5 framerate" << '\n'
                              << "3. 10 framerate" << '\n'
                              << "4. 20 framerate" << '\n';

                    auto framerate_choice = input_choice(1, 4);
                    int framerate{};

                    switch (framerate_choice) {
                      case 1: {
                        framerate = 1;
                      } break;

                      case 2: {
                        framerate = 5;
                      } break;

                      case 3: {
                        framerate = 10;
                      } break;

                      case 4: {
                        framerate = 20;
                      } break;
                    }

                    sf::RenderWindow window(sf::VideoMode(display_side, display_side),
                                            "SIR model (differential equations)");

                    window.setFramerateLimit(framerate);

                    epidemic_SIR::Graph_display display{window, display_side};

                    int duration{};
                    bool running = true;

                    std::vector<epidemic_SIR::Population> temporary_data{};

                    while (running) {
                      window.clear(sf::Color::Black);

                      int const data_size = static_cast<int>(data.size());

                      if (duration < data_size) {
                        temporary_data.push_back(data[duration]);
                        ++duration;
                      }

                      display.draw_axes(temporary_data);
                      display.draw_legend(temporary_data);

                      if (graph_choice == 1 || graph_choice == 4 || graph_choice == 6 ||
                          graph_choice == 7) {
                        display.draw_susceptible(temporary_data);
                      }

                      if (graph_choice == 2 || graph_choice == 4 || graph_choice == 5 ||
                          graph_choice == 7) {
                        display.draw_infectious(temporary_data);
                      }

                      if (graph_choice == 3 || graph_choice == 5 || graph_choice == 6 ||
                          graph_choice == 7) {
                        display.draw_recovered(temporary_data);
                      }

                      window.display();

                      sf::Event event;
                      while (window.pollEvent(event)) {
                        if (event.type == sf::Event::Closed) {
                          running = !running;
                        }
                      }
                    }

                    window.close();

                  } break;

                  case 2: {
                    sf::RenderWindow window(sf::VideoMode(display_side, display_side),
                                            "SIR model (differential equations)");

                    epidemic_SIR::Graph_display display{window, display_side};

                    window.clear(sf::Color::Black);

                    display.draw_axes(data);
                    display.draw_legend(data);

                    if (graph_choice == 1 || graph_choice == 4 || graph_choice == 6 ||
                        graph_choice == 7) {
                      display.draw_susceptible(data);
                    }

                    if (graph_choice == 2 || graph_choice == 4 || graph_choice == 5 ||
                        graph_choice == 7) {
                      display.draw_infectious(data);
                    }

                    if (graph_choice == 3 || graph_choice == 5 || graph_choice == 6 ||
                        graph_choice == 7) {
                      display.draw_recovered(data);
                    }

                    window.display();

                    sf::Event event;
                    while (window.waitEvent(event)) {
                      if (event.type == sf::Event::Closed) {
                        window.close();
                      }
                    }

                  } break;

                    std::cout << '\n';
                }
                break;
              }
            }
          } while (output_choice != 0);

          std::cout << "Do you want to simulate more days?" << '\n'
                    << "1. Yes" << '\n'
                    << "0. Exit" << '\n';

          simulate_more_days_choice = input_choice(0, 1);

        } while (simulate_more_days_choice != 0);
      } break;

      case 2: {
        int const display_side = 600;

        std::cout << "Which grid?" << '\n'
                  << "1. 120 x 120" << '\n'
                  << "2. 60 x 60" << '\n'
                  << "3. 30 x 30" << '\n';

        auto grid_choice = input_choice(1, 3);
        int grid_side{};

        switch (grid_choice) {
          case 1: {
            grid_side = 120;
          } break;

          case 2: {
            grid_side = 60;
          } break;

          case 3: {
            grid_side = 30;
          } break;
        }

        std::cout << "Which speed?" << '\n'
                  << "1. 1 framerate" << '\n'
                  << "2. 5 framerate" << '\n'
                  << "3. 10 framerate" << '\n'
                  << "4. 20 framerate" << '\n';

        auto framerate_choice = input_choice(1, 4);
        int framerate{};

        switch (framerate_choice) {
          case 1: {
            framerate = 1;
          } break;

          case 2: {
            framerate = 5;
          } break;

          case 3: {
            framerate = 10;
          } break;

          case 4: {
            framerate = 20;
          } break;
        }

        auto initial_population = epidemic_SIR_CA::input_initial_population(grid_side);
        auto parameter = epidemic_SIR_CA::input_parameters();

        epidemic_SIR_CA::World world(grid_side);

        world = generate(world, initial_population.s, epidemic_SIR_CA::Cell::Susceptible);
        world = generate(world, initial_population.i, epidemic_SIR_CA::Cell::Infectious);
        world = generate(world, initial_population.r, epidemic_SIR_CA::Cell::Recovered);

        world = update_data(world);

        sf::RenderWindow window{sf::VideoMode(display_side * 2, display_side),
                                "SIR model (cellular automata)"};

        window.setFramerateLimit(framerate);

        epidemic_SIR_CA::Automa_display display{window, display_side};

        bool running = true;

        while (running) {
          window.clear(sf::Color::Black);

          world = evolve(world, parameter);
          auto data = world.get_data();

          display.draw_grid(world);
          display.draw_axes(data);
          display.draw_legend(data);
          display.draw_graph(data);

          window.display();

          sf::Event event;
          while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
              running = !running;
            }
          }
        }

        window.close();
      } break;
    }

    std::cout << "Goodbye" << '\n';
  } catch (std::exception const& exception) {
    std::cerr << exception.what() << '\n';
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Caught unknown exception\n";
    return EXIT_FAILURE;
  }
}
