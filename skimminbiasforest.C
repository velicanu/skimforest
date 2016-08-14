#include "HiForestAnalysis/hiForest.h"
#include <iostream>

void skimminbiasforest(string filename, string outfilename)
{
  TFile *fout = new TFile(outfilename.data(),"recreate");
  fout->mkdir("hiEvtAnalyzer");
  fout->mkdir("akPu4CaloJetAnalyzer");
  fout->mkdir("anaTrack");
  fout->mkdir("skimanalysis");

  TFile * fin       = TFile::Open(filename.data());
  TTree * eventtree = (TTree*) fin->Get("hiEvtAnalyzer/HiTree");
  TTree * jettree   = (TTree*) fin->Get("akPu4CaloJetAnalyzer/t");
  TTree * skimtree  = (TTree*) fin->Get("skimanalysis/HltTree");
  TTree * tracktree = (TTree*) fin->Get("anaTrack/trackTree");

  // Begin event tree
  int hiBin = -99;
  float vz = -99;
  Int_t           hiNevtPlane;
  Float_t         hiEvtPlanes[29];   //[hiNevtPlane]
  eventtree->SetBranchAddress("hiBin", &hiBin);
  eventtree->SetBranchAddress("vz", &vz);
  eventtree->SetBranchAddress("hiNevtPlane", &hiNevtPlane);
  eventtree->SetBranchAddress("hiEvtPlanes", &hiEvtPlanes);

  fout->cd("hiEvtAnalyzer");
  TTree * eventtree_out = new TTree("HiTree","HiTree");
  eventtree_out->Branch("hiBin", &hiBin, "hiBin/I");
  eventtree_out->Branch("vz", &vz, "vz/F");
  eventtree_out->Branch("hiNevtPlane", &hiNevtPlane, "hiNevtPlane/I");
  eventtree_out->Branch("hiEvtPlanes", hiEvtPlanes, "hiEvtPlanes[hiNevtPlane]/F");
  fout->cd();
  // End event tree

  // Begin jet tree
  Int_t           nref;
  Float_t         * chargedSum = (Float_t*) malloc(sizeof(Float_t)*300);   //[nref]
  Float_t         * rawpt = (Float_t*) malloc(sizeof(Float_t)*300);   //[nref]
  Float_t         * jtpt = (Float_t*) malloc(sizeof(Float_t)*300);   //[nref]
  Float_t         * jteta = (Float_t*) malloc(sizeof(Float_t)*300);   //[nref]
  Float_t         * jtphi = (Float_t*) malloc(sizeof(Float_t)*300);   //[nref]
  jettree->SetBranchAddress("nref", &nref);
  jettree->SetBranchAddress("chargedSum", chargedSum);
  jettree->SetBranchAddress("rawpt", rawpt);
  jettree->SetBranchAddress("jtpt", jtpt);
  jettree->SetBranchAddress("jteta", jteta);
  jettree->SetBranchAddress("jtphi", jtphi);

  fout->cd("akPu4CaloJetAnalyzer");
  TTree * jettree_out = new TTree("t","akPu4CaloJetAnalyzer");
  jettree_out->Branch("nref", &nref, "nref/I");
  jettree_out->Branch("chargedSum", chargedSum, "chargedSum[nref]/F");
  jettree_out->Branch("rawpt", rawpt, "rawpt[nref]/F");
  jettree_out->Branch("jtpt", jtpt, "jtpt[nref]/F");
  jettree_out->Branch("jteta", jteta, "jteta[nref]/F");
  jettree_out->Branch("jtphi", jtphi, "jtphi[nref]/F");
  fout->cd();
  // End jet tree

  // Begin skim tree
  Int_t           pcollisionEventSelection;
  Int_t           HBHENoiseFilterResultRun2Loose;
  Int_t           pPAprimaryVertexFilter;
  Int_t           pBeamScrapingFilter;
  skimtree->SetBranchAddress("pcollisionEventSelection", &pcollisionEventSelection);
  skimtree->SetBranchAddress("HBHENoiseFilterResultRun2Loose", &HBHENoiseFilterResultRun2Loose);
  skimtree->SetBranchAddress("pPAprimaryVertexFilter", &pPAprimaryVertexFilter);
  skimtree->SetBranchAddress("pBeamScrapingFilter", &pBeamScrapingFilter);

  fout->cd("skimanalysis");
  TTree * skimtree_out = new TTree("HltTree","skimtree");
  skimtree_out->Branch("pcollisionEventSelection", &pcollisionEventSelection, "pcollisionEventSelection/I");
  skimtree_out->Branch("HBHENoiseFilterResultRun2Loose", &HBHENoiseFilterResultRun2Loose, "HBHENoiseFilterResultRun2Loose/I");
  skimtree_out->Branch("pPAprimaryVertexFilter", &pPAprimaryVertexFilter, "pPAprimaryVertexFilter/I");
  skimtree_out->Branch("pBeamScrapingFilter", &pBeamScrapingFilter, "pBeamScrapingFilter/I");
  fout->cd();
  // End skim tree

  // Begin track tree
  Int_t           nTrk;
  Float_t         trkPt[50000];   //[nTrk]
  Float_t         trkEta[50000];   //[nTrk]
  Float_t         trkPhi[50000];   //[nTrk]
  Bool_t          highPurity[50000];   //[nTrk]
  Float_t         trkPtError[50000];   //[nTrk]
  Float_t         trkDz1[50000];   //[nTrk]
  Float_t         trkDzError1[50000];   //[nTrk]
  Float_t         trkDxy1[50000];   //[nTrk]
  Float_t         trkDxyError1[50000];   //[nTrk]
  Float_t         trkChi2[50000];   //[nTrk]
  UChar_t           trkNdof[50000];   //[nTrk]
  UChar_t           trkNlayer[50000];   //[nTrk]
  UChar_t           trkNHit[50000];   //[nTrk]
  Float_t         pfHcal[50000];   //[nTrk]
  Float_t         pfEcal[50000];   //[nTrk]
  Int_t           nTrk_;
  Float_t         trkPt_[50000];   //[nTrk]
  Float_t         trkEta_[50000];   //[nTrk]
  Float_t         trkPhi_[50000];   //[nTrk]
  Bool_t          highPurity_[50000];   //[nTrk]
  Float_t         trkPtError_[50000];   //[nTrk]
  Float_t         trkDz1_[50000];   //[nTrk]
  Float_t         trkDzError1_[50000];   //[nTrk]
  Float_t         trkDxy1_[50000];   //[nTrk]
  Float_t         trkDxyError1_[50000];   //[nTrk]
  Float_t         trkChi2_[50000];   //[nTrk]
  UChar_t           trkNdof_[50000];   //[nTrk]
  UChar_t           trkNlayer_[50000];   //[nTrk]
  UChar_t           trkNHit_[50000];   //[nTrk]
  Float_t         pfHcal_[50000];   //[nTrk]
  Float_t         pfEcal_[50000];   //[nTrk]
  tracktree->SetBranchAddress("nTrk", &nTrk);
  tracktree->SetBranchAddress("trkPt", &trkPt);
  tracktree->SetBranchAddress("trkEta", &trkEta);
  tracktree->SetBranchAddress("trkPhi", &trkPhi);
  tracktree->SetBranchAddress("highPurity", &highPurity);
  tracktree->SetBranchAddress("trkPtError", &trkPtError);
  tracktree->SetBranchAddress("trkDz1", &trkDz1);
  tracktree->SetBranchAddress("trkDzError1", &trkDzError1);
  tracktree->SetBranchAddress("trkDxy1", &trkDxy1);
  tracktree->SetBranchAddress("trkDxyError1", &trkDxyError1);
  tracktree->SetBranchAddress("trkChi2", &trkChi2);
  tracktree->SetBranchAddress("trkNdof", &trkNdof);
  tracktree->SetBranchAddress("trkNlayer", &trkNlayer);
  tracktree->SetBranchAddress("trkNHit", &trkNHit);
  tracktree->SetBranchAddress("pfHcal", &pfHcal);
  tracktree->SetBranchAddress("pfEcal", &pfEcal);

  fout->cd("anaTrack");
  TTree * tracktree_out = new TTree("trackTree","trackTree");
  tracktree_out->Branch("nTrk", &nTrk_, "nTrk/I");
  tracktree_out->Branch("trkPt", trkPt_, "trkPt[nTrk]/F");
  tracktree_out->Branch("trkEta", trkEta_, "trkEta[nTrk]/F");
  tracktree_out->Branch("trkPhi", trkPhi_, "trkPhi[nTrk]/F");
  tracktree_out->Branch("highPurity", highPurity_, "highPurity[nTrk]/O");
  tracktree_out->Branch("trkPtError", trkPtError_, "trkPtError[nTrk]/F");
  tracktree_out->Branch("trkDz1", trkDz1_, "trkDz1[nTrk]/F");
  tracktree_out->Branch("trkDzError1", trkDzError1_, "trkDzError1[nTrk]/F");
  tracktree_out->Branch("trkDxy1", trkDxy1_, "trkDxy1[nTrk]/F");
  tracktree_out->Branch("trkDxyError1", trkDxyError1_, "trkDxyError1[nTrk]/F");
  tracktree_out->Branch("trkChi2", trkChi2_, "trkChi2[nTrk]/F");
  tracktree_out->Branch("trkNdof", trkNdof_, "trkNdof[nTrk]/b");
  tracktree_out->Branch("trkNlayer", trkNlayer_, "trkNlayer[nTrk]/b");
  tracktree_out->Branch("trkNHit", trkNHit_, "trkNHit[nTrk]/b");
  tracktree_out->Branch("pfHcal", pfHcal_, "pfHcal[nTrk]/F");
  tracktree_out->Branch("pfEcal", pfEcal_, "pfEcal[nTrk]/F");
  fout->cd();
  // End track tree
  int nentries = eventtree->GetEntries();
  for (int i = 0; i < nentries; ++i)
  {
    if(i%1000 == 0) cout<<i<<"/"<<nentries<<endl;
    eventtree->GetEntry(i);
    jettree->GetEntry(i);
    skimtree->GetEntry(i);
    tracktree->GetEntry(i);
    nTrk_ = 0;
    for(int itrk = 0 ; itrk < nTrk ; ++itrk)
    {
      if(trkPt[itrk]<1 || trkPt[itrk]>300 || fabs(trkEta[itrk])>2.4 ) continue;

      if(highPurity[itrk]!=1) continue;
      if(trkPtError[itrk]/trkPt[itrk]>0.1 || TMath::Abs(trkDz1[itrk]/trkDzError1[itrk])>3 || TMath::Abs(trkDxy1[itrk]/trkDxyError1[itrk])>3) continue;
      if(trkChi2[itrk]/(float)trkNdof[itrk]/(float)trkNlayer[itrk]>0.15) continue;
      if(trkNHit[itrk]<11 && trkPt[itrk]>0.7) continue;
      float Et = (pfHcal[itrk]+pfEcal[itrk])/TMath::CosH(trkEta[itrk]);
      if(!(trkPt[itrk]<20 || (Et>0.5*trkPt[itrk]))) continue;

      trkPt_[nTrk_] = trkPt[itrk];   //[nTrk_mix]
      trkEta_[nTrk_] = trkEta[itrk];   //[nTrk_mix]
      trkPhi_[nTrk_] = trkPhi[itrk];   //[nTrk_mix]
      highPurity_[nTrk_] = highPurity[itrk];   //[nTrk_mix]
      trkPtError_[nTrk_] = trkPtError[itrk];   //[nTrk_mix]
      trkDz1_[nTrk_] = trkDz1[itrk];   //[nTrk_mix]
      trkDzError1_[nTrk_] = trkDzError1[itrk];   //[nTrk_mix]
      trkDxy1_[nTrk_] = trkDxy1[itrk];   //[nTrk_mix]
      trkDxyError1_[nTrk_] = trkDxyError1[itrk];   //[nTrk_mix]
      trkChi2_[nTrk_] = trkChi2[itrk];   //[nTrk_mix]
      trkNdof_[nTrk_] = trkNdof[itrk];   //[nTrk_mix]
      trkNlayer_[nTrk_] = trkNlayer[itrk];   //[nTrk_mix]
      trkNHit_[nTrk_] = trkNHit[itrk];   //[nTrk_mix]
      pfHcal_[nTrk_] = pfHcal[itrk];   //[nTrk_mix]
      pfEcal_[nTrk_] = pfEcal[itrk];   //[nTrk_mix]


      nTrk_++;
    }
    eventtree_out->Fill();
    jettree_out->Fill();
    skimtree_out->Fill();
    tracktree_out->Fill();
  }
  fout->cd();
  // ztree->Write();
  // injetTree->Write();
  // trackTree->Write();
  fout->Write();
  fout->Close();
}

int main(int argc, char *argv[])
{
  if(argc != 3)
  {
    cout<<"usage ./skimforest.exe <infile> <outfile> "<<endl;
    cout<<"hint: <jetcollection> = ak3PF for pp and akPu3PF for pPb and PbPb"<<endl;
    return 1;
  }
  skimminbiasforest(argv[1],argv[2]);
  return 0;
}
