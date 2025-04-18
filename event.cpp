#include "event.hpp"

#include <G4AnalysisManager.hh>
#include <G4Event.hh>

namespace riptide {

EventAction::EventAction(Geometry geom)
    : m_geometry{std::move(geom)}
{}

void EventAction::BeginOfEventAction(const G4Event* event)
{
  auto event_id         = event->GetEventID();
  auto analysis_manager = G4AnalysisManager::Instance();

  analysis_manager->CreateH2(
      "photon_hits_y_" + std::to_string(event_id),
      "Photon Hits per Pixel (Y)",
      m_geometry.num_pixels_x,
      -m_geometry.sensor_width / 2.,
      m_geometry.sensor_width / 2.,
      m_geometry.num_pixels_y,
      -m_geometry.sensor_height / 2.,
      m_geometry.sensor_height / 2.);

  analysis_manager->CreateH2(
      "photon_hits_z_" + std::to_string(event_id),
      "Photon Hits per Pixel (Z)",
      m_geometry.num_pixels_x,
      -m_geometry.sensor_width / 2.,
      m_geometry.sensor_width / 2.,
      m_geometry.num_pixels_y,
      -m_geometry.sensor_height / 2.,
      m_geometry.sensor_height / 2.);
}

} // namespace riptide