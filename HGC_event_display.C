#include <TH3F.h>
#include <TH2F.h>
#include <TChain.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>

using namespace std;

void display3D_tc(TString file, bool JB_version = true,
		  int n_event=0, 
		  const vector<int>& tc= vector<int>(),
		  const vector<int>& cl= vector<int>(),
		  const vector<int>& c3d= vector<int>(),
		  const vector<int>& jet= vector<int>(),		  
		  float eta_min=-3, float eta_max=3, 
		  float phi_min=-3.2, float phi_max=3.2, 
		  float layer_min=0, float layer_max=60){

  TH3F* h=new TH3F("h","h",50,eta_min,eta_max,50,phi_min,phi_max,layer_max-layer_min,layer_min,layer_max);

  TChain * tree = new TChain("HGCalTriggerNtuple");
  tree->Add(file);
  TChain * tree_jet = new TChain("jets");
  tree_jet->Add(file);
  
  vector<float> *_tc_eta;
  vector<float> *_tc_phi;
  vector<float> *_tc_energy;
  vector<float> *_tc_pt;
  vector<int> *_tc_HGClayer;
  vector<unsigned int> *_tc_id;

  vector<float> *_cl_eta;
  vector<float> *_cl_phi;
  vector<float> *_cl_energy;
  vector<float> *_cl_pt;
  vector<int> *_cl_HGClayer;

  vector<float> *_cl3d_eta;
  vector<float> *_cl3d_phi;
  vector<float> *_cl3d_energy;
  vector<float> *_cl3d_pt;
  
  vector<float> *_jets_eta;
  vector<float> *_jets_phi;
  vector<float> *_jets_energy;
  vector<float> *_jets_pt;

  vector<unsigned int> *_cl_id;
  vector<vector<int> > *_cl_cells;
  vector<vector<int> > *_cl3d_clusters;
  vector<vector<int> > *_jets_cl3d;

  tree->SetBranchAddress("tc_eta",    &_tc_eta);
  tree->SetBranchAddress("tc_phi",    &_tc_phi);
  tree->SetBranchAddress("tc_energy", &_tc_energy);
  tree->SetBranchAddress("tc_pt",    &_tc_pt);
  tree->SetBranchAddress("tc_HGClayer", &_tc_HGClayer);
  if(JB_version)
    tree->SetBranchAddress("tc_id", &_tc_id);

  tree->SetBranchAddress("cl_eta",    &_cl_eta);
  tree->SetBranchAddress("cl_phi",    &_cl_phi);
  tree->SetBranchAddress("cl_energy", &_cl_energy);
  tree->SetBranchAddress("cl_pt",     &_cl_pt);
  tree->SetBranchAddress("cl_HGClayer",    &_cl_HGClayer);

  tree->SetBranchAddress("cl3d_eta",    &_cl3d_eta);
  tree->SetBranchAddress("cl3d_phi",    &_cl3d_phi);
  tree->SetBranchAddress("cl3d_energy", &_cl3d_energy);
  tree->SetBranchAddress("cl3d_pt",     &_cl3d_pt);

  tree_jet->SetBranchAddress("jets_eta",    &_jets_eta);
  tree_jet->SetBranchAddress("jets_phi",    &_jets_phi);
  tree_jet->SetBranchAddress("jets_energy", &_jets_energy);
  tree_jet->SetBranchAddress("jets_pt",     &_jets_pt);

  tree->SetBranchAddress("cl_cells", &_cl_cells);
  if(JB_version)
    tree->SetBranchAddress("cl_id", &_cl_id);
  tree->SetBranchAddress("cl3d_clusters", &_cl3d_clusters);
  tree_jet->SetBranchAddress("jets_cl3d",&_jets_cl3d);

  _tc_eta = 0;
  _tc_phi = 0;
  _tc_energy = 0;
  _tc_pt = 0;
  _tc_HGClayer = 0;
  _tc_id = 0;
  
  _cl_eta = 0;
  _cl_phi = 0;
  _cl_energy = 0;
  _cl_pt = 0;
  _cl_HGClayer = 0;

  _cl3d_eta = 0;
  _cl3d_phi = 0;
  _cl3d_energy = 0;
  _cl3d_pt = 0;

  _jets_eta = 0;
  _jets_phi = 0;
  _jets_energy = 0;
  _jets_pt = 0;

  _cl_cells = 0;
  _cl_id = 0;
  _cl3d_clusters = 0;
  _jets_cl3d = 0;
  
  tree->GetEntry(n_event);
  tree_jet->GetEntry(n_event);
  
  map<unsigned int, unsigned int> tc_map; //First ID, Second index
  map<unsigned int, unsigned int> cl_map; //First ID, Second index

  for(unsigned int i_tc = 0; i_tc<(*_tc_eta).size(); i_tc++){
      if(JB_version)
	tc_map[(*_tc_id)[i_tc]] = i_tc;
      else
	tc_map[i_tc] = i_tc;
  }

  for(unsigned int i_cl = 0; i_cl<(*_cl_eta).size(); i_cl++){
      if(JB_version)
	cl_map[(*_cl_id)[i_cl]] = i_cl;
      else
	cl_map[i_cl] = i_cl;
  }


  if(tc.size()>0){

    if(tc[0]<0){
      for(unsigned int i_tc = 0; i_tc<(*_tc_eta).size(); i_tc++){
	h->Fill((*_tc_eta)[i_tc],(*_tc_phi)[i_tc],(*_tc_HGClayer)[i_tc],(*_tc_pt)[i_tc]);
      }
    }

    else{
      for(auto & i_tc : tc){
	h->Fill((*_tc_eta)[i_tc],(*_tc_phi)[i_tc],(*_tc_HGClayer)[i_tc],(*_tc_pt)[i_tc]);
	cout<<"Tower "<<i_tc<<endl;
	cout<<"(pt,eta,phi,layer)=("<<(*_tc_pt)[i_tc]<<","<<(*_tc_eta)[i_tc]<<","<<(*_tc_phi)[i_tc]<<","<<(*_tc_HGClayer)[i_tc]<<")"<<endl;
      }
    }

  }


  else if(cl.size()>0){
    for(auto & i_cl : cl){    
      cout<<"C2D "<<i_cl<<" (pt,eta,phi,layer)=("<<(*_cl_pt)[i_cl]<<","<<(*_cl_eta)[i_cl]<<","<<(*_cl_phi)[i_cl]<<","<<(*_cl_HGClayer)[i_cl]<<")"<<endl;      
      for(auto & i_tc : (*_cl_cells)[i_cl]){
	int tc_index = tc_map[i_tc];
	h->Fill((*_tc_eta)[tc_index],(*_tc_phi)[tc_index],(*_tc_HGClayer)[tc_index],(*_tc_pt)[tc_index]);
	cout<<"(pt,eta,phi,layer)=("<<(*_tc_pt)[tc_index]<<","<<(*_tc_eta)[tc_index]<<","<<(*_tc_phi)[tc_index]<<","<<(*_tc_HGClayer)[tc_index]<<")"<<endl;
      }
    }
  }

  else if(c3d.size()>0){
    for(auto & i_c3d : c3d){
      cout<<"C3D "<<i_c3d<<" (pt,eta,phi)=("<<(*_cl3d_pt)[i_c3d]<<","<<(*_cl3d_eta)[i_c3d]<<","<<(*_cl3d_phi)[i_c3d]<<")"<<endl;
      for(auto & i_cl : (*_cl3d_clusters)[i_c3d]){
	int cl_index = cl_map[i_cl];	
	cout<<"cl_index="<<cl_index<<endl;
	for(auto & i_tc : (*_cl_cells)[cl_index]){
	  int tc_index = tc_map[i_tc];
	  h->Fill((*_tc_eta)[tc_index],(*_tc_phi)[tc_index],(*_tc_HGClayer)[tc_index],(*_tc_pt)[tc_index]);
	  cout<<"(pt,eta,phi,layer)=("<<(*_tc_pt)[tc_index]<<","<<(*_tc_eta)[tc_index]<<","<<(*_tc_phi)[tc_index]<<","<<(*_tc_HGClayer)[tc_index]<<")"<<endl;
	}
      }
    }
  }
  
  else if(jet.size()>0){
    for(auto & i_jet : jet){
      cout<<"jet "<<i_jet<<" (pt,eta,phi)=("<<(*_jets_pt)[i_jet]<<","<<(*_jets_eta)[i_jet]<<","<<(*_jets_phi)[i_jet]<<")"<<endl;
      for(auto & i_c3d : (*_jets_cl3d)[i_jet]){
	for(auto & i_cl : (*_cl3d_clusters)[i_c3d]){
	  int cl_index = cl_map[i_cl];		  
	  for(auto & i_tc : (*_cl_cells)[cl_index]){
	    int tc_index = tc_map[i_tc];
	    h->Fill((*_tc_eta)[tc_index],(*_tc_phi)[tc_index],(*_tc_HGClayer)[tc_index],(*_tc_pt)[tc_index]);
	    cout<<"(pt,eta,phi,layer)=("<<(*_tc_pt)[tc_index]<<","<<(*_tc_eta)[tc_index]<<","<<(*_tc_phi)[tc_index]<<","<<(*_tc_HGClayer)[tc_index]<<")"<<endl;
	  }
	}
      }
    }
  }


  h->GetXaxis()->SetTitle("#eta(TC)");
  h->GetXaxis()->SetTitleOffset(1.5);
  h->GetYaxis()->SetTitle("#phi(TC)");
  h->GetZaxis()->SetTitle("layer(TC)");
  h->Draw("BOX2");



}






