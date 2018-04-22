#include "../helpers/config_reader.cpp"
#include "../config/conf.hpp"
#include "../AtlasStyle/AtlasStyle.C"
#include "../AtlasStyle/AtlasUtils.C"

void DrawPlots(TString systematic, int ijet, TH2D* h2_mc, TH2D* h2_data, TString var)
{
  SetAtlasStyle();
  gStyle->SetOptStat(0);
  TString hname=var; hname+="_"; hname+=(ijet+1);
  TCanvas* c1 = new TCanvas( TString("c_")+hname, TString("c_")+hname, 0, 0, 600, 700);
  c1->Divide(1,2);
  
  //first plot the distributions itself
  c1->cd(1)->SetPad(0,0.25, 1.0, 1.0);
  c1->cd(1)->SetLogy();
  TH1D* h_mc=0;
  TH1D* h_data=0;
  if(var=="eta")  // To obtain the 1-D eta distr extract the Y axis  projection.
    {
      h_mc=(TH1D*)h2_mc->ProjectionY();
      h_mc->SetName("tmp_mc");
      h_data=(TH1D*)h2_data->ProjectionY();
      h_data->SetName("tmp_data");
    }
  else if(var=="pt") // To obtain the 1-D pt distr extract the X axis  projection.
    {
      h_mc=(TH1D*)h2_mc->ProjectionX();
      h_mc->SetName("tmp_mc");
      h_data=(TH1D*)h2_data->ProjectionX();
      h_data->SetName("tmp_data");
    }

  h_mc->SetLineColor(kBlue);
  h_mc->SetLineWidth(3);
  h_mc->SetYTitle("#events");

  // ratio plots BEFORE reweighting
  TH1D* h_ratio=(TH1D*)h_data->Clone();
  h_ratio->SetName("ratio");
  h_ratio->Divide(h_mc);

  // print of max/min values
  std::cout << "MC: min="<< h_mc->GetYaxis()->GetXmin()<< " max=" << h_mc->GetYaxis()->GetXmax() << std::endl;
  std::cout << "Data: min="<< h_data->GetYaxis()->GetXmin()<< " max=" << h_data->GetYaxis()->GetXmax() << std::endl;
  std::cout << "MC2: min="<< h_mc->GetMinimum()<< " max=" << h_mc->GetMaximum() << std::endl;
  std::cout << "Data2: min="<< h_data->GetMinimum()<< " max=" << h_data->GetMaximum() << std::endl;
  
  // some cosmetics for the scaling
  double max=h_mc->GetMaximum();
  if(h_data->GetMaximum()>max) max=h_data->GetMaximum();
  h_data->SetMaximum(max);

  // ---- Plotting ---- //
  //h_data->SetLineColor(95);
  h_data->SetLineColor(kGreen);
  h_data->SetLineWidth(3);
  h_data->Draw();

  h_mc->SetMarkerSize(0);
  h_mc->Draw("SAME");


  TLegend *leg = new TLegend(0.75, 0.75, 0.9, 0.85); 
  if (ijet == 0) leg->SetHeader("leading jet");
  else  leg->SetHeader("subleading jet");
  leg->SetTextSize(0.035); 
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetShadowColor(0);
  leg->AddEntry(h_data, "data", "l"); 
  leg->AddEntry(h_mc, "mc", "l"); 
  leg->Draw();
  

  //now plot the ratio
  //make right errors
  h_ratio->GetYaxis()->SetTitle("ratio (data/MC) ");
  h_ratio->GetYaxis()->SetTitleOffset(0.3);
  h_ratio->GetYaxis()->SetTitleSize(0.1);

  c1->cd(2)->SetPad(0, 0.0, 1.0, 0.3);
  //h_ratio->SetMaximum(2.0);
  //h_ratio->SetMinimum(0.3);
  h_ratio->SetLineColor(kBlack);
  h_ratio->SetXTitle(var);
  h_ratio->GetXaxis()->SetTitleOffset(0.5);
  h_ratio->GetXaxis()->SetTitleSize(0.12);
  h_ratio->SetMarkerSize(0);
  h_ratio->Draw("e0");

  /*TLine* l1 = new TLine(h_ratio->GetXaxis()->GetXmin(), 1.0, h_ratio->GetXaxis()->GetXmax(), 1.0);
  l1->SetLineStyle(3);
  l1->SetLineColor(kBlue);
  l1->SetLineWidth(2);
  l1->Draw("same");*/
  
  TString sfold = "plot/";
  c1->Print( sfold+hname+"_datamc.eps");
  c1->Print( sfold+hname+"_datamc.pdf");
  c1->Close();
} // End of the DrawPlots function.


