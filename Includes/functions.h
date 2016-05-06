#ifndef _functions_h_
#define _functions_h_

#include <iostream>
#include <cstdarg>
#include <vector>
#include <TLorentzVector.h>
#include <RooUnfoldResponse.h>

using namespace std;

void barre_de_progression(int);

struct leptonStruct{

    leptonStruct();
    leptonStruct(double pt_, double eta_, double phi_, double en_, double charge_, double id_, double iso_, double scEta_, double trigger_, int lepID_) {
        v.SetPtEtaPhiE(pt_, eta_, phi_, en_);
        charge = charge_;
        id = id_;
        iso = iso_;
        scEta = scEta_;
        trigger = trigger_;
        lepID = lepID_;
    }

    TLorentzVector v;
    double charge, iso, scEta;
    int id, lepID, trigger;
};

struct jetStruct{
    jetStruct();
    jetStruct(double pt_, double eta_, double phi_, double en_, int partonFlavour_, float BDiscCisvV2_, bool isBJet_, float SFb_) {
        v.SetPtEtaPhiE(pt_, eta_, phi_, en_);
        partonFlavour = partonFlavour_;
	BDiscCisvV2 = BDiscCisvV2_ ;
	isBJet = isBJet_;
	SFb = SFb_;
    }

    TLorentzVector v;
    int patIndex;
    int partonFlavour;
    float BDiscCisvV2;
    bool isBJet;
    float SFb;
};
double weightJetEventProducer(std::vector <jetStruct> &, double ) ;
bool LepDescendingOrder(leptonStruct, leptonStruct);
bool JetDescendingOrder(jetStruct, jetStruct);
//--- for WJets ---
bool JetYDescendingOrder(TLorentzVector, TLorentzVector);
double deltaRYPhi(TLorentzVector, TLorentzVector);
//-----------------

vector<double> makeVector(int num, ...);
void insertVector(vector<double>& veca, int num, ...);

double phi0to2pi(double);

double deltaPhi(TLorentzVector, TLorentzVector);

double deltaPhi(double, double);
double deltaR(TLorentzVector, TLorentzVector);
double deltaR(double, double, double, double);
double PHI(TLorentzVector, TLorentzVector, TLorentzVector, TLorentzVector);
double PHI_T(TLorentzVector, TLorentzVector, TLorentzVector, TLorentzVector);
double SpTsub(TLorentzVector, TLorentzVector);
double SpT(TLorentzVector, TLorentzVector, TLorentzVector, TLorentzVector);
double SPhi(TLorentzVector, TLorentzVector, TLorentzVector, TLorentzVector);

class record{
    public:
        double ptLow, ptHi, etaLow, etaHi, effi, effiErrorLow, effiErrorHigh;
        record();
        record(double, double, double, double, double, double, double);
        bool belongTo(double, double);
};

class table{
    public:
        table();
        table(string);
        double getEfficiency(double, double);  
        double getEfficiencyLow(double, double);  
        double getEfficiencyHigh(double, double);  

    private:
        vector<record> recd;
};

double SmearLepPt(double recoPt, double genPt, int smearlepton, double smearFactor);
double SmearJetPt(double, double, double, int);
double SmearJetPt_76X(double, double, double, int);
void bestTwoJetsCandidatesPt(vector<jetStruct>, pair<TLorentzVector, TLorentzVector>&);
void bestTwoJetsCandidatesPhi(vector<jetStruct>, pair<TLorentzVector, TLorentzVector>&);
//void BTagModification(double randNumber, double pt, double eta, int jetFlavour, bool &passBJets);
//double* BTagModification(double randNumber, double pt, double eta, int jetFlavour, bool &passBJets);
void BTagModification(double randNumber, double pt, double eta, int jetFlavour, bool &passBJets, double &SFb_out, double &effb_out, int systDir);
double eventWeightFunction();
/** Open in read mode a file on eos or a local file. For EOS file,
 * performance might be limited and it should be kept to small file:
 * it executes the "xrdfs cat" command (see man xrdfs).
 * @param path file path. It is assumes an EOS file if it starts with
 * root:// string.
 * @param closeFunc pointer where to store the function to call for close
 * the file instead of fclose.
 * @return pointer to the file stream.
 */
FILE* eosOpen(const char* path, int (**closeFunc)(FILE*));

/** Test if a file is a root file. The test is based on the "magic number" 
 * contained in the file which identifies its type. A ROOT file starts with
 * the sequence r,o,o,t,\0
 * @param path path of the file to test.
 * @return true iff the file is a ROOT file.
 */
bool isRootFile(const char* path);

/** Adds histograms and RooUnfoldResponse objects  with same name
 * and definition read from different files and writes the result
 * in a new file.
 * @param src list of input files
 * @param dest output files
 * @return true on success, false on failure
 */
bool mergeHistFiles(const std::vector<std::string>& src, const std::string& dest);

bool isBJetFunction();
#endif
