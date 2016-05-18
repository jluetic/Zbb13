#include <iostream>
#include "doPlot.h"
#include <sstream>

doPlot::doPlot(TH1D *Data_, std::vector<TH1D*> mcHist_){   
    nHistos = mcHist_.size()+1;
    Data = Data_;
    mcHist=mcHist_;
}

void doPlot::prettify(){
    Data->SetMarkerStyle(20);
    Data->SetMarkerColor(Colors[0]);
    Data->SetLineColor(Colors[0]);
    int i=1;
    for(auto hist: mcHist){
        hist->SetMarkerStyle(101);
        hist->SetMarkerColor(Colors[i]);
        hist->SetLineColor(Colors[i]);
	    hist->SetFillStyle(1001);
	    hist->SetFillColor(Colors[i]);
        hist->SetLineColor(Colors[i]);
        i++;
        hSumMC->Add(hist);
    } 
}

void doPlot::setColors(int* Colors_){
    Colors = Colors_;
    if (nHistos>6){
        Colors[nHistos-3] = kGreen-3;
        Colors[nHistos-2] = kGreen-6;
        Colors[nHistos-1] = kGreen-9;
    }
    prettify();
}

void doPlot::setTitle(TString title_){
    title = title_;
}

void doPlot::setEnergy(TString energy_){
    energy = energy_;
}

void doPlot::setLumi(double lumi_){
    lumi = lumi_;
}

void doPlot::setOutputFileName(TString outFileName){
    outputFileName = outFileName;
}

void doPlot::setLabels(int jetEtaMax_, int jetPtMin_){
    jetEtaMax = jetEtaMax_;
    jetPtMin = jetPtMin_;
}

void doPlot::setLegend(TString *legendNames_){
    if (!doPASPlots) { 
        legend = new TLegend(0.72, 0.45, 0.76, 0.86);
        legend->SetTextSize(0.032);
    }
    else {
        legend = new TLegend(0.63, 0.55, 0.81, 0.87);
        legend->SetTextSize(0.042);
    }
    legend->SetFillStyle(0);
    legend->SetBorderSize(0);
    legend->SetTextFont(42);
    legend->AddEntry(Data, legendNames_[0], "ep");
    if (nHistos>6){
        legendNames_[nHistos-3] = "Z+l";
        legendNames_[nHistos-2] = "Z+(c)c";
        legendNames_[nHistos-1] = "Z+(b)b";
    }
    int i=mcHist.size();
    for (i=mcHist.size(); i>=1; --i){
	    legend->AddEntry(mcHist[i-1], legendNames_[i], "f");
    }
}

void doPlot::setLatex(){
    cmsColl->SetTextSize(0.04);
    cmsColl->SetTextFont(61);
    cmsColl->SetLineWidth(2);
    cmsColl->SetTextColor(kBlack);
    cmsColl->SetNDC();
    cmsColl->SetTextAlign(11);

    cmsPrel->SetTextSize(0.04);
    cmsPrel->SetTextFont(52);
    cmsPrel->SetLineWidth(2);
    cmsPrel->SetTextColor(kBlack);
    cmsPrel->SetNDC();
    cmsPrel->SetTextAlign(11);

    jetAlgo->SetTextSize(0.035);
    jetAlgo->SetTextFont(42);
    jetAlgo->SetLineWidth(2);
    jetAlgo->SetTextColor(kBlack);
    jetAlgo->SetNDC();
    jetAlgo->SetTextAlign(11);

    jetCuts->SetTextSize(0.035);
    jetCuts->SetTextFont(42);
    jetCuts->SetLineWidth(2);
    jetCuts->SetTextColor(kBlack);
    jetCuts->SetNDC();
    jetCuts->SetTextAlign(11);

    intLumi->SetTextSize(0.03);
    intLumi->SetTextFont(42);
    intLumi->SetLineWidth(2);  
    intLumi->SetTextColor(kBlack);
    intLumi->SetNDC();
    intLumi->SetTextAlign(31);
}

void doPlot::setDoPasPlot(bool doPASPlot_){

    doPASPlots=doPASPlot_;
}


