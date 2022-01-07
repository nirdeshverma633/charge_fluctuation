

// Header file to access Pythia 8 program elements.
#include"Pythia8/Pythia.h" 

// Stdlib header file for input and output.
#include <iostream>

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
  
  pythia.readString("Random:setSeed=on");
  pythia.readString("Random:seed=546789699");
  
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
  
  pythia.readString("Beams:eCM = 13000."); // 8 TeV CM energy
  pythia.init();  // Initialize; incoming pp beams is default
  
  TFile *outFile = new TFile("Eta7point8.root","RECREATE");
  
  TH1D *mult_charged1= new TH1D("mult_charged1","positive Multiplicity Distribution", 100,-10,190); 
  TH1D *mult_charged2 = new TH1D("mult_charged2","negative particle Multiplicity Distribution", 100,-10,190);
  TH1D *mult_charged3 = new TH1D("mult_charged3","net charge  Multiplicity Distribution", 100,-10,190);

  for(int iEvent=0; iEvent <1 ;iEvent++)
  { 
  
    if (!pythia.next()) continue;   // Generate an(other) event. Fill event record.
    
    int n_poscharged = 0;  //to Find number of all final charged particles
    int n_negcharged = 0;
    
    
    for (int i = 0; i < pythia.event.size(); ++i)     //particle loop to access all particles in event loop
    { 
      if ( pythia.event[i].isFinal())                 //getting data only for outgoing particles 
      { 
        // Find number of all final charged particles. & their Kinematic Properties
	double eta = pythia.event[i].eta();
        if ( pythia.event[i].isFinal() && pythia.event[i].id()>0  && pythia.event[i].pT() > 0.1 && abs(eta) <7.8 )        
             {
             ++n_poscharged;
           
             }                               
        
        if ( pythia.event[i].isFinal() && pythia.event[i].id()<0  && pythia.event[i].pT() > 0.1 && abs(eta) <7.8)        
             {
             ++n_negcharged;
          
             } 
      }
    }
    
    mult_charged1->Fill( n_poscharged );   
    mult_charged2->Fill( n_negcharged );
    mult_charged3->Fill(n_poscharged-n_negcharged);

  }//event generation ends here
  
  
    //writing histograms
 ;
    mult_charged1->Write();
    mult_charged2->Write();
    mult_charged3->Write();
    
    
   
      
  delete outFile;
  pythia.stat();   //To obtain statistics on the number of events generated of the different kinds, and the estimated cross sections
  
 return 0;
}

