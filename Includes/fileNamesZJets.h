//-*- c-basic-offset: 4; -*-
#ifndef _FILENAMESZJETS_h_
#define _FILENAMESZJETS_h_

#include <TString.h>

//-- directory of input root files --------------------
//Commented out, as it should be obtained from the configuration parameter histoDir
//const TString FILESDIRECTORY("HistoFiles/");
//---------- lets add basic information on samples inot common struct -------------------------------------------
struct processInfoStruct{
    TString name;
    char    merge; //'+' indicates the sample be merged to the next one
                   //until '=' is found. Line with '=' refers to the sample
                   //resulting from the merge.
    double NEvents, xsec, xsecFrac, xsecError;
    int colorAN, colorPAS;
    TString legendAN, legendPAS;
};

//--- first element must point to the data
//--- last element must point to the MC Signal(s)
//8TeV colours: MLM: kBlue-10, Sherpa: kOrange-2, FXFX: kGreen-8
const processInfoStruct Samples[] = {
    //--  Name     --- merge - #events -- xsec ---  BR - xsec rel. unc. - colorAN - colorPAS - name on legend
    /* 0*/{"Data",      ' ',     1.,       1.,      1,  1,        kBlack,    kBlack,    " Data", " Data"},
    /* 1*/{"TT",        ' ',     1.,       1.,      1,  0.10,     kBlue,     kBlue,     " TT",   " t#bar{t}"},  
    /* 2*/{"ST_sch",    '+',     1.,       1.,      1,  0.06,     kBlue+2,   kBlue+2,   " Single Top s-ch", " Single Top s-ch"},
    /* 3*/{"ST_tch",    '+',     1.,       1.,      1,  0.06,     kBlue+4,   kBlue+4,   " Single Top t-ch", " Single Top t-ch"},
    /* 4*/{"STbar_tW",  '+',     1.,       1.,      1,  0.06,     kBlue+6,   kBlue+6,   " #bar{t}W", " #bar{t}W"},
    /* 5*/{"ST_tW",     '+',     1.,       1.,      1,  0.06,     kBlue+8,   kBlue+8,   " tW", " t#bar{t}"},  
    /* 6*/{"Top",       '=',     1.,       1.,      1,  0.06,     kMagenta,  kMagenta,  " Single Top", "Single top"},
    /* 7*/{"WToLNu", 	' ',     1.,       1.,      1,  0.06,     kOrange, kOrange, " WJets", " W"},
    /* 8*/{"ZZ",        '+',     1.,       1.,      1,  0.06,     kOrange,   kOrange,   " ZZ", " ZZ"},
    /* 9*/{"WWTo2L2Nu", '+',     1.,       1.,      1,  0.06,     kViolet+5, kViolet+5, " WW", " WW"},
    /*10*/{"WZ",    	'+',     1.,       1.,      1,  0.06,     kRed+1,    kRed+1,    " WZ", " WZ"},
    /*11*/{"VV",    	'=',     1.,       1.,      1,  0.06,     kRed+1,    kRed+1,    " VV", "VV"},              
    /*12*/{"DYJets_UNFOLDING", ' ', 1.,    1.,      1,  0.06,     kGreen-8,  kGreen-8,  " DYJets", " Z+jets"}, 
};

const int NSamples = sizeof(Samples) / sizeof(Samples[0]);
const int DATA(0);
const int DYJETS(NSamples - 1); // Signal MC is the seconf from the last sample of the list.

/** Total number of samples after sample grouping, including real data, background MC, and signal MC
 */
const unsigned int NFILESDYJETS = 6;

/** Number of background MC samples: all minus data and the signal MC
 */
const unsigned int NBGDYJETS = NFILESDYJETS - 2;

/** List of indices of samples from Samples to be used for ZJets analysis
 * When samples are grouped, only the merged sample is included in this list.
 */
const unsigned int FilesDYJets[NFILESDYJETS] = {0, 1, 6, 7, 11, 12 };
//const unsigned int FilesDYJets[NFILESDYJETS] = {0, 1, 2};


//AG
//const TString DYPOWHEGFILENAME("DYJetsToLL_M-50_TuneCUETP8M1_8TeV-amcatnloFXFX-Bonzai_fixed_allWeights");
// const TString DYAMCATNLOFILENAME("DYJetsToLL_M-50_TuneCUETP8M1_8TeV-amcatnloFXFX-Bonzai_fixed_allWeights");
//const TString DYAMCATNLOLEGEND("MG5_aMC + PY8 (#leq 2j NLO + PS)");
//const TString DYSHERPA2FILENAME("DYJets_Sherpa2_0_16000");
//const TString DYSHERPA2LEGEND("SHERPA 2 (#leq 2j NLO 3,4j LO + PS)");

//DYSHERPA14FILENAME: alternate signal sample to use to estimate
//unfolding systematic uncertainties. Use empty string to disable
//the calculaiton
//const TString DYSHERPA14FILENAME("DYJets_UnfUnc_UNFOLDING");
const TString DYSHERPA14FILENAME("");
//const TString DYSHERPA14FILENAME("DYJets_Sherpa_Bugra_1_13_UNFOLDING");

const TString DYSHERPA14LEGEND("Sherpa1.4 LO");
//const TString DYMGPYTHIA8FILENAME("DYJetsToLL_M-50_TuneCUETP8M1_8TeV-MG-MLM-Bonzai");
//const TString DYMGPYTHIA8LEGEND("MG+PYthia8 legend");

#endif
