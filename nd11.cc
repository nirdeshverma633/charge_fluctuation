// charge_fluctuation analysis

// Header file to access Pythia 8 program elements.
#include"Pythia8/Pythia.h" 

// Stdlib header file for input and output.
#include <iostream>
#include "TGraph.h"

// ROOT, for histogramming.
#include "TH1F.h"

// ROOT, for interactive graphics.
#include "TVirtualPad.h"

#include "TApplication.h"

// ROOT, for saving file.
#include "TFile.h"

using namespace Pythia8;  //Let Pythia8:: be implicit.

//int main()
int main(int argc, char* argv[])
{
   
  // Create the ROOT application environment.
  TApplication theApp("hist", &argc, argv);
  
  //setting up the generation
  Pythia pythia;           //Declare Pythia Object
  
  //pythia.readString("Random:setSeed=on");
  // pythia.readString("Random:seed=546789699");
  
  //pythia.readString("HardQCD:all = on");

  //pythia.readString("PhaseSpace:pTHatMin = 20.");

  pythia.readString("Beams:idA = 2212");   //pp collision
  pythia.readString("Beams:idB = 2212");


  // pythia.readString("HardQCD:all = on");


  pythia.readString("SoftQCD::nonDiffractive = on");
  pythia.readString("SoftQCD::singleDiffractive = off");
  pythia.readString("SoftQCD::doubleDiffractive = off");
  pythia.readString("SoftQCD::centralDiffractive = off");
  
  // pythia.readString("PartonLevel:MPI = on");

  pythia.settings.listAll();

  // pythia.readString("Tune:pp = 14");
  
  //pythia.readString("Next:numberShowEvent = 5");
  
  pythia.readString("Beams:eCM = 13000."); // 13 TeV CM energy
  pythia.init();  // Initialize; incoming pp beams is default
  
  TFile *outFile = new TFile("Net_charge_with_per.root","RECREATE");
  
  TH1D *mult_charged1= new TH1D("mult_charged1","positive charged Multiplicity Distribution;positive charged multiplicity;counts", 1000,-10,190); 
  TH1D *mult_charged2 = new TH1D("mult_charged2","negative charged Multiplicity Distribution;negative charged multiplicity;counts", 1000,-10,190);
  TH1D *mult_charged3 = new TH1D("mult_charged3","net charge  Multiplicity Distribution;net  charged multiplicity;counts", 600,-30,30);
  TH1D *mult_charged4= new TH1D("mult_charged4","total charged Multiplicity Distribution;charged particle  multiplicity;counts", 500,0,1000);
  TH1I *NetCharge_Hist[5];
  TH1I *Mult_Hist[5];	
  for(Int_t c=0;c<5;c++){
	  NetCharge_Hist[c]=new TH1I(Form("NetHist_%d",c),Form("Netcharge_%d, |eta|<0.8,pT>0.1;net_charge multiplicity;counts",c),800,-40,40);
	  Mult_Hist[c]=new TH1I(Form("MultHist_%d",c),Form("Multiplicity_%d, |eta|<0.8,pT>0.1;charged particle multiplicity;counts",c),500,0,1000);
	}


  int  m_cut[]={367,229,147,103,1};


  for(int iEvent=0; iEvent <10000 ;iEvent++)
  { 
  
    if (!pythia.next()) continue;   // Generate an(other) event. Fill event record.
    
    int n_poscharged = 0;  //to Find number of all final charged particles
    int n_negcharged = 0;
    int n_charged=0;
    int net_charge;
    
    
    for (int i = 0; i < pythia.event.size(); ++i)     //particle loop to access all particles in event loop
    { 
      if ( pythia.event[i].isFinal() )                 //getting data only for outgoing particles 
      { 
	double eta = pythia.event[i].eta();
	double pt = pythia.event[i].pT();
	double pid=pythia.event[i].id();
        if(TMath::Abs(eta)<0.8 && pt>0.5) {
		  int flag1=pythia.event[i].idAbs()/pythia.event[i].id();
		  if(flag1==+1) n_poscharged++;
		  if(flag1==-1) n_negcharged++;
	       
		 }
        
         
        
              net_charge=n_poscharged-n_negcharged;
	      if(pythia.event[i].isFinal() && pythia.event[i].isCharged() && pt>0.5 && (TMath::Abs(eta)>2.8 & TMath::Abs(eta)<5.1) || (TMath::Abs(eta)>-3.7 & TMath::Abs(eta)<-1.7) );
	      {
                 int flag2=pythia.event[i].idAbs()/pythia.event[i].id();
		 if(flag2!=0)  ++n_charged;


	      }
	     
        

      }
      }//particle loop ends here
     
       if(n_charged>m_cut[0] & n_charged<=1000)
	      {
	    NetCharge_Hist[0]->Fill(net_charge);
	    Mult_Hist[0]->Fill(n_charged);
	      }
	    if(n_charged>m_cut[1] & n_charged<=m_cut[0])
	      {
	    NetCharge_Hist[1]->Fill(net_charge);
	    Mult_Hist[1]->Fill(n_charged);
	      }

	    if(n_charged>m_cut[2] & n_charged<=m_cut[1])
	      {
	    NetCharge_Hist[2]->Fill(net_charge);
	    Mult_Hist[2]->Fill(n_charged);
	      }

	    if(n_charged>m_cut[3] & n_charged<=m_cut[2])
	      {
	    NetCharge_Hist[3]->Fill(net_charge);
	    Mult_Hist[3]->Fill(n_charged);
	      }

	    if(n_charged>m_cut[4] & n_charged<=m_cut[3])
	      {
	    NetCharge_Hist[4]->Fill(net_charge);
	    Mult_Hist[4]->Fill(n_charged);
	      }
      
    
    
       
    
             mult_charged1->Fill(n_poscharged);   
             mult_charged2->Fill(n_negcharged);
             mult_charged3->Fill(net_charge);
             mult_charged4->Fill(n_charged);

  }//event generation loops end here
  
  
    //writing histograms

    for(Int_t y=0;y<5;y++){
    NetCharge_Hist[y]->Write();
    Mult_Hist[y]->Write();}
   
      
    mult_charged1->Write();
    mult_charged2->Write();
    mult_charged3->Write();
    mult_charged4->Write();
    //********************Percentile calculation ....
    Double_t per[]={20,40,60,80,100};
    ofstream outfile1("per.txt");
    Double_t Ent=(Double_t)mult_charged4->GetEntries();
    Double_t p[5];
    Double_t x[5];
    Int_t l_bin=0;
    Int_t u_bin=1500;
    for(Int_t m=0;m<5;m++){
    Double_t Area;
    for(u_bin=1500;u_bin>0;u_bin--){
    
      Area=(Double_t)mult_charged4->Integral(l_bin,u_bin);
      p[m]=(100-100*(Area/Ent));
      if(p[m]>per[m]){
	cout<<"u_bin="<<u_bin+1<<endl;
	break;
      }
    }

    x[m]=mult_charged4->GetXaxis()->GetBinCenter(u_bin+1);
    outfile1<<per[m]<<"   ---   "<<x[m]<<endl; // writing to outfile1
  }
  //**********************End of percentile calcultion .....
     
   Double_t Y[5];
   Double_t X[5];

  for(Int_t j=0;j<5;j++)
    {
      Y[j]=NetCharge_Hist[j]->GetMean();
      X[j]=Mult_Hist[j]->GetMean();
      cout<<Y[j]<<   " ---   "  <<X[j]<<endl;
      
    }
  TGraph *gr2=new TGraph(5,X,Y);
     gr2->SetLineWidth(3);
   gr2->SetMarkerStyle(21);
   gr2->SetLineColor(2);
   gr2->SetTitle(" net charge vs total multiplicity");
  gr2->Write();
  outFile->Close();
  pythia.stat();   //To obtain statistics on the number of events generated of the different kinds, and the estimated cross sections
  
 return 0;
 }
 
 

