#include <iostream>
#include <sstream>
#include <vector>
#include <TFile.h>
#include <TStyle.h>
#include <TH1.h> 
#include <TH2.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TPaveStats.h>
#include <TLatex.h>   
#include <TLegend.h>
#include "getFilesAndHistogramsZJets.h"
#include "ConfigVJets.h"
#include "doPlot.h"
//--  Setting global variables --------------------------------------------------------------
#include "fileNamesZJets.h"
//-------------------------------------------------------------------------------------------

using namespace std;

/** Draw data/MC comparison plots from the histograms of the individual contributions.
 * The list of histograms to superimposed is taken from Samples array defined in fileNamesZJets.h.
 * Histogram colours and labels are defined in the same array.
 * The first element is used as signal data and the last one as signal MC.
 * @param doPASPlots if true PAS style is use, if false AN style is used.
 * @param lepSel DE, DMu. SE, SMu for Z+jet electron channel, Z+jet muon channel, W+jet...
 * @param histDir location of histogram to use as input.
 * @param recoCompDir directory where the produced plots should be stored
 * @param jetPtMin lower bound for jet pt histograms
 * @param jetEtaMax upper bound in jet |eta| for jet eta histograms
 */


    template < int x, int y >
void addZbb(std::vector<TString> vhNames,std::vector<TString> vhNames_b, TFile** fSamples)
{

    TString name;
    TString end[3] = {"_l","_c","_b"};
    TH1D *hist[30];
    int current = 3;

    for (unsigned int i = 0; i< 7; i++){
        for ( unsigned int j=0; j<vhNames.size(); j++){
            //cout << vhNames[j] << endl;
            for(int k=3; k<vhNames_b.size(); k++){
                if (vhNames[j]==vhNames_b[k](0,vhNames_b[k].Length()-2) && vhNames_b[k].EndsWith(end[i-current])){
                    name = vhNames_b[k];
                    break;
                }
                else name = vhNames[j];
            }
            hist[i][j] = getHisto(fSamples[current], name);
        }
    }
}




