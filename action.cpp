#include "action.hpp"
#include "generator.hpp"
#include "run.hpp"

namespace riptide {

ActionInitialization::ActionInitialization(Geometry geom)
    : m_geometry{std::move(geom)}
{}

void ActionInitialization::Build() const
{
  SetUserAction(new PrimaryGeneratorAction{m_geometry});
  SetUserAction(new RunAction{});
}

} // namespace riptide
