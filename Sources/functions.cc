#include "functions.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cstdarg>
#include <cstring>
#include "TH1.h"
#include "TH2.h"
#include "TRandom.h"
#include "TLorentzVector.h"
#include "TFile.h"
#include "TKey.h"
#include "RooUnfoldResponse.h"
#define DEBUG              0

using namespace std;

bool LepDescendingOrder(leptonStruct l1, leptonStruct l2){
    return (l1.v.Pt() > l2.v.Pt());
}

bool JetDescendingOrder(jetStruct j1, jetStruct j2){
    return (j1.v.Pt() > j2.v.Pt());
}

//--- for WJets ---
bool JetYDescendingOrder(TLorentzVector tj1, TLorentzVector tj2){
    return (tj1.Rapidity() > tj2.Rapidity());
}

double deltaRYPhi(TLorentzVector j1, TLorentzVector j2){
    double dY = j1.Rapidity() - j2.Rapidity();
    double dPhi = deltaPhi(j1, j2);
    return sqrt(dY * dY + dPhi * dPhi);
}
//-----------------

vector<double> makeVector(int num, ...)
{
    va_list list;
    va_start(list, num);
    vector<double> vec;
    for (int i(0); i < num; i++) {
        double next = va_arg(list, double);
        vec.push_back(next);
    }
    va_end(list);
    return vec;
}

void insertVector(vector<double>& veca, int num, ...)
{
    va_list list;
    va_start(list, num);
    vector<double> vecb;
    for (int i(0); i < num; i++) {
        double next = va_arg(list, double);
        vecb.push_back(next);
    }
    va_end(list);
    veca.insert(veca.end(), vecb.begin(), vecb.end());
}

double phi0to2pi(double phi){
    double pi = 3.141592653589793238;
    while (phi >= 2.*pi) phi -= 2.*pi;
    while (phi < 0.) phi += 2.*pi;
    return phi;
}

double deltaPhi(TLorentzVector v1, TLorentzVector v2){
    // build the delta Phi angle between the two vectors
    double pi = 3.141592653589793238;
    double phi1 = phi0to2pi(v1.Phi());
    double phi2 = phi0to2pi(v2.Phi());
    double dPhi = phi0to2pi(phi1 - phi2);
    dPhi = (dPhi > (2*pi - dPhi)) ? 2*pi - dPhi : dPhi;
    return dPhi;
} 

double deltaPhi(double Phi1, double Phi2){
    // build the delta Phi angle between the two vectors
    double pi = 3.141592653589793238;
    double phi1 = phi0to2pi(Phi1);
    double phi2 = phi0to2pi(Phi2);
    double dPhi = phi0to2pi(phi1 - phi2);
    dPhi = (dPhi > (2*pi - dPhi)) ? 2*pi - dPhi : dPhi;
    //cout << "      DeltaPhi: " << endl;
    //cout << "      phi1 = " << phi1 << "  phi2 = " << phi2 << endl;
    //cout << "      DeltaPhi = " << dPhi << endl;
    return dPhi;
} 

double deltaR(TLorentzVector v1, TLorentzVector v2){
    double dEta = v1.Eta() - v2.Eta();
    double dPhi = deltaPhi(v1, v2);
    return sqrt(dEta * dEta + dPhi * dPhi);
}

double deltaR(double Phi1, double Eta1, double Phi2, double Eta2){
    double dEta = Eta1 - Eta2;
    double dPhi = deltaPhi(Phi1, Phi2);
    return sqrt(dEta * dEta + dPhi * dPhi);
}

double PHI(TLorentzVector l1, TLorentzVector l2, TLorentzVector j1, TLorentzVector j2){
    // build the angle PHI between the two subsytems (l1+l2, j1+j2) vectors 
    double lPx = (l1.Px() + l2.Px());
    double lPy = (l1.Py() + l2.Py());
    double lPz = (l1.Pz() + l2.Pz());
    double lNorm = sqrt(lPx * lPx + lPy * lPy + lPz * lPz);
    double jPx = (j1.Px() + j2.Px());
    double jPy = (j1.Py() + j2.Py());
    double jPz = (j1.Pz() + j2.Pz());
    double jNorm = sqrt(jPx * jPx + jPy * jPy + jPz * jPz);
    return acos((jPx * lPx + jPy * lPy + jPz * lPz) / (jNorm * lNorm));
}

