
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBrowser.h"
#include "TH2.h"
#include "TRandom.h"
#include <TCanvas.h>
#include <iostream>

#include "TVector3.h"
#include "MTrack.h"

using namespace std;

#if defined(__MAKECINT__)
#pragma link C++ class MTrack;
#endif


void Reducer()
{
  gSystem->Load("libactar.sl");

  Bool_t simuFlag;
  Bool_t gasflag;
  Char_t *simname;
  Char_t *digname;
  Char_t *gasname;
      
  //simname="./root_files/simFile_12Cpp12C_D2_80MeV.root";
  //simname="./root_files/simFile_12C_pp_dp_D2_80MeV.root";
  //simname="./root_files/simFile_12C_pp_dp_D2_80MeV_vertex10to128.root";
  //simname="./root_files/simFile_80MeV_12C_aa_a3he_CC_pp_HeiC4h10_9to1_vertex10to128.root";
  //simname="./root_files/simFile_80MeV_12C_aa_aHe3_CC_pp_HeiC4H10_9to1_noAlMAYA.root";
  //simname="./root_files/simFile_80MeV_12C_aa_aHe3_HeiC4H10_9to1.root";
  //simname="./root_files/simFile_80MeV_He4.root";
  //simname="./root_files/simFile_80MeV_13C_He4.root";
  //simname="./root_files/simFile_60MeV_12C_He4.root";
  //simname="./root_files/simFile_60MeV_He4_He4.root";
  //simname="./root_files/simFile_40MeV_He4_He4.root";
  //simname="./root_files/simFile_30MeV_He4_He4.root";
  //simname="./root_files/simFile_40MeV_He3_He3.root";
  //simname="./root_files/simFile_40MeV_He3He3_He4He4.root";
  simname="./root_files/simFile_40MeV_He3He3_He4He4_iongasmod.root";
  //simname="./root_files/simFile_100MeV_He4_He4.root";

  //Event info;
  TFile *simFile=new TFile(simname);

  //getting the trees
  TTree *simTree=(TTree*)simFile->Get("The_ACTAR_Event_Tree");
  Int_t nentries=simTree->GetEntries();
  cout<<"Number of sim event : "<<nentries<<endl;
  //TTree *T=(TTree*)simFile->Get("digiTree");
      
  //ClonesArray to the silicon Hits
  TClonesArray *silHitsCA=new TClonesArray("ActarSimSilHit",200);
  TBranch *branchSilHits=simTree->GetBranch("silHits");
  branchSilHits->SetAddress(&silHitsCA);
  branchSilHits->SetAutoDelete(kTRUE);
  ActarSimSilHit *silHit=new ActarSimSilHit;
      
  //cout<<"digFile to use: ";
  //cin >> digname;

  //digname="./dig_files/digFile_12Cpp12C_D2_80MeV.root";
  //digname="./dig_files/digFile_12C_pp_dp_D2_80MeV.root";
  //digname="./dig_files/digFile_12C_pp_dp_D2_80MeV_vertex10to128.root";
  //digname="./dig_files/digFile_80MeV_12C_aa_a3he_CC_pp_HeiC4h10_9to1_vertex10to128.root";
  //digname="./dig_files/digFile_80MeV_12C_aa_aHe3_CC_pp_HeiC4H10_9to1_noAlMAYA.root";
  //digname="./dig_files/digFile_80MeV_12C_aa_aHe3_HeiC4H10_9to1.root";
  //digname="./dig_files/digFile_80MeV_He4.root";
  //digname="./dig_files/digFile_80MeV_13C_He4.root";
  //digname="./dig_files/digFile_60MeV_12C_He4.root";
  //digname="./dig_files/digFile_60MeV_He4_He4.root";
  //digname="./dig_files/digFile_40MeV_He4_He4.root";
  //digname="./dig_files/digFile_30MeV_He4_He4.root";
  //digname="./dig_files/digFile_40MeV_He3_He3.root";
  //digname="./dig_files/digFile_40MeV_He3He3_He4He4_noPolya.root";
  digname="./dig_files/digFile_40MeV_He3He3_He4He4_iongasmod.root";
  //digname="./dig_files/digFile_100MeV_He4_He4.root";

  // cout<<"Gas: isobutane (1) or deuterium gas (0)? ";
  // cin >> gasflag;
  // if(gasflag==1)gasname="isobutane";
  // if(gasflag==0)gasname="deuterium";

  gROOT->ProcessLine(".L digit_piotr.h+");
  //gROOT->ProcessLine(".L digit.h+");
  
  padsGeometry thePadsGeometry;
  thePadsGeometry.SetGeometryValues(37.,85.,69.,2.,5.,5.);//digit+
  //thePadsGeometry.SetGeometryValues(32.,85.,64.,2.,0.,0.);//digit+
  //thePadsGeometry.SetGeometryValues(0,0,0,32.,85.,64.,100.,2);
  driftManager theDriftManager;

  Char_t dummy[256];
  Char_t gas[256];
  Double_t v_drift,sigma_trans,sigma_long;
  ifstream *gasfile=new ifstream(gasname);
  gasfile->getline(gas,256);
  cout<<gas<<endl;
  gasfile->getline(dummy,256);
  cout<<dummy<<endl;
  *gasfile>>dummy>>dummy>>v_drift;
  //cout<<dummy<<endl;
  *gasfile>>dummy>>dummy>>sigma_trans;
  //cout<<dummy<<endl;
  *gasfile>>dummy>>dummy>>sigma_long;
  //cout<<dummy<<endl;

  //theDriftManager.SetDriftParameters(2015.,170.,147.5,gasname);
  //theDriftManager.SetDriftParameters(2015.,170.,980.66,gasname);

  //Magboltz Drift paramters for Deuterium Gas
  //theDriftManager.SetDriftVelocity(4.7e-3);
  //theDriftManager.SetDiffusionParameters(1.146e-5,2.342e-5);

  //Magboltz Drift paramters for HeiC4H10 (9:1) Gas
  theDriftManager.SetDriftVelocity(9.084e-3);
  theDriftManager.SetDiffusionParameters(2.356e-5,3.105e-5);

  cout<<"Drift Parameters are:"<<endl;  
  cout<<"v_drift---------> "<<theDriftManager.GetDriftVelocity()<<"mm/ns"<<endl;  
  cout<<"D_long----------> "<<theDriftManager.GetLongitudinalDiffusion()<<"mm^2/ns"<<endl;  
  cout<<"D_trans---------> "<<theDriftManager.GetTransversalDiffusion()<<"mm^2/ns"<<endl;  

  Double_t padSize  = thePadsGeometry.GetPadSize();
  Double_t xLength  = thePadsGeometry.GetXLength();
  Double_t yLength  = thePadsGeometry.GetYLength();
  Double_t zLength  = thePadsGeometry.GetZLength();
 
  cout<<"X length===> "<<xLength<<endl;
  cout<<"Y length===> "<<yLength<<endl;
  cout<<"Z length===> "<<zLength<<endl;

  const Int_t numberOfRows   = thePadsGeometry.GetNumberOfRows();
  const Int_t numberOfColumns= thePadsGeometry.GetNumberOfColumns();

  cout<<"Number of Rows: "<<numberOfRows<<", number of Columns: "<<numberOfColumns<<endl;

  Double_t driftVelocity = theDriftManager.GetDriftVelocity();

  //digname="./dig_files/digFile_alpha_z80_ionGas.root";

  TFile *digFile=new TFile(digname);
  cout<<"Opening digitization file: "<<digname<<endl;

  TTree *digiTree=(TTree*)digFile->Get("digiTree");
  Int_t dentries=digiTree->GetEntries();
  cout<<"Number of digit event : "<<dentries<<endl;
  //Int_t digentries=digiTree->GetEntries();
  //cout<<"Number of digit event : "<<digentries<<endl;
 
  //ClonesArray to the signal
  TClonesArray *padSignalCA=new TClonesArray("ActarPadSignal",4000);
  digiTree->SetBranchAddress("padSignals",&padSignalCA);
  ActarPadSignal *padSignal=new ActarPadSignal;

  Double_t PI=3.1415926535897932384626433;
  Double_t deg=180./PI;
  Double_t rad=PI/180.;
  //==================================================================================//

   //read the Tree generated by tree1w and fill two histograms
   
   //note that we use "new" to create the TFile and TTree objects !
   //because we want to keep these objects alive when we leave this function.

   //Flags

   Bool_t ReadWriteFlag=0;// 0: Read, 1: Write

   //Root file to fill
   //TFile *outfile = new TFile("./output/Output_test.root","RECREATE");

   //TFile *outfile = new TFile("./output/Output_100MeV_He4_He4.root","RECREATE");
   TFile *outfile = new TFile("./output/Output_40MeV_He3He3_He4He4_iongasmod.root","RECREATE");
   //TFile *outfile = new TFile("./output/Output_40MeV_He3He3_He4He4_noPolya.root","RECREATE");
   //TFile *outfile = new TFile("./output/Output_40MeV_He3_He3.root","RECREATE");
   //TFile *outfile = new TFile("./output/Output_30MeV_He4_He4.root","RECREATE");
   //TFile *outfile = new TFile("./output/Output_40MeV_He4_He4.root","RECREATE");
   //TFile *outfile = new TFile("./output/Output_60MeV_He4_He4.root","RECREATE");
   //TFile *outfile = new TFile("./output/Output_60MeV_12C_He4.root","RECREATE");
   //TFile *outfile = new TFile("./output/Output_80MeV_13C_He4.root","RECREATE");
   //TFile *outfile = new TFile("./output/Output_80MeV_He4.root","RECREATE");
   //TFile *outfile = new TFile("./output/Output_80MeV_12C_aa_aHe3_HeiC4H10_9to1.root.root","RECREATE");
   //TFile *outfile = new TFile("./output/Sim_Output_v2_39_25MHz_posy43_matrix.root","RECREATE");
   TTree *out_tree = new TTree("out_tree","out_tree");
   

   ///////////////////////////////////////////////

   Double_t Energy_in_silicon=0.;
   Double_t EnergySil;
   Int_t detectorID;
   Double_t SilPosX;
   Double_t SilPosY;
   Double_t SilPosZ;

   Double_t Qtot;

   TSpline3 *bragX;
   TSpline3 *bragY;
   TSpline3 *bragZ;

   Int_t Rmin=0, Rmax=31;
   Int_t Cmin=0, Cmax=63;

   Double_t threshold = 0;
   Double_t Tthreshold = 1.;

   Double_t charge, Ccharge;
   Double_t Rm,Cm,Zcor,Zm;

   //Matrix for the charge map
   Double_t **padCharge=new Double_t*[numberOfRows];
   Double_t **padTime=new Double_t*[numberOfRows];
   Double_t **padHeight=new Double_t*[numberOfRows];
   for(Int_t j=0;j<numberOfRows;j++){
     padCharge[j]=new Double_t[numberOfColumns];
     padTime[j]=new Double_t[numberOfColumns];
   }
   
   //Double_t *padChargeTest=new Double_t[numberOfRows];
   //Double_t *padTimeTest=new Double_t[numberOfRows];

   //TMatrixD *padCharge=0;
   //TMatrixD *padTime=0;
   //TMatrixD *padHeight=0;

   //TMatrixD padCharge(32,64,2000);
   //TMatrixD padTime(32,64,2000);
   //TMatrixD padHeight(32,64,2000);

   //TMatrixD *padChargeTest=0;
   //TMatrixD *padTimeTest=0;
   TMatrixD padChargeTest;
   TMatrixD padTimeTest;

   TMatrixD a(32,64);
   padChargeTest.ResizeTo(a);
   padTimeTest.ResizeTo(a);

   TMatrixD *pointerCharge = &padChargeTest;
   TMatrixD *pointerTime = &padTimeTest;

   //Silicon charge
   //Double_t SilCharge[16];
   //Int_t SilID[16];

   TVectorD SilCharge;
   SilCharge.ResizeTo(16);
   TVectorD SilID;
   SilID.ResizeTo(16);

   TVectorD *pointerSilCharge=&SilCharge;
   TVectorD *pointerSilID=&SilID;
   //Double_t *SilChargePointer=&SilCharge;

   //Branching the out tree
   //out_tree->Branch("PadCharge","TMatrixD",&padCharge,64000,0);
   //out_tree->Branch("PadTime","TMatrixD",&padTime,64000,0);
   //out_tree->Branch("SilCharge",&SilCharge,64000,0);
   out_tree->Branch("PadCharge","TMatrixD",&pointerCharge,64000,0);
   out_tree->Branch("PadTime","TMatrixD",&pointerTime,64000,0);
   out_tree->Branch("SilCharge","TVectorD",&pointerSilCharge,64000,0);
   out_tree->Branch("SilID","TVectorD",&pointerSilID,64000,0);
   //out_tree->Branch("SilCharge",&SilCharge,"energy/D");


   // out_tree->Branch("Energy",&Qtot,"energy/D");
   // out_tree->Branch("HAngle",&HAngle,"angle/D");
   // out_tree->Branch("VAngle",&VAngle,"angle/D");
   // out_tree->Branch("range",&track_range,"rangeProj/D");
   // out_tree->Branch("rangeXY",&track_rangeXY,"rangeProjXY/D");
   // out_tree->Branch("sourceX",&SourcePosX,"sourceX/D");
   // out_tree->Branch("sourceY",&SourcePosY,"sourceY/D");
   // out_tree->Branch("sourceZ",&SourcePosZ,"sourceZ/D");
   // out_tree->Branch("range_calcX",&range_calcX,"range_calcX/D");
   // out_tree->Branch("range_calcY",&range_calcY,"range_calcY/D");
   // out_tree->Branch("range_calcZ",&range_calcZ,"range_calcZ/D");
   
   //Char_t *dname="./dig_files/digFile_alpha_z80_ionGas.root";
   //Char_t *gasname="isobutane";

   //ReadDig(&dname, &gasname);
   //ReadDig(dname, gasname);
   //ReadDig("./dig_files/digFile_alpha_z80_ionGas.root", "isobutane");
   //ReadDig("./dig_files/digFile_alpha_z80_ionGas.root", "isobutane",&&padCharge,&&padTime);
   //ReadDig("./dig_files/digFile_alpha_z80_ionGas.root", "isobutane",padChargeTest,padTimeTest,i);

   //ReadDig("./dig_files/digFile_alpha_z80_ionGas.root", "isobutane",padCharge,padTime);

    //*********************************************************************************************************//
    //*********************************************************************************************************//
    //**                                                                                                     **//
    //**                                        Event Loop                                                   **//
    //**                                                                                                     **//
    //*********************************************************************************************************//
    //*********************************************************************************************************//

   for (Long64_t jentry=0;jentry<nentries;jentry++) {
     //for (Long64_t jentry=0;jentry<1000;jentry++) {
     //for (Long64_t jentry=5000;jentry<nentries;jentry++) {
     //for (Long64_t jentry=15000;jentry<nentries;jentry++) {
     if(jentry%500==0)cout<<jentry<<endl;
     //if(jentry%2==0)cout<<"¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡   NEW (2nd) EVENT : "<<jentry<<"   !!!!!!!!!!!!!!!!!!!!"<<endl;

     if(jentry%2==0)
       {
	 for(Int_t u=0;u<numberOfRows;u++){
	   for(Int_t k=0;k<numberOfColumns;k++){
	     padChargeTest(u,k)=0.;
	     padTimeTest(u,k)=0.;
	   }
	 }
       }

     silHitsCA->Clear();
     simTree->GetEvent(jentry);

     Int_t nbsilicon= silHitsCA->GetEntries();
     //cout<<" SILICON "<<nbsilicon<<endl;
     
     //if(simuFlag==1)ReadDig(padSignalCA,padSignal,padCharge,padTime);
     //if(simuFlag==1)ReadDig(digFile,padCharge,padTime);

     padSignalCA->Clear();

     digiTree->GetEvent(jentry);

     Int_t numberofpads=padSignalCA->GetEntries();
     //cout<<"# Pads fired "<< numberofpads<<endl;

     if(numberofpads>0){
       for(Int_t k=0;k<numberofpads;k++){
	 padSignal=(ActarPadSignal*)padSignalCA->At(k);
	 Double_t thisCharge = padSignal->GetChargeDeposited();
	 Double_t thisTime   = (padSignal->GetInitTime()+padSignal->GetFinalTime())/2.;
	 Double_t thisSigmaTime=padSignal->GetSigmaTime();
	 Int_t PadRow=padSignal->GetPadRow();
	 Int_t PadColumn=padSignal->GetPadColumn();
	 //if(k%20==0)cout<<"thisSigmaTime: "<<thisSigmaTime<<endl;
	 if (thisCharge!=0){

	   padChargeTest(padSignal->GetPadRow()-1,padSignal->GetPadColumn()-1)+= thisCharge;
	   padTimeTest(padSignal->GetPadRow()-1,padSignal->GetPadColumn()-1)+= thisCharge*thisTime;

	   // padCharge[padSignal->GetPadRow()-1][padSignal->GetPadColumn()-1]
	   //   += thisCharge;
	   // padTime[padSignal->GetPadRow()-1][padSignal->GetPadColumn()-1]
	   //   += thisCharge*thisTime;
	 }
       }//Loop on ActarPadSignals
	 

       if(jentry%2==1)//To have the 2 parts of the reaction
	 {
	   // Loop on rows & columns
	   for(Int_t c=0;c<numberOfColumns;c++){ //LOOP on Columns
	     for(Int_t r=0;r<numberOfRows;r++){ //LOOP on Rows
	       padTimeTest(r,c)=padTimeTest(r,c)/padChargeTest(r,c);
	       //padTime[r][c]=padTime[r][c]/padCharge[r][c];
	       //padTime[r][c]=gRandom->Gaus(padTime[r][c],sigma_time);
	     }
	   }//End of Loop on rows & columns
	 
	 }

     }



     if(jentry%2!=0){
 
       ///////////Getting the Silicon Hits////////////////

       for(Int_t s=0;s<16;s++){//Loop on the 12 MAYA Si & 4 DSSD
	 //SilCharge[s]=0.;
	 SilCharge(s)=0.;
	 SilID(s)=0.;
       }

       Int_t SilHitID;
       Energy_in_silicon=0.;
       Int_t nbsiliconhits=silHitsCA->GetEntries();
       //cout<<"SilHits->"<<nbsiliconhits<<endl;
       if(nbsiliconhits>2)cout<<"SilHits-> "<<nbsiliconhits<<" Event "<<jentry<<endl;

       if(nbsiliconhits>0){

	 for(Int_t h=0;h<nbsiliconhits;h++){

	   SilHitID=0;
	   silHit=(ActarSimSilHit*)silHitsCA->At(h);
	   //cout<<"SilHits->ParticleID: "<< silHit->GetParticleID()<<", SilHits->ParticleMass: "<< silHit->GetParticleMass()<<", SilHits->DetectorID: "<<silHit->GetDetectorID()<<endl;
	   EnergySil=silHit->GetEnergy();
	   Energy_in_silicon=EnergySil;
	   //if(silHit->GetDetectorID()<5)Energy_in_silicon=gRandom->Gaus(EnergySil,0.004*EnergySil);
	   //else if(silHit->GetDetectorID()>4)Energy_in_silicon=gRandom->Gaus(EnergySil,0.00167*EnergySil);
	   //Energy_in_silicon=gRandom->Gaus(EnergySil,0.008*EnergySil);
	   detectorID=silHit->GetDetectorID();
	   SilPosX=silHit->GetXPos();
	   SilPosY=silHit->GetYPos();
	   SilPosZ=silHit->GetXPos();
	   //cout<<"Detector "<< detectorID<<" hit Energy "<<EnergySil<<" Energy Before "<<silHit->GetEBeforeSil()<<" Energy After "<<silHit->GetEAfterSil()<<" Energy in Silicium "<<Energy_in_silicon<<endl;
	   //cout<<"Energy in silicon: "<<Energy_in_silicon<<endl;
	   //SilCharge[detectorID-1]=Energy_in_silicon;  	    
	   SilCharge(detectorID-1)=Energy_in_silicon;  
	   //SilID(detectorID-1)=silHit->GetDetectorID();
	   if(silHit->GetParticleID()==2212)SilID(detectorID-1)=1;//p
	   else if(silHit->GetParticleID()==1000020030)SilID(detectorID-1)=2;//3He
	   else if(silHit->GetParticleID()==1000020040)SilID(detectorID-1)=3;//4He
	   else if(silHit->GetParticleID()==1000060120)SilID(detectorID-1)=4;//12C
	   else if(silHit->GetParticleID()==1000060130)SilID(detectorID-1)=5;//13C

	   //break;
	   //cout<<" SilFront "<<SilFront<<" SilLeft "<<SilLeft<<" SilRight "<<SilRight<<endl;

	 }
       }

       if(nbsiliconhits!=0)out_tree->Fill();
   
     }

   }

   out_tree->Write();
   outfile->Close();
    //*********************************************************************************************************//
    //*********************************************************************************************************//
    //**                                                                                                     **//
    //**                                       End of Event Loop                                             **//
    //**                                                                                                     **//
    //*********************************************************************************************************//
    //*********************************************************************************************************//


} 

