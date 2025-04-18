#ifndef RUN_HPP
#define RUN_HPP

// #include "TFile.h"
// #include "TH2F.h"
// #include "TTree.h"
#include <G4AnalysisManager.hh>
#include <G4UserRunAction.hh>

namespace riptide {

class RunAction : public G4UserRunAction
{
 public:
  void BeginOfRunAction(G4Run const*) override;
  void EndOfRunAction(G4Run const*) override;
};

} // namespace riptide

#endif