double PHI_T(TLorentzVector l1, TLorentzVector l2, TLorentzVector j1, TLorentzVector j2){
    // build the angle PHI between the two subsytems (l1+l2, j1+j2) vectors in the transverse plane
    double lPx = (l1.Px() + l2.Px());
    double lPy = (l1.Py() + l2.Py());
    double lNorm = sqrt(lPx * lPx + lPy * lPy);
    double jPx = (j1.Px() + j2.Px());
    double jPy = (j1.Py() + j2.Py());
    double jNorm = sqrt(jPx * jPx + jPy * jPy);
    return acos((jPx * lPx + jPy * lPy) / (jNorm * lNorm));
}

double SpTsub(TLorentzVector v1, TLorentzVector v2){
    return sqrt(pow(v1.Px() + v2.Px(), 2) + pow(v1.Py() + v2.Py(), 2)) / (v1.Pt() + v2.Pt());
}

double SpT(TLorentzVector l1, TLorentzVector l2, TLorentzVector j1, TLorentzVector j2){
    return sqrt( pow(SpTsub(l1, l2), 2) + pow(SpTsub(j1, j2), 2)  ) / sqrt(2.);
} 

double SPhi(TLorentzVector l1, TLorentzVector l2, TLorentzVector j1, TLorentzVector j2){
    return sqrt(deltaPhi(l1, l2) * deltaPhi(l1, l2) + deltaPhi(j1, j2) * deltaPhi(j1, j2)) / sqrt(2.);
}

record::record(): 
    ptLow(0), ptHi(0), etaLow(0), etaHi(0), effi(0),effiErrorLow(0), effiErrorHigh(0)
{
}

record::record(double pt1, double pt2, double eta1, double eta2, double eff, double effLow, double effHigh):
    ptLow(pt1), ptHi(pt2), etaLow(eta1), etaHi(eta2), effi(eff),effiErrorLow(effLow), effiErrorHigh(effHigh)
{
}

bool record::belongTo(double pt, double eta)
{
    return (pt < ptHi && pt >= ptLow) && (eta < etaHi && eta >= etaLow);
}

table::table()
{
}

table::table(string filename)
{
    ifstream file(filename.c_str());
    if (!file) {
        cerr << "Could not open " << filename << endl;
    }
    double data[7];
    while (file) {
        for (int i(0); i < 7; i++) {
            file >> data[i];
        }
        recd.push_back(record(data[2], data[3], data[0], data[1], data[4], data[5], data[6]));
    }

}

double table::getEfficiency(double pt, double eta){
    double hiPtBin= 0;
    for (unsigned int i=0; i != recd.size(); i++) {
        // if finds the proper bin, then return the efficiency
        if ((recd[i]).belongTo(pt, eta)) return recd[i].effi;
        // else store the average pt of the current bin efficency but do not return and try the next bin
        if ((recd[i]).belongTo(0.5*(recd[i].ptHi + recd[i].ptLow), eta)) hiPtBin = recd[i].effi;
    }
    return hiPtBin;
}
double table::getEfficiencyLow(double pt, double eta){
    double hiPtBin= 0;
    for (unsigned int i=0; i != recd.size(); i++) {
        if ((recd[i]).belongTo(pt, eta)) return recd[i].effi-recd[i].effiErrorLow;
        if ((recd[i]).belongTo(350, eta)) hiPtBin = recd[i].effi;
    }
    return hiPtBin;
}
double table::getEfficiencyHigh(double pt, double eta){
    double hiPtBin= 0;
    for (unsigned int i=0; i != recd.size(); i++) {
        if ((recd[i]).belongTo(pt, eta)) return recd[i].effi+recd[i].effiErrorHigh;
        if ((recd[i]).belongTo(350, eta)) hiPtBin = recd[i].effi;
    }
    return hiPtBin;
}

double SmearLepPt(double recoPt, double genPt, int smearlepton, double smearFactor){

    double smearedPt(0);

    if (smearlepton == 0) {
        smearedPt = std::max(0., recoPt);
    }

    else if (smearlepton == 1) {
        smearedPt = std::max(0., genPt + (1.0 + smearFactor)*(recoPt - genPt));
    }

    else if (smearlepton == -1) {
        smearedPt = std::max(0., genPt + (1.0 - smearFactor)*(recoPt - genPt));
    }

    return smearedPt;
}

