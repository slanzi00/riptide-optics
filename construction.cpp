#include "construction.hpp"
#include "detector.hpp"

#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4Material.hh>
#include <G4MaterialPropertiesTable.hh>
#include <G4NistManager.hh>
#include <G4PVPlacement.hh>
#include <G4SDManager.hh>
#include <G4Sphere.hh>
#include <G4SubtractionSolid.hh>
#include <G4SystemOfUnits.hh>
#include <G4ThreeVector.hh>
#include <G4VPhysicalVolume.hh>

namespace {

G4LogicalVolume* create_world(double size)
{
  auto material = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
  std::vector<double> photon_energy_range{1. * eV, 10. * eV};
  material->SetMaterialPropertiesTable(new G4MaterialPropertiesTable());

  auto world_solid = new G4Box("world_solid", size / 2., size / 2., size / 2.);
  return new G4LogicalVolume(world_solid, material, "world_logical");
}

G4LogicalVolume* create_lens(std::string const& name, double radius, double cutout_offset)
{
  auto material = G4NistManager::Instance()->FindOrBuildMaterial("G4_Pyrex_Glass");
  std::vector<double> photon_energy_range{1. * eV, 10. * eV};
  auto properties = new G4MaterialPropertiesTable();
  properties->AddProperty("RINDEX", photon_energy_range, {1.47, 1.47});
  material->SetMaterialPropertiesTable(properties);

  auto sphere = new G4Sphere(name + "_sphere", 0., radius, 0., 2 * M_PI, 0., M_PI);
  auto cube   = new G4Box(name + "_cube", radius, radius, radius);
  auto lens_solid =
      new G4SubtractionSolid(name + "_solid", sphere, cube, 0, G4ThreeVector(0, 0, cutout_offset));
  return new G4LogicalVolume(lens_solid, material, name + "_logical");
}

G4LogicalVolume* create_power_meter()
{
  auto material = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
  std::vector<double> photon_energy_range{1. * eV, 10. * eV};
  material->SetMaterialPropertiesTable(new G4MaterialPropertiesTable());

  auto solid = new G4Box("power_meter_solid", 10. * mm / 2., 10. * mm / 2., 1. * mm / 2.);
  return new G4LogicalVolume(solid, material, "power_meter_logical");
}

} // namespace

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  auto world_logical = create_world(100. * cm);
  auto world_physical =
      new G4PVPlacement(0, {}, world_logical, "world_physical", 0, false, 0, true);

  auto lens_1_logical = create_lens("lens_1", 28.2 * mm, 16 * mm);
  new G4PVPlacement(0, {}, lens_1_logical, "lens_1_physical", world_logical, false, 0, true);

  auto lens_2_logical  = create_lens("lens_2", 35.25 * mm, 13 * mm);
  auto lens_2_rotation = new G4RotationMatrix();
  lens_2_rotation->rotateX(180 * deg);
  new G4PVPlacement(
      lens_2_rotation,
      {0, 0, -64 * mm},
      lens_2_logical,
      "lens_2_physical",
      world_logical,
      false,
      0,
      true);

  auto power_meter_logical = create_power_meter();
  new G4PVPlacement(
      0,
      {0, 0, -65. * mm},
      power_meter_logical,
      "power_meter_physical",
      world_logical,
      false,
      0,
      true);

  return world_physical;
}

void DetectorConstruction::ConstructSDandField()
{
  auto power_meter = new SensitiveDetector("power_meter");
  G4SDManager::GetSDMpointer()->AddNewDetector(power_meter);
  SetSensitiveDetector("power_meter_logical", power_meter);
}