void FindMax(TSpline3 *sp,Double_t *maximum,Double_t *x){
  Double_t max;
  Double_t posmax=0;
  Double_t x0=0;
  Double_t xmax=32;
  Double_t step=0.2;
  for(Double_t val=x0;val<xmax;val=val+step){
    Double_t value=sp->Eval(val);
    if(value>max){
      max=value;
      posmax=val;
    }
    //cout<<val<<" "<<value<<endl;
  }
  // cout<<"Maximum is at--> "<<posmax<<" "<<max<<endl;
  *maximum=max;
  *x=posmax;
  return; 
}

void FindRangeX(TSpline3 *sp,Double_t maxval,Double_t maxpos,Double_t theta,Double_t *x){
  Double_t max;
  Double_t posmax=0;
  Double_t x0=maxpos;
  Double_t xmax=128;
  Double_t step=0.2;
  if(theta>0){
    for(Double_t val=x0;val<xmax;val=val+step){
    Double_t value=sp->Eval(val);
    if(value>=maxval/4){
      max=value;
      posmax=val;
    }
    }
  }

  else if(theta<0){
    for(Double_t val=x0;val>0;val=val-step){
    Double_t value=sp->Eval(val);
    if(value>=maxval/4){
      max=value;
      posmax=val;
    }
    }

  }
  //cout<<"X Range is at--> "<<posmax<<" "<<max<<endl;
   //*maximum=max;
  *x=posmax;
  return; 
}

