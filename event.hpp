#ifndef RIPTIDE_EVENT_ACTION_HPP
#define RIPTIDE_EVENT_ACTION_HPP

#include "config.hpp"
#include <G4UserEventAction.hh>
#include <map>

class G4Event;

namespace riptide {

class EventAction : public G4UserEventAction
{
  Geometry m_geometry;

 public:
  explicit EventAction(Geometry geom);
  void BeginOfEventAction(const G4Event*) override;
};

} // namespace riptide

#endif