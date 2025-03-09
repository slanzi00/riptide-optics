#include "physics.hpp"

#include <G4DecayPhysics.hh>
#include <G4EmStandardPhysics.hh>
#include <G4OpticalPhysics.hh>

namespace riptide {

PhysicsList::PhysicsList()
{
  RegisterPhysics(new G4EmStandardPhysics{});
  RegisterPhysics(new G4OpticalPhysics{});
  RegisterPhysics(new G4DecayPhysics{});
}

} // namespace riptide
