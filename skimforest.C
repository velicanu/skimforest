#include "HiForestAnalysis/hiForest.h"
#include <iostream>

void skimforest(string filename, string outfilename, string myjet, int minjetpt)
{
  int filtered = 0;
  HiForest * c = new HiForest(filename.data(),"forest",cPPb,0,myjet);
  c->InitTree();
  c->SetOutputFile(outfilename.data());
  for (int i = 0; i < c->GetEntries(); ++i)
  {
    if(i%1000 == 0) cout<<i<<"/"<<c->GetEntries()<<endl;
    c->GetEntry(i);
    if(!c->skim.pPAcollisionEventSelectionPA ) continue;
    if(!c->skim.pHBHENoiseFilter ) continue;
    
    float cmetashift = 0.0;
    if( c->hlt.Run >= 210498 && c->hlt.Run < 211313 ) cmetashift = 0.4654;  //pPb range
    if( c->hlt.Run >= 211313 && c->hlt.Run <= 211631 ) cmetashift = -0.4654; //Pbp range
    bool writeev = false;
    for(int i = 0 ; i < c->myjet.nref ; ++i)
    {
      if(c->myjet.jtpt[i] < minjetpt) continue;
      if(fabs(c->myjet.jteta[i] + cmetashift) > 1.5) continue;
      writeev = true;
      filtered++;
      break;
    }
    if(writeev) c->FillOutput();
  }
  delete c;
  cout<<"filtered: "<<filtered<<endl;
}

int main(int argc, char *argv[])
{
  if(argc != 5)
  {
    cout<<"usage ./skimforest.exe <infile> <outfile> <jetcollection> <minjetpt>"<<endl;
    cout<<"hint: <jetcollection> = ak3PF for pp and akPu3PF for pPb and PbPb"<<endl;
    return 1;
  }
  skimforest(argv[1],argv[2],argv[3],atoi(argv[4]));
  return 0;
}