double SmearJetPt(double recoPt, double genPt, double eta, int smearJet){
    // Fall 2015 resolution scale factor
    // twiki.cern.ch/twiki/bin/view/CMS/JetResolution
    double centralSF(1.00);
    if      (fabs(eta) < 0.8) centralSF = 1.061;
    else if (fabs(eta) < 1.3) centralSF = 1.088;
    else if (fabs(eta) < 1.9) centralSF = 1.106;
    else if (fabs(eta) < 2.5) centralSF = 1.126;
    else if (fabs(eta) < 3.0) centralSF = 1.343;
    else if (fabs(eta) < 3.2) centralSF = 1.303;
    else if (fabs(eta) < 5.0) centralSF = 1.320;
    else centralSF = 1.320;

    double upSF(1.00);
    if      (fabs(eta) < 0.8) upSF = 1.084;
    else if (fabs(eta) < 1.3) upSF = 1.117;
    else if (fabs(eta) < 1.9) upSF = 1.136;
    else if (fabs(eta) < 2.5) upSF = 1.220;
    else if (fabs(eta) < 3.0) upSF = 1.466;
    else if (fabs(eta) < 3.2) upSF = 1.414;
    else if (fabs(eta) < 5.0) upSF = 1.606;
    else upSF = 1.606;

    double downSF(1.00);
    if      (fabs(eta) < 0.8) downSF = 1.038;
    else if (fabs(eta) < 1.3) downSF = 1.059;
    else if (fabs(eta) < 1.9) downSF = 1.076;
    else if (fabs(eta) < 2.5) downSF = 1.032;
    else if (fabs(eta) < 3.0) downSF = 1.220;
    else if (fabs(eta) < 3.2) downSF = 1.192;
    else if (fabs(eta) < 5.0) downSF = 1.034;
    else downSF = 1.034;

    double smearedPt(0);

    if (smearJet == 0) {
        smearedPt = std::max(0., genPt + centralSF*(recoPt - genPt));
    }
    else if (smearJet == 1) {
        smearedPt = std::max(0., genPt + upSF*(recoPt - genPt));
    }
    else if (smearJet == -1) {
        smearedPt = std::max(0., genPt + downSF*(recoPt - genPt));
    }

    return smearedPt;

}

double SmearJetPt_76X(double recoPt, double genPt, double eta, int smearJet){
    // Fall 2015 resolution scale factor
    // twiki.cern.ch/twiki/bin/view/CMS/JetResolution
    double centralSF(1.00);
    if      (fabs(eta) < 0.5) centralSF = 1.095;
    else if (fabs(eta) < 0.8) centralSF = 1.120;
    else if (fabs(eta) < 1.1) centralSF = 1.097;
    else if (fabs(eta) < 1.3) centralSF = 1.103;
    else if (fabs(eta) < 1.7) centralSF = 1.118;
    else if (fabs(eta) < 1.9) centralSF = 1.100;
    else if (fabs(eta) < 2.1) centralSF = 1.162;
    else if (fabs(eta) < 2.3) centralSF = 1.160;
    else if (fabs(eta) < 2.5) centralSF = 1.161;
    else if (fabs(eta) < 2.8) centralSF = 1.209;
    else if (fabs(eta) < 3.0) centralSF = 1.564;
    else if (fabs(eta) < 3.2) centralSF = 1.384;
    else if (fabs(eta) < 5.0) centralSF = 1.216;
    else centralSF = 1.320;

    double upSF(1.00);
    if      (fabs(eta) < 0.5) upSF = 1.095+0.018;
    else if (fabs(eta) < 0.8) upSF = 1.120+0.028;
    else if (fabs(eta) < 1.1) upSF = 1.097+0.017;
    else if (fabs(eta) < 1.3) upSF = 1.103+0.033;
    else if (fabs(eta) < 1.7) upSF = 1.118+0.014;
    else if (fabs(eta) < 1.9) upSF = 1.100+0.033;
    else if (fabs(eta) < 2.1) upSF = 1.162+0.044;
    else if (fabs(eta) < 2.3) upSF = 1.160+0.048;
    else if (fabs(eta) < 2.5) upSF = 1.161+0.060;
    else if (fabs(eta) < 2.8) upSF = 1.209+0.059;
    else if (fabs(eta) < 3.0) upSF = 1.564+0.321;
    else if (fabs(eta) < 3.2) upSF = 1.384+0.033;
    else if (fabs(eta) < 5.0) upSF = 1.216+0.050;
    else upSF = 1.606;

    double downSF(1.00);
    if      (fabs(eta) < 0.5) downSF = 1.095-0.018;
    else if (fabs(eta) < 0.8) downSF = 1.120-0.028;
    else if (fabs(eta) < 1.1) downSF = 1.097-0.017;
    else if (fabs(eta) < 1.3) downSF = 1.103-0.033;
    else if (fabs(eta) < 1.7) downSF = 1.118-0.014;
    else if (fabs(eta) < 1.9) downSF = 1.100-0.033;
    else if (fabs(eta) < 2.1) downSF = 1.162-0.044;
    else if (fabs(eta) < 2.3) downSF = 1.160-0.048;
    else if (fabs(eta) < 2.5) downSF = 1.161-0.060;
    else if (fabs(eta) < 2.8) downSF = 1.209-0.059;
    else if (fabs(eta) < 3.0) downSF = 1.564-0.321;
    else if (fabs(eta) < 3.2) downSF = 1.384-0.033;
    else if (fabs(eta) < 5.0) downSF = 1.216-0.050;
    else downSF = 1.034;

    double smearedPt(0);

    if (smearJet == 0) {
        smearedPt = std::max(0., genPt + centralSF*(recoPt - genPt));
    }
    else if (smearJet == 1) {
        smearedPt = std::max(0., genPt + upSF*(recoPt - genPt));
    }
    else if (smearJet == -1) {
        smearedPt = std::max(0., genPt + downSF*(recoPt - genPt));
    }

    return smearedPt;

}

