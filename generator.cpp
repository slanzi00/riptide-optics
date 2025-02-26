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

  auto radius = 2.5 * mm;

  for (int i{}, num_photons{1000}; i != num_photons; ++i) {
    auto phi = 2 * M_PI * G4UniformRand();
    auto r   = radius * std::sqrt(G4UniformRand());
    G4ThreeVector position{r * std::cos(phi), r * std::sin(phi), 0.0};
    G4ThreeVector momentum_direction{0.0, 0.0, -1.0};

    m_particle_gun->SetParticlePosition(position);
    m_particle_gun->SetParticleMomentumDirection(momentum_direction);
    m_particle_gun->SetParticleEnergy(2.0 * eV);
    m_particle_gun->SetParticleDefinition(particle);

    m_particle_gun->GeneratePrimaryVertex(event);
  }
}
