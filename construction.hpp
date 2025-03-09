#ifndef DETECTOR_CONSTRUCTION_HPP
#define DETECTOR_CONSTRUCTION_HPP

#include <G4VUserDetectorConstruction.hh>

class G4VPhysicalVolume;
class G4LogicalVolume;

namespace riptide {

class DetectorConstruction : public G4VUserDetectorConstruction
{
  double m_world_size;
  double m_scintillator_side;
  double m_cube_lens_dist;
  double m_lens_sensor_dist;
  G4LogicalVolume* m_cmos_sensor_lv{nullptr};

  G4LogicalVolume* create_world();
  G4LogicalVolume* create_scintillator();
  G4LogicalVolume* create_lens_system_lv();
  void create_and_place_cmos(G4LogicalVolume*);

 public:
  DetectorConstruction(double, double, double, double);
  virtual G4VPhysicalVolume* Construct() override;
  virtual void ConstructSDandField() override;
};

} // namespace riptide

#endif
