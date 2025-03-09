#include "config.hpp"

#include <nlohmann/json.hpp>
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

  Geometry geom;
  geom.world_size        = j["geometry"]["world_size"].get<double>();
  geom.scintillator_side = j["geometry"]["scintillator_side"].get<double>();
  geom.cube_lens_dist    = j["geometry"]["cube_lens_distance"].get<double>();
  geom.lens_sensor_dist  = j["geometry"]["lens_sensor_distance"].get<double>();

  return geom;
}

} // namespace riptide