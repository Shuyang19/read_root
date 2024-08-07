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
if(denominator == 0.0f)
{
std::cout << "warning" << std::endl;
}    
return numerator / denominator;

}

void read() {
    // Open the ROOT file
    //TFile *file1 = TFile::Open("BToKPhi_MuonLLPDecayGenFilter_PhiToPi0Pi0_mPhi0p5_ctau500.root");
    //std::cout << "check point 1" << std::endl;    
    TFile *file2 = TFile::Open("BToKPhi_MuonLLPDecayGenFilter_PhiToPi0Pi0_mPhi0p5_ctau5000.root");
    // std::cout << "check point 2" << std::endl;    

    
    // if (!file1 || file1->IsZombie()) {
    //     std::cerr << "Error opening file" << std::endl;
    //     return;
    // }
    if (!file2 || file2->IsZombie()) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }

    // Get the TTree from the file
   //std::cout <<"check point 3"<< std::endl; 
    // TTree *tree_500 = nullptr;
    // file1->GetObject("MuonSystem", tree_500);
    // if (!tree_500) {
    //     std::cerr << "Error retrieving MuonSystem TTree" << std::endl;
    //     file1->Close();
    //     return;
    // }
   //std::cout << "check point 4" << std::endl;
    TTree *tree_5000 = nullptr;
    file2->GetObject("MuonSystem", tree_5000);
    if (!tree_5000) {
        std::cerr << "Error retrieving MuonSystem TTree" << std::endl;
        file2->Close();
        return;
    }

    // set your binning
    //std::cout << "check point 5" << std::endl;
    // TH1F *h_gLLP_ctau_500 = new TH1F("h_gLLP_ctau_500", "h_gLLP_ctau_500", 1000, 0, 1000);
    // TH1F *h_gLLP_ctau_500_reweighted = new TH1F("h_gLLP_ctau_500_reweighted", "h_gLLP_ctau_500_reweighted", 1000, 0, 1000);
    TH1F *h_gLLP_ctau_5000 = new TH1F("h_gLLP_ctau_5000", "h_gLLP_ctau_5000", 1000, 0, 1000);
    TH1F *h_gLLP_ctau_5000_reweighted = new TH1F("h_gLLP_ctau_5000_reweighted", "h_gLLP_ctau_5000_reweighted", 1000, 0, 1000);
    
    //std::cout << "check point 6" << std::endl;
    //try more bins
    

    // Set branch address
    //float gLLP_ctau_500;
    float gLLP_ctau_5000;
    
    //tree_500->SetBranchAddress("gLLP_ctau", &gLLP_ctau_500);
    tree_5000->SetBranchAddress("gLLP_ctau", &gLLP_ctau_5000);

    //std::cout << "check point 7" << std::endl;
    // Loop over the tree entries and fill the histogram
    // Long64_t nEntries_500 = tree_500->GetEntries();
    // //std::cout <<"number of entries"<<nEntries_300<<std::endl;
    // for (Long64_t i = 0; i < nEntries_500; i++) {
    //     float w = 1.0;
    //     tree_500->GetEntry(i);
    //     h_gLLP_ctau_500->Fill(gLLP_ctau_500, w);
    // }
    // //std::cout << "fill the graph" << std::endl;    
    // h_gLLP_ctau_500->Scale(1./h_gLLP_ctau_500->Integral(0,-1));

    // for (Long64_t i = 0; i < nEntries_500; i++) {
    //     tree_500->GetEntry(i);
    //     float w = 1.0;
    //     float reweight_factor = ctau_reweighter(gLLP_ctau_500, 50, 500);
    //     w *= reweight_factor;
    //     h_gLLP_ctau_500_reweighted->Fill(gLLP_ctau_500, w);
    // }
    // h_gLLP_ctau_500_reweighted->Scale(1./h_gLLP_ctau_500_reweighted->Integral(0,-1)); 
    

    Long64_t nEntries_5000 = tree_5000->GetEntries();

    for (Long64_t i = 0; i < nEntries_5000; i++) {
        float w = 1.0;
        tree_5000->GetEntry(i);
        h_gLLP_ctau_5000->Fill(gLLP_ctau_5000, w);
        
    }
    h_gLLP_ctau_5000->Scale(1./h_gLLP_ctau_5000->Integral(0,-1));

    for (Long64_t i = 0; i < nEntries_5000; i++) {
        tree_5000->GetEntry(i);
        float w = 1.0;
        float reweight_factor = ctau_reweighter(gLLP_ctau_5000, 500, 50);
	w *=reweight_factor;        
h_gLLP_ctau_5000_reweighted->Fill(gLLP_ctau_5000, w);

    }
    h_gLLP_ctau_5000_reweighted->Scale(1./h_gLLP_ctau_5000->Integral(0,-1));




    // float ratio_closest_to_1 = h_gLLP_ctau_300->GetBinContent(1)/h_gLLP_ctau_1000->GetBinContent(1);
    // int NumBin = 1;
    // int binNum_300 = h_gLLP_ctau_300->GetNbinsX();
    // for (int i = 1; i<= binNum_300; i++)
    // {
    //     if(h_gLLP_ctau_1000->GetBinContent(i) != 0)
    //     {
    //         if (abs(1-(h_gLLP_ctau_300->GetBinContent(i)/h_gLLP_ctau_1000->GetBinContent(i))) < abs(1-ratio_closest_to_1))
    //         {
    //         ratio_closest_to_1 = h_gLLP_ctau_300->GetBinContent(i)/h_gLLP_ctau_1000->GetBinContent(i);
    //         NumBin = i;
    //         }
    //     }
        
    // }
    // float x_cor = h_gLLP_ctau_300->GetBinCenter(NumBin);
    // std::cout<<"x coordinate of the intersection is: " << x_cor << " with ratio: " << ratio_closest_to_1<< std::endl;
    // std::cout<<"The bin number is: " << NumBin << endl;



    // For Plotting
    //combined graph
    // TCanvas *c1 = new TCanvas("c1", "gLLP_ctau Distribution", 800, 600);
    // h_gLLP_ctau_500->SetLineColor(kBlue);
    // h_gLLP_ctau_500_reweighted->SetLineColor(kRed);
    // h_gLLP_ctau_500->Draw();
    // h_gLLP_ctau_500_reweighted->Draw("same"); 

    // TLatex *text = new TLatex();
    // text->SetNDC();
    // text->SetTextSize(0.04);
    // text->SetTextAlign(22); 
    TCanvas *c2 = new TCanvas("c2", "gLLP_ctau Distribution", 800, 600);
    h_gLLP_ctau_5000->SetLineColor(kBlue);
    h_gLLP_ctau_5000_reweighted->SetLineColor(kRed);
    h_gLLP_ctau_5000->Draw();
    h_gLLP_ctau_5000_reweighted->Draw("same"); 
    //     // Convert float to string
    // std::ostringstream oss_x;
    // std::ostringstream oss_ratio;
    // oss_x << x_cor;
    // oss_ratio << ratio_closest_to_1;
    // std::string x_cor_str = oss_x.str();
    // std::string ratio_str = oss_ratio.str();
    // std::string annotation = "x-intersection " + x_cor_str + " ratio: " + ratio_str;
    // text->DrawLatex(0.5, 0.8, annotation.c_str());
    //c1->SaveAs("h_gLLP_Pi0Pi0_mPhi0p3_ctau500_with_reweighted5000_combined.png");
    c2->SaveAs("h_gLLP_Pi0Pi0_mPhi0p3_ctau5000_with_reweighted500_combined.png");

    //seperate histograms
    // TCanvas *c2 = new TCanvas("c2", "gLLP_ctau Distribution", 800, 600);
    // h_gLLP_ctau_300->Draw();
    
    // TCanvas *c3 = new TCanvas("c3", "gLLP_ctau Distribution", 800, 600);
    // h_gLLP_ctau_1000->Draw();


    // c2->SaveAs("reweight200_h_gLLP_PhiToPi0Pi0_ctau300.png");
    // c3->SaveAs("reweight900_h_gLLP_PhiToPi0Pi0_ctau1000.png");

    // c3->SaveAs("h_gLLP_PhiToPi0Pi0_ctau1000.png");

    // Close the ROOT file
    //file1->Close();
    file2->Close();
}




