//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class
//
//
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Geantino.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VSolid.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Polycone.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),fParticleGun(0)
{
  G4int n_particle = 1;
  fParticleGun  = std::make_shared<G4ParticleGun>(n_particle);
  
  fParticleGun->SetParticleEnergy(0*eV);
  fParticleGun->SetParticlePosition(G4ThreeVector(0.*cm,0.*cm,0.*cm));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  DefineMacroCommands();
  fPositionX = 0*cm;
  fPositionY = 0*cm;
  fPositionZ = 0*cm;
  fDepthInTarget = 0*um;
  fTargetSolidType = "";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  fParticleGun.reset();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  if (fParticleGun->GetParticleDefinition() == G4Geantino::Geantino()) {  
      G4int Z = 95, A = 241;
      G4double ionCharge   = 0.*eplus;
      G4double excitEnergy = 0.*keV;
      
      G4ParticleDefinition* ion
        = G4IonTable::GetIonTable()->GetIon(Z,A,excitEnergy);
      fParticleGun->SetParticleDefinition(ion);
      fParticleGun->SetParticleCharge(ionCharge);
  }    
  //create vertex
  // start with standard cylindrical target, later with commands:
  G4PhysicalVolumeStore* pvs = G4PhysicalVolumeStore::GetInstance();
  G4VPhysicalVolume* targetPhys = pvs->GetVolume("TargetPhys");
  G4LogicalVolume* detVolume = G4LogicalVolumeStore::GetInstance()->GetVolume("Detector");
  if (!detVolume) {
      G4ExceptionDescription msg;
      msg << "Cannot find detector volume !";
      G4Exception("PrimaryGeneratorAction::GeneratePrimaries()",
        "MyCode0000", FatalException, msg);
  }
  G4VSolid* detSolid = detVolume->GetSolid();
  G4Tubs* detCylSolid = dynamic_cast<G4Tubs*>(detSolid);
  G4Box* detBoxSolid = dynamic_cast<G4Box*>(detSolid);
  if (!detCylSolid && !detBoxSolid) {
      G4ExceptionDescription msg;
      msg << "Detector solid is neither a G4Tubs nor a G4Box !";
      G4Exception("PrimaryGeneratorAction::GeneratePrimaries()",
        "MyCode0003", FatalException, msg);
  }
  G4double detLength = detCylSolid->GetZHalfLength();

  G4LogicalVolume* TargetVolume = G4LogicalVolumeStore::GetInstance()->GetVolume("Target");
  if (!TargetVolume) {
      G4ExceptionDescription msg;
      msg << "Cannot find target volume !";
      G4Exception("PrimaryGeneratorAction::GeneratePrimaries()",
        "MyCode0001", FatalException, msg);
  }
  G4VSolid* TargetSolid = TargetVolume->GetSolid();
  G4Tubs* TargetSolidCyl = dynamic_cast<G4Tubs*>(TargetSolid);
  G4Box* TargetSolidBox = dynamic_cast<G4Box*>(TargetSolid);
  G4Polycone* TargetSolidPolycone = dynamic_cast<G4Polycone*>(TargetSolid);
  if (TargetSolidCyl) {
    fTargetSolidType = "G4Tubs";
  }
  if (TargetSolidBox) {
    fTargetSolidType = "G4Box";
    
  }
  if (TargetSolidPolycone) {
    fTargetSolidType = "G4Polycone";
    G4cout << "Found target solid: G4Polycone" << G4endl;
  }
  if (!TargetSolidCyl && !TargetSolidBox && !TargetSolidPolycone) {
      G4ExceptionDescription msg;
      msg << "Target solid is neither a G4Tubs, G4Box nor G4Polycone !";
      G4Exception("PrimaryGeneratorAction::GeneratePrimaries()",
        "MyCode0004", FatalException, msg);
  }

  G4double x, y, z;
  if (fPositionMode == "InBulk") {
      if (fTargetSolidType == "G4Tubs") {
        G4ThreeVector target_pos = targetPhys->GetTranslation();
        if (targetPhys->GetMotherLogical()->GetName() == "Detector") {
          // If the target is placed inside the detector, we need to take into account the position of the target in the detector
          G4VPhysicalVolume* detectorPhys = pvs->GetVolume("Detector");
          G4ThreeVector det_pos = detectorPhys->GetTranslation();
          target_pos += det_pos;
        }
        G4double radius = TargetSolidCyl->GetOuterRadius();
        G4double halfLength = TargetSolidCyl->GetZHalfLength();
        G4double r = radius * std::sqrt(G4UniformRand());
        G4double theta = 2.*CLHEP::pi * G4UniformRand();
        x = r * std::cos(theta) + target_pos.x();
        y = r * std::sin(theta) + target_pos.y();
        G4double z_max = halfLength + target_pos.z();
        // G4double dz = 11 * um; // Range of 6 MeV alpha in Gd
        G4double dz = fDepthInTarget; // Range of 6 MeV alpha in Gd, reduced for better visualization of the effect of the step size
        z = z_max - dz * G4UniformRand();


        // G4cout << "Generated Event: x=" << x/cm << " cm, y=" << y/cm << " cm, z=" << z/cm << " cm" << G4endl;
      } else if (fTargetSolidType == "G4Box") {
        G4ThreeVector target_pos = targetPhys->GetTranslation();
        if (targetPhys->GetMotherLogical()->GetName() == "Detector") {
          // If the target is placed inside the detector, we need to take into account the position of the target in the detector
          G4VPhysicalVolume* detectorPhys = pvs->GetVolume("Detector");
          G4ThreeVector det_pos = detectorPhys->GetTranslation();
          target_pos += det_pos;
        }
        G4double halfLengthX = TargetSolidBox->GetXHalfLength();
        G4double halfLengthY = TargetSolidBox->GetYHalfLength();
        G4double halfLengthZ = TargetSolidBox->GetZHalfLength();
        x = (2.*G4UniformRand()-1.) * halfLengthX + target_pos.x();
        y = (2.*G4UniformRand()-1.) * halfLengthY + target_pos.y();
        G4double z_max = halfLengthZ + target_pos.z();
        // G4double dz = 11 * um; // Range of 6 MeV alpha in Gd
        G4double dz = fDepthInTarget; // Range of 6 MeV alpha in Gd, reduced for better visualization of the effect of the step size
        z = z_max - dz * G4UniformRand();
      } else if (fTargetSolidType == "G4Polycone") {
        G4ExceptionDescription msg;
        msg << "InBulk mode is not implemented for G4Polycone targets !";
        G4Exception("PrimaryGeneratorAction::GeneratePrimaries()",
          "MyCode0005", FatalException, msg);
      }
      
  }
  if (fPositionMode == "OnSurface") {
      if (fTargetSolidType == "G4Tubs") {
        G4ThreeVector target_pos = targetPhys->GetTranslation();
        if (targetPhys->GetMotherLogical()->GetName() == "Detector") {
          // If the target is placed inside the detector, we need to take into account the position of the target in the detector
          G4VPhysicalVolume* detectorPhys = pvs->GetVolume("Detector");
          G4ThreeVector det_pos = detectorPhys->GetTranslation();
          target_pos += det_pos;
        }
        G4double radius = TargetSolidCyl->GetOuterRadius();
        G4double halfLength = TargetSolidCyl->GetZHalfLength();
        G4double r = radius * std::sqrt(G4UniformRand());
        G4double theta = 2.*CLHEP::pi * G4UniformRand();
        x = r * std::cos(theta) + target_pos.x();
        y = r * std::sin(theta) + target_pos.y();
        z = halfLength + target_pos.z(); // Start just above the target surface
      } else if (fTargetSolidType == "G4Box") {
        G4ThreeVector target_pos = targetPhys->GetTranslation();
        if (targetPhys->GetMotherLogical()->GetName() == "Detector") {
          // If the target is placed inside the detector, we need to take into account the position of the target in the detector
          G4VPhysicalVolume* detectorPhys = pvs->GetVolume("Detector");
          G4ThreeVector det_pos = detectorPhys->GetTranslation();
          target_pos += det_pos;
        }
        G4double halfLengthX = TargetSolidBox->GetXHalfLength();
        G4double halfLengthY = TargetSolidBox->GetYHalfLength();
        G4double halfLengthZ = TargetSolidBox->GetZHalfLength();
        x = (2.*G4UniformRand()-1.) * halfLengthX + target_pos.x();
        y = (2.*G4UniformRand()-1.) * halfLengthY + target_pos.y();
        z = halfLengthZ + target_pos.z(); // Start just above the target surface
      } else if (fTargetSolidType == "G4Polycone") {
        G4ExceptionDescription msg;
        msg << "OnSurface mode is not implemented for G4Polycone targets !";
        G4Exception("PrimaryGeneratorAction::GeneratePrimaries()",
          "MyCode0006", FatalException, msg);
      }
  }
  if (fPositionMode == "Point") {
      x = fPositionX;
      y = fPositionY;
      z = fPositionZ; // Start just above the target surface
  }

  G4ThreeVector pos = G4ThreeVector(x, y, z);

  fParticleGun->SetParticlePosition(pos);

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

void PrimaryGeneratorAction::DefineMacroCommands() {

  // Define macro commands for setting the position of the particle source
  m_cmdSetPositionMode = make_shared<G4UIcmdWithAString>("/generator/setPositionMode", this);
  m_cmdSetPositionMode->SetGuidance("Set the mode for setting the position of the particle or parent nuclei origin");
  m_cmdSetPositionMode->SetParameterName("mode", false);
  m_cmdSetPositionMode->AvailableForStates(G4State_PreInit, G4State_Idle);
  G4cout << "Defined command: /generator/setPositionMode. Current modes: InBulk, OnSurface, Point" << G4endl;

  m_cmdSetDepthInTarget = make_shared<G4UIcmdWithADoubleAndUnit>("/generator/setDepthInTarget", this);
  m_cmdSetDepthInTarget->SetGuidance("Set the depth of the particle source in the target");
  m_cmdSetDepthInTarget->SetParameterName("depth", false);
  m_cmdSetDepthInTarget->SetDefaultUnit("um");
  m_cmdSetDepthInTarget->AvailableForStates(G4State_PreInit, G4State_Idle);
  G4cout << "Defined command: /generator/setDepthInTarget. Default unit: um" << G4endl;

  m_cmdSetPositionX = make_shared<G4UIcmdWithADoubleAndUnit>("/generator/setPositionX", this);
  m_cmdSetPositionX->SetGuidance("Set the X position of the particle source");
  m_cmdSetPositionX->SetParameterName("posX", false);
  m_cmdSetPositionX->SetDefaultUnit("cm");
  m_cmdSetPositionX->AvailableForStates(G4State_PreInit, G4State_Idle);
  G4cout << "Defined command: /generator/setPositionX. Default unit: cm" << G4endl;

  m_cmdSetPositionY = make_shared<G4UIcmdWithADoubleAndUnit>("/generator/setPositionY", this);
  m_cmdSetPositionY->SetGuidance("Set the Y position of the particle source");
  m_cmdSetPositionY->SetParameterName("posY", false);
  m_cmdSetPositionY->SetDefaultUnit("cm");
  m_cmdSetPositionY->AvailableForStates(G4State_PreInit, G4State_Idle);
  G4cout << "Defined command: /generator/setPositionY. Default unit: cm" << G4endl;

  m_cmdSetPositionZ = make_shared<G4UIcmdWithADoubleAndUnit>("/generator/setPositionZ", this);
  m_cmdSetPositionZ->SetGuidance("Set the Z position of the particle source");
  m_cmdSetPositionZ->SetParameterName("posZ", false);
  m_cmdSetPositionZ->SetDefaultUnit("cm");
  m_cmdSetPositionZ->AvailableForStates(G4State_PreInit, G4State_Idle);
  G4cout << "Defined command: /generator/setPositionZ. Default unit: cm" << G4endl;
}

void PrimaryGeneratorAction::SetNewValue(G4UIcommand* command, G4String newValue) {
  if (command == m_cmdSetPositionMode.get()) {
    fPositionMode = newValue;
    G4cout << "Position mode set to: " << fPositionMode << G4endl;
  } else if (command == m_cmdSetDepthInTarget.get()) {
    fDepthInTarget = m_cmdSetDepthInTarget->GetNewDoubleValue(newValue);
    G4cout << "Depth in target set to: " << fDepthInTarget / um << " um" << G4endl;
  } else if (command == m_cmdSetPositionX.get()) {
    fPositionX = m_cmdSetPositionX->GetNewDoubleValue(newValue);
    G4cout << "Position X set to: " << fPositionX / cm << " cm" << G4endl;
  } else if (command == m_cmdSetPositionY.get()) {
    fPositionY = m_cmdSetPositionY->GetNewDoubleValue(newValue);
    G4cout << "Position Y set to: " << fPositionY / cm << " cm" << G4endl;
  } else if (command == m_cmdSetPositionZ.get()) {
    fPositionZ = m_cmdSetPositionZ->GetNewDoubleValue(newValue);
    G4cout << "Position Z set to: " << fPositionZ / cm << " cm" << G4endl;
  } else {
    G4cout << "Unknown command: " << command->GetCommandPath() << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