void bestTwoJetsCandidatesPt(vector<jetStruct> jets, pair<TLorentzVector, TLorentzVector>& bestTwoJets)
{
    int nGoodJets(jets.size());
    if (nGoodJets >= 2){
        //cout << "\nMore than 2 jets, selecting best pair" << endl;
        double minPt(999999.);
        for (int i(0); i < nGoodJets - 1; i++) {
            TLorentzVector jeti = jets[i].v;
            for (int j(i + 1); j < nGoodJets; j++) {
                TLorentzVector jetj = jets[j].v;
                TLorentzVector jetij = jeti + jetj;
                //cout << i << " " << j << ": Pair pt = " << jetij.Pt() << endl;
                if (jetij.Pt() < minPt){
                    bestTwoJets.first = jeti; 
                    bestTwoJets.second = jetj;
                    minPt = jetij.Pt();
                    //cout << "Smallest pt = " << jetij.Pt() << endl;
                }
            }
        } 
    }
}

void bestTwoJetsCandidatesPhi(vector<jetStruct> jets, pair<TLorentzVector, TLorentzVector>& bestTwoJets)
{
    int nGoodJets(jets.size());
    if (nGoodJets >= 2){
        //cout << "\nMore than 2 jets, selecting best pair" << endl;
        double maxdPhi(-0.0001);
        for (int i(0); i < nGoodJets - 1; i++) {
            TLorentzVector jeti = jets[i].v;
            for (int j(i + 1); j < nGoodJets; j++) {
                TLorentzVector jetj = jets[j].v;
                double dPhi = deltaPhi(jeti, jetj);
                //cout << i << " " << j << ": dPhi = " << dPhi << endl;
                if (dPhi > maxdPhi){
                    bestTwoJets.first = jeti; 
                    bestTwoJets.second = jetj;
                    maxdPhi = dPhi;
                    //cout << "Biggest dPhi = " << dPhi << endl;
                }
            }
        } 
    }
}