void FindRangeY(TSpline3 *sp,Double_t maxval,Double_t maxpos,Double_t *x){
  Double_t max;
  Double_t posmax=0;
  Double_t x0=maxpos;
  Double_t xmax=64;
  Double_t step=0.2;
  for(Double_t val=x0;val<xmax;val=val+step){
    Double_t value=sp->Eval(val);
    if(value>=maxval/4){
      max=value;
      posmax=val;
    }
 
  }
  //cout<<"X Range is at--> "<<posmax<<" "<<max<<endl;
   //*maximum=max;
  *x=posmax;
  return; 
}

void FindRangeZ(TSpline3 *sp,Double_t maxval,Double_t maxpos,Double_t phi,Double_t maxZ, Double_t *x){
  Double_t max;
  Double_t posmax=0;
  Double_t x0=maxpos;
  Double_t xmax=maxZ;
  //Double_t xmax=170;
  Double_t step=0.2;
  if(phi>0){
    for(Double_t val=x0;val<xmax;val=val+step){
      Double_t value=sp->Eval(val);
      if(value>=maxval/4){
	max=value;
	posmax=val;
      }
    }
  }

  else if(phi<0){
    for(Double_t val=x0;val>0;val=val-step){
      Double_t value=sp->Eval(val);
      if(value>=maxval/4){
	max=value;
	posmax=val;
      }
    }

  }
  //cout<<"Y Range is at--> "<<posmax<<" "<<max<<endl;
   //*maximum=max;
  *x=posmax;
  return; 
}


