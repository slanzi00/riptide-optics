#ifndef RIPTIDE_CONFIG_HPP
#define RIPTIDE_CONFIG_HPP

#include <string_view>


namespace riptide {

struct Geometry
{
  double world_size;
  double scintillator_side;
  double cube_lens_dist;
  double lens_sensor_dist;
};

Geometry load_geometry(std::string_view filename);

} // namespace riptide

#endif