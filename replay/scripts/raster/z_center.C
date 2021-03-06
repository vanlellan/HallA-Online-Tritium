void z_center(Int_t run=0, Double_t current=0){

  TChain *rootfile = new TChain("T");
  
  //Load rootfiles
  if(run==0){
    cout << "Please make the first parameter the desired run number" << endl << endl;
    return;
  }else{
    int i = 1;
  
    if(!gSystem->AccessPathName(TString::Format("/volatile/halla/triton/Marathon_Rootfiles/t1h/test/tritium_%d.root",run),kFileExists)){
      rootfile->Add(TString::Format("/volatile/halla/triton/Marathon_Rootfiles/t1h/test/tritium_%d.root",run));
      cout << "Added file: tritium_" << run << ".root" << endl;
    }else{
      cout << "Requested run has not been replayed. Exiting." << endl << endl;
      return;
    }
  
    while(!gSystem->AccessPathName(TString::Format("/volatile/halla/triton/Marathon_Rootfiles/t1h/test/tritium_%d_%d.root",run,i),kFileExists)){
      rootfile->Add(TString::Format("/volatile/halla/triton/Marathon_Rootfiles/t1h/test/tritium_%d_%d.root",run,i));
      cout << "Added file: tritium_" << run << "_" << i << ".root" << endl;
      i=i+1;
    }                      
  }

  TH2D *zvx = new TH2D("zvx","Z vs Raster X",36,54000,90000,300,-.02,.01);

  TString cut;

  if(run<90000){
    cut = "L.tr.n==1&&(evLeftdnew_r*0.0003358)>";
    cut += current;
    rootfile->Draw("rpl.z:Lrb.Raster.rawcur.x>>zvx",TCut(cut));
  }else{
    cut = "R.tr.n==1&&(evRightdnew_r*0.0003358)>";
    cut += current;
    rootfile->Draw("rpr.z:Rrb.Raster.rawcur.x>>zvx",TCut(cut));
  }

  TF1 *gaus_fit = new TF1("Gaussian","gaus",-.02,.01);
  //TObjArray arr;

  zvx->FitSlicesY(gaus_fit,1,36,0,"QNR");
  
  TCanvas *c2 = new TCanvas();
  c2->cd(0);
  TH1D *out = (TH1D*)gDirectory->Get("zvx_1");
  out->Draw();
}