void BTagModification(double randNumber, double pt, double eta, int jetFlavour, bool &passBJets, double &SF_out, double &eff_out, int systDir=0 )
//double BTagModification(double randNumber, double pt, double eta, int jetFlavour, bool &passBJets )
{
    SF_out = 0. ;
     
    if (abs(jetFlavour) == 5) {
//MWP
        float SFb = 0.934588*((1.+(0.00678184*pt))/(1.+(0.00627144*pt))); 
        if (pt < 30.) SFb = 0.934588*((1.+(0.00678184*30))/(1.+(0.00627144*30))); 
        if (pt > 670.) SFb = 0.934588*((1.+(0.00678184*670))/(1.+(0.00627144*670)));
//TWP
/*        float SFb =0.886376*((1.+(0.00250226*pt))/(1.+(0.00193725*pt))); 
        if (pt < 30.) SFb =0.886376*((1.+(0.00250226*30))/(1.+(0.00193725*30)));
        if (pt > 670.) SFb =0.886376*((1.+(0.00250226*670))/(1.+(0.00193725*670)));
*/

        float SFb_error = 0;

	if (systDir == 0) SF_out = SFb;
	else {
		if      (pt >= 30  && pt < 50)  SFb_error = 0.018076473847031593; 
		else if (pt >= 50  && pt < 70)  SFb_error =0.024799736216664314; 
		else if (pt >= 70  && pt < 100)  SFb_error = 0.024073265492916107;
		else if (pt >= 100  && pt < 140)  SFb_error = 0.020040607079863548;
		else if (pt >= 140  && pt < 200)  SFb_error = 0.016540588811039925;
		else if (pt >= 200  && pt < 300)  SFb_error = 0.025977084413170815;
		else if (pt >= 300  && pt < 670) SFb_error = 0.027120551094412804;
		else if (pt >= 670) SFb_error = 2*0.027120551094412804;


		float SFb_up = SFb + SFb_error;
		float SFb_down = SFb - SFb_error;

		if(systDir > 0) SF_out = SFb_up;
		else if(systDir < 0) SF_out = SFb_down;
	}
    }
    // ---------------- For Real C-jets--------------- //
    else if (abs(jetFlavour) == 4) {
	float SFc = 0.934588*((1.+(0.00678184*pt))/(1.+(0.00627144*pt))); 
    float SFc_error = 0.;

	if (systDir == 0) SF_out = SFc ;

	else{
		if      (pt >= 30  && pt < 50)  SFc_error = 0.018076473847031593; 
		else if (pt >= 50  && pt < 70)  SFc_error =0.024799736216664314; 
		else if (pt >= 70  && pt < 100)  SFc_error = 0.024073265492916107;
		else if (pt >= 100  && pt < 140)  SFc_error = 0.020040607079863548;
		else if (pt >= 140  && pt < 200)  SFc_error = 0.016540588811039925;
		else if (pt >= 200  && pt < 300)  SFc_error = 0.025977084413170815;
		else if (pt >= 300  && pt < 670) SFc_error = 0.027120551094412804;
		else if (pt >= 670) SFc_error = 2*0.027120551094412804;


		float SFc_up = SFc + 2*SFc_error;
		float SFc_down = SFc - 2*SFc_error;

		if(systDir > 0) SF_out = SFc_up;
		else if(systDir < 0) SF_out = SFc_down;
	}
    }
    // ---------------- For REAL Light-jets --------------- //
    else {

        float SFlight;
        float SFlight_up;
        float SFlight_down;

//MWP 
	if (abs(eta) < 0.8){ 
		SFlight=((0.994351+(0.000250077*pt))+(9.24801e-07*(pt*pt)))+(-8.73293e-10*(pt*(pt*pt))); 	
		SFlight_down=((0.949401+(-0.000356232*pt))+(2.24887e-06*(pt*pt)))+(-1.66011e-09*(pt*(pt*pt)));
		SFlight_up=((1.03928+(0.000857422*pt))+(-4.02756e-07*(pt*pt)))+(-8.45836e-11*(pt*(pt*pt)));
		}
	else if (abs(eta) > 0.8 && abs(eta) < 1.6){
		SFlight=((1.00939+(0.000461283*pt))+(-6.30306e-07*(pt*pt)))+(3.53075e-10*(pt*(pt*pt)));	
		SFlight_down=((0.964857+(-2.19898e-05*pt))+(4.74117e-07*(pt*pt)))+(-3.36548e-10*(pt*(pt*pt)));
		SFlight_up=((1.05392+(0.000944135*pt))+(-1.73386e-06*(pt*pt)))+(1.04242e-09*(pt*(pt*pt)));
	}
	else if (abs(eta) > 1.6 && abs(eta)< 2.4){ 
		SFlight=((0.955798+(0.00146058*pt))+(-3.76689e-06*(pt*pt)))+(2.39196e-09*(pt*(pt*pt))); 	
		SFlight_down=((0.910086+(0.00116371*pt))+(-3.02747e-06*(pt*pt)))+(1.86906e-09*(pt*(pt*pt)));
		SFlight_up=((1.00151+(0.00175547*pt))+(-4.50251e-06*(pt*pt)))+(2.91473e-09*(pt*(pt*pt)));
	}


//TWP
        //float SFlight=0.992339;
        //float SFlight_up=1.17457;
        //float SFlight_down=0.810103;

	if (systDir == 0) SF_out = SFlight ;
	else if(systDir > 0) SF_out = SFlight_up;
	else if(systDir < 0) SF_out = SFlight_down;

    }   ////////flavour lop                     
}

