void e_peak(Int_t run=0, Double_t current=0){

  TChain *rootfile = new TChain("T");
  
  //Load rootfiles
  if(run==0){
    cout << "Please make the first parameter the desired run number" << endl << endl;
    return;
  }else{
    int i = 1;
  
    if(!gSystem->AccessPathName(TString::Format("/chafs1/work1/tritium/tmp_data/SRC/tritium_%d.root",run),kFileExists)){
      rootfile->Add(TString::Format("/chafs1/work1/tritium/tmp_data/SRC/tritium_%d.root",run));
      cout << "Added file: tritium_" << run << ".root" << endl;
    }else{
      cout << "Requested run has not been replayed. Exiting." << endl << endl;
      return;
    }
  
    while(!gSystem->AccessPathName(TString::Format("/chafs1/work1/tritium/tmp_data/SRC/tritium_%d_%d.root",run,i),kFileExists)){
      rootfile->Add(TString::Format("/chafs1/work1/tritium/tmp_data/SRC/tritium_%d_%d.root",run,i));
      cout << "Added file: tritium_" << run << "_" << i << ".root" << endl;
      i=i+1;
    }                      
  }

  TH2D *wvy = new TH2D("wvy","W vs Raster Y",11,67000,78000,150,.932,.942);

  TString cut;

  if(run<90000){
    cut = "L.tr.n==1&&(evLeftdnew_r*0.0003358)>";
    cut += current;
    rootfile->Draw("TMath::Sqrt(EKLx.W2):Lrb.Raster.rawcur.y>>wvy",TCut(cut));
  }else{
    cut = "R.tr.n==1&&(evRightdnew_r*0.0003358)>";
    cut += current;
    rootfile->Draw("TMath::Sqrt(EKRx.W2):Rrb.Raster.rawcur.y>>wvy",TCut(cut));
  }

  TF1 *gaus_fit = new TF1("Gaussian","gaus",.92,.95);
  //TObjArray arr;

  wvy->FitSlicesY(gaus_fit,1,11,0,"NR");
  
  TCanvas *c2 = new TCanvas();
  c2->cd(0);
  TH1D *out = (TH1D*)gDirectory->Get("wvy_1");
  out->Draw();
}
