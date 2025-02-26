#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <G4VUserPrimaryGeneratorAction.hh>

class G4ParticleGun;
class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  G4ParticleGun* m_particle_gun;
 public:
  PrimaryGeneratorAction();
  virtual ~PrimaryGeneratorAction();
  virtual void GeneratePrimaries(G4Event*) override;
};

#endif
