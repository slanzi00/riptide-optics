#ifndef USER_ACTION_HPP
#define USER_ACTION_HPP

#include "config.hpp"

#include <G4VUserActionInitialization.hh>

namespace riptide {

class ActionInitialization : public G4VUserActionInitialization
{
  Geometry m_geometry;

 public:
  explicit ActionInitialization(Geometry geom);
  void Build() const override;
};

} // namespace riptide

#endif