double weightJetEventProducer(std::vector <jetStruct> &Jetovi, double cut){
	double w0=1.;
	double w1=0.;
	double w2=1.;
	int cntB=0;
	int j0=0,j1=0,j2=0;

	// 0 b-jets
	for (auto jet:Jetovi){
		w0 *=(1- jet.SFb);
		if (jet.BDiscCisvV2>cut) cntB++;
	}
	if (cntB==0) return 1.;
	if (cntB==1) return (1.-w0);
	// 1 b-jet
	for (auto &jet:Jetovi){
		double w1temp=1;
		for (auto &jet1:Jetovi){
			if (&jet != &jet1 ) w1temp *=(1.- jet1.SFb); 
			else w1temp*=jet.SFb;
		}
		w1 += w1temp;
	}
	// 2 b-jets
	w2=1.-w0-w1;
	if (cntB > 1) return w2;
	return 1;
}

FILE* eosOpen(const char* path, int (**closeFunc)(FILE*)){
    TString tspath(path);
    if(tspath.BeginsWith("root://")){
	*closeFunc = pclose;
	tspath.Remove(0, strlen("root://"));
	Ssiz_t p = tspath.First("/");
	if(p==TString::kNPOS) return 0;
	TString server(tspath(0, p));
	TString filepath(tspath(p + 1, tspath.Length() - p));
	//   std::cout << ">>> server: " << server << ", path: " << filepath << "\n";
	return popen(TString::Format("xrdfs %s cat %s", server.Data(), filepath.Data()), "r");
    } else{
	*closeFunc = fclose;
	return fopen(path, "r");
    }
}

bool isRootFile(const char* path){
    int (*funcClose)(FILE*);
    FILE* f = eosOpen(path, &funcClose);

    char buffer[5];
    if(f){
	bool rc = (fread(buffer, 5, 1, f)==1) && (memcmp(buffer, "root\0", 5)==0);
	funcClose(f);
	return rc; 
    } else{
	std::cerr << "Warning: failed to read file " << path
		  << ". File type determined from its extension.\n";
	return TString(path).EndsWith(".root");
    }
}

bool mergeHistFiles(const std::vector<std::string>& src, const std::string& dest)
{
    TH1::SetDefaultSumw2();

    std::cerr << "Creating file " << dest << std::endl;
    TFile fout(dest.c_str(), "RECREATE");
    if(fout.IsZombie()){
	std::cerr << "Error: failed to create file " << dest 
		  << " (" << __FILE__ << ":" << __LINE__ << ").\n\n";
	return false;
    }

    union u { TObject* o; TH1* h; RooUnfoldResponse* r; } obj;
    std::map<std::string,u> objs;
    for(unsigned iFile = 0; iFile < src.size(); ++iFile){
	TFile in(src[iFile].c_str());
	if(in.IsZombie()) continue;
	TIter next(in.GetListOfKeys());
	TKey* key;
	while((key = (TKey*) next())){
	    obj.o = key->ReadObj();
	    enum {kHist, kResp, kOther} type = kOther;
	    if(obj.o->InheritsFrom("TH1")) type = kHist;
	    if(obj.o->InheritsFrom("RooUnfoldResponse")) type = kResp;
	    if(type == kOther) continue;
	    if(iFile == 0){
		if(type==kHist) obj.h->SetDirectory(0);
		objs[obj.o->GetName()].o = obj.o;
	    } else{
		std::map<std::string,u>::iterator it = objs.find(obj.o->GetName());
		if(it!=objs.end()){
		    if(type==kHist) (*it).second.h->Add(obj.h);
		    if(type==kResp) (*it).second.r->Add(*(obj.r));
		}
	    }
	} //next Object
	in.Close();
    }//next File

    fout.cd();
    for(std::map<std::string,u>::iterator it = objs.begin();
	it != objs.end(); ++it){
	it->second.o->Write();
	delete it->second.o;
    }
    fout.Close();
    return true;
}
