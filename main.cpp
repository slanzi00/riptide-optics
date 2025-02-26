#include "action.hpp"
#include "construction.hpp"
#include "generator.hpp"
#include "physics.hpp"

#include <G4RunManager.hh>
#include <G4UIExecutive.hh>
#include <G4UImanager.hh>
#include <G4VisExecutive.hh>

int main(int argc, char** argv)
{
  G4UIExecutive* ui        = new G4UIExecutive(argc, argv);
  G4RunManager* runManager = new G4RunManager();
  runManager->SetUserInitialization(new DetectorConstruction());
  runManager->SetUserInitialization(new PhysicsList());
  runManager->SetUserInitialization(new ActionInitialization());
  runManager->Initialize();

  G4VisManager* visManager = new G4VisExecutive();
  visManager->Initialize();
  G4UImanager::GetUIpointer()->ApplyCommand("/control/execute vis.mac");
  ui->SessionStart();

  delete visManager;
  delete ui;
  delete runManager;
}
