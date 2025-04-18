#include "construction.hpp"
#include "detector.hpp"

#include <G4Box.hh>
#include <G4Ellipsoid.hh>
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
#include <G4RunManager.hh>

namespace riptide {

G4LogicalVolume* DetectorConstruction::create_world()
{
  auto void_material = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");

  auto void_properties = new G4MaterialPropertiesTable();
  void_properties->AddProperty("RINDEX", {1. * eV, 10. * eV}, {1.0, 1.0});
  void_material->SetMaterialPropertiesTable(void_properties);

  auto world_solid = new G4Box(
      "world_sv",
      m_geometry.world_size / 2.,
      m_geometry.world_size / 2.,
      m_geometry.world_size / 2.);
  return new G4LogicalVolume(world_solid, void_material, "world_lv");
}

G4LogicalVolume* DetectorConstruction::create_scintillator()
{
  auto pvt_material = G4NistManager::Instance()->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

  auto pvt_properties = new G4MaterialPropertiesTable();
  pvt_properties->AddProperty("RINDEX", {1 * eV, 10 * eV}, {1.58, 1.58});
  pvt_properties->AddProperty("ABSLENGTH", {1 * eV, 10 * eV}, {210 * cm, 210 * cm});
  pvt_properties->AddProperty("SCINTILLATIONCOMPONENT1", {1 * eV, 10 * eV}, {1.34e-4, 1.34e-4});
  pvt_properties->AddConstProperty("SCINTILLATIONYIELD", m_geometry.scintillation_yield / MeV);
  pvt_properties->AddConstProperty("RESOLUTIONSCALE", 1.0);
  pvt_properties->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 1 * ns);
  pvt_properties->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 10 * ns);
  pvt_properties->AddConstProperty("SCINTILLATIONYIELD1", 0.8);

  pvt_material->SetMaterialPropertiesTable(pvt_properties);

  auto solid = new G4Box(
      "scintillator_sv",
      m_geometry.scintillator_side / 2.,
      m_geometry.scintillator_side / 2.,
      m_geometry.scintillator_side / 2.);

  return new G4LogicalVolume(solid, pvt_material, "scintillator_lv");
}

G4LogicalVolume* DetectorConstruction::create_lens_system_lv()
{
  auto pyrex_material           = G4NistManager::Instance()->FindOrBuildMaterial("G4_Pyrex_Glass");
  auto pyrex_optical_properties = new G4MaterialPropertiesTable();
  pyrex_optical_properties->AddProperty("RINDEX", {1 * eV, 10 * eV}, {1.47, 1.47});
  pyrex_material->SetMaterialPropertiesTable(pyrex_optical_properties);

  auto const r75 = 38.6 * mm, h75 = 12.5 * mm; // by Thorlabs
  auto lens75_solid = new G4Ellipsoid("lens75_solid", r75, r75, r75, r75 - h75, r75);

  auto const r60 = 30.9 * mm, h60 = 16.3 * mm; // by Thorlabs
  auto lens60_solid = new G4Ellipsoid("lens60_solid", r60, r60, r60, r60 - h60, r60);

  auto lens60_rotation = new G4RotationMatrix();
  lens60_rotation->rotateX(180 * deg);

  auto lenses_solid = new G4UnionSolid(
      "lenses_solid", lens75_solid, lens60_solid, lens60_rotation, G4ThreeVector(0, 0, 69.5 * mm));

  auto lenses_logical = new G4LogicalVolume(lenses_solid, pyrex_material, "lenses_logical");

  return lenses_logical;
}