void display3D_cl(TString file, bool JB_version = true,
		  int n_event=0, 
		  const vector<int>& cl= vector<int>(),
		  const vector<int>& c3d= vector<int>(),
		  const vector<int>& jet= vector<int>(),
		  float eta_min=-3, float eta_max=3, 
		  float phi_min=-3.2, float phi_max=3.2, 
		  float layer_min=0, float layer_max=60){

 

  TH3F* h=new TH3F("h","h",50,eta_min,eta_max,50,phi_min,phi_max,layer_max-layer_min,layer_min,layer_max);

  TChain * tree = new TChain("HGCalTriggerNtuple");
  tree->Add(file);
  
  vector<float> *_cl_eta;
  vector<float> *_cl_phi;
  vector<float> *_cl_energy;
  vector<float> *_cl_pt;
  vector<int> *_cl_HGClayer;
  vector<unsigned int> *_cl_id;

  vector<float> *_cl3d_eta;
  vector<float> *_cl3d_phi;
  vector<float> *_cl3d_energy;
  vector<float> *_cl3d_pt;
  
  vector<float> *_jets_eta;
  vector<float> *_jets_phi;
  vector<float> *_jets_energy;
  vector<float> *_jets_pt;

  vector<vector<int> > *_cl3d_clusters;
  vector<vector<int> > *_jets_cl3d;

  tree->SetBranchAddress("cl_eta",    &_cl_eta);
  tree->SetBranchAddress("cl_phi",    &_cl_phi);
  tree->SetBranchAddress("cl_energy", &_cl_energy);
  tree->SetBranchAddress("cl_HGClayer", &_cl_HGClayer);
  tree->SetBranchAddress("cl_pt", &_cl_pt);
  if(JB_version)
    tree->SetBranchAddress("cl_id", &_cl_id);

  tree->SetBranchAddress("cl3d_eta",    &_cl3d_eta);
  tree->SetBranchAddress("cl3d_phi",    &_cl3d_phi);
  tree->SetBranchAddress("cl3d_energy", &_cl3d_energy);
  tree->SetBranchAddress("cl3d_pt",     &_cl3d_pt);

  tree->SetBranchAddress("jets_eta",    &_jets_eta);
  tree->SetBranchAddress("jets_phi",    &_jets_phi);
  tree->SetBranchAddress("jets_energy", &_jets_energy);
  tree->SetBranchAddress("jets_pt",     &_jets_pt);
      
  tree->SetBranchAddress("cl3d_clusters", &_cl3d_clusters);
  tree->SetBranchAddress("jets_cl3d",&_jets_cl3d);

  _cl_eta = 0;
  _cl_phi = 0;
  _cl_energy = 0;
  _cl_pt = 0;
  _cl_HGClayer = 0;
  _cl_id = 0;
  
  _cl3d_eta = 0;
  _cl3d_phi = 0;
  _cl3d_energy = 0;
  _cl3d_pt = 0;

  _jets_eta = 0;
  _jets_phi = 0;
  _jets_energy = 0;
  _jets_pt = 0;

  _cl3d_clusters = 0;
  _jets_cl3d = 0;
  
  cout<<"ok0"<<endl;

  tree->GetEntry(n_event);

  cout<<"ok1"<<endl;

  map<unsigned int, unsigned int> cl_map; //First ID, Second index

  for(unsigned int i_cl = 0; i_cl<(*_cl_eta).size(); i_cl++){
      if(JB_version)
	cl_map[(*_cl_id)[i_cl]] = i_cl;
      else
	cl_map[i_cl] = i_cl;
  }
  
  cout<<"ok2"<<endl;

  
  if(cl.size()>0){
    

    if(cl[0]<0){
      for(unsigned int i_cl = 0; i_cl<(*_cl_eta).size(); i_cl++){
	h->Fill((*_cl_eta)[i_cl],(*_cl_phi)[i_cl],(*_cl_HGClayer)[i_cl],(*_cl_pt)[i_cl]);
	if((*_cl_eta)[i_cl]>=eta_min && (*_cl_eta)[i_cl]<=eta_max && (*_cl_phi)[i_cl]>=phi_min && (*_cl_phi)[i_cl]<=phi_max && (*_cl_HGClayer)[i_cl]>=layer_min && (*_cl_HGClayer)[i_cl]<=layer_max)
	  cout<<"(pt,eta,phi,layer)=("<<(*_cl_pt)[i_cl]<<","<<(*_cl_eta)[i_cl]<<","<<(*_cl_phi)[i_cl]<<","<<(*_cl_HGClayer)[i_cl]<<")"<<endl;
      }
    }

    else{
      for(auto & i_cl : cl){
	h->Fill((*_cl_eta)[i_cl],(*_cl_phi)[i_cl],(*_cl_HGClayer)[i_cl],(*_cl_pt)[i_cl]);
	cout<<"C2D "<<i_cl<<endl;
	cout<<"(pt,eta,phi,layer)=("<<(*_cl_pt)[i_cl]<<","<<(*_cl_eta)[i_cl]<<","<<(*_cl_phi)[i_cl]<<","<<(*_cl_HGClayer)[i_cl]<<")"<<endl;
      }
    }
  }

  else if(c3d.size()>0){
    for(auto & i_c3d : c3d){
      cout<<"C3D "<<i_c3d<<" (pt,eta,phi)="<<(*_cl3d_pt)[i_c3d]<<","<<(*_cl3d_eta)[i_c3d]<<","<<(*_cl3d_phi)[i_c3d]<<")"<<endl;
      for(auto & i_cl : (*_cl3d_clusters)[i_c3d]){
	int cl_index = cl_map[i_cl];
	h->Fill((*_cl_eta)[cl_index],(*_cl_phi)[cl_index],(*_cl_HGClayer)[cl_index],(*_cl_pt)[cl_index]);
	cout<<"(pt,eta,phi,layer)=("<<(*_cl_pt)[cl_index]<<","<<(*_cl_eta)[cl_index]<<","<<(*_cl_phi)[cl_index]<<","<<(*_cl_HGClayer)[cl_index]<<")"<<endl;
      }
    }
  }
  
  else if(jet.size()>0){
    for(auto & i_jet : jet){
      cout<<"jet "<<i_jet<<" (pt,eta,phi)="<<(*_jets_pt)[i_jet]<<","<<(*_jets_eta)[i_jet]<<","<<(*_jets_phi)[i_jet]<<")"<<endl;
      for(auto & i_c3d : (*_jets_cl3d)[i_jet]){
	for(auto & i_cl : (*_cl3d_clusters)[i_c3d]){
	  int cl_index = cl_map[i_cl];
	  h->Fill((*_cl_eta)[cl_index],(*_cl_phi)[cl_index],(*_cl_HGClayer)[cl_index],(*_cl_pt)[cl_index]);
	  cout<<"(pt,eta,phi,layer)=("<<(*_cl_pt)[cl_index]<<","<<(*_cl_eta)[cl_index]<<","<<(*_cl_phi)[cl_index]<<","<<(*_cl_HGClayer)[cl_index]<<")"<<endl;
	}
      }
    }
  }

  h->Draw("BOX2");



}






