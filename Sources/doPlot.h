#ifndef DOPLOT_H
#define DOPLOT_H

#include <iostream>
#include "TLatex.h"
#include "TH1D.h"
#include "THStack.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TPad.h"
#include <vector>
#include <sstream>

class doPlot
{
    private:
        int nHistos = 0;
        THStack* hSumMC = new THStack();         
        TH1D* Data;
        double lumi=-1.;
        bool doPASPlots=false;
        std::vector<TH1D*> mcHist;
        TString outputFileName;
        int *Colors;
        TString title;
        TString energy;
        int jetEtaMax;
        int jetPtMin;

        // TLatex
        TLatex *cmsColl = new TLatex();
        TLatex *cmsPrel = new TLatex();
        TLatex *jetAlgo = new TLatex();
        TLatex *jetCuts = new TLatex();
        TLatex *intLumi = new TLatex();
        void setLatex();
        TLegend *legend;
        void prettify();

    public:
        doPlot(TH1D*, std::vector<TH1D*>);
        void setColors(int*);
        void setDoPasPlot(bool);
        void setLegend(TString*);
        void setLumi(double);
        void setTitle(TString);
        void setLabels(int, int);
        void setEnergy(TString);
        void setOutputFileName(TString);
        void makePlot();

};


#endif
