#include "run.hpp"
#include "config.hpp"

#include <G4AnalysisManager.hh>
#include <G4Run.hh>

namespace riptide {

void RunAction::BeginOfRunAction(const G4Run*)
{
  auto analysis_manager = G4AnalysisManager::Instance();
  analysis_manager->OpenFile("output.root");
  auto geom   = riptide::load_geometry("config.json");
}

void RunAction::EndOfRunAction(const G4Run*)
{
  auto analysis_manager = G4AnalysisManager::Instance();
  analysis_manager->Write();
  analysis_manager->CloseFile();
}

} // namespace riptide
