#include "action.hpp"
#include "config.hpp"
#include "construction.hpp"
#include "generator.hpp"
#include "physics.hpp"

#include <G4RunManager.hh>
#include <G4UIExecutive.hh>
#include <G4UImanager.hh>
#include <G4VisExecutive.hh>

int main()
{
  G4RunManager run_manager{};

  auto geom = riptide::load_geometry("config.json");
  run_manager.SetUserInitialization(new riptide::DetectorConstruction{geom});
  run_manager.SetUserInitialization(new riptide::PhysicsList{});
  run_manager.SetUserInitialization(new riptide::ActionInitialization{geom});
  run_manager.Initialize();
  run_manager.BeamOn(1);
  G4UImanager::GetUIpointer()->ApplyCommand("/run/verbose 0");
  G4UImanager::GetUIpointer()->ApplyCommand("/event/verbose 0");
  G4UImanager::GetUIpointer()->ApplyCommand("/tracking/verbose 0");

  // G4VisExecutive vis_manager{};
  // vis_manager.Initialize();
  // G4UImanager::GetUIpointer()->ApplyCommand("/control/execute vis.mac");
}
