#include <iostream>
#include <algorithm>
#include "TFile.h"
#include "TH1F.h"
#include "TTree.h"
#include "TCanvas.h"

Float_t ctau_reweighter(Float_t t, Float_t tau0, Float_t tau1) 
{
    Float_t numerator = (1.0f / tau1) * expf(-t / tau1);
    Float_t denominator = (1.0f / tau0) * expf(-t / tau0);
    return numerator / denominator;

}

void read() {
    // Open the ROOT file
    TFile *file1 = TFile::Open("BToKPhi_MuonLLPDecayGenFilter_PhiToPi0Pi0_mPhi1p0_ctau300.root");
    TFile *file2 = TFile::Open("BToKPhi_MuonLLPDecayGenFilter_PhiToPi0Pi0_mPhi1p0_ctau1000.root");
    
    
    if (!file1 || file1->IsZombie()) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }
    if (!file2 || file2->IsZombie()) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }

    // Get the TTree from the file
    TTree *tree_300 = nullptr;
    file1->GetObject("MuonSystem", tree_300);
    if (!tree_300) {
        std::cerr << "Error retrieving MuonSystem TTree" << std::endl;
        file1->Close();
        return;
    }
    TTree *tree_1000 = nullptr;
    file2->GetObject("MuonSystem", tree_1000);
    if (!tree_1000) {
        std::cerr << "Error retrieving MuonSystem TTree" << std::endl;
        file2->Close();
        return;
    }

    // set your binning
    TH1F *h_gLLP_ctau_300 = new TH1F("h_gLLP_ctau_300", "h_gLLP_ctau_300", 1000, 0, 1000);
    TH1F *h_gLLP_ctau_1000 = new TH1F("h_gLLP_ctau_1000", "h_gLLP_ctau_1000", 1000, 0, 1000);
    //try more bins
    

    // Set branch address
    float gLLP_ctau_300;
    float gLLP_ctau_1000;
    
    tree_300->SetBranchAddress("gLLP_ctau", &gLLP_ctau_300);
    tree_1000->SetBranchAddress("gLLP_ctau", &gLLP_ctau_1000);

    // Loop over the tree entries and fill the histogram
    // Long64_t nEntries_300 = tree_300->GetEntries();
    // float w = 1.0;//weight
    // for (Long64_t i = 0; i < nEntries_300; i++) {
    //     tree_300->GetEntry(i);
    //     h_gLLP_ctau_300->Fill(gLLP_ctau_300, w);
    // }
    //h_gLLP_ctau_300->Scale(1./h_gLLP_ctau_300->Integral(0,-1));
    Long64_t nEntries_300 = tree_300->GetEntries();

    for (Long64_t i = 0; i < nEntries_300; i++) {
        tree_300->GetEntry(i);
        float w = 1.0;
        float reweight_factor = ctau_reweighter(gLLP_ctau_300, 30, 20);
        w *= reweight_factor;
        h_gLLP_ctau_300->Fill(gLLP_ctau_300, w);
    }
    h_gLLP_ctau_300->Scale(1./h_gLLP_ctau_300->Integral(0,-1)); 
    

    Long64_t nEntries_1000 = tree_1000->GetEntries();
    for (Long64_t i = 0; i < nEntries_1000; i++) {
        tree_1000->GetEntry(i);
        float w = 1.0;
        float reweight_factor = ctau_reweighter(gLLP_ctau_1000, 100, 90);
        h_gLLP_ctau_1000->Fill(gLLP_ctau_1000, w);

    }
    h_gLLP_ctau_1000->Scale(1./h_gLLP_ctau_1000->Integral(0,-1));




    float ratio_closest_to_1 = h_gLLP_ctau_300->GetBinContent(1)/h_gLLP_ctau_1000->GetBinContent(1);
    int NumBin = 1;
    int binNum_300 = h_gLLP_ctau_300->GetNbinsX();
    for (int i = 1; i<= binNum_300; i++)
    {
        if(h_gLLP_ctau_1000->GetBinContent(i) != 0)
        {
            if (abs(1-(h_gLLP_ctau_300->GetBinContent(i)/h_gLLP_ctau_1000->GetBinContent(i))) < abs(1-ratio_closest_to_1))
            {
            ratio_closest_to_1 = h_gLLP_ctau_300->GetBinContent(i)/h_gLLP_ctau_1000->GetBinContent(i);
            NumBin = i;
            }
        }
        
    }
    float x_cor = h_gLLP_ctau_300->GetBinCenter(NumBin);
    std::cout<<"x coordinate of the intersection is: " << x_cor << " with ratio: " << ratio_closest_to_1<< std::endl;
    std::cout<<"The bin number is: " << NumBin << endl;



    // For Plotting
    //combined graph
    // TCanvas *c1 = new TCanvas("c1", "gLLP_ctau Distribution", 800, 600);
    // h_gLLP_ctau_300->SetLineColor(kBlue);
    // h_gLLP_ctau_1000->SetLineColor(kRed);
    // h_gLLP_ctau_300->Draw();
    // h_gLLP_ctau_1000->Draw("same"); 

    // TLatex *text = new TLatex();
    // text->SetNDC();
    // text->SetTextSize(0.04);
    // text->SetTextAlign(22); 

        // Convert float to string
    // std::ostringstream oss_x;
    // std::ostringstream oss_ratio;
    // oss_x << x_cor;
    // oss_ratio << ratio_closest_to_1;
    // std::string x_cor_str = oss_x.str();
    // std::string ratio_str = oss_ratio.str();
    // std::string annotation = "x-intersection " + x_cor_str + " ratio: " + ratio_str;
    // text->DrawLatex(0.5, 0.8, annotation.c_str());
    // c1->SaveAs("h_gLLP_PhiToPi0Pi0_ctau_combined.png");

    //seperate histograms
    TCanvas *c2 = new TCanvas("c2", "gLLP_ctau Distribution", 800, 600);
    h_gLLP_ctau_300->Draw();
    
    TCanvas *c3 = new TCanvas("c3", "gLLP_ctau Distribution", 800, 600);
    h_gLLP_ctau_1000->Draw();


    c2->SaveAs("reweight200_h_gLLP_PhiToPi0Pi0_ctau300.png");
    c3->SaveAs("reweight900_h_gLLP_PhiToPi0Pi0_ctau1000.png");

    // c3->SaveAs("h_gLLP_PhiToPi0Pi0_ctau1000.png");

    // Close the ROOT file
    file1->Close();
    file2->Close();
}