void doPlot::makePlot(){

    setLatex();
    TCanvas *canvas = new TCanvas(title, title, 700, 900);
    canvas->cd();

    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1);
    pad1->SetTopMargin(0.11);
    pad1->SetBottomMargin(0.);
    pad1->SetRightMargin(0.03);
    pad1->SetTicks();
    pad1->SetLogy();
    pad1->Draw();
    pad1->cd();

    TH1D *hRatio = (TH1D*) hSumMC->GetStack()->Last()->Clone();

    // Need to draw MC Stack first other wise
    // cannot access Xaxis !!!
    hSumMC->Draw("HIST"); 
    if (title.Index("ZMass_Z") >= 0){
        Data->GetXaxis()->SetRangeUser(71,110.9);
        hSumMC->GetXaxis()->SetRangeUser(71,110.9);
        hRatio->GetXaxis()->SetRangeUser(71,110.9);
    }


    hSumMC->SetTitle(""); 
    hSumMC->GetYaxis()->SetLabelSize(0.04); 
    hSumMC->GetYaxis()->SetLabelOffset(0.002); 
    hSumMC->GetYaxis()->SetTitle("# Events"); 
    hSumMC->GetYaxis()->SetTitleSize(0.04); 
    hSumMC->GetYaxis()->SetTitleOffset(1.32); 
    hSumMC->SetMinimum(8);
    hSumMC->SetMaximum(100*hSumMC->GetMaximum()); 

    // first pad plots
    Data->DrawCopy("e same");
    legend->Draw();
    cmsColl->DrawLatex(0.13,0.82, "CMS Preliminary");
    if(lumi >= 0) intLumi->DrawLatex(0.97, 0.9, TString::Format("%.3g fb^{-1} (%s)", lumi, energy.Data()));
    if (title.Index("inc0") < 0){
        if (!doPASPlots) {
            std::ostringstream ptLegend;
            if (title.Index("JetPt_Zinc") > 0) {
                ptLegend << "p_{T}^{jet} > 20 GeV,  |y^{jet}| < " << (0.1*jetEtaMax);
            }
            else {
                ptLegend << "p_{T}^{jet} > " << jetPtMin << "GeV,  |y^{jet}| < " << (0.1*jetEtaMax);
            }
            jetAlgo->DrawLatex(0.13,0.68, "anti-k_{t} jets,  R = 0.4");
            jetCuts->DrawLatex(0.13,0.63, ptLegend.str().c_str());
        }
        pad1->Draw();
    }
    canvas->cd();
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0, 1, 0.3);
    pad2->SetTopMargin(0.);
    pad2->SetBottomMargin(0.3);
    pad2->SetRightMargin(0.03);
    pad2->SetGridy();
    pad2->SetTicks();
    pad2->Draw();
    pad2->cd();

    hRatio->SetStats(0);
    hRatio->SetTitle("");

    hRatio->SetMarkerStyle(20);
    hRatio->SetMarkerColor(Colors[0]);
    hRatio->SetLineColor(Colors[0]);

    hRatio->GetXaxis()->SetTickLength(0.03);
    hRatio->GetXaxis()->SetTitleSize(0.1);
    hRatio->GetXaxis()->SetTitleOffset(1.2);
    hRatio->GetXaxis()->SetLabelSize(0.12);
    hRatio->GetXaxis()->SetLabelOffset(0.017);

    hRatio->GetYaxis()->SetRangeUser(0.51,1.49);
    hRatio->GetYaxis()->SetNdivisions(5,5,0);
    hRatio->GetYaxis()->SetTitle("Simulation/Data");
    hRatio->GetYaxis()->SetTitleSize(0.1);
    hRatio->GetYaxis()->SetTitleOffset(0.5);
    hRatio->GetYaxis()->CenterTitle();
    hRatio->GetYaxis()->SetLabelSize(0.08);

    hRatio->Divide(Data);
    hRatio->DrawCopy("EP");

    canvas->cd();
    canvas->Update();

    TString outputFilePDF = outputFileName + "/" + title + ".pdf";
    canvas->Print(outputFilePDF);
    //outputFile->cd();
    //canvas->Write();

    TString outputFileBase = outputFileName + "/" + title;
    canvas->SaveAs(outputFileBase + ".root");
    canvas->SaveAs(outputFileBase + ".C");

    hSumMC->SetMaximum(1.5*hSumMC->GetMaximum());
    TCanvas *tmpCanvas = (TCanvas*) canvas->Clone();
    tmpCanvas->cd();
    tmpCanvas->Draw();
    TPad *tmpPad = (TPad*) tmpCanvas->GetPrimitive("pad1");
    tmpPad->SetLogy(0);
    title += "_Lin";
    tmpCanvas->SetTitle(title);
    tmpCanvas->SetName(title);
    tmpCanvas->Update();
    TString outputFileLinPDF = outputFileName + "/" + title + ".pdf";
    tmpCanvas->Print(outputFileLinPDF);
    //outputFile->cd();
    //tmpCanvas->Write();

	TString outputFileLinBase = outputFileName + "/" + title;
	tmpCanvas->SaveAs(outputFileLinBase + ".root");
	tmpCanvas->SaveAs(outputFileLinBase + ".C");

    }