void RecoComparison(bool doPASPlots, TString lepSel, TString histoDir, TString recoCompDir, int jetPtMin, int jetEtaMax)
{
    TH1::SetDefaultSumw2();
    gStyle->SetOptStat(0);

    //    TString energy = "13TeV";
    ConfigVJets cfg;
    TString energy = TString::Format("%gTeV", cfg.getD("energy"));

    int Colors[NFILESDYJETS+2];
    TString legendNames[NFILESDYJETS+2];

    //-- get the files, legend names and colors -----------------------------------------------------------
    TFile *fSamples[NFILESDYJETS];
    for (unsigned short i = 0; i < NFILESDYJETS; ++i){

        int iSample = FilesDYJets[i];

        if(iSample < 0) continue;

        //--- get the file ---
        TString syst = "0";
        if (iSample != 0) syst = "0";
        fSamples[i] = getFile(histoDir, lepSel, energy, Samples[iSample].name, jetPtMin, jetEtaMax, "", syst);
        if (!fSamples[i]) return;

        //-- set the legend name for the current file ---
        if (iSample == 0) {
            if (lepSel == "DMu")      legendNames[i] = " #mu#mu Data";
            else if (lepSel == "DE")  legendNames[i] = " ee Data";
            else if (lepSel == "SMu") legendNames[i] = " #mu Data";
            else if (lepSel == "SE")  legendNames[i] = " e Data";
            else                      legendNames[i] = " Data";
        }
        //        else if (i == NFILESDYJETS-1) 
        //    legendNames[i] = (lepSel == "DMu") ? " Z/#gamma^{*} #rightarrow #mu#mu" : "Z/#gamma^{*} #rightarrow ee"; 
        else 
            legendNames[i] = doPASPlots ? Samples[iSample].legendPAS : Samples[iSample].legendAN; 
        //--- set the legend color for the current file ---
        Colors[i] = doPASPlots ? Samples[iSample].colorPAS : Samples[iSample].colorAN;    
    }


    //-----------------------------------------------------------------------------------------------------

    TString outputFileName = recoCompDir;
    system("mkdir -p " + recoCompDir);
    if (doPASPlots) outputFileName += "PAS_";
    outputFileName += "Comparison_" + lepSel + "_" + energy + "_Data_All_MC";
    outputFileName += "_JetPtMin_";
    outputFileName += jetPtMin;
    outputFileName += "_JetEtaMax_";
    outputFileName += jetEtaMax;
    //--- create the directory if it doesn't exist ---
    system("mkdir -p " + outputFileName);
    TString outputFileRoot = outputFileName + ".root";
    cout << "Output directory is: " << outputFileName << endl;
    cout << "Output root file is: " << outputFileRoot << endl;
    TFile *outputFile = new TFile(outputFileRoot, "RECREATE");
    outputFile->cd();

    //--- vector to contain the names and titles of the reco histograms ---
    vector<TString> vhNames;
    vector<TString> vhTitles;
    vector<TString> vhNames_b;
    vector<TString> vhTitles_b;
    //---------------------------------------------------------------------

    //--- total number of histograms inside the data file 
    //    (included gen and TH2 that we dont want for comparison) ---
    unsigned short nTotHist = fSamples[0]->GetListOfKeys()->GetEntries();
    for (unsigned short i = 0; i < nTotHist; ++i) {
        TString hName = fSamples[0]->GetListOfKeys()->At(i)->GetName();
        TH1D *hTemp = (TH1D*) fSamples[0]->Get(hName);
        TString hTitle = hTemp->GetName();
        //--- skip histogram if it is gen or has no entry or is not a TH1 ---
        if (hName.Index("gen") >= 0 || hTemp->GetEntries() < 1 || !hTemp->InheritsFrom(TH1D::Class()) || hName.EndsWith("_b") || hName.EndsWith("_c") || hName.EndsWith("_l")) continue;
        if (hName.Index("Ratio") >= 0 || hTemp->GetEntries() < 1 || !hTemp->InheritsFrom(TH1D::Class()) ) continue;
        //--- store the histograme name  and title ---
        vhNames.push_back(hName);
        //cout << hName << endl;
        vhTitles.push_back(hTitle);
    } 
    for (unsigned short i = 0; i < nTotHist; ++i) {
        TString hName = fSamples[0]->GetListOfKeys()->At(i)->GetName();
        TH1D *hTemp = (TH1D*) fSamples[0]->Get(hName);
        TString hTitle = hTemp->GetName();
        //--- skip histogram if it is gen or has no entry or is not a TH1 ---
        if (hName.EndsWith("_b")){
            //--- store the histograme name  and title ---
            cout << "B  names: " << hName << endl;
            vhNames_b.push_back(hName);
            vhTitles_b.push_back(hTitle);}
    } 

    std::vector<TH1D*> hist;
    //reads integrated luminosity
    double lumi = -1;
    TH1* Lumi;
    fSamples[0]->GetObject("Lumi", Lumi);
    if(Lumi) lumi = Lumi->GetBinContent(1) / 1000.;
    else cerr << "Warning: Lumi histogram was not found. The integrated luminosity indicaion will be missing from the plots.\n";

    // getting histos
    int nHist = vhNames.size();
    for (int j = 1; j < nHist; ++j) {
        for (unsigned int i = 1; i < NFILESDYJETS; ++i) {
            hist.push_back(getHisto(fSamples[i], vhNames[j]));
            if(!hist[i-1]) {
                std::cerr << "Histogram " << vhNames[j] 
                    << " was not found for sample " << Samples[FilesDYJets[i]].name << "\n";
                continue;
            }
        }
        doPlot *plot = new doPlot(getHisto(fSamples[0], vhNames[j]),hist);
        plot->setLumi(lumi);
        plot->setColors(Colors);
        plot->setDoPasPlot(doPASPlots);
        plot->setLegend(legendNames);
        plot->setEnergy(energy);
        plot->setOutputFileName(outputFileName);
        plot->setTitle(vhNames[j]);
        plot->makePlot();
        delete plot;
        hist.clear();
        //break;
    }

    int nHist_b = vhNames_b.size();
    int last;
    for (int j = 2; j < nHist_b; ++j) {
        for (unsigned int i = 1; i < NFILESDYJETS-1; ++i) {
            hist.push_back(getHisto(fSamples[i], vhNames_b[j](0,vhNames_b[j].Length()-2)));
            if(!hist[i-1]) {
                std::cerr << "Histogram " << vhNames[j] 
                    << " was not found for sample " << Samples[FilesDYJets[i]].name << "\n";
                continue;
            }
            last=i;
        }
        hist.push_back(getHisto(fSamples[last+1], vhNames_b[j](0,vhNames_b[j].Length()-2)+"_l"));
        hist.push_back(getHisto(fSamples[last+1], vhNames_b[j](0,vhNames_b[j].Length()-2)+"_c"));
        hist.push_back(getHisto(fSamples[last+1], vhNames_b[j](0,vhNames_b[j].Length()-2)+"_b"));
        doPlot *plot = new doPlot(getHisto(fSamples[0], vhNames_b[j](0,vhNames_b[j].Length()-2)),hist);
        plot->setLumi(lumi);
        plot->setColors(Colors);
        plot->setDoPasPlot(doPASPlots);
        plot->setLegend(legendNames);
        plot->setEnergy(energy);
        plot->setOutputFileName(outputFileName);
        plot->setTitle(vhNames_b[j](0,vhNames_b[j].Length()-2));
        plot->makePlot();
        delete plot;
        hist.clear();
        //break;
    }    

    outputFile->cd();
    outputFile->Close();

    //-- Close all the files ------------------------------
    for (unsigned short i(0); i < NFILESDYJETS; ++i) closeFile(fSamples[i]);
    //-----------------------------------------------------

}
