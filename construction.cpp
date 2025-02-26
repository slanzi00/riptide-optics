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

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  auto nist            = G4NistManager::Instance();
  auto pyrex_material  = nist->FindOrBuildMaterial("G4_Pyrex_Glass");
  auto vacuum_material = nist->FindOrBuildMaterial("G4_Galactic");

  std::vector<double> photon_energy_range{1. * eV, 10. * eV};
  std::vector<double> refractive_index_pyrex{1.47, 1.47};

  auto pyrex_optical_properties = new G4MaterialPropertiesTable();
  pyrex_optical_properties->AddProperty("RINDEX", photon_energy_range, refractive_index_pyrex);
  pyrex_material->SetMaterialPropertiesTable(pyrex_optical_properties);

  std::vector<double> refractive_index_vacuum{1.0, 1.0};
  auto vacuum_optical_properties = new G4MaterialPropertiesTable();
  vacuum_optical_properties->AddProperty("RINDEX", photon_energy_range, refractive_index_vacuum);
  vacuum_material->SetMaterialPropertiesTable(vacuum_optical_properties);

  auto const world_size = 100. * cm;
  auto world_solid    = new G4Box("world_solid", world_size / 2., world_size / 2., world_size / 2.);
  auto world_logical  = new G4LogicalVolume(world_solid, vacuum_material, "world_logical");
  auto world_physical = new G4PVPlacement(
      0, G4ThreeVector(0, 0, 0), world_logical, "world_physical", 0, false, 0, true);

  auto const lens_1_radius        = 28.2 * mm;
  auto const lens_1_cutout_offset = 16 * mm;

  auto lens_1_sphere_solid =
      new G4Sphere("lens_1_sphere_solid", 0., lens_1_radius, 0., 2 * M_PI, 0., M_PI);

  auto lens_1_cube_solid =
      new G4Box("lens_1_cube_solid", lens_1_radius, lens_1_radius, lens_1_radius);

  auto lens_1_solid = new G4SubtractionSolid(
      "lens_1_solid",
      lens_1_sphere_solid,
      lens_1_cube_solid,
      0,
      G4ThreeVector(0, 0, lens_1_cutout_offset));

  auto lens_1_logical = new G4LogicalVolume(lens_1_solid, pyrex_material, "lens_1_logical");

  new G4PVPlacement(
      0, G4ThreeVector(0, 0, 0), lens_1_logical, "lens_1_physical", world_logical, false, 0, true);

  auto const lens_2_radius           = 35.25 * mm;
  auto const lens_2_cutout_offset    = 13 * mm;
  auto const lens_2_placement_offset = -64 * mm;

  auto lens_2_sphere_solid =
      new G4Sphere("lens_2_sphere_solid", 0., lens_2_radius, 0., 2 * M_PI, 0., M_PI);

  auto lens_2_cube_solid =
      new G4Box("lens_2_cube_solid", lens_2_radius, lens_2_radius, lens_2_radius);

  auto lens_2_solid = new G4SubtractionSolid(
      "lens_2_solid",
      lens_2_sphere_solid,
      lens_2_cube_solid,
      0,
      G4ThreeVector(0, 0, lens_2_cutout_offset));

  auto lens_2_logical = new G4LogicalVolume(lens_2_solid, pyrex_material, "lens_2_logical");

  auto lens_2_rotation = new G4RotationMatrix();
  lens_2_rotation->rotateX(180 * deg);

  new G4PVPlacement(
      lens_2_rotation,
      G4ThreeVector(0, 0, lens_2_placement_offset),
      lens_2_logical,
      "lens_2_physical",
      world_logical,
      false,
      0,
      true);

  auto power_meter_solid =
      new G4Box("power_meter_solid", 10. * mm / 2., 10. * mm / 2., 1. * mm / 2.);

  auto power_meter_logical =
      new G4LogicalVolume(power_meter_solid, vacuum_material, "power_meter_logical");

  new G4PVPlacement(
      0,
      G4ThreeVector(0, 0, -65. * mm),
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