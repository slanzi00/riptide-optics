#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include "config.hpp"
#include <G4VUserPrimaryGeneratorAction.hh>

class G4ParticleGun;
class G4Event;

namespace riptide {

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  Geometry m_geometry;
  G4ParticleGun* m_particle_gun;

 public:
  explicit PrimaryGeneratorAction(Geometry geom);
  ~PrimaryGeneratorAction();
  void GeneratePrimaries(G4Event*) override;
};

} // namespace riptide

#endif
