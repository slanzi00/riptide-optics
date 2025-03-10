#ifndef DETECTOR_HPP
#define DETECTOR_HPP

#include <G4VSensitiveDetector.hh>

namespace riptide {

class SensitiveDetector : public G4VSensitiveDetector
{
  int m_detector_id;

 public:
  SensitiveDetector(std::string_view, int);
  bool ProcessHits(G4Step*, G4TouchableHistory*) override;
};

} // namespace riptide

#endif