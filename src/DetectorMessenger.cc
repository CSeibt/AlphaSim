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
/// \file hadronic/Hadr03/src/DetectorMessenger.cc
/// \brief Implementation of the DetectorMessenger class
//
// $Id: DetectorMessenger.cc 70755 2013-06-05 12:17:48Z ihrivnac $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction* Det)
:G4UImessenger(),
 fDetector(Det), fRdecayDir(0), fDetDir(0),
 fTargMatCmd(0), fDetectMatCmd(0), fTargRadiusCmd(0),
 fDetectThicknessCmd(0), fTargLengthCmd(0), fDetectLengthCmd(0), fSimWaferCmd(0)
{
  fRdecayDir = new G4UIdirectory("/alpha/");
  fRdecayDir->SetGuidance("commands specific to this example");

  G4bool broadcast = false;
  fDetDir = new G4UIdirectory("/alpha/det/",broadcast);
  fDetDir->SetGuidance("detector construction commands");

  fTargMatCmd = new G4UIcmdWithAString("/alpha/det/setTargetMate",this);
  fTargMatCmd->SetGuidance("Select material of the target");
  fTargMatCmd->SetParameterName("choice",false);
  fTargMatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fTargRadiusCmd = new G4UIcmdWithADoubleAndUnit("/alpha/det/setTargetRadius", this);
  fTargRadiusCmd->SetGuidance("Set the Target Radius.");
  fTargRadiusCmd->SetUnitCategory("Length");
  fTargRadiusCmd->SetParameterName("choice",false);
  fTargRadiusCmd->AvailableForStates(G4State_PreInit,G4State_Idle);


  fTargLengthCmd = new G4UIcmdWithADoubleAndUnit("/alpha/det/setTargetLength", this);
  fTargLengthCmd->SetGuidance("Set the Target Length.");
  fTargLengthCmd->SetUnitCategory("Length");
  fTargLengthCmd->SetParameterName("choice",false);
  fTargLengthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);


  fDetectMatCmd = new G4UIcmdWithAString("/alpha/det/setDetectorMate",this);
  fDetectMatCmd->SetGuidance("Select Material of the Detector.");
  fDetectMatCmd->SetParameterName("choice",false);
  fDetectMatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fDetectThicknessCmd = new G4UIcmdWithADoubleAndUnit("/alpha/det/setDetectorThickness",this);
  fDetectThicknessCmd->SetGuidance("Set the Detector Thickness.");
  fDetectThicknessCmd->SetUnitCategory("Length");
  fDetectThicknessCmd->SetParameterName("choice",false);
  fDetectThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fDetectLengthCmd = new G4UIcmdWithADoubleAndUnit("/alpha/det/setDetectorLength",this);
  fDetectLengthCmd->SetGuidance("Set the Detector Length.");
  fDetectLengthCmd->SetUnitCategory("Length");
  fDetectLengthCmd->SetParameterName("choice",false);
  fDetectLengthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fStepMaxDetCmd = new G4UIcmdWithADoubleAndUnit("/alpha/det/stepMaxDetector",this);
  fStepMaxDetCmd->SetGuidance("Define a step max in detector.");
  fStepMaxDetCmd->SetParameterName("stepMaxDet",false);
  fStepMaxDetCmd->SetUnitCategory("Length");
  fStepMaxDetCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fStepMaxTarCmd = new G4UIcmdWithADoubleAndUnit("/alpha/det/stepMaxTarget",this);
  fStepMaxTarCmd->SetGuidance("Define a step max in Target.");
  fStepMaxTarCmd->SetParameterName("stepMaxTar",false);
  fStepMaxTarCmd->SetUnitCategory("Length");
  fStepMaxTarCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fTargetXCmd = new G4UIcmdWithADoubleAndUnit("/alpha/det/setTargetX",this);
  fTargetXCmd->SetGuidance("Set Target X");
  fTargetXCmd->SetParameterName("choice",false);
  fTargetXCmd->SetUnitCategory("Length");
  fTargetXCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // fCoverThicknessCmd = new G4UIcmdWithADoubleAndUnit("/alpha/det/setCoverThickness",this);
  // fCoverThicknessCmd->SetGuidance("Set Cover Thickness");
  // fCoverThicknessCmd->SetParameterName("choice",false);
  // fCoverThicknessCmd->SetUnitCategory("Length");
  // fCoverThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fTargetYCmd = new G4UIcmdWithADoubleAndUnit("/alpha/det/setTargetY",this);
  fTargetYCmd->SetGuidance("Set Target Y");
  fTargetYCmd->SetParameterName("choice",false);
  fTargetYCmd->SetUnitCategory("Length");
  fTargetYCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fTargetZCmd = new G4UIcmdWithADoubleAndUnit("/alpha/det/setTargetZ",this);
  fTargetZCmd->SetGuidance("Set Target Z");
  fTargetZCmd->SetParameterName("choice",false);
  fTargetZCmd->SetUnitCategory("Length");
  fTargetZCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fTargetShapeCmd = new G4UIcmdWithAString("/alpha/det/setTargetShape",this);
  fTargetShapeCmd->SetGuidance("Sets the shape of the Target. Options are Cube or Cylinder.");
  fTargetShapeCmd->SetParameterName("choice",false);
  fTargetShapeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fSimWaferCmd = new G4UIcmdWithABool("/alpha/det/simWafer", this);
  fSimWaferCmd->SetGuidance("Simulate the wafer in the detector or not.");
  fSimWaferCmd->SetParameterName("choice", false);
  fSimWaferCmd->SetDefaultValue("true");
  fSimWaferCmd->AvailableForStates(G4State_PreInit, G4State_Idle);


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
  delete fTargMatCmd;
  delete fDetectMatCmd;
  delete fTargRadiusCmd;
  delete fDetectThicknessCmd;
  delete fTargLengthCmd;
  delete fDetectLengthCmd;
  delete fDetDir;
  delete fRdecayDir;
  delete fStepMaxDetCmd;
  delete fStepMaxTarCmd;
  delete fTargetXCmd;
  delete fTargetYCmd;
  delete fTargetZCmd;
  delete fTargetShapeCmd;
  delete fSimWaferCmd;
  // delete fCoverThicknessCmd;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if (command == fTargMatCmd ){
    fDetector->SetTargetMaterial(newValue);
  }
  else if (command == fTargLengthCmd ){
    fDetector->SetTargetLength(fTargLengthCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fTargRadiusCmd ){
    fDetector->SetTargetRadius(fTargLengthCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fDetectMatCmd ){
    fDetector->SetDetectorMaterial(newValue);
  }
  else if (command == fDetectLengthCmd ){
    fDetector->SetDetectorLength(fDetectLengthCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fDetectThicknessCmd ){
    fDetector->SetDetectorThickness(fDetectThicknessCmd->GetNewDoubleValue(newValue));
  }
  else if( command == fStepMaxDetCmd)
  {
    fDetector->SetMaxStepDetector(fStepMaxDetCmd->GetNewDoubleValue(newValue));
  }
  else if( command == fStepMaxTarCmd)
  {
    fDetector->SetMaxStepTarget(fStepMaxTarCmd->GetNewDoubleValue(newValue));
  }
  else if( command == fTargetXCmd)
  {
    fDetector->SetTargetX(fTargetXCmd->GetNewDoubleValue(newValue));
  }
  else if( command == fTargetYCmd)
  {
    fDetector->SetTargetY(fTargetYCmd->GetNewDoubleValue(newValue));
  }
  else if( command == fTargetZCmd)
  {
    fDetector->SetTargetZ(fTargetZCmd->GetNewDoubleValue(newValue));
  }
  else if( command == fTargetShapeCmd)
  {
    fDetector->SetTargetShape(newValue);
  }
  else if( command == fSimWaferCmd)
  {
    fDetector->SetSimWafer(fSimWaferCmd->GetNewBoolValue(newValue));
  }
  // else if( command == fCoverThicknessCmd){
    // fDetector->SetCoverThickness(fCoverThicknessCmd->GetNewDoubleValue(newValue));
  // }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
