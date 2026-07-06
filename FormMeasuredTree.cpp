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
#include "TTreeIndex.h"

using namespace std;

TTree* GetMeasuredTree(TTree* edep_tree, int verbose = 0, double time_window = 1e-4) { // 100 mus default time window
    if (!edep_tree) {
        cerr << "Error: input tree is null" << endl;
        return nullptr;
    }

    // Constuct TreeIndex:
    edep_tree->BuildIndex("EventID", "Time");
    TTreeIndex* index = (TTreeIndex*)edep_tree->GetTreeIndex();
    if (!index) {
        cerr << "Error: failed to build tree index" << endl;
        return nullptr;
    }
    Long64_t* index_list = index->GetIndex();
    if (!index_list) {
        cerr << "Error: tree index array is null" << endl;
        return nullptr;
    }
    Long64_t nEntries = edep_tree->GetEntries();
    
    // Create a new tree to store measured energy deposits
    Double_t EventID, TrackID, Energy, VolumeID, Time, x, y, z;
    edep_tree->SetBranchAddress("EventID", &EventID);
    edep_tree->SetBranchAddress("TrackID", &TrackID);
    edep_tree->SetBranchAddress("Energy", &Energy);
    edep_tree->SetBranchAddress("VolumeID", &VolumeID);
    edep_tree->SetBranchAddress("Time", &Time);
    edep_tree->SetBranchAddress("x", &x);
    edep_tree->SetBranchAddress("y", &y);
    edep_tree->SetBranchAddress("z", &z);

    double energy_target = 0.0;
    double energy_detector = 0.0;
    double current_event_id = -1;
    double current_time = -1.0;

    TTree* measured_tree = new TTree("Measured", "Measured Energy Deposits");
    measured_tree->Branch("EventID", &current_event_id, "EventID/D");
    measured_tree->Branch("Time", &current_time, "Time/D");
    measured_tree->Branch("EnergyTarget", &energy_target, "EnergyTarget/D");
    measured_tree->Branch("EnergyDetector", &energy_detector, "EnergyDetector/D");
    int percentage = 0;
    for (Long64_t i = 0; i < nEntries; ++i) {
        if (verbose > 0) {
            if (i == int(nEntries * percentage / 100)) {
                cout << "Processing entry " << i << " of " << nEntries << " (" << percentage << "%)" << endl;
                percentage++;
            }
        }
        edep_tree->GetEntry(index_list[i]);
        if (EventID != current_event_id) {
            // New event, save the previous event's data
            if (current_event_id != -1) {
                measured_tree->Fill();
            }
            current_event_id = EventID;
            current_time = Time;
            energy_target = 0.0;
            energy_detector = 0.0;
        }
        else if (fabs(Time - current_time) > time_window) {
            // Time window exceeded, save the previous event's data and start a new time window
            measured_tree->Fill();
            current_time = Time;
            energy_target = 0.0;
            energy_detector = 0.0;
        }
        // Accumulate energy deposits within the time window
        if (VolumeID == 1) { // VolumeID 1 corresponds to the target
            energy_target += Energy;
        } else if (VolumeID == 2) { // VolumeID 2 corresponds to the detector
            energy_detector += Energy;
        }
    }
    if (current_event_id != -1) {
        measured_tree->Fill(); // Fill the last event's data
    }

    return measured_tree;
}


int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <input_file.root> " << "verbose" << endl;
        return 1;
    }

    TString input_file = argv[1];
    int verbose = 0;
    if (argc == 3) {
        verbose = atoi(argv[2]);
    }
    TFile* file = new TFile(input_file, "UPDATE");
    if (!file || file->IsZombie()) {
        cout << "Error opening file: " << input_file << endl;
        return 1;
    }

    TTree* tree = (TTree*)file->Get("EDeps");
    if (!tree) {
        cout << "Error: Tree 'EDeps' not found in file: " << input_file << endl;
        return 1;
    }
    TTree* measured_tree = GetMeasuredTree(tree, verbose, 1e-4); // 100 mus time window
    if (!measured_tree) {
        file->Close();
        delete file;
        return 1;
    }
    file->cd();
    measured_tree->Write("Measured", TObject::kOverwrite);
    file->Close();
    delete file;

    return 0;
}