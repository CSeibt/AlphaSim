#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <dirent.h>
#include <algorithm>
#include <cmath>
#include "TString.h"
#include "TFile.h"
#include "TF1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TGraph.h"
#include "TMultiGraph.h"

using namespace std;

TH1D* GetEnergyInDetector(TTree* tree, TTreeIndex* index, int verbose = 0) {
    TH1D* hist = new TH1D("EnergyInDetector", "Energy Deposited in Detector;Energy (MeV);Counts", 1000, 0, 10);
    Double_t EventID, TrackID, Energy, VolumeID, x, y, z;
    tree->SetBranchAddress("EventID", &EventID);
    tree->SetBranchAddress("TrackID", &TrackID);
    tree->SetBranchAddress("Energy", &Energy);
    tree->SetBranchAddress("VolumeID", &VolumeID);
    tree->SetBranchAddress("x", &x);
    tree->SetBranchAddress("y", &y);
    tree->SetBranchAddress("z", &z);

    Long64_t* index_list = index->GetIndex();
    Long64_t nEntries = tree->GetEntries();

    Double_t current_event_id = -1;
    Double_t current_energy_sum = 0.0;

    for (Long64_t i = 0; i < nEntries; ++i) {
        tree->GetEntry(index_list[i]);
        if (EventID != current_event_id) {
            if (verbose > 0) {cout << "Current Event ID: " << EventID << ", finished Event ID: " << current_event_id << ", Energy Sum: " << current_energy_sum << " MeV" << endl;}
            if (current_energy_sum > 0) {hist->Fill(current_energy_sum);}
            current_event_id = EventID;
            current_energy_sum = 0.0;
        }
        if (VolumeID == 2) { // VolumeID 2 corresponds to the detector
            current_energy_sum += Energy;
        }
    }
    hist->Fill(current_energy_sum); // Fill the last event's energy sum
    return hist;
}

void EvalEdepTree() {
    TString file_name = "../alpha_build/run238U.root";
    TString tree_name = "EDeps";

    TFile *file = new TFile(file_name, "READ");
    TTree *tree = (TTree*)file->Get(tree_name);
    
    Long64_t nEntries = tree->GetEntries();
    cout << "Number of entries: " << nEntries << endl;
    Double_t EventID, TrackID, Energy, VolumeID, x, y, z;
    tree->SetBranchAddress("EventID", &EventID);
    tree->SetBranchAddress("TrackID", &TrackID);
    tree->SetBranchAddress("Energy", &Energy);
    tree->SetBranchAddress("VolumeID", &VolumeID);
    tree->SetBranchAddress("x", &x);
    tree->SetBranchAddress("y", &y);
    tree->SetBranchAddress("z", &z);
    tree->BuildIndex("EventID");
    TTreeIndex* index = (TTreeIndex*)tree->GetTreeIndex();
    if (!index) {
        cerr << "Error: Tree index not found!" << endl;
        file->Close();
        return;
    }

    TH1D* energy_hist = GetEnergyInDetector(tree, index);
    TCanvas* canvas = new TCanvas("canvas", "Energy Deposited in Detector", 1200, 600);
    energy_hist->Draw();


    //delete file;
}