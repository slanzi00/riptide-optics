#ifndef DETECTOR_CONSTRUCTION_HPP
#define DETECTOR_CONSTRUCTION_HPP

#include <G4VUserDetectorConstruction.hh>

class G4VPhysicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction
{
 public:
  virtual G4VPhysicalVolume* Construct() override;
  virtual void ConstructSDandField() override;
};

#endif
