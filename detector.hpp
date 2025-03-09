#ifndef DETECTOR_HPP
#define DETECTOR_HPP

#include <G4VSensitiveDetector.hh>

namespace riptide {

class SensitiveDetector : public G4VSensitiveDetector
{
 public:
  using G4VSensitiveDetector::G4VSensitiveDetector;
  bool ProcessHits(G4Step*, G4TouchableHistory*) override;
};

} // namespace riptide

#endif