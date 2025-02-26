#include "generator.hpp"

#include <G4Event.hh>
#include <G4ParticleGun.hh>
#include <G4ParticleTable.hh>
#include <G4RandomTools.hh>
#include <G4SystemOfUnits.hh>

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
  auto particle       = particle_table->FindParticle("opticalphoton");

  G4double radius  = 2.5 * mm;
  G4int numPhotons = 1000;

  for (G4int i = 0; i < numPhotons; i++) {
    G4double phi = 2 * M_PI * G4UniformRand();
    G4double r   = radius * std::sqrt(G4UniformRand());
    G4ThreeVector position{r * std::cos(phi), r * std::sin(phi), 0.0};
    G4ThreeVector momentum_direction{0.0, 0.0, -1.0};

    m_particle_gun->SetParticlePosition(position);
    m_particle_gun->SetParticleMomentumDirection(momentum_direction);
    m_particle_gun->SetParticleEnergy(2.0 * eV);
    m_particle_gun->SetParticleDefinition(particle);

    m_particle_gun->GeneratePrimaryVertex(event);
  }
}
