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
/// \file radioactivedecay/rdecay02/include/DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class
//
// $Id: DetectorConstruction.hh 66586 2012-12-21 10:48:39Z ihrivnac $
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4LogicalVolume;
class G4Material;
class DetectorMessenger;
class G4UserLimits;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    DetectorConstruction();
   ~DetectorConstruction();

  public:

    virtual G4VPhysicalVolume* Construct();

    void SetTargetLength (G4double value);
    void SetTargetRadius (G4double value);
    void SetTargetMaterial (G4String);

    void SetTargetX(G4double val);
    void SetTargetY(G4double val);
    void SetTargetZ(G4double val);

    void SetDetectorLength(G4double value);
    void SetDetectorThickness(G4double value);
    void SetDetectorMaterial(G4String);

    void SetMaxStepDetector(G4double val);
    void SetMaxStepTarget(G4double val);

    void PrintParameters();

    void SetTargetShape(G4String val);

    void SetSimWafer(G4bool val);

    // void SetCoverThickness(G4double val);

  public:

    G4double GetTargetLength();
    G4double GetTargetRadius();
    G4Material* GetTargetMaterial();
    G4LogicalVolume* GetLogicTarget();

    G4double GetTargetX();
    G4double GetTargetY();
    G4double GetTargetZ();

    G4double GetDetectorLength();
    G4double GetDetectorThickness();
    G4Material* GetDetectorMaterial();
    G4LogicalVolume* GetLogicDetector();
    G4LogicalVolume* GetLogicAnode();
    //G4LogicalVolume* GetLogicGridUp();
    //G4LogicalVolume* GetLogicGridDo();

    G4String GetTargetShape();

    G4double GetGridToAnodeDist();

  private:
    // Taregt params: 
    G4double            fTargetLength;
    G4double            fTargetRadius;
    G4Material*         fTargetMater;
    G4LogicalVolume*    fLogicTarget;
    G4String            fTargetMaterOpt;
    G4bool              fSimWafer;

    // Detector params:
    G4double            fDetectorLength;
    G4double            fDetectorThickness;
    G4Material*         fDetectorMater;
    G4LogicalVolume*    fLogicDetector;

    // World params:
    G4double            fWorldLength;
    G4double            fWorldRadius;
    G4Material*         fWorldMater;
    G4VPhysicalVolume*  fPhysiWorld;
    // Anode Params:
    G4double            fAnodeRadius;
    G4double            fAnodeThickness;
    // Grid params:
    G4double            fGridHolderThickness1;
    G4double            fGridHolderThickness2;
    G4double            fGridHolderORadius;
    G4double            fGridHolderIRadius;
    G4double            fGridToAnodeDist;
    // Grid Logicals
    G4LogicalVolume*    fLogicGridUpHolder1;
    G4LogicalVolume*    fLogicGridUpHolder2;
    G4LogicalVolume*    fLogicGridDownHolder1;
    G4LogicalVolume*    fLogicGridDownHolder2;

    // POM distance pieces params
    G4double            fRodAnodeGridHolder;
    G4double            fRodGridUpHolderTop;
    G4double            fRodORadius;
    G4double            fRodIRadius;
    G4double            fRodPlaceRadius;
    G4double            fShortRodPlaceHeight;
    G4double            fLongRodPlaceHeight;
    G4LogicalVolume*    fLRodShort;
    G4LogicalVolume*    fLRodLong;
  //  G4double           fCoverLength;
  //  G4Material*        fCoverMater;
    G4Material*         fChamberMater;
    G4Material*         fAnodeMater;
    G4Material*         fRodMater;
    G4LogicalVolume*    fLogicAnode;
    //G4LogicalVolume*    fLogicGridUp;
    //G4LogicalVolume*    fLogicGridDo;

    // Wafer Params: 
    G4double            fWaferRadius;
    G4double            fWaferThickness;
    G4double            fWaferZPosition;
    G4Material*         fWaferMater;
    G4LogicalVolume*    fLogicWafer;


    DetectorMessenger*  fDetectorMessenger;

    G4UserLimits*       fStepLimitDet;            // pointer to user step limits
    G4UserLimits*       fStepLimitTar;            // pointer to user step limits

    G4double            fTarX;
    G4double            fTarY;
    G4double            fTarZ;

    G4String            fTargetShape;


  private:

    void               DefineMaterials();
    G4VPhysicalVolume* ConstructVolumes();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#endif
