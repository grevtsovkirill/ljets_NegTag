#include "../config/conf.hpp"
#include "../helpers/OutputHelper.hpp"
#include "/afs/desy.de/user/s/saimpert/atlas/atlasstyle-00-03-05/AtlasStyle.C"

void add_mcstat(bool flag_corrections=1, bool leading_jet=1)
{
  // Load ATLAS style
  SetAtlasStyle();

  // utilities
  std::map<int, std::map<int, std::string>> kin_labels = getKinLabels(); // take [ipt][ieta] and returns a string "ptXetaY"

  //**********OUTPUT*********///
  std::string output_name = "with_mcstat.root";
  if(!leading_jet)      output_name += ".subldg";

  if(!flag_corrections) output_name += ".noCorrections";
  else                  output_name += ".pTetaCorrections";

  TFile *f_out = new TFile(output_name.c_str(),"RECREATE");

  //**********INPUT*********///
  std::string input_name = "template_fit.root";
  if(!leading_jet)      input_name += ".subldg";

  if(!flag_corrections) input_name += ".noCorrections";
  else                  input_name += ".pTetaCorrections";

  TFile *f_in = TFile::Open(input_name.c_str(),"READ");

  // original histo - unc. from data stat. only
  TH2D *h_fracc_data_raw = (TH2D*)f_in->Get("h_fracc_data");
  TH2D *h_fracb_data_raw = (TH2D*)f_in->Get("h_fracb_data");
  TH2D *h_fracc_var_raw = (TH2D*)f_in->Get("h_fracc_var");
  TH2D *h_fracb_var_raw = (TH2D*)f_in->Get("h_fracb_var");
  TH2D *h_chi2_raw = (TH2D*)f_in->Get("h_chi2");
 
  //**********HISTO*********///
  f_out->cd();

  // new histo. unc from data stat. + MC stat.
  TH2D *h_fracc_data_updated = (TH2D*)h_fracc_data_raw->Clone("h_fracc_data_updated");
  TH2D *h_fracb_data_updated = (TH2D*)h_fracb_data_raw->Clone("h_fracb_data_updated");
  TH2D *h_fracc_var_updated = (TH2D*)h_fracc_var_raw->Clone("h_fracc_var_updated");
  TH2D *h_fracb_var_updated = (TH2D*)h_fracb_var_raw->Clone("h_fracb_var_updated");
  TH2D *h_chi2_updated = (TH2D*)h_chi2_raw->Clone("h_chi2_updated");
 
  //**********INTERMEDIARY HISTO*********///
  std::vector<TH1D*> h_fracc_bootstrap;
  std::vector<TH1D*> h_fracb_bootstrap;
  std::vector<TH1D*> h_fracc_var_bootstrap;
  std::vector<TH1D*> h_fracb_var_bootstrap;

  for (auto p_pt: kin_labels){

    int ipt = p_pt.first; // pT bin number

    for (auto p_eta: p_pt.second){

      int ieta = p_eta.first; // eta bin number

      std::string fracc_name = "fracc_" + p_eta.second;
      std::string fracb_name = "fracb_" + p_eta.second;
      std::string fracc_var_name = "fracc_var_" + p_eta.second;
      std::string fracb_var_name = "fracb_var_" + p_eta.second;

      TH1D* h_dummy1 = new TH1D(fracc_name.c_str(),"", 1000., 0.2*h_fracc_data_raw->GetBinContent(ipt, ieta), 1.8*h_fracc_data_raw->GetBinContent(ipt, ieta));
      TH1D* h_dummy2 = new TH1D(fracb_name.c_str(),"", 1000., 0.2*h_fracb_data_raw->GetBinContent(ipt, ieta), 1.8*h_fracb_data_raw->GetBinContent(ipt, ieta));

      TH1D* h_dummy3 = new TH1D(fracc_var_name.c_str(),"", 1000., 0.2*h_fracc_var_raw->GetBinContent(ipt, ieta), 1.8*h_fracc_var_raw->GetBinContent(ipt, ieta));
      TH1D* h_dummy4 = new TH1D(fracb_var_name.c_str(),"", 1000., 0.2*h_fracb_var_raw->GetBinContent(ipt, ieta), 1.8*h_fracb_var_raw->GetBinContent(ipt, ieta));

      h_fracc_bootstrap.push_back(h_dummy1); 
      h_fracb_bootstrap.push_back(h_dummy2); 
      h_fracc_var_bootstrap.push_back(h_dummy3); 
      h_fracb_var_bootstrap.push_back(h_dummy4); 
    }
  } 

  //**********LOOP ON THE bootstrap***********//
  TFile *f_bootstrap = ((TFile *)0);

  for(int ibootstrap=1; ibootstrap<1001; ibootstrap++)
  {
    std::string input_name_bootstrap = "bootstrap/" + input_name + "_" + to_string(ibootstrap); 
    if (f_bootstrap) delete f_bootstrap;
    f_bootstrap = TFile::Open(input_name_bootstrap.c_str(),"READ");

    TH2D *h_fracc_data_bootstrap = (TH2D*)f_bootstrap->Get("h_fracc_data");
    TH2D *h_fracb_data_bootstrap = (TH2D*)f_bootstrap->Get("h_fracb_data");

    TH2D *h_fracc_mc_bootstrap = (TH2D*)f_bootstrap->Get("h_fracc_mc");
    TH2D *h_fracb_mc_bootstrap = (TH2D*)f_bootstrap->Get("h_fracb_mc");

    // loop on pT/eta bins
    for (auto p_pt: kin_labels){
  
      int ipt = p_pt.first; // pT bin number
  
      for (auto p_eta: p_pt.second){
  
        int ieta = p_eta.first; // eta bin number

         h_fracc_bootstrap.at(2*(ipt-1)+(ieta-1))->Fill(h_fracc_data_bootstrap->GetBinContent(ipt, ieta)); 
         h_fracb_bootstrap.at(2*(ipt-1)+(ieta-1))->Fill(h_fracb_data_bootstrap->GetBinContent(ipt, ieta)); 
 
         h_fracc_var_bootstrap.at(2*(ipt-1)+(ieta-1))->Fill( (h_fracc_data_bootstrap->GetBinContent(ipt, ieta)/h_fracc_mc_bootstrap->GetBinContent(ipt, ieta))-1);
         h_fracb_var_bootstrap.at(2*(ipt-1)+(ieta-1))->Fill( (h_fracb_data_bootstrap->GetBinContent(ipt, ieta)/h_fracb_mc_bootstrap->GetBinContent(ipt, ieta))-1);

      }

    }

    delete f_bootstrap;
    f_bootstrap = ((TFile *)0);
  }

  // update the uncertainties of fracc and fracb
  // loop on pT/eta bins
  for (auto p_pt: kin_labels){

    int ipt = p_pt.first; // pT bin number

    for (auto p_eta: p_pt.second){

      int ieta = p_eta.first; // eta bin number

      // FRAC

      // c-quark
      double mean_bootstrap = h_fracc_bootstrap.at(2*(ipt-1)+(ieta-1))->GetMean();  
      double rms_bootstrap = h_fracc_bootstrap.at(2*(ipt-1)+(ieta-1))->GetRMS();  

      double mean_raw = h_fracc_data_raw->GetBinContent(ipt,ieta);
      double unc_raw = h_fracc_data_raw->GetBinError(ipt,ieta);

      double final_unc = sqrt( unc_raw*unc_raw + (mean_bootstrap-mean_raw)*(mean_bootstrap-mean_raw) + rms_bootstrap*rms_bootstrap);  // fit unc. (i.e. data unc. + fit ) + delta(bootstrap_mean,nominal) + model stat. unc.
      
      h_fracc_data_updated->SetBinError(ipt,ieta,final_unc);

      // b-quark
      mean_bootstrap = h_fracb_bootstrap.at(2*(ipt-1)+(ieta-1))->GetMean();  
      rms_bootstrap = h_fracb_bootstrap.at(2*(ipt-1)+(ieta-1))->GetRMS();  

      mean_raw = h_fracb_data_raw->GetBinContent(ipt,ieta);
      unc_raw = h_fracb_data_raw->GetBinError(ipt,ieta);

      final_unc = sqrt( unc_raw*unc_raw + (mean_bootstrap-mean_raw)*(mean_bootstrap-mean_raw) + rms_bootstrap*rms_bootstrap);  // fit unc. (i.e. data unc. + fit ) + delta(bootstrap_mean,nominal) + model stat. unc.
      
      h_fracb_data_updated->SetBinError(ipt,ieta,final_unc);

      // VAR FRAC

      // c-quark
      mean_bootstrap = h_fracc_var_bootstrap.at(2*(ipt-1)+(ieta-1))->GetMean();
      rms_bootstrap = h_fracc_var_bootstrap.at(2*(ipt-1)+(ieta-1))->GetRMS();

      mean_raw = h_fracc_var_raw->GetBinContent(ipt,ieta);
      unc_raw = h_fracc_var_raw->GetBinError(ipt,ieta);

      final_unc = sqrt( unc_raw*unc_raw + (mean_bootstrap-mean_raw)*(mean_bootstrap-mean_raw) + rms_bootstrap*rms_bootstrap);  // fit unc. (i.e. data unc. + fit ) + delta(bootstrap_mean,nominal) + model stat. unc.

      h_fracc_var_updated->SetBinError(ipt,ieta,final_unc);

      // b-quark
      mean_bootstrap = h_fracb_var_bootstrap.at(2*(ipt-1)+(ieta-1))->GetMean();
      rms_bootstrap = h_fracb_var_bootstrap.at(2*(ipt-1)+(ieta-1))->GetRMS();

      mean_raw = h_fracb_var_raw->GetBinContent(ipt,ieta);
      unc_raw = h_fracb_var_raw->GetBinError(ipt,ieta);

      final_unc = sqrt( unc_raw*unc_raw + (mean_bootstrap-mean_raw)*(mean_bootstrap-mean_raw) + rms_bootstrap*rms_bootstrap);  // fit unc. (i.e. data unc. + fit ) + delta(bootstrap_mean,nominal) + model stat. unc.

      h_fracb_var_updated->SetBinError(ipt,ieta,final_unc);

    }

  }

  // recompute chi2 including MC stat. uncertainties
  // loop on pT/eta bins
  std::vector<TH1D*> h_model_bootstrap;
  for (auto p_pt: kin_labels){

    int ipt = p_pt.first; // pT bin number

    for (auto p_eta: p_pt.second){

      int ieta = p_eta.first; // eta bin number

      std::cout << "***************PT/ETA bins: " << p_eta.second << "****************" << std::endl;

      std::string identifier_data = "w__" + p_eta.second + "_MV2c10";
      std::string identifier_mc = "h_incl_datafit_" + p_eta.second + "_MV2c10";

      TH1D *h_data = (TH1D*)f_in->Get(identifier_data.c_str());
      TH1D *h_mc = (TH1D*)f_in->Get(identifier_mc.c_str());

      // one histo per MV2c10 Output bin
      f_out->cd();

      for(int ibin=1; ibin<h_mc->GetNbinsX()+1; ibin++)
      {
        std::string dummy_name = "dummy_" + p_eta.second + "_" + to_string(ibin);
        TH1D* h_dummy = new TH1D(dummy_name.c_str(),"", 1000., 0.2*h_mc->GetBinContent(ibin), 1.8*h_mc->GetBinContent(ibin));
        h_model_bootstrap.push_back(h_dummy);
      }

      // loop on the bootstrap
      for(int ibootstrap=1; ibootstrap<1001; ibootstrap++)
      {
        std::string input_name_bootstrap = "bootstrap/" + input_name + "_" + to_string(ibootstrap); 
        if (f_bootstrap) delete f_bootstrap;
        f_bootstrap = TFile::Open(input_name_bootstrap.c_str(),"READ");
        TH1D *h_mc_bootstrap = (TH1D*)f_bootstrap->Get(identifier_mc.c_str());

        for(int ibin=1; ibin<h_mc->GetNbinsX()+1; ibin++) 
        {
          h_model_bootstrap.at(h_mc->GetNbinsX()*(2*(ipt-1)+(ieta-1))+ibin-1)->Fill(h_mc_bootstrap->GetBinContent(ibin));
        }

        delete f_bootstrap;
        f_bootstrap = ((TFile *)0);

      }

      // chi2 computation - first term is regular chi2
      double dof = -3; // 3 fitted parameters
      double first_term = 0;
      double second_term = 0;
      for(int ibin=1; ibin<h_mc->GetNbinsX()+1; ibin++)
      {
        if(h_data->GetBinContent(ibin)==0 || h_mc->GetBinContent(ibin)==0)
        {
          std::cout << "WARNING, bins with no observed and/or expected events" << std::endl;
        } 
        dof++;

        double oi = h_data->GetBinContent(ibin);
        double ei = h_mc->GetBinContent(ibin);

        double otot = h_data->Integral();
        double etot = h_mc->Integral();

        double oi_unc = h_data->GetBinError(ibin);
        // from bootstrap
        double ei_unc = sqrt( (h_mc->GetBinContent(ibin)-h_model_bootstrap.at(h_mc->GetNbinsX()*(2*(ipt-1)+(ieta-1))+ibin-1)->GetMean())*(h_mc->GetBinContent(ibin)-h_model_bootstrap.at(h_mc->GetNbinsX()*(2*(ipt-1)+(ieta-1))+ibin-1)->GetMean()) +  h_model_bootstrap.at(h_mc->GetNbinsX()*(2*(ipt-1)+(ieta-1))+ibin-1)->GetRMS()*h_model_bootstrap.at(h_mc->GetNbinsX()*(2*(ipt-1)+(ieta-1))+ibin-1)->GetRMS() );
        // sum w2
        double ei_unc2 = h_mc->GetBinError(ibin);

        std::cout << "Unc. from fit results with bootstrap model: " << ei_unc << " MC sumW2: " << ei_unc2 << std::endl;

        double pi = ( otot*oi/(oi_unc*oi_unc) +  etot*ei/(ei_unc*ei_unc) ) / ( otot*otot/(oi_unc*oi_unc) + etot*etot/(ei_unc*ei_unc) );
        //double pi = (etot*ei - otot*ei_unc*ei_unc + sqrt( (etot*ei-otot*ei_unc*ei_unc)*(etot*ei-otot*ei_unc*ei_unc) + 4*etot*etot*ei_unc*ei_unc*oi ) ) / (2*etot*etot);

        first_term += ((oi - pi*otot)*(oi - pi*otot))/(oi_unc*oi_unc);
        second_term += ((ei - pi*etot)*(ei - pi*etot))/ (ei_unc*ei_unc); 

      }

      double chi2 = first_term + second_term;
      double chi2_xcheck = h_data->Chi2Test(h_mc, "WWPCHI2");

      std::cout << "Corrected chi2: " << chi2 << " Old chi2: " << chi2_xcheck << std::endl; 

      double chi2_norm = chi2 / dof; 
      double chi2_xcheck_norm = chi2_xcheck / dof; 
      
      std::cout << "Corrected chi2/dof: " << chi2_norm << " Old chi2/dof: " << chi2_xcheck_norm << std::endl; 

      h_chi2_updated->SetBinContent(ipt,ieta,chi2_norm);
    }

  } 

  f_out->Write();

}
