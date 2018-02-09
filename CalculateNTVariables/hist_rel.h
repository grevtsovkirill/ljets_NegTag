// relative difference between hists syst VS. std
TH1D* hist_rel(TH1D* h_nom, TH1D* h_sys){
  TH1D* h_rel = (TH1D*) h_sys->Clone((string("rel_") + h_sys->GetName()).c_str());
  h_rel->Add(h_nom, -1.0);
  h_rel->Divide(h_nom);

  for (int ibin=1; ibin<=h_rel->GetNbinsX(); ibin++){
    if(fabs(h_rel->GetBinContent(ibin)) > 1.0){
      cout<<"WARNING, uncertainty >100% for sys: "<<(string(h_sys->GetName())).c_str()<<" ib "<<ibin<<" sys val "<<h_sys->GetBinContent(ibin)<<" nom val "<<h_nom->GetBinContent(ibin)<<" rel "<<h_rel->GetBinContent(ibin)<<endl;
    }
  }

  return h_rel;
}

// relative difference among hists s1 VS. s2 VS. s3
TH1* hist_rel(TH1* h_s1, TH1* h_s2, TH1* h_s3){
  
  TH1* h_rel = (TH1*) h_s1->Clone((string("rel_") + h_s1->GetName()).c_str());
  h_rel->Reset();
  for (int ibin=1; ibin<=h_s1->GetNbinsX(); ibin++){
    double val1 = h_s1->GetBinContent(ibin);
    double val2 = h_s2->GetBinContent(ibin);
    double val3 = h_s3->GetBinContent(ibin);
    double vavg = (val1 + val2 + val3) / 3.0;

    double r_ge = 0;
    if(vavg > 0){ //half of the difference 
      double r_s1 = 0.5 * fabs(val1 - val2) / vavg;
      double r_s2 = 0.5 * fabs(val2 - val3) / vavg;
      double r_s3 = 0.5 * fabs(val3 - val1) / vavg;
      r_ge = r_s1 > r_s2 ? r_s1>r_s3?r_s1:r_s3 : r_s2>r_s3?r_s2:r_s3;
    }
    h_rel->SetBinContent(ibin, r_ge);    
  }

  return h_rel;
}

TH1* hist_rel_n(vector<TH1*> h_sn){

  int nHists = h_sn.size();
  if (nHists < 2){
    cout << "not enough histograms supplied" << endl;
    return 0;
  }

  TH1* h_rel = (TH1*) h_sn[0]->Clone((string("rel_") + h_sn[0]->GetName()).c_str());
  h_rel->Reset();
  
  for (int ibin=1; ibin<=h_sn[0]->GetNbinsX(); ibin++){
    double vals[nHists];
    double sumVals = 0.0;
    for (int ih = 0; ih < nHists; ++ih){
      vals[ih] = h_sn[ih]->GetBinContent(ibin);
      sumVals += vals[ih];
    }
    if (sumVals == 0.0){
      cout << "----> sum of unc 0, atm set it to 1" << endl;
      h_rel->SetBinContent(ibin, 1);
      continue;
    }
    double vavg = sumVals/ (1.0*nHists);

    double r_ge = 0;
    if(vavg > 0){ //half of the difference 
      for (int i=0; i<nHists; ++i){
	for (int j=0; j<nHists; ++j){
	  double diff = 0.5 * fabs(vals[i] - vals[j]);
	  if (diff > r_ge) 
	    r_ge = diff;
	}
      }
    }
    h_rel->SetBinContent(ibin, r_ge/vavg);
  }

  return h_rel;
}
