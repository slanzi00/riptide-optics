#include "run.hpp"

#include <G4AnalysisManager.hh>

namespace riptide {

void RunAction::BeginOfRunAction(const G4Run*)
{
  auto analysis_manager = G4AnalysisManager::Instance();
  analysis_manager->OpenFile("output.root");
  analysis_manager->CreateH2(
      "PhotonHits", "Photon Hits per Pixel", 38 * 2, -9.5, 9.5, 20 * 2, -5., 5.);
}

void RunAction::EndOfRunAction(const G4Run*)
{
  auto analysis_manager = G4AnalysisManager::Instance();
  analysis_manager->Write();
  analysis_manager->CloseFile();
}

} // namespace riptide