void DetectorConstruction::create_and_place_cmos_y(G4LogicalVolume* world_lv)
{
  auto silicon_material   = G4NistManager::Instance()->FindOrBuildMaterial("G4_Si");
  auto silicon_properties = new G4MaterialPropertiesTable();
  silicon_properties->AddProperty("RINDEX", {1 * eV, 10 * eV}, {3.42, 3.42});
  silicon_material->SetMaterialPropertiesTable(silicon_properties);

  auto const sensor_thickness = 0.1 * mm;
  auto const pixel_size_x     = m_geometry.sensor_width / m_geometry.num_pixels_x;
  auto const pixel_size_y     = m_geometry.sensor_height / m_geometry.num_pixels_y;

  auto cmos_sensor_sv =
      new G4Box("cmos_sensor_y_sv", pixel_size_x / 2., sensor_thickness / 2., pixel_size_y / 2.);

  m_cmos_sensor_y_lv = new G4LogicalVolume(cmos_sensor_sv, silicon_material, "cmos_sensor_y_lv");

  for (int i{0}; i != m_geometry.num_pixels_x; ++i) {
    for (int j{0}; j != m_geometry.num_pixels_y; ++j) {
      new G4PVPlacement(
          0,
          G4ThreeVector(
              (-m_geometry.sensor_width / 2. + (i + 0.5) * pixel_size_x),
              (12.5 + 16.3 + m_geometry.scintillator_side / 2. + m_geometry.lens_sensor_dist
               + m_geometry.cube_lens_dist)
                  * mm,
              (-m_geometry.sensor_height / 2. + (j + 0.5) * pixel_size_y)),
          m_cmos_sensor_y_lv,
          "cmos_sensor_y_pv",
          world_lv,
          false,
          i * m_geometry.num_pixels_y + j,
          false);
    }
  }
}

void DetectorConstruction::create_and_place_cmos_z(G4LogicalVolume* world_lv)
{
  auto silicon_material   = G4NistManager::Instance()->FindOrBuildMaterial("G4_Si");
  auto silicon_properties = new G4MaterialPropertiesTable();
  silicon_properties->AddProperty("RINDEX", {1 * eV, 10 * eV}, {3.42, 3.42});
  silicon_material->SetMaterialPropertiesTable(silicon_properties);

  auto const sensor_thickness = 0.1 * mm;
  auto const pixel_size_x     = m_geometry.sensor_width / m_geometry.num_pixels_x;
  auto const pixel_size_y     = m_geometry.sensor_height / m_geometry.num_pixels_y;

  auto cmos_sensor_sv =
      new G4Box("cmos_sensor_z_sv", pixel_size_x / 2., pixel_size_y / 2., sensor_thickness / 2.);

  m_cmos_sensor_z_lv = new G4LogicalVolume(cmos_sensor_sv, silicon_material, "cmos_sensor_z_lv");

  for (int i{0}; i != m_geometry.num_pixels_x; ++i) {
    for (int j{0}; j != m_geometry.num_pixels_y; ++j) {
      new G4PVPlacement(
          0,
          G4ThreeVector(
              (-m_geometry.sensor_width / 2. + (i + 0.5) * pixel_size_x),
              (-m_geometry.sensor_height / 2. + (j + 0.5) * pixel_size_y),
              (12.5 + 16.3 + m_geometry.scintillator_side / 2. + m_geometry.lens_sensor_dist
               + m_geometry.cube_lens_dist)
                  * mm),
          m_cmos_sensor_z_lv,
          "cmos_sensor_z_pv",
          world_lv,
          false,
          i * m_geometry.num_pixels_y + j,
          false);
    }
  }
}

DetectorConstruction::DetectorConstruction(Geometry geom)
    : m_geometry{std::move(geom)}
{}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  auto world_lv = create_world();
  auto world_pv = new G4PVPlacement(0, {}, world_lv, "world_physical", 0, false, 0, true);

  auto pvt_lv = create_scintillator();
  new G4PVPlacement(0, {}, pvt_lv, "scintillator_pv", world_lv, false, 0, true);

  auto lens_system_lv = create_lens_system_lv();

  new G4PVPlacement(
      0,
      {0, 0, (3.9 + m_geometry.cube_lens_dist) * mm},
      lens_system_lv,
      "lens_system_physical",
      world_lv,
      false,
      0,
      true);

  auto lenses_y_rotation = new G4RotationMatrix();
  lenses_y_rotation->rotateX(90 * deg);

  new G4PVPlacement(
      lenses_y_rotation,
      {0, (3.9 + m_geometry.cube_lens_dist) * mm, 0},
      lens_system_lv,
      "lens_system_physical",
      world_lv,
      false,
      0,
      true);

  create_and_place_cmos_y(world_lv);
  create_and_place_cmos_z(world_lv);

  return world_pv;
}

void DetectorConstruction::ConstructSDandField()
{
  auto cmos_sensor_y = new SensitiveDetector(m_cmos_sensor_y_lv->GetName(), 0);
  auto cmos_sensor_z = new SensitiveDetector(m_cmos_sensor_z_lv->GetName(), 1);

  m_cmos_sensor_y_lv->SetSensitiveDetector(cmos_sensor_y);
  m_cmos_sensor_z_lv->SetSensitiveDetector(cmos_sensor_z);
}

} // namespace riptide