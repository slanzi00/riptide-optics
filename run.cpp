#include "run.hpp"
#include "config.hpp"

#include <G4AnalysisManager.hh>

namespace riptide {

void RunAction::BeginOfRunAction(const G4Run*)
{
  auto analysis_manager = G4AnalysisManager::Instance();
  analysis_manager->OpenFile("output.root");
  auto geom = riptide::load_geometry("config.json");

  analysis_manager->CreateH2(
      "photon_hits_y",
      "Photon Hits per Pixel (Y)",
      geom.num_pixels_x,
      -geom.sensor_width / 2.,
      geom.sensor_width / 2.,
      geom.num_pixels_y,
      -geom.sensor_height / 2.,
      geom.sensor_height / 2.);

  analysis_manager->CreateH2(
      "photon_hits_z",
      "Photon Hits per Pixel (Z)",
      geom.num_pixels_x,
      -geom.sensor_width / 2.,
      geom.sensor_width / 2.,
      geom.num_pixels_y,
      -geom.sensor_height / 2.,
      geom.sensor_height / 2.);
}

void RunAction::EndOfRunAction(const G4Run*)
{
  auto analysis_manager = G4AnalysisManager::Instance();
  analysis_manager->Write();
  analysis_manager->CloseFile();
}

} // namespace riptide
