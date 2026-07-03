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
/// \file PrimaryGeneratorAction.hh
/// \brief Definition of the PrimaryGeneratorAction class
//
//
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

#include "G4UImessenger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

#include <memory>

using namespace std;

class G4Event;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction, public G4UImessenger
{
  public:
    PrimaryGeneratorAction();    
   ~PrimaryGeneratorAction();

  public:
    virtual void GeneratePrimaries(G4Event*);
    void SetNewValue(G4UIcommand* command, G4String newValue) override;
    shared_ptr<G4ParticleGun> GetParticleGun() {return fParticleGun;};
    shared_ptr<G4UIcmdWithAString> m_cmdSetPositionMode;
    shared_ptr<G4UIcmdWithADoubleAndUnit> m_cmdSetPositionX;
    shared_ptr<G4UIcmdWithADoubleAndUnit> m_cmdSetPositionY;
    shared_ptr<G4UIcmdWithADoubleAndUnit> m_cmdSetPositionZ;
    shared_ptr<G4UIcmdWithADoubleAndUnit> m_cmdSetDepthInTarget;
    void DefineMacroCommands();

  private:
    shared_ptr<G4ParticleGun>  fParticleGun;        //pointer a to G4 service class
    G4double fDepthInTarget; // depth of the particle source in the target, used for visualization of the effect of the step size
    G4double fPositionX; // X position of the particle source, used for visualization of the effect of the step size
    G4double fPositionY; // Y position of the particle source, used for visualization of the effect of the step size
    G4double fPositionZ; // Z position of the particle source, used for visualization of the effect of the step size
    G4String fPositionMode; // Mode for setting the position of the particle or parent nuclei origin
    G4String fTargetSolidType; // Type of the target solid (e.g., "G4Tubs" or "G4Box")
};
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


