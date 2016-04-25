#ifndef _UNFOLDINGZJETS_h_
#define _UNFOLDINGZJETS_h_

#include <tuple>
#include <RooUnfold.h>
#include <TCanvas.h>
#include <TString.h>

//void UnfoldingZJets(TString lepSel, TString algo, TString histoDir, TString unfoldDir, int jetPtMin, int jetEtaMax, TString gen1, TString gen2, TString variable = "", bool noramlized = false);
void UnfoldingZJets(TString lepSel, TString algo, TString histoDir, TString unfoldDir, int jetPtMin, int jetEtaMax, TString variable = "", bool noramlized = false, int whichSyst = -1);
int UnfoldData(const TString lepSel, const TString algo, int svdKterm, RooUnfoldResponse *resp, TH1D *hRecData, TH1D* &hUnfData, TH2D* &hUnfDataStatCov, TH2D* &hUnfMCStatCov, TString name, double integratedLumi, bool logy = false,
	       TH1D *hRecDataMinusFakesOdd = 0, TH1D *hRecDataMinusFakesEven = 0);
TH2D* M2H(TMatrixD m);
TH2D* makeCovFromUpAndDown(const TH1D* hUnfDataCentral, const TH1D* hUnfDataUp, const TH1D* hUnfDataDown, TString name);
TH1D* makeCrossSectionHist(TH1D* hGenDYJets, double integratedLumi);
TH1D* foldUnfData(TH1 *hUnfData, RooUnfoldResponse *resp);
void createTable(TString outputFileName, TString lepSel, TString variable, bool doNormalized, TH1D *hUnfData, TH2D *hCov[]);
void test();
void createSystPlots(TString outputFileName, TString lepSel, TString variable, TH1D* hUnf[], bool logy = false);
double MyChi2Test(TH1 *h1, TH1 *h2, int nBinsToSkip = 0, Double_t* res = 0);
void RemoveFakes(TH1* hRecData, TH1* hFakes, TH1* hPurity);
#endif