void FitStep(Int_t nstep, Double_t *x, Double_t *z, Double_t &a, Double_t &b)
{
	Int_t s;
	Double_t A, B, C, UEV, Q, X, Xg, Y, Yg;
	A=B=C=UEV=Q=X=Y=Xg=Yg=0.;
	
	for (Int_t s=0;s<nstep;s++)
	  {
	    Q+=1;
	    Xg+=z[s];
	    Yg+=x[s];
	  }

	Xg/=Q;
	Yg/=Q;
	//cout<<"Xg sim : "<<Xg<<" Yg sim : "<<Yg<<endl;


	for (Int_t s=0;s<nstep;s++)
	  {
	    A+=(z[s]-Xg)*(z[s]-Xg);
	    B+=(x[s]-Yg)*(z[s]-Xg);
	    C+=(x[s]-Yg)*(x[s]-Yg);
	  }

	UEV=0.5*(A+C+sqrt((A+C)*(A+C)-4*(A*C-B*B)));
	a=B/(UEV-C);
	b=Yg-a*Xg;
}


void FitMat(Double_t **PADNET, Int_t Rmin, Int_t Rmax, Int_t Cmin, Int_t Cmax, Double_t threshold, Double_t &a, Double_t &b)
{
	Int_t Row, Col;
	Double_t A, B, C, UEV, Q, X, Xg, Y, Yg;
	A=B=C=UEV=Q=X=Y=Xg=Yg=0.;
	for (Row=Rmin;Row<=Rmax;Row++)
		for (Col=Cmin;Col<=Cmax;Col++)
			if(PADNET[Row][Col]>threshold)
			{
			  X= Col*2.+1.;
			  Y= Row*2.+1.;
			  Q+=PADNET[Row][Col];
			  Xg+=X*PADNET[Row][Col];
			  Yg+=Y*PADNET[Row][Col];
			}
	Xg/=Q;
	Yg/=Q;
	//cout<<"Xg: "<<Xg<<" Yg: "<<Yg<<endl;

	for (Row=Rmin;Row<=Rmax;Row++)
		for(Col=Cmin;Col<=Cmax;Col++)
			if(PADNET[Row][Col]>threshold)
			{
				X= Col*2.+1.;
				Y= Row*2.+1.;
				A+=PADNET[Row][Col]*(X-Xg)*(X-Xg);
				B+=PADNET[Row][Col]*(X-Xg)*(Y-Yg);
				C+=PADNET[Row][Col]*(Y-Yg)*(Y-Yg);
			}

	UEV=0.5*(A+C+TMath::Sqrt((A+C)*(A+C)-4*(A*C-B*B)));
	a=B/(UEV-C);
	b=Yg-a*Xg;

}


  void FitMatZ(Double_t **PADNET, Double_t **TIME, Int_t Rmin, Int_t Rmax, Int_t Cmin, Int_t Cmax, Double_t threshold, Double_t Tthreshold, Double_t &a, Double_t &b)
{
	Int_t Row, Col;
	const Int_t NRow=32;
        Double_t A, B, C, UEV, Q, X, Xg, Y, Yg;
        Double_t Qrow[NRow],Cm[NRow],Zm[NRow];
	Int_t min_col,max_col,ncol;
	//cout<<"Ttreshold: "<<Tthreshold<<", NRow: "<<NRow<<endl;
	A=B=C=UEV=Q=X=Y=Xg=Yg=0.;
	for (Row=Rmin;Row<=Rmax;Row++)
	  {
	    Zm[Row]=0;
	    Qrow[Row]=0;
	    Cm[Row]=0.;
	    min_col=max_col=ncol=0;
	    for (Col=Cmin;Col<=Cmax;Col++)
	      {
		if(PADNET[Row][Col]>threshold && TIME[Row][Col]>Tthreshold)
		  {	
		    ncol++;
		    if(min_col==0)min_col=Col;
		    max_col=Col;
		    Cm[Row]+=PADNET[Row][Col]*(Col+1);//+1 to avoid problem at Col=0
		    Qrow[Row]+=PADNET[Row][Col];
		    //cout<<"Row: "<<Row<<", minC: "<<min_col<<", maxC: "<<max_col<<", PAD: "<<PADNET[Row][Col]<<", NRow: "<<NRow<<endl;
		  }
	      }

	    if(ncol!=0)
	      {
		Cm[Row]/=Qrow[Row];
		Cm[Row]-=1.;	   
		for(Col=min_col;Col<max_col+1;Col++)
		  {
		    if(PADNET[Row][Col]>threshold && TIME[Row][Col]>Tthreshold)
		      {
			Zm[Row]+=TMath::Sqrt(TIME[Row][Col]*TIME[Row][Col]-4*(Col-Cm[Row])*(Col-Cm[Row]));	  
		      }
		  }
		Zm[Row]/=ncol; 
		//cout<<"Row: "<<Row<<", QCol: "<<Qrow[Row]<<", ZmCol: "<<Zm[Row]<<endl;
	      }
	  }
	

	for (Row=Rmin;Row<=Rmax;Row++)
	  {

	    if(Cm[Row] && Zm[Row])
	      {
		X= Row*2.+1.;
		Y= Zm[Row];//*2.;
		Q+=Qrow[Row];
		Xg+=X*Qrow[Row];
		Yg+=Y*Qrow[Row];
		//cout<<"Row: "<<Row<<",Zm: "<<Zm[Row]<<",Cm: "<<Cm[Row]<<endl;
	      }


	  }

	Xg/=Q;
	Yg/=Q;
	//cout<<"Xg: "<<Xg<<",Yg: "<<Yg<<endl;

	for (Row=Rmin;Row<=Rmax;Row++)
	  {
	    if(Cm[Row] && Zm[Row])
	      {
		X= Row*2.+1.;
		Y= Zm[Row];//*2.;
		A+=Qrow[Row]*(X-Xg)*(X-Xg);
		B+=Qrow[Row]*(X-Xg)*(Y-Yg);
		C+=Qrow[Row]*(Y-Yg)*(Y-Yg);
	      }
	  }

	UEV=0.5*(A+C+TMath::Sqrt((A+C)*(A+C)-4*(A*C-B*B)));
	a=B/(UEV-C);
	b=Yg-a*Xg;
}

