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


TH1D* GetCalibratedHist(TTree* chain, double slope) {
    TH1D* h1_ph_anode_cal = new TH1D("h1_ph_anode_cal", " ; Energy (keV); Counts / bin", 2000, 0, 20);
    chain->Draw(Form("PulseHeightAnode*%f>>h1_ph_anode_cal", slope), "IsLowerChamber==1 && IsSlowPileupAnode==0 && IsFastPileupAnode==0", "goff");
    return h1_ph_anode_cal;
}

void GetPositionHistogram(TTree* tree, string name) {
    double x_max =  11;
    double y_max =  11;
    double z_min =   3;
    double z_max = 3.5;
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

double GetScalingFactor(TH1D* hist_sim, TH1D* hist_data, double min = 4, double max = 6) {
    int sim_bin_min = hist_sim->FindBin(min);
    int sim_bin_max = hist_sim->FindBin(max);

    int data_bin_min = hist_data->FindBin(min);
    int data_bin_max = hist_data->FindBin(max);

    double integral_sim = hist_sim->Integral(sim_bin_min, sim_bin_max);
    double integral_data = hist_data->Integral(data_bin_min, data_bin_max);
    cout << "Integral of simulation histogram in range [" << min << ", " << max << "] MeV: " << integral_sim << endl;
    cout << "Integral of data histogram in range [" << min << ", " << max << "] MeV: " << integral_data << endl;
    if (integral_sim == 0) {
        cout << "Warning: Integral of simulation histogram is zero in the specified range." << endl;
        return 0;
    }
    return integral_data / integral_sim;
}

void GetHistCalibration(){
    string nuclide1 = "241Am";
    string file_name1 = "../results/run241Am_1um.root";

    string file_name2 = "../results/run241Am_100nm.root";


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
    hist1->SetName("hist_Am_1um");

    TFile* sim_file2 = new TFile(file_name2.c_str(), "READ");
    if (!sim_file2 || sim_file2->IsZombie()) {
        cout << "Error opening file: " << file_name2 << endl;
        return;
    }
    TH1D* hist2 = (TH1D*)sim_file2->Get("H11");
    if (!hist2) {
        cout << "Histogram H11 not found in file " << file_name2 << endl;
        return;
    }
    hist2->SetName("hist_Am_100nm");
    TF1* fwhm_fit = new TF1("fwhm_fit", "[0] + [1]*x", 0, 10);
    fwhm_fit->SetParameters(0.0121, 0.0128); //
    TH1D* hist1_smeard = GetSmearedHist(hist1, fwhm_fit);
    TH1D* hist2_smeard = GetSmearedHist(hist2, fwhm_fit);

    // Get Calibration Data:

    string data_filename = "Run001_820VLG_1225VA_Proc.root";
    string path = "/SHARE/raw/projects/np_alpha/processed/20260415/";

    TFile* data_file = new TFile((path + data_filename).c_str(), "READ");
    if (!data_file || data_file->IsZombie()) {
        cout << "Error opening file: " << (path + data_filename) << endl;
        return;
    }
    TTree* data_tree = (TTree*)data_file->Get("DataTreeProcessed");
    if (!data_tree) {
        cout << "Tree 'DataTreeProcessed' not found in file " << (path + data_filename) << endl;
        return;
    }
    double slope = 0.00119375; // Calibration slope from previous analysis
    TH1D* data_hist = GetCalibratedHist(data_tree, slope);

    // Get scaling factors:
    double scaling_1um = GetScalingFactor(hist1_smeard, data_hist, 4, 6);
    double scaling_100nm = GetScalingFactor(hist2_smeard, data_hist, 4, 6);
    cout << "Scaling factor for 1um: " << scaling_1um << endl;
    cout << "Scaling factor for 100nm: " << scaling_100nm << endl;

    hist1->Scale(scaling_1um);
    hist1_smeard->Scale(scaling_1um);
    hist2->Scale(scaling_100nm);
    hist2_smeard->Scale(scaling_100nm);


    TCanvas* c1 = new TCanvas("c1", "Calibration Histogram", 1200, 600);
    hist2->SetLineColor(kGreen);
    hist2->SetLineWidth(2);
    hist2->SetStats(0);
    hist2->Draw("HIST");
    hist2_smeard->SetLineColor(kGreen+2);
    hist2_smeard->SetLineWidth(2);
    hist2_smeard->Draw("HIST SAME");

    hist1->SetLineColor(kBlue);
    hist1->SetLineWidth(2);
    hist1->GetXaxis()->SetTitle("Energy (MeV)");
    hist1->GetYaxis()->SetTitle("Counts");
    hist1->Draw("HIST SAME");
    hist1_smeard->SetLineColor(kBlue+2);
    hist1_smeard->SetLineWidth(2);
    hist1_smeard->Draw("HIST SAME");

    data_hist->SetLineColor(kRed);
    data_hist->SetLineWidth(2);
    data_hist->Draw("HIST SAME");
    
    TLegend* legend = new TLegend(0.7, 0.7, 0.9, 0.9);
    legend->AddEntry(hist1, "Original Histogram", "l");
    legend->AddEntry(hist1_smeard, "Smeared Histogram", "l");
    legend->AddEntry(hist2, "Original Histogram (100nm)", "l");
    legend->AddEntry(hist2_smeard, "Smeared Histogram (100nm)", "l");
    legend->Draw();
}