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
/// \file RunAction.cc
/// \brief Implementation of the RunAction class
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "RunAction.hh"
#include "Run.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "HistoManager.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "Randomize.hh"
#include <iomanip>
#include <random>
#include <ctime>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(DetectorConstruction* det, PrimaryGeneratorAction* prim)
  : G4UserRunAction(),
    fDetector(det), fPrimary(prim), fRun(0), fHistoManager(0)
{
 // Book predefined histograms
 // Define command to change output file name
 {
    m_cmdFileName = make_shared<G4UIcmdWithAString>("/run/fileName", this);
    m_cmdFileName->SetParameterName("file name", false);
 }
  fHistoManager = new HistoManager();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
 delete fHistoManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Run* RunAction::GenerateRun()
{ 
  fRun = new Run(fDetector); 
  return fRun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{    
  /* change 23.01.26 try without master check
  // save Rndm status
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
  if (isMaster) G4Random::showEngineStatus();
  
  // keep run condition  
  
  if (fPrimary) { 
    G4ParticleDefinition* particle 
      = fPrimary->GetParticleGun()->GetParticleDefinition();
    G4double energy = fPrimary->GetParticleGun()->GetParticleEnergy();
    fRun->SetPrimary(particle, energy);
  }
  */
             
  //histograms
  //
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  if ( analysisManager->IsActive() ) {
    analysisManager->OpenFile(fHistoFileName);
  }  

  G4cout << "*******************"
         << "\n*** AUTOSEED ON ***"
         << "\n*******************" << G4endl;
  long seeds[2];

  // New rng generator using current time to generate seeds:
  seeds[0] = std::time(nullptr);
  std::mt19937 rng(static_cast<unsigned int>(seeds[0]));
  std::uniform_int_distribution<long> uni(1, 1000000000);
  seeds[1] = seeds[0] - uni(rng);
#ifdef G4MULTITHREADED
  seeds[1] += G4Threading::G4GetThreadId();
#endif
  G4Random::setTheSeeds(seeds);

  // Print the random engine status?
  if (true) {
      G4Random::showEngineStatus();
      G4cout << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run*)
{
  G4cout << " *** End of Run begins now *** " << G4endl;
  if (isMaster) { 
    G4cout << " End of Run in Master Thread " << G4endl;
    fRun->EndOfRun(); } 
  
  //save histograms      
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  if ( analysisManager->IsActive() ) {
    G4cout << "----> Histograms are saved in " 
           << analysisManager->GetFileName() << G4endl;
    analysisManager->Write();
    analysisManager->CloseFile();
    G4cout << " End of Run histograms done " << G4endl;
  }
      
  // show Rndm status
  if (isMaster) G4Random::showEngineStatus();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::SetNewValue(G4UIcommand* command, G4String newValue) {
  if (command == static_cast<G4UIcommand*>(m_cmdFileName.get())) {
    fHistoFileName = newValue;
    G4cout << "### RunAction: Output file name set to " << fHistoFileName << G4endl;
  } else {
    G4cout << "### RunAction: Unknown command " << command->GetCommandPath() << G4endl;
  }
}
