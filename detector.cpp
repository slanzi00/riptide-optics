#include "detector.hpp"

#include <G4AnalysisManager.hh>
#include <G4SystemOfUnits.hh>

SensitiveDetector::SensitiveDetector(std::string const& name)
    : G4VSensitiveDetector{name}
{}

bool SensitiveDetector::ProcessHits(G4Step*, G4TouchableHistory*)
{
  auto analysis_manager = G4AnalysisManager::Instance();
  analysis_manager->FillH1(0, 1.0, 1.0);
  return true;
}