//Double_t FitMat3D(Int_t T, TMatrixD PADNET, TMatrixD TIME, Int_t Rmin, Int_t Rmax, Int_t Cmin, Int_t Cmax, Double_t threshold, MTrack* T)
Double_t FitMat3D(Double_t **PADNET, Double_t **HEIGHT, Int_t Rmin, Int_t Rmax, Int_t Cmin, Int_t Cmax, Double_t threshold, MTrack* T)
{
	// adapted from: http://fr.scribd.com/doc/31477970/Regressions-et-trajectoires-3D
	Int_t R, C;
	Double_t Q,X,Y,Z;
	Double_t Xm,Ym,Zm;
	Double_t Sxx,Sxy,Syy,Sxz,Szz,Syz;
	Double_t theta;
	Double_t K11,K22,K12,K10,K01,K00;
	Double_t c0,c1,c2;
	Double_t p,q,r,dm2;
	Double_t rho,phi;
	Double_t a,b;
	
	Double_t PI=3.1415926535897932384626433;

	Q=Xm=Ym=Zm=0.;
	Sxx=Syy=Szz=Sxy=Sxz=Syz=0.;
	
	for (R=Rmin;R<=Rmax;R++)
		for (C=Cmin;C<=Cmax;C++)
			if(PADNET[R][C]>threshold && HEIGHT[R][C])
			{
			  X= C*2.+1.;
			  Y= R*2.+1.;
			  Z= HEIGHT[R][C];//*VDRIFT;
			  Q+=PADNET[R][C];
			  Xm+=X*PADNET[R][C];
			  Ym+=Y*PADNET[R][C];
			  Zm+=Z*PADNET[R][C];
			  Sxx+=X*X*PADNET[R][C];
			  Syy+=Y*Y*PADNET[R][C];
			  Szz+=Z*Z*PADNET[R][C];
			  Sxy+=X*Y*PADNET[R][C];
			  Sxz+=X*Z*PADNET[R][C];
			  Syz+=Y*Z*PADNET[R][C];
			}
	Xm/=Q;
	Ym/=Q;
	Zm/=Q;
	Sxx/=Q;
	Syy/=Q;
	Szz/=Q;
	Sxy/=Q;
	Sxz/=Q;
	Syz/=Q;
	Sxx-=(Xm*Xm);
	Syy-=(Ym*Ym);
	Szz-=(Zm*Zm);
	Sxy-=(Xm*Ym);
	Sxz-=(Xm*Zm);
	Syz-=(Ym*Zm);
	
	theta=0.5*atan((2.*Sxy)/(Sxx-Syy));
	
	K11=(Syy+Szz)*pow(cos(theta),2)+(Sxx+Szz)*pow(sin(theta),2)-2.*Sxy*cos(theta)*sin(theta);
	K22=(Syy+Szz)*pow(sin(theta),2)+(Sxx+Szz)*pow(cos(theta),2)+2.*Sxy*cos(theta)*sin(theta);
	K12=-Sxy*(pow(cos(theta),2)-pow(sin(theta),2))+(Sxx-Syy)*cos(theta)*sin(theta);
	K10=Sxz*cos(theta)+Syz*sin(theta);
	K01=-Sxz*sin(theta)+Syz*cos(theta);
	K00=Sxx+Syy;
	
	c2=-K00-K11-K22;
	c1=K00*K11+K00*K22+K11*K22-K01*K01-K10*K10;
	c0=K01*K01*K11+K10*K10*K22-K00*K11*K22;
		
	p=c1-pow(c2,2)/3.;
	q=2.*pow(c2,3)/27.-c1*c2/3.+c0;
	r=pow(q/2.,2)+pow(p,3)/27.;
	
	if(r>0) {dm2=-c2/3.+pow(-q/2.+sqrt(r),1./3.)+pow(-q/2.-sqrt(r),1./3.);cout<<"R>0"<<endl;}
	if(r<0)
	{
	  //cout<<"R<0"<<endl;
	  rho=sqrt(-pow(p,3)/27.);
	  phi=acos(-q/(2.*rho));
	  //dm2=min(-c2/3.+2.*pow(rho,1./3.)*cos(phi/3.),min(-c2/3.+2.*pow(rho,1./3.)*cos((phi+2.*PI)/3.),-c2/3.+2.*pow(rho,1./3.)*cos((phi+4.*PI)/3.)));
	  dm2=TMath::Min(-c2/3.+2.*pow(rho,1./3.)*cos(phi/3.),TMath::Min(-c2/3.+2.*pow(rho,1./3.)*cos((phi+2.*PI)/3.),-c2/3.+2.*pow(rho,1./3.)*cos((phi+4.*PI)/3.)));
	}
	a=-K10*cos(theta)/(K11-dm2)+K01*sin(theta)/(K22-dm2);
	b=-K10*sin(theta)/(K11-dm2)-K01*cos(theta)/(K22-dm2);

	T->Xm=Xm;
	T->Ym=Ym;
	T->Zm=Zm;
	T->Xh=((1.+b*b)*Xm-a*b*Ym+a*Zm)/(1.+a*a+b*b);
	T->Yh=((1.+a*a)*Ym-a*b*Xm+b*Zm)/(1.+a*a+b*b);
	T->Zh=((a*a+b*b)*Zm+a*Xm+b*Ym)/(1.+a*a+b*b);
	
	T->L2DXY->SetX1(T->Xm);
	T->L2DXY->SetY1(T->Ym);
	T->L2DXY->SetX2(T->Xh);
	T->L2DXY->SetY2(T->Yh);
	
	T->L2DXZ->SetX1(T->Xm);
	T->L2DXZ->SetY1(T->Zm);
	T->L2DXZ->SetX2(T->Xh);
	T->L2DXZ->SetY2(T->Zh);
	
	T->L2DYZ->SetX1(T->Ym);
	T->L2DYZ->SetY1(T->Zm);
	T->L2DYZ->SetX2(T->Yh);
	T->L2DYZ->SetY2(T->Zh);
	
	return(dm2/Q);
}

