#include "generator.hpp"

#include <G4Event.hh>
#include <G4ParticleGun.hh>
#include <G4ParticleTable.hh>
#include <G4RandomTools.hh>
#include <G4SystemOfUnits.hh>

namespace riptide {

PrimaryGeneratorAction::PrimaryGeneratorAction()
    : m_particle_gun{new G4ParticleGun{1}}
{}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete m_particle_gun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
  auto particle_table = G4ParticleTable::GetParticleTable();
  auto particle       = particle_table->FindParticle("proton");

  G4ThreeVector position{0.0, 0.0, 0.0};
  G4ThreeVector momentum_direction{1.0, 0.0, 0.0};
  auto const proton_energy = 30.0 * MeV;

  m_particle_gun->SetParticlePosition(position);
  m_particle_gun->SetParticleMomentumDirection(momentum_direction);
  m_particle_gun->SetParticleEnergy(proton_energy);
  m_particle_gun->SetParticleDefinition(particle);

  m_particle_gun->GeneratePrimaryVertex(event);
}

} // namespace riptide
