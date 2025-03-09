#ifndef DETECTOR_CONSTRUCTION_HPP
#define DETECTOR_CONSTRUCTION_HPP

#include <G4VUserDetectorConstruction.hh>

class G4VPhysicalVolume;
class G4LogicalVolume;

namespace riptide {

class DetectorConstruction : public G4VUserDetectorConstruction
{
  G4LogicalVolume* m_cmos_sensor_lv{nullptr};

  G4LogicalVolume* create_world(double);
  G4LogicalVolume* create_scintillator(double);
  G4LogicalVolume* create_lens_system_lv();
  void create_and_place_cmos(G4LogicalVolume*, double);

 public:
  virtual G4VPhysicalVolume* Construct() override;
  virtual void ConstructSDandField() override;
};

} // namespace riptide

#endif
