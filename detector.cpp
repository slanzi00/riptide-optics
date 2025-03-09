#include "detector.hpp"

#include <G4AnalysisManager.hh>
#include <G4SystemOfUnits.hh>

namespace riptide {

G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  auto track = step->GetTrack();
  track->SetTrackStatus(fStopAndKill);

  const auto touchable = step->GetPreStepPoint()->GetTouchable();
  auto physical_volume = touchable->GetVolume();
  auto pos_sensor      = physical_volume->GetTranslation();

  auto analysis_manager = G4AnalysisManager::Instance();
  analysis_manager->FillH2(0, pos_sensor[0], pos_sensor[1]);

  return true;
}

} // namespace riptide
