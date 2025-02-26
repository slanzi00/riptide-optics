#include "run.hpp"

#include <G4AnalysisManager.hh>

void RunAction::BeginOfRunAction(G4Run const*)
{
  auto analysis_manager = G4AnalysisManager::Instance();
  analysis_manager->OpenFile("output.root");
  analysis_manager->CreateH1("h1", "Photon number", 1, 0.0, 1.0);
}

void RunAction::EndOfRunAction(G4Run const*)
{
  auto analysis_manager = G4AnalysisManager::Instance();
  analysis_manager->Write();
  analysis_manager->CloseFile();
}