void display2D_c3d(TString file, int n_event=0, 
		  const vector<int>& c3d= vector<int>(),
		  const vector<int>& jet= vector<int>(),
		  float eta_min=-3, float eta_max=3, 
		  float phi_min=-3.2, float phi_max=3.2){

  TH2F* h=new TH2F("h","h",50,eta_min,eta_max,50,phi_min,phi_max);

  TChain * tree = new TChain("HGCalTriggerNtuple");
  tree->Add(file);
  
  vector<float> *_cl3d_eta;
  vector<float> *_cl3d_phi;
  vector<float> *_cl3d_energy;
  vector<float> *_cl3d_pt;
  
  vector<float> *_jets_eta;
  vector<float> *_jets_phi;
  vector<float> *_jets_energy;
  vector<float> *_jets_pt;

  vector<vector<int> > *_jets_cl3d;

  tree->SetBranchAddress("cl3d_eta",    &_cl3d_eta);
  tree->SetBranchAddress("cl3d_phi",    &_cl3d_phi);
  tree->SetBranchAddress("cl3d_energy", &_cl3d_energy);
  tree->SetBranchAddress("cl3d_pt", &_cl3d_pt);

  tree->SetBranchAddress("jets_eta",    &_jets_eta);
  tree->SetBranchAddress("jets_phi",    &_jets_phi);
  tree->SetBranchAddress("jets_energy", &_jets_energy);
  tree->SetBranchAddress("jets_pt", &_jets_pt);
      
  tree->SetBranchAddress("jets_cl3d",&_jets_cl3d);

  _cl3d_eta = 0;
  _cl3d_phi = 0;
  _cl3d_energy = 0;
  _cl3d_pt = 0;

  _jets_eta = 0;
  _jets_phi = 0;
  _jets_energy = 0;
  _jets_pt = 0;
  
  _jets_cl3d = 0;
  
  tree->GetEntry(n_event);
  
  if(c3d.size()>0){

    if(c3d[0]<0){
      for(unsigned int i_c3d = 0; i_c3d<(*_cl3d_eta).size(); i_c3d++){
	h->Fill((*_cl3d_eta)[i_c3d],(*_cl3d_phi)[i_c3d],(*_cl3d_pt)[i_c3d]);
	cout<<"(pt,eta,phi)=("<<(*_cl3d_pt)[i_c3d]<<","<<(*_cl3d_eta)[i_c3d]<<","<<(*_cl3d_phi)[i_c3d]<<")"<<endl;
      }
    }

    else{
      for(auto & i_c3d : c3d){     
	cout<<"C3D "<<i_c3d<<" (pt,eta,phi)="<<(*_cl3d_pt)[i_c3d]<<","<<(*_cl3d_eta)[i_c3d]<<","<<(*_cl3d_phi)[i_c3d]<<")"<<endl;
	h->Fill((*_cl3d_eta)[i_c3d],(*_cl3d_phi)[i_c3d],(*_cl3d_pt)[i_c3d]);      
      }
    }
  }
  
  else if(jet.size()>0){
    for(auto & i_jet : jet){
      cout<<"jet "<<i_jet<<" (pt,eta,phi)="<<(*_jets_pt)[i_jet]<<","<<(*_jets_eta)[i_jet]<<","<<(*_jets_phi)[i_jet]<<")"<<endl;
      for(auto & i_c3d : (*_jets_cl3d)[i_jet]){
	cout<<"(pt,eta,phi)="<<(*_cl3d_pt)[i_c3d]<<","<<(*_cl3d_eta)[i_c3d]<<","<<(*_cl3d_phi)[i_c3d]<<")"<<endl;
	h->Fill((*_cl3d_eta)[i_c3d],(*_cl3d_phi)[i_c3d],(*_cl3d_pt)[i_c3d]);   
      }
    }
  }

  h->GetXaxis()->SetTitle("#eta(C3D)");
  h->GetXaxis()->SetTitleOffset(1.2);
  h->GetYaxis()->SetTitle("#phi(C3D)");
  h->GetZaxis()->SetTitle("p_{T}(C3D) [GeV]");
  h->SetTitle("");
  h->SetStats(0);

  TCanvas* c=new TCanvas("c","c",650,600);
  c->SetLeftMargin(0.15);
  c->SetRightMargin(0.15);

  gStyle->SetPaintTextFormat("4.1f");
  if(c3d.size()>0 && c3d[0]<0)
    h->Draw("colz");
  else
    h->Draw("colztext");

}





