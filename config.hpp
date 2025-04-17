#ifndef RIPTIDE_CONFIG_HPP
#define RIPTIDE_CONFIG_HPP

#include <array>
#include <string>

namespace riptide {

struct Geometry
{
  double world_size;
  double scintillator_side;
  double scintillation_yield;
  double cube_lens_dist;
  double lens_sensor_dist;
  double sensor_width;
  double sensor_height;
  int num_pixels_x;
  int num_pixels_y;
  std::string projectile_type;
  double projectile_energy;
  std::array<double, 3> projectile_initial_position;
  std::array<double, 3> projectile_initial_direction;
};

Geometry load_geometry(std::string_view filename);

} // namespace riptide

#endif