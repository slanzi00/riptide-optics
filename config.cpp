#include "config.hpp"

#include "json.hpp"

#include <fstream>

namespace riptide {

using json = nlohmann::json;

Geometry load_geometry(std::string_view filename)
{
  std::ifstream file{filename.data()};
  if (!file) {
    throw std::runtime_error("Error: config.json file not open");
  }

  json j;
  file >> j;

  Geometry geom{};
  geom.world_size          = j["geometry"]["world_size"].get<double>();
  geom.scintillator_side   = j["geometry"]["scintillator_side"].get<double>();
  geom.scintillation_yield = j["geometry"]["scintillation_yield"].get<double>();
  geom.cube_lens_dist      = j["geometry"]["cube_lens_distance"].get<double>();
  geom.lens_sensor_dist    = j["geometry"]["lens_sensor_distance"].get<double>();
  geom.sensor_width        = j["geometry"]["sensor_width"].get<double>();
  geom.sensor_height       = j["geometry"]["sensor_height"].get<double>();
  geom.num_pixels_x        = j["geometry"]["num_pixels_x"].get<int>();
  geom.num_pixels_y        = j["geometry"]["num_pixels_y"].get<int>();
  geom.projectile_type     = j["geometry"]["projectile_type"].get<std::string>();
  geom.projectile_energy   = j["geometry"]["projectile_energy"].get<double>();
  geom.projectile_initial_position =
      j["geometry"]["projectile_initial_position"].get<std::array<double, 3>>();
  geom.projectile_initial_direction =
      j["geometry"]["projectile_initial_direction"].get<std::array<double, 3>>();

  return geom;
}

} // namespace riptide