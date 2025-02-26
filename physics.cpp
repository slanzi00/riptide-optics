#include "physics.hpp"

#include <G4DecayPhysics.hh>
#include <G4EmStandardPhysics.hh>
#include <G4OpticalPhysics.hh>

PhysicsList::PhysicsList()
{
  this->RegisterPhysics(new G4EmStandardPhysics());
  this->RegisterPhysics(new G4OpticalPhysics());
  this->RegisterPhysics(new G4DecayPhysics());
}
