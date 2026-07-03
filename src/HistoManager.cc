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
/// \file HistoManager.cc
/// \brief Implementation of the HistoManager class
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "HistoManager.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
{
  Book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book()
{
  // Create or get analysis manager
  //
  G4AnalysisManager* analysis = G4AnalysisManager::Instance();

  // Merge ntuples (Multithreaded only)
  analysis->SetNtupleMerging(true);
  
  analysis->SetDefaultFileType("root");
  analysis->SetVerboseLevel(1);
  analysis->SetActivation(true);     //enable inactivation of histos, nTuples
    
  // Default values (to be reset via /analysis/h1/set command)               
  G4int nbins = 2000;
  G4double vmin = 0.;
  G4double vmax = 20.;

  // Create all histograms as inactivated 
  // as we have not yet set nbins, vmin, vmax
  //
  ////analysis->SetHistoDirectoryName("histo");  
  ////analysis->SetFirstHistoId(1);
    
  G4int id = analysis->CreateH1("H10","Energy deposit (MeV) in the target",
                       nbins, vmin, vmax);
  analysis->SetH1Activation(id, true);
    
  id = analysis->CreateH1("H11","Energy deposit (MeV) in the detector",
                 nbins, vmin, vmax);
  analysis->SetH1Activation(id, true);

  id = analysis->CreateH1("H12","Total energy (MeV) in target and detector",
                 nbins, vmin, vmax);
  analysis->SetH1Activation(id, true);

  /* // Omitted, coincidence analysis can be done in post-proc
  // Change names and ID'S if reactivated
  id = analysis->CreateH1("H13",
                "Coincidence spectrum (MeV) between the target and detector",
                 nbins, vmin, vmax);
  analysis->SetH1Activation(id, true);  

  id = analysis->CreateH1("H14",
                "Anti-coincidence spectrum (MeV) in the target",
                 nbins, vmin, vmax);
  analysis->SetH1Activation(id, true);

  id = analysis->CreateH1("H15",
                "Anti-coincidence spectrum (MeV) in the detector",
                 nbins, vmin, vmax);
  analysis->SetH1Activation(id, true);  
  */
  id = analysis->CreateH1("H13","Decay emission spectrum (0 - 20 MeV)",
                 nbins, vmin, vmax);
  analysis->SetH1Activation(id, true);  
  
  //id = analysis->CreateH1("H17","Decay emission spectrum (0 - 20 MeV)",
  //               nbins, vmin, vmax);
  //analysis->SetH1Activation(id, true);

  //id = analysis->CreateH1("H18","Decay emission spectrum (0 - 20 MeV)",
  //               nbins, vmin, vmax);
  //analysis->SetH1Activation(id, true);
  
  // nTuples
  //
  ////analysis->SetNtupleDirectoryName("ntuple");
  ////analysis->SetFirstNtupleId(1);
  //
  
  /* // Emitted Particles (from radioactive decay), cut out, as the information is already in the RDecayProducts ntuple. IMportant: CHange all id's by +1 if reactivated
  analysis->CreateNtuple("PEmit", "Emitted Particles");
  analysis->CreateNtupleDColumn("PID");       //column 0
  analysis->CreateNtupleDColumn("Energy");    //column 1
  analysis->CreateNtupleDColumn("Time");      //column 2
  analysis->CreateNtupleDColumn("Weight");    //column 3
  analysis->FinishNtuple();
  */
  /*
  analysis->CreateNtuple("RadioIsotopes", "RadioIsotopes");
  analysis->CreateNtupleDColumn("PID");       //column 0
  analysis->CreateNtupleDColumn("Time");      //column 1
  analysis->CreateNtupleDColumn("Weight");    //column 2
  analysis->FinishNtuple();
  */

  analysis->CreateNtuple("ParentPos", "Parent nuclei and positions");
  analysis->CreateNtupleDColumn("PID");       //column 0
  analysis->CreateNtupleDColumn("Z");         //column 2
  analysis->CreateNtupleDColumn("A");         //column 1
  analysis->CreateNtupleDColumn("x");         //column 3
  analysis->CreateNtupleDColumn("y");         //column 4
  analysis->CreateNtupleDColumn("z");         //column 5

  analysis->CreateNtuple("EDeps", "Energy depositions");
  analysis->CreateNtupleDColumn("Energy");    //column 0
  analysis->CreateNtupleDColumn("Time");      //column 1
  analysis->CreateNtupleDColumn("Weight");    //column 2
  analysis->CreateNtupleDColumn("VolumeID");  //column 3
  analysis->CreateNtupleDColumn("EventID");   //column 4
  analysis->CreateNtupleDColumn("TrackID");   //column 5
  analysis->CreateNtupleDColumn("x");         //column 6
  analysis->CreateNtupleDColumn("y");         //column 7
  analysis->CreateNtupleDColumn("z");         //column 8
  analysis->FinishNtuple();
  
  analysis->CreateNtuple("RDecayProducts", "All Products of RDecay");
  analysis->CreateNtupleDColumn("PID");       //column 0
  analysis->CreateNtupleDColumn("Z");         //column 1
  analysis->CreateNtupleDColumn("A");         //column 2    
  analysis->CreateNtupleDColumn("Energy");    //column 3
  analysis->CreateNtupleDColumn("Time");      //column 4
  analysis->CreateNtupleDColumn("Weight");    //column 5
  analysis->FinishNtuple();

  
  analysis->SetNtupleActivation(true);          
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
