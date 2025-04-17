#include "generator.hpp"

#include <G4Event.hh>
#include <G4ParticleGun.hh>
#include <G4ParticleTable.hh>
#include <G4RandomTools.hh>
#include <G4SystemOfUnits.hh>

namespace riptide {

PrimaryGeneratorAction::PrimaryGeneratorAction(Geometry geom)
    : m_geometry{std::move(geom)}
    , m_particle_gun{new G4ParticleGun{1}}
{}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete m_particle_gun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
  auto particle_table = G4ParticleTable::GetParticleTable();
  auto particle       = particle_table->FindParticle(m_geometry.projectile_type);

  G4ThreeVector position{
      m_geometry.projectile_initial_position[0] * mm,
      m_geometry.projectile_initial_position[1] * mm,
      m_geometry.projectile_initial_position[2] * mm};

  G4ThreeVector momentum_direction{
      m_geometry.projectile_initial_direction[0],
      m_geometry.projectile_initial_direction[1],
      m_geometry.projectile_initial_direction[2]};

  auto const proton_energy = m_geometry.projectile_energy * MeV;

  m_particle_gun->SetParticlePosition(position);
  m_particle_gun->SetParticleMomentumDirection(momentum_direction);
  m_particle_gun->SetParticleEnergy(proton_energy);
  m_particle_gun->SetParticleDefinition(particle);

  m_particle_gun->GeneratePrimaryVertex(event);
}

} // namespace riptide
