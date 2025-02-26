#ifndef RUN_HPP
#define RUN_HPP

#include <G4AnalysisManager.hh>
#include <G4UserRunAction.hh>

class RunAction : public G4UserRunAction
{
 public:
  virtual void BeginOfRunAction(G4Run const*) override;
  virtual void EndOfRunAction(G4Run const*) override;
};

#endif