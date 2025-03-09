#include "action.hpp"
#include "generator.hpp"
#include "run.hpp"

namespace riptide {

void ActionInitialization::Build() const
{
  SetUserAction(new PrimaryGeneratorAction{});
  SetUserAction(new RunAction{});
}

} // namespace riptide