void calculate_jetptetareweighting( string dname="data_FlavourTagging_Nominal_0.root",string mname="mc_FlavourTagging_Nominal_Pythia.root",int dosyst=0)
{
  // separate reweighting for leading and next-to-leading pt jet in pt:eta

  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  
  // load jet pT:eta distributions

  TFile* ff_data = new TFile(dname.c_str());
  TFile* ff_mc = new TFile(mname.c_str());
  const int njet = 2;
  TH2D* h_ratio[njet];
  TH2D* h_ratio_bootstrap_data[njet][conf::n_bootstrap]; // bootstrap replicas
  TH2D* h_ratio_bootstrap_mc[njet][conf::n_bootstrap]; // bootstrap replicas

  TFile* ff_out = new TFile("rew.root", "RECREATE");

  // The read_list function is implemented in config_reader.cpp => systematics is a string vector. 
  auto systematics = read_list("../NtupleDumper/.systematics.auto");
  if(dosyst!=0){
  systematics.push_back("FlavourTagging_PRW_DATASF__1down");
  systematics.push_back("FlavourTagging_PRW_DATASF__1up");
  systematics.push_back("FlavourTagging_JVT_effSF__1down");
  systematics.push_back("FlavourTagging_JVT_effSF__1up");
  systematics.push_back("conversions__1down");
  systematics.push_back("conversions__1up");
  systematics.push_back("hadronic__1down");
  systematics.push_back("hadronic__1up");
  systematics.push_back("longlivedparticles");
  }

  for (auto syst: systematics)
    {
      cout << "####\n# working on: " << syst << "\n####" << endl;
      // Create a directory inside of the root file ff_out.root
      TDirectory* subdir = ff_out->mkdir(syst); 
      // At the end of the loop there will be a folder for each systematic error 
      subdir->cd();                             

      for (int i = 0; i<njet; ++i) 
	{
	  TH2D* h_mc = (TH2D*)ff_mc->Get( (string(syst +"/pteta")+char('1'+i)+"_mc_w" ).c_str() );
	  TH2D* h_data = (TH2D*)ff_data->Get((string(syst +"/pteta")+char('1'+i)+"_mc_w" ).c_str());
	  if (!h_data) // necessary for data who only has the nominal branch.
	    { // fall back to nominal branch
	      h_data = (TH2D*)ff_data->Get((string("FlavourTagging_Nominal/pteta")+char('1'+i)+"_mc_w" ).c_str());
	      cout << "falling back to nominal data for: " << syst <<  endl;
	    }
          // copiyng h_data in h_ratio[i] and divide it to h_mc.
	  h_ratio[i] = (TH2D*) h_data->Clone( (string("ptscale")+char('1'+i) ).c_str() );
	  h_ratio[i]->Divide(h_mc);

          // some printing
	  for (int ix=1; ix<=h_mc->GetNbinsX(); ix++)
	    {
	      for (int iy=1; iy<=h_mc->GetNbinsY(); iy++)
		{
		  if(i==0 && h_ratio[i]->GetBinContent(ix, iy) >0) // Done only for the leading jet.
		    {
		      cout<<"ix "<<ix<<" iy "<< iy <<" mc "<<h_mc->GetBinContent(ix, iy)
			  <<" data "<<h_data->GetBinContent(ix, iy)
			  <<" ratio "<<h_ratio[i]->GetBinContent(ix, iy)
			  <<" mc-rew "<<h_mc->GetBinContent(ix, iy) * h_ratio[i]->GetBinContent(ix, iy)<<endl; // mc*ratio == data
		    }
		}
	    }

	  // h_ratio[i]->Draw("box");
	  h_ratio[i]->Write(); // pT/eta weights 

          // Some plotting ** before reweighting **
	  DrawPlots(syst, i, h_mc, h_data,"eta");
	  DrawPlots(syst, i, h_mc, h_data,"pt");
	  delete h_mc;
	  delete h_data;
          /*
          // bootstrap replicas
          if (syst.EqualTo("FlavourTagging_Nominal"))
          {
             for(int ibootstrap=0; ibootstrap<conf::n_bootstrap; ibootstrap++)
              {
                // mcstat
                TH2D* h_mc_bootstrap_mc = (TH2D*)ff_mc->Get( (string(syst +"/pteta")+char('1'+i)+"_mc_w_"+to_string(ibootstrap) ).c_str() );
	        TH2D* h_data = (TH2D*)ff_data->Get((string(syst +"/pteta")+char('1'+i)+"_mc_w" ).c_str());
                // datastat
	        TH2D* h_mc = (TH2D*)ff_mc->Get( (string(syst +"/pteta")+char('1'+i)+"_mc_w" ).c_str() );
	        TH2D* h_data_bootstrap = (TH2D*)ff_data->Get((string(syst +"/pteta")+char('1'+i)+"_mc_w_"+to_string(ibootstrap) ).c_str());

                // ratio
                h_ratio_bootstrap_mc[i][ibootstrap] = (TH2D*) h_data->Clone( (string("ptscale")+char('1'+i)+string("_mc_")+to_string(ibootstrap) ).c_str() );      
                h_ratio_bootstrap_mc[i][ibootstrap]->Divide(h_mc_bootstrap_mc);

                h_ratio_bootstrap_data[i][ibootstrap] = (TH2D*) h_data_bootstrap->Clone( (string("ptscale")+char('1'+i)+string("_data_")+to_string(ibootstrap) ).c_str() );      
                h_ratio_bootstrap_data[i][ibootstrap]->Divide(h_mc);

                h_ratio_bootstrap_mc[i][ibootstrap]->Write();
                h_ratio_bootstrap_data[i][ibootstrap]->Write();

                delete h_mc_bootstrap_mc; 
                delete h_data;
                delete h_mc;
                delete h_data_bootstrap;
              }
	  }//bootstrap loop
	 // */
	} // End of the loop on the 2 jets.
    } // End of the loop on the systematics.

  ff_out->Close();


} // End of the calculate_jetptetareweighting macro.
