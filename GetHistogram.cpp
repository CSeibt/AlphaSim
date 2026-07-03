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

void GetHistogram() {
    TString file_name = "../alpha_build/run238U.root";
    TString histname = "H11";

    TFile *file = new TFile(file_name, "READ");
    TH1D* hist = (TH1D*)file->Get(histname);
    if (!hist) {
        cout << "Histogram " << histname << " not found in file " << file_name << endl;
        return;
    }
    TCanvas* canvas = new TCanvas("canvas", "Histogram", 1200, 600);
    hist->SetLineColor(kBlue);
    hist->SetStats(0);
    hist->SetTitle("Energy Deposited in Detector;Energy [MeV];Counts per 10 keV");
    hist->Draw("hist");

}