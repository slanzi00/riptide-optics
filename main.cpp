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
  G4UIExecutive ui{argc, argv};
  G4RunManager run_manager{};
  run_manager.SetUserInitialization(new riptide::DetectorConstruction{});
  run_manager.SetUserInitialization(new riptide::PhysicsList{});
  run_manager.SetUserInitialization(new riptide::ActionInitialization{});
  run_manager.Initialize();

  G4VisExecutive vis_manager{};
  vis_manager.Initialize();
  G4UImanager::GetUIpointer()->ApplyCommand("/control/execute vis.mac");
  ui.SessionStart();
}
