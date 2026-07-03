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
/// \file hadronic/Hadr03/src/DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
//
// $Id: DetectorConstruction.cc 70755 2013-06-05 12:17:48Z ihrivnac $
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4VSolid.hh"
// #include "G4Polycone.hh"
// #include "G4GenericPolycone.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4RunManager.hh"
#include "G4VisAttributes.hh"

#include "G4UserLimits.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4UnitsTable.hh"

#include "G4NistManager.hh"//added for extra matirials

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
:G4VUserDetectorConstruction(),
 fTargetLength(0.1*cm), fTargetRadius(1.*cm), fTargetMater(0), fLogicTarget(0), fTargetMaterOpt("Gadolinium"),
 fDetectorLength(271.2 * mm), fDetectorThickness(318./2. * mm), fDetectorMater(0), fLogicDetector(0),
 fWorldLength(fDetectorLength*2.+fTargetLength), fWorldRadius(std::max(fTargetRadius,fDetectorThickness)), fWorldMater(0), fPhysiWorld(0),
 // fCoverLength(1. * nm),fCoverMater(0), fChamberMater(0), fAnodeMater(0), fRodMater(0),fLogicAnode(0), fLogicGridUp(0), fLogicGridDo(0),
 fChamberMater(0), fAnodeMater(0), fRodMater(0),fLogicAnode(0), fLogicGridUpHolder1(0), fLogicGridUpHolder2(0), fLogicGridDownHolder1(0), fLogicGridDownHolder2(0),
 fDetectorMessenger(0), fStepLimitDet(NULL),fStepLimitTar(NULL),
 fTarX(0.), fTarY(0.), fTarZ(0.), fTargetShape("Cylinder"), fGridToAnodeDist(35.*mm)
{
 // fTargetLength      = 1. * cm;
  // fTargetRadius      = 1. * cm;
  // fDetectorLength    = 271.2 * mm;    //From schematics of chamber
  //fDetectorThickness = 318./2. * mm;  //From schematics of chamber

  // fTargetShape = "Cylinder";

  // fCoverLength       = 2. * nm;

  // fWorldLength = std::max(fTargetLength,fDetectorLength);
  // fWorldLength = fDetectorLength*2.+fTargetLength;
  // fWorldRadius = std::max(fTargetRadius,fDetectorThickness);


  fDetectorMessenger = new DetectorMessenger(this);
  // DefineMaterials();


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
  delete fDetectorMessenger;
  delete fStepLimitDet;
  delete fStepLimitTar;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  return ConstructVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{
  //--------- Material definition ---------

    //=====================Material Manager ==================
    //Mostly using the default material and NIST materials
    //fMaterialsManager = new exrdmMaterial();
    G4NistManager* man = G4NistManager::Instance();
    man->SetVerbose(1);

   //=======================Stainless Steal====================
    //Define StainlessSteel
    // Define elements from NIST
    G4Element* C  = man->FindOrBuildElement("C");
    G4Element* Si = man->FindOrBuildElement("Si");
    G4Element* Cr = man->FindOrBuildElement("Cr");
    G4Element* Mn = man->FindOrBuildElement("Mn");
    G4Element* Fe = man->FindOrBuildElement("Fe");
    G4Element* Ni = man->FindOrBuildElement("Ni");

    G4Material* StainlessSteel = new G4Material("StainlessSteel", 8.06*g/cm3,6);
    StainlessSteel->AddElement(C, 0.1*perCent);
    StainlessSteel->AddElement(Si,0.7*perCent);
    StainlessSteel->AddElement(Cr,18.*perCent);
    StainlessSteel->AddElement(Mn,1.*perCent);
    StainlessSteel->AddElement(Fe,71.2*perCent);
    StainlessSteel->AddElement(Ni,9.*perCent);

   //=======================Materila for Anode and holder =====
   //Assiming that the rods are plastic (G4_POLYPROPYLENE)
    G4Material* Copper = man->FindOrBuildMaterial("G4_Cu");
    G4Material* Plastic = man->FindOrBuildMaterial( "G4_POLYPROPYLENE");


   //=======================Define P10 ========================
   //Construct some P10 (to a PPM level)
   //values from http://www.airliquide.de/gasekatalog/pdb/argon90-methan10-p10.pdf
   //Assuming max contamination, Assuming the contamination is 0.0068% from the
   //purity of Argon and Methan, then assumed that the contamination has the same
   //ratio there.

    //TODO check is the dencity is the same at standert pressure and temp
    //TODO the dencity temp amd press is at "stp", 0 degC and 101.325 kPa
    //Dencitys taken from Wiki

    //Oxygen gas
    G4Element* O  = man->FindOrBuildElement("O");
    G4Material* Oxygen = new G4Material("Oxygen", 1.429*(kg/m3),1,kStateGas);
    Oxygen->AddElement(O,2);

    //Nitrogen gas
    G4Element* N  = man->FindOrBuildElement("N");
    G4Material* Nitrogen = new G4Material("Nitrogen", 1.251*(kg/m3),1,kStateGas);
    Nitrogen->AddElement(N,2);

    //Helium gas
    G4Element* H  = man->FindOrBuildElement("H");
    G4Material* Hydrogen = new G4Material("Hydrogen", 0.08988*(kg/m3),1,kStateGas);
    Hydrogen->AddElement(H,2);

    //Water vapour gas
    G4Material* Water = new G4Material("Water", 0.804*(kg/m3),2,kStateGas);
    Water->AddElement(H,2);
    Water->AddElement(O,1);

    //Assumung this means Butane
    G4Material* HC6 = man->FindOrBuildMaterial("G4_BUTANE");

    //Argon and Methane
    G4Material* Ar = man->FindOrBuildMaterial("G4_Ar");
    G4Material* Methane = man->FindOrBuildMaterial("G4_METHANE");

    //Biuld P10
    G4Material* P10 = new G4Material("P10", 1.57*kg/m3,7,kStateGas);
    P10->AddMaterial(Ar,       89.9982     *perCent);
    P10->AddMaterial(Methane,  9.995       *perCent);
    P10->AddMaterial(Nitrogen, 0.0030222222*perCent);
    P10->AddMaterial(Oxygen,   0.0007555556*perCent);
    P10->AddMaterial(Hydrogen, 0.0007555556*perCent);
    P10->AddMaterial(Water,    0.0007555556*perCent);
    P10->AddMaterial(HC6,      0.0015111111*perCent);

    //Build P5
    G4Material* P5 = new G4Material("P5", 1.7*kg/m3, 2, kStateGas);
    P5->AddMaterial(Ar, 95.*perCent);
    P5->AddMaterial(Methane, 5.*perCent);

    // Build POM
    G4Material* POM = new G4Material("POM", 1.42*g/cm3, 3, kSTateSolid);
    POM->AddMaterial(C, nAtoms=1);
    POM->AddMaterial(H, nAtoms=2);
    POM->AddMaterial(O, nAtoms=1);
    //G4Material* P5 = new G4Material("P5")

   //========================Target Materials===================
   //I wanted controle over the isotropic abandance for later use.
   //The abandances are from an ICP-MS we took of a Sm sample

    if(fTargetMaterOpt == "Samarium"){
      G4Element* Sm144 = new G4Element("Sm144","Sm",62.,144.*(g/mole));
      G4Element* Sm147 = new G4Element("Sm147","Sm",62.,147.*(g/mole));
      G4Element* Sm148 = new G4Element("Sm148","Sm",62.,148.*(g/mole));
      G4Element* Sm149 = new G4Element("Sm149","Sm",62.,149.*(g/mole));
      G4Element* Sm150 = new G4Element("Sm150","Sm",62.,150.*(g/mole));
      G4Element* Sm152 = new G4Element("Sm152","Sm",62.,152.*(g/mole));
      G4Element* Sm154 = new G4Element("Sm154","Sm",62.,154.*(g/mole));

      G4Material* Samarium = new G4Material("Samarium",7.5164*g/cm3,7,kStateSolid); //could put temp and pressure in?
      Samarium->AddElement(Sm144,0.0301);
      Samarium->AddElement(Sm147,0.1477);
      Samarium->AddElement(Sm148,0.1122);
      Samarium->AddElement(Sm149,0.1391);
      Samarium->AddElement(Sm150,0.0708);
      Samarium->AddElement(Sm152,0.2647);
      Samarium->AddElement(Sm154,0.235);

      fTargetMater   = Samarium;
    }
    else if(fTargetMaterOpt == "Samarium144"){
      G4Element* Sm144 = new G4Element("Sm144","Sm",62.,144.*(g/mole));
      G4Element* Sm147 = new G4Element("Sm147","Sm",62.,147.*(g/mole));
      G4Element* Sm148 = new G4Element("Sm148","Sm",62.,148.*(g/mole));
      G4Element* Sm149 = new G4Element("Sm149","Sm",62.,149.*(g/mole));
      G4Element* Sm150 = new G4Element("Sm150","Sm",62.,150.*(g/mole));
      G4Element* Sm152 = new G4Element("Sm152","Sm",62.,152.*(g/mole));
      G4Element* Sm154 = new G4Element("Sm154","Sm",62.,154.*(g/mole));

      G4Material* Samarium = new G4Material("Samarium",7.2327235967*g/cm3,7,kStateSolid); //using the A to correct the dencity of 7.52g/cm3
      Samarium->AddElement(Sm144,0.884);
      Samarium->AddElement(Sm147,0.038);
      Samarium->AddElement(Sm148,0.019);
      Samarium->AddElement(Sm149,0.020);
      Samarium->AddElement(Sm150,0.007);
      Samarium->AddElement(Sm152,0.018);
      Samarium->AddElement(Sm154,0.014);

      fTargetMater   = Samarium;
    }
    else if(fTargetMaterOpt == "Platinum"){
      //Values taken from the IRMM-010 data sheet
      G4Element* Pt190 = new G4Element("Pt190","Pt",78.,189.959930*(g/mole));
      G4Element* Pt192 = new G4Element("Pt192","Pt",78.,191.9610350*(g/mole));
      G4Element* Pt194 = new G4Element("Pt194","Pt",78.,193.9626630*(g/mole));
      G4Element* Pt195 = new G4Element("Pt195","Pt",78.,194.9647740*(g/mole));
      G4Element* Pt196 = new G4Element("Pt196","Pt",78.,195.9649340*(g/mole));
      G4Element* Pt198 = new G4Element("Pt198","Pt",78.,197.967875*(g/mole));


      G4Material* Platinum = new G4Material("Platinum",21.45*g/cm3,6,kStateSolid); //could put temp and pressure in?

      Platinum->AddElement(Pt190,0.0117*perCent);
      Platinum->AddElement(Pt192,0.782*perCent);
      Platinum->AddElement(Pt194,32.86*perCent);
      Platinum->AddElement(Pt195,33.78*perCent);
      Platinum->AddElement(Pt196,25.21*perCent);
      Platinum->AddElement(Pt198,7.356*perCent);

      fTargetMater   = Platinum;
    }
    else if(fTargetMaterOpt == "Silicon"){
      //Silicon for the ion implantation
      G4Material* Silicon = man->FindOrBuildMaterial("G4_Si"); // assuming only silicon with no impuritys
      fTargetMater  = Silicon;

    }
    else if(fTargetMaterOpt == "Bismuth"){
      //Bismuth for Bi210 studies
      G4Material* Bismuth = man->FindOrBuildMaterial("G4_Bi"); // Bismuth
      fTargetMater = Bismuth;
    }
    else if(fTargetMaterOpt == "Hafnium"){
      //Bismuth for Bi210 studies
      G4Material* Hafnium = man->FindOrBuildMaterial("G4_Hf"); // Bismuth
      fTargetMater = Hafnium;
    }
    else if(fTargetMaterOpt == "Hafnium_Oxide"){
      //Bismuth for Bi210 studies

      G4Material* HafniumOx = new G4Material("HafniumOx", 9.68*g/cm3, 2);

      G4Element* Hf = new G4Element("Hafnium","Hf" ,72 ,177.2665*g/mole);
      // G4Element* O  = new G4Element("Oxygen" ,"O" ,8. ,16.00*g/mole);

      HafniumOx->AddElement(Hf , 1);
      HafniumOx->AddElement(O , 2);

      fTargetMater = HafniumOx;
    }
    else if(fTargetMaterOpt == "Neodymium_Oxide"){
      //Bismuth for Bi210 studies

      G4Material* NeodymiumOx = new G4Material("NeodymiumOx", 7.24*g/cm3, 2);

      G4Element* Nd = new G4Element("Neodymium","Nd" ,60 ,144.242*g/mole);
      // G4Element* O  = new G4Element("Oxygen" ,"O" ,8. ,16.00*g/mole);

      NeodymiumOx->AddElement(Nd , 2);
      NeodymiumOx->AddElement(O , 3);

      fTargetMater = NeodymiumOx;
    }
    else if(fTargetMaterOpt == "Ytterbium"){
      G4Material* Ytterbium = man->FindOrBuildMaterial("G4_Yb"); // Ytterbium
      fTargetMater = Ytterbium;
    }
    else if(fTargetMaterOpt == "Neodymium"){
      G4Material* Neodymium = man->FindOrBuildMaterial("G4_Nd"); // Ytterbium
      fTargetMater = Neodymium;
    }
    else if(fTargetMaterOpt == "Delrin"){
      G4Material* Delrin = man->FindOrBuildMaterial("G4_POLYOXYMETHYLENE"); // Ytterbium
      fTargetMater = Delrin;
    }
    else if(fTargetMaterOpt == "Aluminium"){
      G4Material* Aluminium = man->FindOrBuildMaterial("G4_Al"); // Aluminium
      fTargetMater = Aluminium;
    }
    else if(fTargetMaterOpt == "Americium"){
      G4Material* Americium = man->FindOrBuildMaterial("G4_Am"); // Americium
      fTargetMater = Americium;
    }
    else if(fTargetMaterOpt == "Vacuum"){
      G4Material* Aluminium = man->FindOrBuildMaterial("G4_Galactic"); // Ytterbium
      fTargetMater = Aluminium;
    }
    else if(fTargetMaterOpt == "P10"){
      // G4Material* Delrin = man->FindOrBuildMaterial("G4_POLYOXYMETHYLENE"); // Ytterbium
      fTargetMater = P10;
    }
    else if (fTargetMaterOpt == "Gadolinium"){
      G4Material* Gadolinium = man->FindOrBuildMaterial("G4_Gd"); // Gadolinium
      fTargetMater = Gadolinium;
    }
    else {
      G4cout << "WRONG MATERIAL SELECTED!!!!"  << G4endl;
      return;
    }
   //========================Target Materials===================
    //G4Material* Chromium = man->FindOrBuildMaterial("G4_Cr");

    //======================Add the Material =================
    fWorldMater    = man->FindOrBuildMaterial("G4_Galactic");
//    fCoverMater    = Chromium;
    fDetectorMater = P5;
    fChamberMater  = StainlessSteel;
    fAnodeMater    = Copper;
    fRodMater      = POM;


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::ConstructVolumes()
{
  /*===========================================
                  Initialize
  ===========================================*/
  //Cleans up old geomitry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  //Define materials in construction so that the
  //messenger can comunicate with it
  DefineMaterials();

  /*===========================================
                  Constructor parameters
  ===========================================*/
  //I defining this here as I need to know how big to make the world

  // Define the varibles of the chamber outside
  G4double ChamWallThick = 2.*mm;

  // Define Cover Propertys
  G4double ChamCoverThick = 2.*cm;
  G4double ChamCoverRadius = (350./2.)*mm;

  /*===========================================
                  World
  ===========================================*/
  //Determines the world variables
  fWorldLength = 2.*fDetectorLength+ChamCoverThick*2.;
  fWorldRadius = std::max(ChamCoverRadius, std::max(fTargetRadius, fDetectorThickness+ChamCoverThick));

  //Devinse the world size and places at 0,0,0
  G4Tubs* sWorld = new G4Tubs("World", 0.,fWorldRadius, 0.5*fWorldLength, 0.,twopi);
  G4LogicalVolume* lWorld = new G4LogicalVolume(sWorld,fWorldMater,"World");
  fPhysiWorld = new G4PVPlacement(0,G4ThreeVector(),lWorld,"World",0,false,  0);

  /*===========================================
                  Detecton
  ===========================================*/
  //Defines and places detector volume in world
  G4Tubs* sDetector = new G4Tubs("Detector", 0.,fDetectorThickness , 0.5*fDetectorLength, 0.,twopi);
  fLogicDetector = new G4LogicalVolume(sDetector, fDetectorMater, "Detector");
  new G4PVPlacement(0,G4ThreeVector(0.,0.,0.5*fDetectorLength),fLogicDetector,"Detector",lWorld,false,0);

  /*===========================================
                  Target
  ===========================================*/
  //Defines and places target volume
  if(fTargetShape == "Cylinder"){
    G4Tubs* sTarget = new G4Tubs("Target", 0., fTargetRadius, 0.5*fTargetLength, 0.,twopi);
    fLogicTarget = new G4LogicalVolume(sTarget, fTargetMater,"Target");
    new G4PVPlacement(0, G4ThreeVector(fTarX,fTarY,-0.5*fDetectorLength+0.5*fTargetLength+fTarZ),fLogicTarget,"TargetPhys",fLogicDetector,false,0);
  }
  else if(fTargetShape == "Square"){
    G4Box* sTarget = new G4Box("target",0.5*fTargetRadius ,0.5*fTargetRadius ,0.5*fTargetLength);
    fLogicTarget = new G4LogicalVolume(sTarget, fTargetMater,"Target");
    new G4PVPlacement(0, G4ThreeVector(fTarX,fTarY,-0.5*fDetectorLength+0.5*fTargetLength+fTarZ),fLogicTarget,"TargetPhys",fLogicDetector,false,0);
  }
  else if(fTargetShape == "ThreeFourInchDisc"){
    //Position of the smaples from drawing and measuring
    // x  -  y    pt sample
    G4double x1 =  -2.9329*cm; G4double y1 =  5.08*cm;
    G4double x2 =  -2.9329*cm; G4double y2 = -5.08*cm;
    G4double x3 =   5.8659*cm; G4double y3 =  0.00*cm;
    //============
    //From pictures
    // G4double x1 =  4.989*cm; G4double y1 = 2.708*cm;
    // G4double x2 = -4.762*cm; G4double y2 = 3.304*cm;
    // G4double x3 = -0.757*cm; G4double y3 =-5.653*cm;
    //This will just be one of the targets, on the default
    //====================
    //Thickness from mass measurnemt
    G4double t1 = 392.7499242044*nm;
    G4double t2 = 432.4274421694*nm;  //should have a 1% error
    G4double t3 = 420.3516758322*nm;

    G4Tubs* Target1 = new G4Tubs("Target1", 0., 0.5*10.16*cm, 0.5*t1, 0.,twopi);
    G4Tubs* Target2 = new G4Tubs("Target2", 0., 0.5*10.16*cm, 0.5*t2, 0.,twopi);
    G4Tubs* Target3 = new G4Tubs("Target3", 0., 0.5*10.16*cm, 0.5*t3, 0.,twopi);
    fLogicTarget = new G4LogicalVolume(Target1, fTargetMater,"Target1");
    G4LogicalVolume* LogicTarget2 = new G4LogicalVolume(Target2, fTargetMater,"Target2");
    G4LogicalVolume* LogicTarget3 = new G4LogicalVolume(Target3, fTargetMater,"Target3");
    new G4PVPlacement(0, G4ThreeVector(x1,y1,-0.5*fDetectorLength+0.5*t1+fTarZ),fLogicTarget,"TargetPhys",fLogicDetector,false,0);
    new G4PVPlacement(0, G4ThreeVector(x2,y2,-0.5*fDetectorLength+0.5*t2+fTarZ),LogicTarget2,"TargetPhys",fLogicDetector,false,0);
    new G4PVPlacement(0, G4ThreeVector(x3,y3,-0.5*fDetectorLength+0.5*t3+fTarZ),LogicTarget3,"TargetPhys",fLogicDetector,false,0);

    LogicTarget2   ->SetVisAttributes(G4Colour(1.0, 0.0 , 0.0, 1.0)); //red for target
    LogicTarget3   ->SetVisAttributes(G4Colour(1.0, 0.0 , 0.0, 1.0)); //red for target

  }
  else if(fTargetShape == "SiTarget"){
    // The target for the ion implantation
    G4double SiThick = 0.5*mm;
    // G4double SiThick = 10*mm;
    fTargetLength = SiThick;  // Overide it for particle gun orientation
    G4Tubs* sTarget = new G4Tubs("Target", 0., fTargetRadius, 0.5*fTargetLength, 0.,twopi);
    fLogicTarget = new G4LogicalVolume(sTarget, fTargetMater,"Target");
    new G4PVPlacement(0, G4ThreeVector(fTarX,fTarY,-0.5*fDetectorLength+0.5*fTargetLength+fTarZ),fLogicTarget,"TargetPhys",fLogicDetector,false,0);

    //set step size in target
    //G4double TargetMaxStep = 0.1*nm;
    //G4UserLimits *TargetStepLimit = new G4UserLimits(TargetMaxStep);
    //fLogicTarget->SetUserLimits(TargetStepLimit);

  }
  else if (fTargetShape == "Gaussian"){
    // G4double sigma = 8.30844e+00;
    G4double sigma = fTargetRadius;
    // G4double thick = 4.79373e+02;
    G4double thick = fTargetLength;
    G4double mean = 0.0;

    // const G4int number = 5000+1;
    // const G4int number = 3500;
    // const G4int number = 0;

    // G4double r[number*2+1];
    // G4double z[number*2+1];

    // G4double r[number+3];
    // G4double z[number+3];

    // G4int cnt = 0;

    //Acording to G4Polycone.hh "Alternative constructor, but limited to increasing-only Z sections:"

    // for(G4int i = 0; i  < number; i++){
      // z[cnt] = 0.0;
      // r[cnt] = G4double(i)*0.01*mm;
      // cnt++;
    // }
    // for(G4int i = number; i >= 0; i--){
      // z[cnt] = thick*exp(-((G4double(i)*0.01-mean)*(G4double(i)*0.01-mean))/(2.*sigma*sigma));
      // r[cnt] = G4double(i)*0.01*mm;
      // cnt++;
    // }
//    double step_r = (2./number)*cm;
//    for(G4int i = 0; i < number+1; i++){
//      r[cnt] = G4double(i)*step_r;
//      // z[cnt] = thick*exp(-((G4double(i)*0.01-mean)*(G4double(i)*0.01-mean))/(2.*sigma*sigma));
//      z[cnt] = thick*exp(-((r[cnt]-mean)*(r[cnt]-mean))/(2.*sigma*sigma));
//      cnt++;
//    }

//    r[cnt] = r[cnt-1] ; z[cnt] = 0.*cm; cnt++;
//    r[cnt] = 0.*cm ; z[cnt] = 0.*cm;

    // r[2] = 0.*cm ; z[2] = 0.*cm;
    // r[1] = 2.*cm ; z[1] = 0.*cm;
    // r[0] = 0.*cm ; z[0] = thick;

    // r[1] = 1.*cm ; z[1] = 0.*cm;
    // r[3] = 2.*cm ; z[3] = thick;
    // r[4] = 1.*cm ; z[4] = thick;

    // r[0] = 0.*cm ; z[0] = thick;
    // r[1] = 1.*cm ; z[1] = thick;
    // r[2] = 2.*cm ; z[2] = thick;
    // r[3] = 2.*cm ; z[3] = 0.*cm;
    // r[4] = 1.*cm ; z[4] = 0.*cm;
    // r[5] = 0.*cm ; z[5] = 0.*cm;

    // G4VSolid *sTarget = new G4Polycone("Target",0.0*deg,360.0*deg,number*2+1,r,z);
    // G4GenericPolycone *sTarget = new G4GenericPolycone("Target",0.0*deg,360.0*deg,number*2+1,r,z);
    // G4GenericPolycone *sTarget = new G4GenericPolycone("Target",0.0*deg,360.0*deg,number+3,r,z);
    // G4GenericPolycone *sTarget = new G4GenericPolycone("Target",0.0*deg,360.0*deg,number+3,r,z);

    //number of G4Cons to make
    const G4int n = 100;

    //number of points for G4Cons
    // const G4int np = n*2;

    // G4double slice_thick = 0.5*(thick/G4double(n)); // half thick of thick / n

    // G4double r[n+1] = {0.*cm,1.*cm,2.*cm};
    G4double r_max = 2.1*cm;
    G4double step_r = r_max/G4double(n);
    G4double r[n+1];// = {0.*cm,1.*cm,2.*cm};
    G4double z[n+1];

    for(int i = 0; i < n+1; i++){
      r[i] = step_r*i;
    }

    G4double z_sl[n];

    // G4double rmin[np];
    // G4double rmax[np];

    //makes the radii out of the z values
    for(int i = 0; i < n+1; i++){
      // rmin[i] = 0.*cm;
      // rmax[i] = r[i];
      z[i] = thick*exp(-((r[i]-mean)*(r[i]-mean))/(2.*sigma*sigma));
      G4cout << "z =" << z[i] << G4endl;
    }

    for(int i = 0; i < n; i++){
      z_sl[i] = z[i] - z[i+1];
      G4cout << "z_sl =" << z_sl[i] << G4endl;
    }

    G4Cons *sTarget[n];
    G4LogicalVolume *LogicTarget[n-1]; //fLogicTarget is already defined

    //defines shape, logic and places
    G4double place = 0.;
    for(G4int i = 0 ; i < n; i++){
      std::stringstream name; name << "Target" << i+1;
      // sTarget[i] = new G4Cons(name.str().data(),rmin[i+1],rmax[i+1],rmin[i],rmax[i],slice_thick,0.0*deg,360.0*deg);
      sTarget[i] = new G4Cons(name.str().data(),0.*cm,r[i+1],0.*cm,r[i],0.5*z_sl[i],0.0*deg,360.0*deg);
      if(i==0){
        fLogicTarget = new G4LogicalVolume(sTarget[i], fTargetMater,name.str().data());
        new G4PVPlacement(0, G4ThreeVector(fTarX,fTarY,-0.5*fDetectorLength+fTarZ+fTargetLength-0.5*z_sl[i]-place),fLogicTarget,"TargetPhys",fLogicDetector,false,0);
      }
      else {  //fLogicTarget muse be defined otherwise seg vio 11
        LogicTarget[i-1] = new G4LogicalVolume(sTarget[i], fTargetMater,name.str().data());
        new G4PVPlacement(0, G4ThreeVector(fTarX,fTarY,-0.5*fDetectorLength+fTarZ+fTargetLength-0.5*z_sl[i]-place),LogicTarget[i-1],"TargetPhys",fLogicDetector,false,0);
        LogicTarget[i-1]->SetVisAttributes(G4Colour(1.0, 0.0 , 0.0, 1.0)); //red for target
      }
      place = place + z_sl[i];
    }

    G4Tubs *TBot = new G4Tubs("Target", 0., r[n],0.5*(fTargetLength-place),0.0*deg,360.0*deg);
    G4LogicalVolume *LogicTBot = new G4LogicalVolume(TBot, fTargetMater,"Target");
    LogicTBot->SetVisAttributes(G4Colour(1.0, 0.0 , 0.0, 1.0)); //red for target
    new G4PVPlacement(0, G4ThreeVector(fTarX,fTarY,-0.5*fDetectorLength+fTarZ+0.5*(fTargetLength-place)),LogicTBot,"TargetPhys",fLogicDetector,false,0);

    // G4double rmin_1 = 0*cm;
    // G4double rmax_1 = 1*cm;

    // G4double rmin_2 = 0*cm;
    // G4double rmax_2 = 0*cm;


    //G4Cons (pName,  pRmin1,  pRmax1,  pRmin2,  pRmax2,  pDz,  pSPhi,  pDPhi)
    // G4Cons *sTarget2 = new G4Cons("Target2",0.*cm,2.*cm,0.*cm,1.*cm,thick*0.25,0.0*deg,360.0*deg);
    // G4LogicalVolume *LogicTarget2 = new G4LogicalVolume(sTarget2, fTargetMater,"Target2");
    // new G4PVPlacement(0, G4ThreeVector(fTarX,fTarY,-0.5*fDetectorLength+fTarZ+fTargetLength*0.25),LogicTarget2,"TargetPhys",fLogicDetector,false,0);
    // new G4PVPlacement(0, G4ThreeVector(fTarX,fTarY,fTarZ),fLogicTarget,"TargetPhys",fLogicDetector,false,0);

    //set step size in target
    G4double TargetMaxStep = 10.*nm;
    G4UserLimits *TargetStepLimit = new G4UserLimits(TargetMaxStep);
    fLogicTarget->SetUserLimits(TargetStepLimit);
    fLogicDetector->SetUserLimits(TargetStepLimit);

  }
  else {
    G4cout << "Target"  << G4endl;
    G4cout << "****************************************************"<< G4endl;
    G4cout << "*** Shape " << fTargetShape << " not found!!! ***" << G4endl;
    G4cout << "****************************************************"<< G4endl;
     return fPhysiWorld;
  }

  /*===========================================
                  Cover
  ===========================================*/
  //Postion cover on target

  //Target has the same size as the target, but different material and thickness
  //G4Box *SolidCov = new G4Box("cover",fD/2.,fD/2.,fCoverLength/2.);

  // if(fTargetShape == "Cylinder"){
    // G4ThreeVector positionCover = G4ThreeVector(fTarX,fTarY,-0.5*fDetectorLength+fTargetLength+0.5*fCoverLength+fTarZ);
    // G4Tubs* SolidCov = new G4Tubs("SolidCov", 0., fTargetRadius, 0.5*fCoverLength, 0.,twopi);
    // G4LogicalVolume* LogicCover = new G4LogicalVolume(SolidCov,fCoverMater,"Cover",0,0,0);
    // new G4PVPlacement(0,positionCover, LogicCover, "Target", fLogicDetector, false, 0);
    // LogicCover->SetVisAttributes(G4Colour(0.5, 0.5 , 0.5, 1.0));
  // }
  // else if(fTargetShape == "Square"){
    // G4ThreeVector positionCover = G4ThreeVector(fTarX,fTarY,-0.5*fDetectorLength+fTargetLength+0.5*fCoverLength+fTarZ);
    // G4Box* SolidCov = new G4Box("SolidCov",0.5*fTargetRadius ,0.5*fTargetRadius ,0.5*fCoverLength);
    // G4LogicalVolume* LogicCover = new G4LogicalVolume(SolidCov,fCoverMater,"Cover",0,0,0);
    // new G4PVPlacement(0,positionCover, LogicCover, "Target", fLogicDetector, false, 0);
    // LogicCover->SetVisAttributes(G4Colour(0.5, 0.5 , 0.5, 1.0));
  // }
  // else if(fTargetShape == "ThreeFourInchDisc" || fTargetShape == "SiTarget"){
    // G4cout << "There is no cover on this target!" <<  G4endl;
  // }
  // else {
    // G4cout << "Cover"  << G4endl;
    // G4cout << "****************************************************"<< G4endl;
    // G4cout << "*** Shape " << fTargetShape << " not found!!! ***" << G4endl;
    // G4cout << "****************************************************"<< G4endl;
    // return fPhysiWorld;
  // }

  //===========================================================================
  //          Paper
  //===========================================================================

  // Paper amterial, based on wood...
  //Dencity from google
/*  G4Material* paperMat = new G4Material("paperMat", 1.20*g/cm3, 3);

  G4Element* H = new G4Element("Hydrogen","H" ,1. ,1.00794*g/mole);
  G4Element* C  = new G4Element("Carbon" ,"C" ,6. ,12.011*g/mole);
  G4Element* O  = new G4Element("Oxygen" ,"O" ,8. ,16.00*g/mole);

  paperMat->AddElement(H , 4);
  paperMat->AddElement(O , 1);
  paperMat->AddElement(C , 2);

  //The pice of paper is located 1cm away and is 0.2 x 1.0 cm^2 in area
  G4double r_papar = 0.2*cm;  // radius of where the paper is at
  G4double h_paper = 1.0*cm;  //height of pice of paper
  G4double w_paper = 1.0*cm;   // Width of paper
  G4double t_paper = 0.01*cm; // Thickness of paper ~0.1 mm
  G4ThreeVector paperPos = G4ThreeVector(r_papar,0.,-0.5*fDetectorLength+0.5*h_paper);

  //Paper is to be a box
  G4Box *SolidPaper = new G4Box("SolidPaper",0.5*t_paper,0.5*w_paper ,0.5*h_paper);
  G4LogicalVolume *LogicPaper = new G4LogicalVolume( SolidPaper, paperMat, "Paper", 0, 0, 0);

  // Placed at a radius away from the source
  new G4PVPlacement(0,paperPos, LogicPaper,"Paper",fLogicDetector,false,0);
*/
  //===========================================================================
  //          Chamber Bottom
  //===========================================================================

  //Chamber bottom is located under the target
  G4ThreeVector positionChamberBot = G4ThreeVector(0.,0.,-1.*ChamCoverThick/2.);

  //Chamber cover has the thickness ChamCoverThick ans radius ChamCoverRadius
  G4Tubs *SolidChamberBot = new G4Tubs("chamberbot",0.,ChamCoverRadius,ChamCoverThick/2.,0.,twopi);
  G4LogicalVolume *LogicCamberBot = new G4LogicalVolume( SolidChamberBot, fChamberMater, "ChamberBot", 0, 0, 0);

  // Plased at the Bottom
  new G4PVPlacement(0,positionChamberBot, LogicCamberBot,"ChamberBot",lWorld,false,0);

  //===========================================================================
  //          Chamber Top
  //===========================================================================

  //Chamber bottom is located under the target
  G4ThreeVector positionChamberTop = G4ThreeVector(0.,0.,fDetectorLength+ChamCoverThick/2.);

  //Chamber cover has the thickness ChamCoverThick ans radius ChamCoverRadius
  G4Tubs *SolidChamberTop = new G4Tubs("chambertop",0.,ChamCoverRadius,ChamCoverThick/2.,0.,twopi);
  G4LogicalVolume *LogicCamberTop = new G4LogicalVolume( SolidChamberTop, fChamberMater, "ChamberTop", 0, 0, 0);

  // Plased at the Top
  new G4PVPlacement(0,positionChamberTop,LogicCamberTop,"ChamberTop",lWorld,false,0);

  //===========================================================================
  //          Chamber Wall
  //===========================================================================

  //Position, Material and Placement
  G4ThreeVector positionChamberWall = G4ThreeVector(0.,0.,fDetectorLength/2.);
  G4Tubs *SolidChamberWall = new G4Tubs("chamberwall",fDetectorThickness,fDetectorThickness+ChamWallThick,fDetectorLength/2.,0.,twopi);
  G4LogicalVolume *LogicCamberWall = new G4LogicalVolume( SolidChamberWall, fChamberMater, "ChamberWall", 0, 0, 0);
  new G4PVPlacement(0,positionChamberWall,LogicCamberWall,"ChamberWall",lWorld,false,0);

  //===========================================================================
  //          Anode
  //===========================================================================

  //Position, Material and Placement
  fAnodeRadius = 260./2.*mm; // From previous simulation, should be the same as the grid
  fAnodeThickness = 2.*mm; // From previous simulation, should be the same as the grid
  G4ThreeVector positionAnode = G4ThreeVector(0.,0.,0.);
  G4Tubs *SolidAnode = new G4Tubs("anode",0.,fAnodeRadius,fAnodeThickness/2.,0.,twopi);
  fLogicAnode = new G4LogicalVolume( SolidAnode, fAnodeMater, "Anode", 0, 0, 0);
  new G4PVPlacement(0,positionAnode,fLogicAnode,"Anode",fLogicDetector,false,0);


  //===========================================================================
  // Grid holder params:
  fGridHolderThickness1 = 4.5*mm; // Thickness of the grid holder (for lower Grid upper part and upper Grid lower part)
  fGridHolderThickness2 = 2.0*mm; // Thickness of the grid holder (vise versa, 2 mm is just a guess for now)
  fGridHolderIRadius = 260./2.*mm-10.*mm; // Inner radius of the grid holder, from previous simulation
  fGridHolderORadius = 260./2.*mm; // Outer radius of the grid holder, from drawing
  //===========================================================================
  //          Grid1
  //===========================================================================

  // Position and Placement Vectors
  G4ThreeVector positionGrid = G4ThreeVector(0.,0.,fGridToAnodeDist+fAnodeThickness/2.);
  G4ThreeVector offsetHolder1 = G4ThreeVector(0., 0., -fGridHolderThickness1/2.);
  G4ThreeVector offsetHolder2 = G4ThreeVector(0., 0., fGridHolderThickness2/2.);

  // Construction of the grid holders
  G4Tubs *SGridUpHolder1 = new G4Tubs("SGridUpHolder1",fGridHolderIRadius,fGridHolderORadius,fGridHolderThickness1/2.,0.,twopi);
  fLogicGridUpHolder1 = new G4LogicalVolume(SGridUpHolder1, fAnodeMater, "LGridUpHolder1", 0, 0, 0);
  new G4PVPlacement(0, positionGrid+offsetHolder1, fLogicGridUpHolder1, "PGridUpHolder1", fLogicDetector, false, 0);

  G4Tubs *SGridUpHolder2 = new G4Tubs("SGridUpHolder2",fGridHolderIRadius,fGridHolderORadius,fGridHolderThickness2/2.,0.,twopi);
  fLogicGridUpHolder2 = new G4LogicalVolume(SGridUpHolder2, fAnodeMater, "LGridUpHolder2", 0, 0, 0);
  new G4PVPlacement(0, positionGrid+offsetHolder2, fLogicGridUpHolder2, "PGridUpHolder2", fLogicDetector, false, 0);
  //===========================================================================
  //          Grid2
  //===========================================================================

  //Position, Material and Placement
  G4Tubs *SGridDownHolder1 = new G4Tubs("SGridDownHolder1",fGridHolderIRadius,fGridHolderORadius,fGridHolderThickness1/2.,0.,twopi);
  fLogicGridDownHolder1 = new G4LogicalVolume(SGridDownHolder1, fAnodeMater, "LGridDownHolder1", 0, 0, 0);
  new G4PVPlacement(0,-1*(positionGrid+offsetHolder1), fLogicGridDownHolder1, "PGridDownHolder1", fLogicDetector, false, 0);

  G4Tubs *SGridDownHolder2 = new G4Tubs("SGridDownHolder2",fGridHolderIRadius,fGridHolderORadius,fGridHolderThickness2/2.,0.,twopi);
  fLogicGridDownHolder2 = new G4LogicalVolume(SGridDownHolder2, fAnodeMater, "LGridDownHolder2", 0, 0, 0);
  new G4PVPlacement(0,-1*(positionGrid+offsetHolder2), fLogicGridDownHolder2, "PGridDownHolder2", fLogicDetector, false, 0);

  //===========================================================================
  // distance rods between the grid holders
  //===========================================================================
  fRodORadius = 5.*mm; // Outer radius of the distance rods, guessed diameter = 10 mm
  fRodIRadius = 4.*mm; // Inner radius of the distance rods, rod wall thickness ~ 1 mm
  fRodAnodeGridHolder = fGridToAnodeDist - fGridHolderThickness1;
  fRodGridUpHolderTop = fDetectorLength/2. - fAnodeThickness/2. - fGridToAnodeDist - fGridHolderThickness2;
  fRodPlaceRadius = (fGridHolderORadius + fGridHolderIRadius)/2.;
  fShortRodPlaceHeight = fRodAnodeGridHolder/2. + fGridHolderThickness1/2.;

  G4Tubs* SRodShort = new G4Tubs("SRodShort",fRodIRadius,fRodORadius,fRodAnodeGridHolder/2.,0.,twopi);
  G4LogicalVolume* LRodShort = new G4LogicalVolume(SRodShort, fRodMater, "LRodShort", 0, 0, 0);
  for (G4int i = 0; i<3; i++){
    G4double phi = 120.*deg*i;
    G4double x = fRodPlaceRadius*cos(phi);
    G4double y = fRodPlaceRadius*sin(phi);
    G4ThreeVector posRodShortUp = G4ThreeVector(x,y,fAnodeThickness/2. + fRodAnodeGridHolder/2.);
    G4ThreeVector posRodShortDown = G4ThreeVector(x,y,-1*(fAnodeThickness/2. + fRodAnodeGridHolder/2.));
    G4String nameUp = "PRodShortUp" + std::to_string(i);
    G4String nameDown = "PRodShortDown" + std::to_string(i);
    new G4PVPlacement(0,posRodShortUp,LRodShort,nameUp, fLogicDetector, false, 0);
    new G4PVPlacement(0,posRodShortDown,LRodShort,nameDown, fLogicDetector, false, 0);
  }

  /*===========================================
                  SetVisAtributes
  ===========================================*/
  lWorld->SetVisAttributes(G4VisAttributes::GetInvisible());
  fLogicDetector  ->SetVisAttributes(G4Colour(0.0, 0.0 , 0.0, 0.0));  //green for detector
  fLogicTarget    ->SetVisAttributes(G4Colour(1.0, 0.0 , 0.0, 1.0)); //red for target
  LogicCamberBot  ->SetVisAttributes(G4Colour(1.0, 1.0 , 1.0, 0.3));
  LogicCamberTop  ->SetVisAttributes(G4Colour(1.0, 1.0 , 1.0, 0.3));
  LogicCamberWall ->SetVisAttributes(G4Colour(1.0, 1.0 , 1.0, 0.3));
  fLogicGridUpHolder1  ->SetVisAttributes(G4Colour(1.0, 0.55, 0.0, 1.0));
  fLogicGridUpHolder2  ->SetVisAttributes(G4Colour(1.0, 0.0, 0.1, 1.0));
  fLogicGridDownHolder1->SetVisAttributes(G4Colour(1.0, 0.55, 0.0, 1.0));
  fLogicGridDownHolder2->SetVisAttributes(G4Colour(1.0, 0.0, 0.1, 1.0));
  // fLogicGridUp    ->SetVisAttributes(G4Colour(1.0, 0.55, 0.0, 1.0));
  fLogicAnode     ->SetVisAttributes(G4Colour(1.0, 0.55, 0.0, 1.0));

  //Prints the parameters
  PrintParameters();


  //set step size in detector
  // G4double maxStep = 1*mm;
  // fStepLimit = new G4UserLimits(maxStep);

  fLogicDetector->SetUserLimits(fStepLimitDet);
  fLogicTarget->SetUserLimits(fStepLimitTar);

  // G4double maxStepT = 1.*nm;
  // G4UserLimits *StepLimit = new G4UserLimits(maxStepT);
  // fLogicTarget->SetUserLimits(StepLimit);

  //always return the root volume
  return fPhysiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::PrintParameters()
{
  G4cout << "\n Target : Length = " << G4BestUnit(fTargetLength,"Length")
         << " Radius = " << G4BestUnit(fTargetRadius,"Length")
         << " Material = " << fTargetMater->GetName();
  G4cout << "\n Detector : Length = " << G4BestUnit(fDetectorLength,"Length")
         << " Tickness = " << G4BestUnit(fDetectorThickness,"Length")
         << " Material = " << fDetectorMater->GetName() << G4endl;
  G4cout << "\n" << fTargetMater << "\n" << fDetectorMater << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetTargetMaterial(G4String materialChoice)
{

  //Just some protection
  if (materialChoice=="Samarium") {
    fTargetMaterOpt = materialChoice;
  }
  if (materialChoice=="Samarium144") {
    fTargetMaterOpt = materialChoice;
  }
  else if(materialChoice=="Platinum"){
    fTargetMaterOpt = materialChoice;

  }
  else if(materialChoice=="Silicon"){
    fTargetMaterOpt = materialChoice;
  }
  else if(materialChoice=="Bismuth"){
    fTargetMaterOpt = materialChoice;
  }
  else if(materialChoice=="Ytterbium"){
    fTargetMaterOpt = materialChoice;
  }
  else if(materialChoice=="Neodymium"){
    fTargetMaterOpt = materialChoice;
  }
  else if(materialChoice=="Delrin"){
    fTargetMaterOpt = materialChoice;
  }
  else if(materialChoice=="Aluminium"){
    fTargetMaterOpt = materialChoice;
  }
  else if(materialChoice=="Americium"){
    fTargetMaterOpt = materialChoice;
  }
  else if(materialChoice=="Neodymium_Oxide"){
    fTargetMaterOpt = materialChoice;
  }
  else if(materialChoice=="Hafnium"){
    fTargetMaterOpt = materialChoice;
  }
  else if(materialChoice=="Vacuum"){
    fTargetMaterOpt = materialChoice;
  }
  else if(materialChoice=="Hafnium_Oxide"){
    fTargetMaterOpt = materialChoice;
  }
  else if(materialChoice=="P10"){
    fTargetMaterOpt = materialChoice;
  }
  else {
    G4cout << "\n--> warning from DetectorConstruction::SetTargetMaterial : "
           << materialChoice << " not found" << G4endl;
    G4cout << "Using Samarium" << G4endl;
    fTargetMaterOpt = "Samarium";
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetDetectorMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial =
     G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);

  if (pttoMaterial) {
    fDetectorMater = pttoMaterial;
    if(fLogicDetector) { fLogicDetector->SetMaterial(fDetectorMater); }
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
  } else {
    G4cout << "\n--> warning from DetectorConstruction::SetDetectorMaterial : "
           << materialChoice << " not found" << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetTargetRadius(G4double value)
{
  fTargetRadius = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetTargetLength(G4double value)
{
  fTargetLength = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetDetectorThickness(G4double value)
{
  fDetectorThickness = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetDetectorLength(G4double value)
{
  fDetectorLength = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double DetectorConstruction::GetTargetLength()
{
  return fTargetLength;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double DetectorConstruction::GetTargetRadius()
{
  return fTargetRadius;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* DetectorConstruction::GetTargetMaterial()
{
  return fTargetMater;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4LogicalVolume* DetectorConstruction::GetLogicTarget()
{
  return fLogicTarget;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double DetectorConstruction::GetDetectorLength()
{
  return fDetectorLength;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double DetectorConstruction::GetDetectorThickness()
{
  return fDetectorThickness;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* DetectorConstruction::GetDetectorMaterial()
{
  return fDetectorMater;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4LogicalVolume* DetectorConstruction::GetLogicDetector()
{
  return fLogicDetector;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetMaxStepDetector(G4double maxStepDet)
{
  // G4cout << "CLACK!" << maxStep << G4endl;
  // if ((fStepLimit)&&(maxStep>0.)){
  if (maxStepDet>0.){
    fStepLimitDet = new G4UserLimits(maxStepDet);
    // G4cout << "CLICK!" << maxStep << G4endl;
    // fStepLimit->SetMaxAllowedStep(maxStep);
  }
  // if (maxStep>0.) fStepLimit->SetMaxAllowedStep(maxStep);
}

void DetectorConstruction::SetMaxStepTarget(G4double maxStepTar)
{
  if (maxStepTar>0.){
    fStepLimitTar = new G4UserLimits(maxStepTar);
  }

}

void DetectorConstruction::SetTargetX(G4double val){
  fTarX = val;
}

void DetectorConstruction::SetTargetY(G4double val){
  fTarY = val;
}

void DetectorConstruction::SetTargetZ(G4double val){
  fTarZ = val;
}

G4double DetectorConstruction::GetTargetX(){
  return fTarX;
}

G4double DetectorConstruction::GetTargetY(){
  return fTarY;
}

G4double DetectorConstruction::GetTargetZ(){
  return fTarZ;
}

G4String DetectorConstruction::GetTargetShape(){
  return fTargetShape;
}

void DetectorConstruction::SetTargetShape(G4String val){
    fTargetShape = val;
}

//void DetectorConstruction::SetCoverThickness(G4double val){
//  fCoverLength = val;
//}

G4LogicalVolume* DetectorConstruction::GetLogicAnode(){
  return fLogicAnode;
}

// Commented out as currently the grid is not installed
//G4LogicalVolume* DetectorConstruction::GetLogicGridUp(){
//  return fLogicGridUp;
//}

//G4LogicalVolume* DetectorConstruction::GetLogicGridDo(){
//  return fLogicGridDo;
//}

G4double DetectorConstruction::GetGridToAnodeDist(){
  return fGridToAnodeDist;
}