void HeightCorrection(Double_t **PADNET, Double_t **HEIGHT, Int_t Rmin, Int_t Rmax, Int_t Cmin, Int_t Cmax, Double_t threshold, Double_t Tthreshold)
{

  Int_t Row, Col;
  const Int_t NRow=32;
  Double_t Qrow[NRow],Cm[NRow],Zm[NRow];
  Int_t min_col,max_col,ncol;

  for (Row=Rmin;Row<=Rmax;Row++)
    {
      Zm[Row]=0;
      Qrow[Row]=0;
      Cm[Row]=0.;
      min_col=max_col=ncol=0;

      for (Col=Cmin;Col<=Cmax;Col++)
	{
	  if(PADNET[Row][Col]>threshold && HEIGHT[Row][Col]>Tthreshold)
	    {	
	      ncol++;
	      if(min_col==0)min_col=Col;
	      max_col=Col;
	      Cm[Row]+=PADNET[Row][Col]*(Col+1);//+1 to avoid problem at Col=0
	      Qrow[Row]+=PADNET[Row][Col];
	      //cout<<"Row: "<<Row<<", minC: "<<min_col<<", maxC: "<<max_col<<", PAD: "<<PADNET[Row][Col]<<", NRow: "<<NRow<<endl;
	    }
	}

      if(ncol!=0)
	{
	  Cm[Row]/=Qrow[Row];
	  Cm[Row]-=1.;	   
	  for(Col=min_col;Col<max_col+1;Col++)
	    {
	      if(PADNET[Row][Col]>threshold && HEIGHT[Row][Col]>Tthreshold)
		{
		  Zm[Row]+=TMath::Sqrt(HEIGHT[Row][Col]*HEIGHT[Row][Col]-4*(Col-Cm[Row])*(Col-Cm[Row]));	  
		}
	    }

	  Zm[Row]/=ncol; 
	  //cout<<"Row: "<<Row<<", QCol: "<<Qrow[Row]<<", ZmCol: "<<Zm[Row]<<endl;
	  for(Col=min_col;Col<max_col+1;Col++)
	    {
	      if(PADNET[Row][Col]>threshold && HEIGHT[Row][Col]>Tthreshold)
		{
		  HEIGHT[Row][Col]=Zm[Row];	  
		}
	    }

	}
    }

}
