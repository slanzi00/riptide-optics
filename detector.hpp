#ifndef DETECTOR_HPP
#define DETECTOR_HPP

#include <G4VSensitiveDetector.hh>

class SensitiveDetector : public G4VSensitiveDetector
{
 public:
  SensitiveDetector(std::string const& name);
  virtual bool ProcessHits(G4Step*, G4TouchableHistory*) override;
};

#endif