void display2D_jets(TString file, int n_event=0, 
		    const vector<int>& jet= vector<int>(),
		    float eta_min=-3, float eta_max=3, 
		    float phi_min=-3.2, float phi_max=3.2){

  TH2F* h=new TH2F("h","h",50,eta_min,eta_max,50,phi_min,phi_max);

  TChain * tree = new TChain("HGCalTriggerNtuple");
  tree->Add(file);
  
  vector<float> *_jets_eta;
  vector<float> *_jets_phi;
  vector<float> *_jets_energy;
  vector<float> *_jets_pt;

  tree->SetBranchAddress("jets_eta",    &_jets_eta);
  tree->SetBranchAddress("jets_phi",    &_jets_phi);
  tree->SetBranchAddress("jets_energy", &_jets_energy);
  tree->SetBranchAddress("jets_pt", &_jets_pt);     

  _jets_eta = 0;
  _jets_phi = 0;
  _jets_energy = 0;
  _jets_pt = 0;
  
  
  tree->GetEntry(n_event);
  
  if(jet.size()>0){

    if(jet[0]<0){
      for(unsigned int i_jet = 0; i_jet<(*_jets_eta).size(); i_jet++){
	h->Fill((*_jets_eta)[i_jet],(*_jets_phi)[i_jet],(*_jets_pt)[i_jet]);
	cout<<"(pt,eta,phi)=("<<(*_jets_pt)[i_jet]<<","<<(*_jets_eta)[i_jet]<<","<<(*_jets_phi)[i_jet]<<")"<<endl;
      }
    }

    else{
      for(auto & i_jet : jet){   
	cout<<"Jet "<<i_jet<<"(pt,eta,phi)=("<<(*_jets_pt)[i_jet]<<","<<(*_jets_eta)[i_jet]<<","<<(*_jets_phi)[i_jet]<<")"<<endl;  
	h->Fill((*_jets_eta)[i_jet],(*_jets_phi)[i_jet],(*_jets_pt)[i_jet]);
      }
    }
  }

  h->GetXaxis()->SetTitle("#eta(jet)");
  h->GetXaxis()->SetTitleOffset(1.5);
  h->GetYaxis()->SetTitle("#phi(jet)");
  h->GetZaxis()->SetTitle("p_{T}(jet) [GeV]");
  h->SetTitle("");
  h->SetStats(0);

  TCanvas* c=new TCanvas("c","c",650,600);
  c->SetLeftMargin(0.15);
  c->SetRightMargin(0.15);

  gStyle->SetPaintTextFormat("4.1f");
  h->Draw("colz");

}
