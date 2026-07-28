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

TH1D* GetHistFromFile(string nuclide, string file_path, TFile* output_file, string hist_name="H11") {
    TString file_name = file_path + "/run" + nuclide + "_10nm.root";
    TFile *file = new TFile(file_name, "READ");
    if (!file || file->IsZombie()) {
        cout << "Error opening file: " << file_name << endl;
        return nullptr;
    }
    TH1D* hist = (TH1D*)file->Get(hist_name.c_str());
    if (!hist) {
        cout << "Histogram " << hist_name << " not found in file " << file_name << endl;
        return nullptr;
    }
    output_file->cd();
    TH1D* hist_clone = (TH1D*)hist->Clone(("hist_" + nuclide).c_str());
    hist_clone->Write();
    file->Close();
    delete file;
    return hist_clone;
}

TH1D* GetSmearedHist(TH1D* hist, TF1* fwhm_fit) {
    TRandom3* gRandom = new TRandom3();
    TH1D* hist_smeared = (TH1D*)hist->Clone((string(hist->GetName()) + "_smeared").c_str());
    hist_smeared->Reset();
    for (int i = 1; i <= hist->GetNbinsX(); ++i) {
        double energy = hist->GetBinCenter(i);
        double counts = hist->GetBinContent(i);
        if (counts > 0) {
            double fwhm = fwhm_fit->Eval(energy);
            double sigma = fwhm / 2.355; // Convert FWHM to sigma
            for (int j = 0; j < counts; ++j) {
                double smeared_energy = gRandom->Gaus(energy, sigma);
                hist_smeared->Fill(smeared_energy);
            }
        }
    }
    return hist_smeared;
}

void GetHistAll() {
    //vector<string> nuclides = {"238U", "234U", "230Th", "226Ra", "222Rn", "218Po", "214Po", "210Po"};
    vector<string> nuclides = {"238U", "226Ra", "210Po"};
    vector<TH1D*> histograms;
    vector<TH1D*> smeared_histograms;
    TFile* output_file = new TFile("all_238Uchain_10nm.root", "RECREATE");
    TF1* fwhm_fit = new TF1("fwhm_fit", "[0] + [1]*x", 0, 10);
    fwhm_fit->SetParameters(0.0121, 0.0128); //
    string file_path = "../results";
    for (const auto& nuclide : nuclides) {
        TH1D* hist = GetHistFromFile(nuclide, file_path, output_file);
        if (hist) {
            histograms.push_back(hist);
            TH1D* smeared_hist = GetSmearedHist(hist, fwhm_fit);
            if (smeared_hist) {
                smeared_histograms.push_back(smeared_hist);
            }
        }
    }
    cout << "Total histograms retrieved: " << histograms.size() << endl;
    cout << "First histogram: " << histograms[0] << endl;
    for (int i = 0; i < histograms.size(); ++i) {
        cout << "Drawing histogram for " << nuclides[i] << endl;
        TCanvas* canvas = new TCanvas(("canvas_" + to_string(i)).c_str(), ("Histogram for " + nuclides[i]).c_str(), 1200, 600);
        histograms[i]->SetLineColor(kBlue);
        histograms[i]->SetStats(0);
        histograms[i]->SetTitle(("Energy Deposited in Detector for " + nuclides[i] + ";Energy [MeV];Counts per 10 keV").c_str());
        histograms[i]->Draw("hist");
    }
    for (int i = 0; i < smeared_histograms.size(); ++i) {
        cout << "Drawing smeared histogram for " << nuclides[i] << endl;
        TCanvas* canvas = new TCanvas(("canvas_smeared_" + to_string(i)).c_str(), ("Smeared Histogram for " + nuclides[i]).c_str(), 1200, 600);
        smeared_histograms[i]->SetLineColor(kRed);
        smeared_histograms[i]->SetStats(0);
        smeared_histograms[i]->SetTitle(("Smeared Energy Deposited in Detector for " + nuclides[i] + ";Energy [MeV];Counts per 10 keV").c_str());
        smeared_histograms[i]->Draw("hist");
    }
}