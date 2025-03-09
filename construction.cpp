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
#include <G4UnionSolid.hh>
#include <G4VPhysicalVolume.hh>

namespace riptide {

G4LogicalVolume* DetectorConstruction::create_world()
{
  auto void_material = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");

  auto void_properties = new G4MaterialPropertiesTable();
  void_properties->AddProperty("RINDEX", {1. * eV, 10. * eV}, {1.0, 1.0});
  void_material->SetMaterialPropertiesTable(void_properties);

  auto world_solid = new G4Box("world_sv", m_world_size / 2., m_world_size / 2., m_world_size / 2.);
  return new G4LogicalVolume(world_solid, void_material, "world_lv");
}

G4LogicalVolume* DetectorConstruction::create_scintillator()
{
  auto pvt_material = G4NistManager::Instance()->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

  auto pvt_properties = new G4MaterialPropertiesTable();
  pvt_properties->AddProperty("RINDEX", {1 * eV, 10 * eV}, {1.58, 1.58});
  pvt_properties->AddProperty("ABSLENGTH", {1 * eV, 10 * eV}, {210 * cm, 210 * cm});
  pvt_properties->AddProperty("SCINTILLATIONCOMPONENT1", {1 * eV, 10 * eV}, {1.34e-4, 1.34e-4});
  pvt_properties->AddConstProperty("SCINTILLATIONYIELD", 10000. / MeV);
  pvt_properties->AddConstProperty("RESOLUTIONSCALE", 1.0);
  pvt_properties->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 1 * ns);
  pvt_properties->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 10 * ns);
  pvt_properties->AddConstProperty("SCINTILLATIONYIELD1", 0.8);

  pvt_material->SetMaterialPropertiesTable(pvt_properties);

  auto solid = new G4Box(
      "scintillator_sv",
      m_scintillator_side / 2.,
      m_scintillator_side / 2.,
      m_scintillator_side / 2.);
  return new G4LogicalVolume(solid, pvt_material, "scintillator_lv");
}

G4LogicalVolume* DetectorConstruction::create_lens_system_lv()
{
  auto pyrex_material = G4NistManager::Instance()->FindOrBuildMaterial("G4_Pyrex_Glass");

  auto pyrex_optical_properties = new G4MaterialPropertiesTable();
  pyrex_optical_properties->AddProperty("RINDEX", {1 * eV, 10 * eV}, {1.47, 1.47});
  pyrex_material->SetMaterialPropertiesTable(pyrex_optical_properties);

  const G4double lens_1_radius        = 28.2 * mm;
  const G4double lens_1_cutout_offset = 16.3 * mm;

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

  const G4double lens_2_radius           = 35.25 * mm;
  const G4double lens_2_cutout_offset    = 12.5 * mm;
  const G4double lens_2_placement_offset = -64 * mm;

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

  auto lens_2_rotation = new G4RotationMatrix();
  lens_2_rotation->rotateX(180 * deg);

  auto lenses_solid = new G4UnionSolid(
      "lenses_solid",
      lens_1_solid,
      lens_2_solid,
      lens_2_rotation,
      G4ThreeVector(0, 0, lens_2_placement_offset));

  auto lenses_logical = new G4LogicalVolume(lenses_solid, pyrex_material, "lenses_logical");

  return lenses_logical;
}

void DetectorConstruction::create_and_place_cmos(G4LogicalVolume* world_lv)
{
  auto silicon_material   = G4NistManager::Instance()->FindOrBuildMaterial("G4_Si");
  auto silicon_properties = new G4MaterialPropertiesTable();
  silicon_properties->AddProperty("RINDEX", {1 * eV, 10 * eV}, {3.42, 3.42});
  silicon_material->SetMaterialPropertiesTable(silicon_properties);

  const G4double sensor_width     = 19. * mm;
  const G4double sensor_height    = 10. * mm;
  const G4double sensor_thickness = 0.1 * mm;
  const G4int num_pixels_x        = 38 * 2;
  const G4int num_pixels_y        = 20 * 2;
  const G4double pixel_size_x     = sensor_width / num_pixels_x;
  const G4double pixel_size_y     = sensor_height / num_pixels_y;

  auto cmos_sensor_sv =
      new G4Box("cmos_sensor_sv", pixel_size_x / 2., pixel_size_y / 2., sensor_thickness / 2.);

  m_cmos_sensor_lv = new G4LogicalVolume(cmos_sensor_sv, silicon_material, "cmos_sensor_lv");

  for (int i = 0; i < num_pixels_x; i++) {
    for (int j = 0; j < num_pixels_y; j++) {
      new G4PVPlacement(
          0,
          G4ThreeVector(
              (-sensor_width / 2. + (i + 0.5) * pixel_size_x),
              (-sensor_height / 2. + (j + 0.5) * pixel_size_y),
              (79.5 + m_lens_sensor_dist) * mm),
          m_cmos_sensor_lv,
          "cmos_sensor_pv",
          world_lv,
          false,
          i * num_pixels_y + j,
          false);
    }
  }
}

DetectorConstruction::DetectorConstruction(
    double world_size, double scintillator_side, double cube_lens_dist, double lens_sensor_dist)
    : m_world_size(world_size)
    , m_scintillator_side(scintillator_side)
    , m_cube_lens_dist(cube_lens_dist)
    , m_lens_sensor_dist(lens_sensor_dist)
{}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  auto world_lv = create_world();
  auto world_pv = new G4PVPlacement(0, {}, world_lv, "world_physical", 0, false, 0, true);

  auto pvt_lv = create_scintillator();
  new G4PVPlacement(0, {}, pvt_lv, "scintillator_pv", world_lv, false, 0, true);

  // const G4double cube_lens_dist = 18.87 * mm;

  auto lens_system_lv = create_lens_system_lv();

  auto lenses_rotation = new G4RotationMatrix();
  lenses_rotation->rotateX(180 * deg);

  new G4PVPlacement(
      lenses_rotation,
      {0, 0, (18.5 + m_cube_lens_dist) * mm},
      lens_system_lv,
      "lens_system_physical",
      world_lv,
      false,
      0,
      true);

  create_and_place_cmos(world_lv);

  return world_pv;
}

void DetectorConstruction::ConstructSDandField()
{
  auto cmos_sensor = new SensitiveDetector("cmos_sensor");
  m_cmos_sensor_lv->SetSensitiveDetector(cmos_sensor);
}

} // namespace riptide