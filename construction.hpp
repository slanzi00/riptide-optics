#ifndef DETECTOR_CONSTRUCTION_HPP
#define DETECTOR_CONSTRUCTION_HPP

#include "config.hpp"
#include <G4VUserDetectorConstruction.hh>

class G4VPhysicalVolume;
class G4LogicalVolume;

namespace riptide {

class DetectorConstruction : public G4VUserDetectorConstruction
{
  Geometry m_geometry;
  G4LogicalVolume* m_cmos_sensor_y_lv{nullptr};
  G4LogicalVolume* m_cmos_sensor_z_lv{nullptr};

  G4LogicalVolume* create_world();
  G4LogicalVolume* create_scintillator();
  G4LogicalVolume* create_lens_system_lv();
  void create_and_place_cmos_z(G4LogicalVolume*);
  void create_and_place_cmos_y(G4LogicalVolume*);

 public:
  DetectorConstruction(Geometry);
  virtual G4VPhysicalVolume* Construct() override;
  virtual void ConstructSDandField() override;
};

} // namespace riptide

#endif
