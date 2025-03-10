#include "detector.hpp"

#include <G4AnalysisManager.hh>
#include <G4SystemOfUnits.hh>

namespace riptide {

SensitiveDetector::SensitiveDetector(std::string_view name, int detector_id)
    : G4VSensitiveDetector{name.data()}
    , m_detector_id{detector_id}
{}

G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  auto track = step->GetTrack();
  track->SetTrackStatus(fStopAndKill);

  const auto touchable = step->GetPreStepPoint()->GetTouchable();
  auto physical_volume = touchable->GetVolume();
  auto pos_sensor      = physical_volume->GetTranslation();

  auto analysis_manager = G4AnalysisManager::Instance();
  analysis_manager->FillH2(m_detector_id, pos_sensor.x(), m_detector_id == 0 ? pos_sensor.z() : pos_sensor.y());

  return true;
}

} // namespace riptide
