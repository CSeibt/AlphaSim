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

#include "GetHistAll.cpp"

void GetPositionHistogram(TTree* tree, string name) {
    double x_max = 11;
    double y_max = 11;
    double z_min = 2;
    double z_max = 3;
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

void GetHistCalibration(){
    string nuclide1 = "241Am";
    string file_name1 = "../results/run241Am_1um.root";

    TFile* sim_file = new TFile(file_name1.c_str(), "READ");
    if (!sim_file || sim_file->IsZombie()) {
        cout << "Error opening file: " << file_name1 << endl;
        return;
    }
    TTree* parent_tree = (TTree*)sim_file->Get("ParentPos");
    if (!parent_tree) {
        cout << "Tree ParentPos not found in file " << file_name1 << endl;
        return;
    }
    GetPositionHistogram(parent_tree, "ParentPos");
    TH1D* hist1 = (TH1D*)sim_file->Get("H11");
    if (!hist1) {
        cout << "Histogram H11 not found in file " << file_name1 << endl;
        return;
    }
    TF1* fwhm_fit = new TF1("fwhm_fit", "[0] + [1]*x", 0, 10);
    fwhm_fit->SetParameters(0.0121, 0.0128); //
    TH1D* hist1_smeard = GetSmearedHist(hist1, fwhm_fit);
    TCanvas* c1 = new TCanvas("c1", "Calibration Histogram", 1200, 600);
    hist1->SetLineColor(kBlue);
    hist1->SetLineWidth(2);
    hist1->GetXaxis()->SetTitle("Energy (MeV)");
    hist1->GetYaxis()->SetTitle("Counts");
    hist1->Draw("HIST");
    hist1_smeard->SetLineColor(kRed);
    hist1_smeard->SetLineWidth(2);
    hist1_smeard->Draw("HIST SAME");
    TLegend* legend = new TLegend(0.7, 0.7, 0.9, 0.9);
    legend->AddEntry(hist1, "Original Histogram", "l");
    legend->AddEntry(hist1_smeard, "Smeared Histogram", "l");
    legend->Draw();
}