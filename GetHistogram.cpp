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

void GetPositionHistogram(TTree* tree, string name) {
    double x_max = 120;
    double y_max = 120;
    double z_min = 1;
    double z_max = 2;
    TH2D* h2_x_y = new TH2D((name + "_h2_x_y").c_str(), (name + "_h2_x_y").c_str(), 120, -x_max, x_max, 120, -y_max, y_max);
    TH2D* h2_x_z = new TH2D((name + "_h2_x_z").c_str(), (name + "_h2_x_z").c_str(), 120, -x_max, x_max, 90, z_min, z_max);
    TH2D* h2_y_z = new TH2D((name + "_h2_y_z").c_str(), (name + "_h2_y_z").c_str(), 120, -y_max, y_max, 90, z_min, z_max);
    TH3D* h3_xyz = new TH3D((name + "_h3_xyz").c_str(), (name + "_h3_xyz").c_str(), 120, -x_max, x_max, 120, -y_max, y_max, 90, z_min, z_max);
    tree->Draw(("y:x>>" + name + "_h2_x_y").c_str(), "", "goff");
    tree->Draw(("z:x>>" + name + "_h2_x_z").c_str(), "", "goff");
    tree->Draw(("z:y>>" + name + "_h2_y_z").c_str(), "", "goff");
    tree->Draw(("z:y:x>>" + name + "_h3_xyz").c_str(), "", "goff");

    TCanvas* c1 = new TCanvas((name + "_c1").c_str(), (name + "_c1").c_str(), 1200, 1200);
    c1->Divide(2, 2);
    c1->cd(1);
    gPad->SetRightMargin(0.15);
    h2_x_y->GetZaxis()->SetTitleOffset(1.4);
    h2_x_y->SetTitle("x-y");
    h2_x_y->GetXaxis()->SetTitle("x [mm]");
    h2_x_y->GetYaxis()->SetTitle("y [mm]");
    h2_x_y->Draw("colz");
    c1->cd(2);
    gPad->SetRightMargin(0.15);
    h2_x_z->GetZaxis()->SetTitleOffset(1.4);
    h2_x_z->SetTitle("x-z");
    h2_x_z->GetXaxis()->SetTitle("x [mm]");
    h2_x_z->GetYaxis()->SetTitle("z [mm]");
    h2_x_z->Draw("colz");
    c1->cd(3);
    gPad->SetRightMargin(0.15);
    h2_y_z->GetZaxis()->SetTitleOffset(1.4);
    h2_y_z->SetTitle("y-z");
    h2_y_z->GetXaxis()->SetTitle("y [mm]");
    h2_y_z->GetYaxis()->SetTitle("z [mm]");
    h2_y_z->Draw("colz");
    c1->cd(4);
    gPad->SetRightMargin(0.15);
    h3_xyz->GetZaxis()->SetTitleOffset(1.4);
    h3_xyz->SetTitle("x-y-z");
    h3_xyz->GetXaxis()->SetTitle("x [mm]");
    h3_xyz->GetYaxis()->SetTitle("y [mm]");
    h3_xyz->GetZaxis()->SetTitle("z [mm]");
    h3_xyz->Draw("box");
    //c1->SaveAs((name + "_position.png").c_str());
}


void GetEfficiency(TH1D* hist, Long64_t n_events, double Emin, double Emax) {
    int bin_min = hist->FindBin(Emin);
    int bin_max = hist->FindBin(Emax);
    double counts = hist->Integral(bin_min, bin_max);
    double efficiency = counts / static_cast<double>(n_events);
    cout << "Efficiency between " << Emin << " MeV and " << Emax << " MeV: " << efficiency << endl;
}

void GetHistogram() {
    TString file_name = "../alpha_build/run238U_full_chain.root";
    TString histname = "H11";
    TString parent_tree_name = "ParentPos";


    TFile *file = new TFile(file_name, "READ");
    TH1D* hist = (TH1D*)file->Get(histname);
    if (!hist) {
        cout << "Histogram " << histname << " not found in file " << file_name << endl;
        return;
    }
    TTree* parent_tree = (TTree*)file->Get(parent_tree_name);
    if (!parent_tree) {
        cout << "Tree " << parent_tree_name << " not found in file " << file_name << endl;
        return;
    }
    Long64_t n_events = parent_tree->GetEntries();
    GetPositionHistogram(parent_tree, "ParentPos");
    double E_238U = 4.20;
    double E_Min_238U = E_238U - 0.84;
    double E_Max_238U = E_238U + 0.07;

    TCanvas* canvas = new TCanvas("canvas", "Histogram", 1200, 600);
    hist->SetLineColor(kBlue);
    hist->SetStats(0);
    hist->SetTitle("Energy Deposited in Detector;Energy [MeV];Counts per 10 keV");
    hist->Draw("hist");

}