#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <TString.h>
#include <iostream>
#include    <TObjString.h>
//#include    "TString.h"
#include    <sstream>
#include <vector>
#include <TDirectory.h>
#include <TFile.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TKey.h>
#include <TH1.h>
#include <TH2.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TPaveStats.h>
#include <TText.h>
#include <TLegend.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
void getMuonID(){

//TFile* file=new TFile("Muon_ID_iso_Efficiencies_Run_2012ABCD_53X.root");
TFile file("MuonID_Z_RunCD_Reco76X_Feb15.root");

TH2F* a;
FILE *outFile = fopen("Muon_ID_iso_2015D_76X.txt","w");
// ID TIGHT
a=(TH2F*) file.Get("MC_NUM_TightIDandIPCut_DEN_genTracks_PAR_pt_spliteta_bin1/pt_abseta_ratio");
const int xbins=a->GetXaxis()->GetNbins();
const int ybins=a->GetYaxis()->GetNbins();
const int r = xbins;
const int b = ybins;
gROOT->SetBatch(true);
double xLow[r+1];
double yLow[b+1];
a->Draw();
a->GetXaxis()->GetLowEdge(xLow);
a->GetYaxis()->GetLowEdge(yLow);
xLow[r]=1000.;
yLow[b]=2.4;
int x,y;
cout << xbins << " " << ybins << endl;
for (int y =1; y<ybins+1; y++){
	for (x=1; x<xbins+1; x++){
		cout << yLow[y-1] << "\t" << yLow[y] << "\t" << xLow[x-1] << "\t" << xLow[x] << "\t" << a->GetBinContent(x,y) << "\t" << a->GetBinErrorLow(x,y) << "\t" << a->GetBinErrorUp(x,y) << endl;
	}
}


TFile isoFile("MuonIso_Z_RunCD_Reco76X_Feb15.root");
a=(TH2F*) isoFile.Get("MC_NUM_LooseRelIso_DEN_TightID_PAR_pt_spliteta_bin1/pt_abseta_ratio");
const int xbins=a->GetXaxis()->GetNbins();
const int ybins=a->GetYaxis()->GetNbins();
const int r = xbins;
const int b = ybins;
gROOT->SetBatch(true);
double xLow[r+1];
double yLow[b+1];
a->Draw();
a->GetXaxis()->GetLowEdge(xLow);
a->GetYaxis()->GetLowEdge(yLow);
xLow[r]=1000.;
yLow[b]=2.4;
int x,y;
cout << xbins << " " << ybins << endl;
for (int y =1; y<ybins+1; y++){
	for (x=1; x<xbins+1; x++){
		cout << yLow[y-1] << "\t" << yLow[y] << "\t" << xLow[x-1] << "\t" << xLow[x] << "\t" << a->GetBinContent(x,y) << "\t" << a->GetBinErrorLow(x,y) << "\t" << a->GetBinErrorUp(x,y) << endl;
	}
}


////////////////// isolation
/*FILE *outFile = fopen("iso_2015_ReReco.txt","w");
  for (int i = 0; i < nHist; i++) {
      string  histoNameTemp = file.GetListOfKeys()->At(i)->GetName();
	fprintf( outFile, "%d  ",i);
	 if (histoNameTemp.find("DATA_over_MC_combRelIsoPF04dBeta")  != string::npos  && histoNameTemp.find("012_Tight_pt")   != string::npos ) {
	cout<<histoNameTemp<<endl;
	a[j]=(TGraphAsymmErrors* ) file.Get(histoNameTemp.c_str());
	TCanvas *BB=new TCanvas(histoNameTemp.c_str(),histoNameTemp.c_str(),700,500);
	BB->cd();
	a[j]->Draw("AP");
	a[j]->GetN();
	cout<<  a[j]->GetN()<<endl;
	double x=0.,y=0.;
	for (int ll=0; ll<a[j]->GetN(); ll++){
		 a[j]->GetPoint(ll,x,y);
		//cout<< a[j]->GetPoint(ll,x,y)<<"   "<<x<<"   "<<y<<endl;
		//cout<<" tu sam \n"<<endl;	
		 if ( j == 0 )  {fprintf( outFile," 0	0.9	"); 
				cout<<" 0    0.9    ";} 
		 if ( j == 1 )  cout<<" 0.9    1.2    "  ;
		 if ( j == 2 )  cout<<" 1.2    2.1    "  ;
	//	 if ( j == 2 )  out<<" 1.2    2.1    "  
	

		if (x < 20 ){    cout<<"10      20    ";
				fprintf( outFile,"10      20	");}
		else if ( x < 25 ) cout<<"20      25    ";
		else if ( x < 30 ) cout<<"25      30    ";
		else if ( x < 35 ) cout<<"30      35    ";
		else if ( x < 40 ) cout<<"35      40    ";
		else if ( x < 50 ) cout<<"40      50    ";
		else if ( x < 60 ) cout<<"50      60    ";
		else if ( x < 90 ) cout<<"60      90    ";
		else if ( x < 140 ) cout<<"90      140    ";
		else if ( x < 500 ) cout<<"140      500    ";
		fprintf( outFile, " %e \n",y);
		cout<<y<<endl;
		}
	j++;
	} 
}

*/
}
