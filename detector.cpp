#include "detector.hpp"
#include "event.hpp"

#include <G4AnalysisManager.hh>
#include <G4RunManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4TouchableHistory.hh>

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
  // G4int eventID         = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
  auto analysis_manager = G4AnalysisManager::Instance();
  auto event_id         = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

  analysis_manager->FillH2(
      (m_detector_id == 0 ? event_id * 2 : event_id * 2 + 1),
      pos_sensor.x(),
      m_detector_id == 0 ? pos_sensor.z() : pos_sensor.y());

  return true;
}

} // namespace riptide
