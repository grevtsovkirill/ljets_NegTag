#include "../config/subTagger.hpp"
#include "../AtlasStyle/AtlasStyle.C"
#include <sys/stat.h>
#include <sys/types.h>

void data_3mc_comparison_mc_w(string compaigne="a", string dname="data_FlavourTagging_Nominal_0.root",string mname="mc_FlavourTagging_Nominal_Pythia.root",string mHname="mc_FlavourTagging_Nominal_Herwig.root",string mSname="")
{
  SetAtlasStyle();

  TFile *f_data = new TFile(dname.c_str(), "read");
  TFile *f_mc = new TFile(mname.c_str(), "read");
  //TFile *f_mc = new TFile("mc.root", "read");
  TFile *f_mc_HERWIG = new TFile(mHname.c_str(), "read");
  TFile *f_mc_SHERPA; 
  if(!mSname.empty()){
    f_mc_SHERPA= new TFile(mSname.c_str(), "read");
  }
  string outfolder="Control_plots_"+compaigne;
  //Directory::CreateDirectory(outfolder.c_str(), NULL);
  //Directory::CreateDirectory((outfolder+"/pdf").c_str(), NULL);
  //_mkdir(outfolder.c_str());

  cout << "compaigne "<< compaigne<< "; compaigne.find(a) = "<< compaigne.find("a")<<endl;
  string text2="";
  string text1="#sqrt{s} = 13 TeV, ";
  if(compaigne.find("a") || compaigne.find("15") || compaigne.find("16")){
    text2="36 fb^{-1} (2015+2016)";
    cout << "found a, 15 or 16"<< endl; 
  }
  else if(compaigne.find("d") || compaigne.find("17") ){
    cout << "             found d"<< endl; 
    text2="46 fb^{-1} (2017)";
  }
  else text2="";
  
  text1+=text2;
  
  system(("mkdir "+outfolder).c_str());
  system(("mkdir "+outfolder+"/pdf").c_str());
  TFile *f_output = new TFile((outfolder+"/control_plots.root").c_str(), "recreate");
  cout<< "sherpa input: "<<mSname<<endl;
  std::string suffix = "_mc_w";
  std::vector<std::string> histo_var = {"pt", "eta", "pt_thin", "eta_thin", 
                                        "phi", "njets_event", "averageInteractionsPerCrossing", 
                                        "ntrk_IP3DNeg",
                                        "ntrk_IP3D",
                                        "ntrk_IP2DNeg",
                                        "ntrk_IP2D",
                                        "ntrk_SV1Flip",
                                        "ntrk_SV1",
                                        "ntrk_JetFitterFlip",
                                        "ntrk_JetFitter"};


  std::vector<std::string> histo_Xlabels = {"p_{T}^{jet} [GeV]", "|#eta^{jet}|",  
                                            "p_{T}^{jet} [GeV]", "|#eta^{jet}|", 
                                            "#phi^{jet} [rad.]", "N_{jet}", "#mu", 
                                            "N_{trk}^{IP3DNeg}",
                                            "N_{trk}^{IP3D}",
                                            "N_{trk}^{IP2DNeg}",
                                            "N_{trk}^{IP2D}",
                                            "N_{trk}^{SV1Flip}",
                                            "N_{trk}^{SV1}",
                                            "N_{trk}^{JetFitterFlip}",
                                            "N_{trk}^{JetFitter}"};

  for (const auto &name: subtagger::floats){
    histo_var.push_back(name.first.c_str());
    histo_Xlabels.push_back((name.first+" Output").c_str());
  }

  for (const auto &name: subtagger::ints){
    histo_var.push_back(name.first.c_str());
    histo_Xlabels.push_back((name.first+" Output").c_str());
  }

  for (const auto &name: subtagger::doubles){
    histo_var.push_back(name.first.c_str());
    histo_Xlabels.push_back((name.first+" Output").c_str());
  }

  for(int ivar=0; ivar<histo_var.size(); ivar++)
  {
    std::cout << histo_var[ivar] <<  "..." ;

    bool flag_logx = false;
    if (histo_var.at(ivar)=="pt" || histo_var.at(ivar)=="pt_thin") flag_logx = true;

    bool flag_logy = true;
    if (histo_var.at(ivar)=="jet_SV1_efracsvx" || 
        histo_var.at(ivar)=="jet_SV1Flip_efracsvx") flag_logy = false;
 
    for(int ijet=1; ijet<3; ijet++)
    {
      std::string jet_index = to_string(ijet);
      if( (histo_var.at(ivar)=="njets_event" || histo_var.at(ivar)=="averageInteractionsPerCrossing") && ijet==1) jet_index="";
      else if( (histo_var.at(ivar)=="njets_event" || histo_var.at(ivar)=="averageInteractionsPerCrossing") && ijet>1) continue;

      // get histogram
      std::string histo_name = "FlavourTagging_Nominal/" + 
                               histo_var.at(ivar) + jet_index + suffix; 

      TH1D *h_tmp = (TH1D*)f_data->Get(histo_name.c_str());
      std::string histo_data_name = "data_" + histo_var.at(ivar) + jet_index + suffix; 
      TH1D *h_data = (TH1D*)h_tmp->Clone(histo_data_name.c_str());

      h_tmp = (TH1D*)f_mc->Get(histo_name.c_str());
      std::string histo_mc_name = "mc_" + histo_var.at(ivar) + jet_index + suffix; 
      TH1D *h_mc = (TH1D*)h_tmp->Clone(histo_mc_name.c_str());

      h_tmp = (TH1D*)f_mc_HERWIG->Get(histo_name.c_str());
      std::string histo_mc_HERWIG_name = "mc_HERWIG_" + histo_var.at(ivar) + jet_index + suffix; 
      TH1D *h_mc_HERWIG = (TH1D*)h_tmp->Clone(histo_mc_HERWIG_name.c_str());

      // TH1D *h_mc_SHERPA;
      // if(!mSname.empty()){
      // 	h_tmp = (TH1D*)f_mc_SHERPA->Get(histo_name.c_str());
      // 	std::string histo_mc_SHERPA_name = "mc_SHERPA_" + histo_var.at(ivar) + jet_index + suffix; 
      // 	h_mc_SHERPA = (TH1D*)h_tmp->Clone(histo_mc_SHERPA_name.c_str());
      // }

      h_tmp =NULL;

      if(histo_var.at(ivar)=="jet_SV1_Lxy" ||
         histo_var.at(ivar)=="jet_SV1_L3d" ||
         histo_var.at(ivar)=="jet_JetFitter_significance3d" ||
         histo_var.at(ivar)=="jet_SV1Flip_Lxy" ||
         histo_var.at(ivar)=="jet_SV1Flip_L3d" ||
         histo_var.at(ivar)=="jet_JetFitterFlip_significance3d")
      {
        h_data->Rebin(5);
        h_mc->Rebin(5);
        h_mc_HERWIG->Rebin(5);
	//	if(!mSname.empty()) h_mc_SHERPA->Rebin(5);
      }
 
      if(histo_var.at(ivar)=="jet_SV1_normdist" ||
         histo_var.at(ivar)=="jet_SV1Flip_normdist")
       {
        h_data->Rebin(25);
        h_mc->Rebin(25);
        h_mc_HERWIG->Rebin(25);
	//if(!mSname.empty()) h_mc_SHERPA->Rebin(25);
      }
 
      if(histo_var.at(ivar)=="jet_JetFitter_dRFlightDir" ||
         histo_var.at(ivar)=="jet_JetFitterFlip_dRFlightDir")
      {
        h_data->GetXaxis()->SetRangeUser(0,2);
        h_mc->GetXaxis()->SetRangeUser(0,2);
        h_mc_HERWIG->GetXaxis()->SetRangeUser(0,2);
	//if(!mSname.empty()) h_mc_SHERPA->GetXaxis()->SetRangeUser(0,2);
      }
  
      if(histo_var.at(ivar)=="jet_SV1_NGTinSvx" ||
         histo_var.at(ivar)=="jet_SV1_N2Tpair" ||
         histo_var.at(ivar)=="jet_JetFitter_N2Tpair" ||
         histo_var.at(ivar)=="jet_JetFitter_nTracksAtVtx" ||
         histo_var.at(ivar)=="jet_SV1Flip_NGTinSvx" ||
         histo_var.at(ivar)=="jet_SV1Flip_N2Tpair" ||
         histo_var.at(ivar)=="jet_JetFitterFlip_N2Tpair")
      {
        h_data->GetXaxis()->SetRangeUser(0,30);
        h_mc->GetXaxis()->SetRangeUser(0,30);
        h_mc_HERWIG->GetXaxis()->SetRangeUser(0,30);
	//if(!mSname.empty()) h_mc_SHERPA->GetXaxis()->SetRangeUser(0,30);
      }
  
      if(histo_var.at(ivar)=="jet_JetFitter_nSingleTracks" ||
         histo_var.at(ivar)=="jet_JetFitter_nVTX" ||
         histo_var.at(ivar)=="jet_JetFitterFlip_nSingleTracks" ||
         histo_var.at(ivar)=="jet_JetFitterFlip_nVTX")
      {
        h_data->GetXaxis()->SetRangeUser(0,10);
        h_mc->GetXaxis()->SetRangeUser(0,10);
        h_mc_HERWIG->GetXaxis()->SetRangeUser(0,10);
	//if(!mSname.empty()) h_mc_SHERPA->GetXaxis()->SetRangeUser(0,10);
      }
  
      if(histo_var.at(ivar)=="jet_SV1_Lxy" ||
         histo_var.at(ivar)=="jet_SV1_L3d" ||
         histo_var.at(ivar)=="jet_JetFitter_significance3d" ||
         histo_var.at(ivar)=="jet_SV1Flip_Lxy" ||
         histo_var.at(ivar)=="jet_SV1Flip_L3d" ||
         histo_var.at(ivar)=="jet_JetFitterFlip_significance3d")
      {
        h_data->GetXaxis()->SetRangeUser(0,200);
        h_mc->GetXaxis()->SetRangeUser(0,200);
        h_mc_HERWIG->GetXaxis()->SetRangeUser(0,200);
        //h_mc_SHERPA->GetXaxis()->SetRangeUser(0,200);
      }

      if(flag_logy)
      {
        h_data->SetMaximum(h_data->GetMaximum()*10000);
        h_mc->SetMaximum(h_mc->GetMaximum()*10000);
        h_mc_HERWIG->SetMaximum(h_mc_HERWIG->GetMaximum()*10000);
	///if(!mSname.empty())         h_mc_SHERPA->SetMaximum(h_mc_SHERPA->GetMaximum()*10000);
      }
      if(!flag_logy)
      {
        h_data->SetMaximum(h_data->GetMaximum()*1.3);
        h_mc->SetMaximum(h_mc->GetMaximum()*1.3);
        h_mc_HERWIG->SetMaximum(h_mc_HERWIG->GetMaximum()*1.3);
	//if(!mSname.empty()) h_mc_SHERPA->SetMaximum(h_mc_SHERPA->GetMaximum()*1.3);
      }

      std::string histo_ratio_name = "ratio_" + histo_var.at(ivar) + jet_index + suffix; 
      std::string histo_ratio_HERWIG_name = "ratio_HERWIG_" + histo_var.at(ivar) + jet_index + suffix; 
      //std::string histo_ratio_SHERPA_name = "ratio_SHERPA_" + histo_var.at(ivar) + jet_index + suffix; 
      TH1D *h_ratio = (TH1D*)h_data->Clone(histo_ratio_name.c_str());
      TH1D *h_ratio_HERWIG = (TH1D*)h_data->Clone(histo_ratio_HERWIG_name.c_str());
      TH1D *h_ratio_SHERPA;
      // if(!mSname.empty()) h_ratio_SHERPA = (TH1D*)h_data->Clone(histo_ratio_SHERPA_name.c_str());
      h_ratio->Divide(h_mc); 
      h_ratio_HERWIG->Divide(h_mc_HERWIG); 
      //if(!mSname.empty()) h_ratio_SHERPA->Divide(h_mc_SHERPA); 

      h_ratio->SetMaximum(5);
      h_ratio_HERWIG->SetMaximum(5);
      //if(!mSname.empty()) h_ratio_SHERPA->SetMaximum(5);
      h_ratio->SetMinimum(0);
      h_ratio->SetMinimum(0);

      // cosmetics
      h_mc->SetLineColor(kRed+1);
      h_mc->SetMarkerColor(kRed+1);
      h_mc->SetXTitle(histo_Xlabels.at(ivar).c_str());
      h_mc->SetYTitle("Events");
      h_mc->SetTitle("");
      h_mc->GetXaxis()->SetLabelSize(0);
      h_mc->GetXaxis()->SetTitleSize(0);
      h_mc->GetYaxis()->SetLabelSize(0.06);
      h_mc->GetYaxis()->SetTitleSize(0.06);
      h_mc->GetYaxis()->SetTitleOffset(1.30);
      h_mc->GetYaxis()->SetNdivisions(505);

      h_mc_HERWIG->SetLineColor(kGreen-8);
      h_mc_HERWIG->SetMarkerColor(kGreen-8);
      h_mc_HERWIG->SetXTitle(histo_Xlabels.at(ivar).c_str());
      h_mc_HERWIG->SetYTitle("Events");
      h_mc_HERWIG->SetTitle("");
      h_mc_HERWIG->GetXaxis()->SetLabelSize(0);
      h_mc_HERWIG->GetXaxis()->SetTitleSize(0);
      h_mc_HERWIG->GetYaxis()->SetLabelSize(0.06);
      h_mc_HERWIG->GetYaxis()->SetTitleSize(0.06);
      h_mc_HERWIG->GetYaxis()->SetTitleOffset(1.30);
      h_mc_HERWIG->GetYaxis()->SetNdivisions(505);

      // if(!mSname.empty()){
      // h_mc_SHERPA->SetLineColor(kBlue-3);
      // h_mc_SHERPA->SetMarkerColor(kBlue-3);
      // h_mc_SHERPA->SetXTitle(histo_Xlabels.at(ivar).c_str());
      // h_mc_SHERPA->SetYTitle("Events");
      // h_mc_SHERPA->SetTitle("");
      // h_mc_SHERPA->GetXaxis()->SetLabelSize(0);
      // h_mc_SHERPA->GetXaxis()->SetTitleSize(0);
      // h_mc_SHERPA->GetYaxis()->SetLabelSize(0.06);
      // h_mc_SHERPA->GetYaxis()->SetTitleSize(0.06);
      // h_mc_SHERPA->GetYaxis()->SetTitleOffset(1.30);
      // h_mc_SHERPA->GetYaxis()->SetNdivisions(505);
      // }

      h_ratio->SetXTitle(histo_Xlabels.at(ivar).c_str());
      h_ratio->SetYTitle("Data / MC");
      h_ratio->SetTitle("");
      h_ratio->GetYaxis()->SetLabelSize(0.12);
      h_ratio->GetYaxis()->SetTitleSize(0.12);
      h_ratio->GetYaxis()->SetTitleOffset(0.60);
      h_ratio->GetYaxis()->SetNdivisions(505);
      h_ratio->GetXaxis()->SetLabelSize(0.12);
      h_ratio->GetXaxis()->SetTitleSize(0.12);       
      h_ratio->GetXaxis()->SetTitleOffset(1.36);       

      h_ratio_HERWIG->SetXTitle(histo_Xlabels.at(ivar).c_str());
      h_ratio_HERWIG->SetYTitle("Data / MC");
      h_ratio_HERWIG->SetTitle("");
      h_ratio_HERWIG->GetYaxis()->SetLabelSize(0.12);
      h_ratio_HERWIG->GetYaxis()->SetTitleSize(0.12);
      h_ratio_HERWIG->GetYaxis()->SetTitleOffset(0.60);
      h_ratio_HERWIG->GetYaxis()->SetNdivisions(505);
      h_ratio_HERWIG->GetXaxis()->SetLabelSize(0.12);
      h_ratio_HERWIG->GetXaxis()->SetTitleSize(0.12);       
      h_ratio_HERWIG->GetXaxis()->SetTitleOffset(1.36);       

      // if(!mSname.empty()){
      // h_ratio_SHERPA->SetXTitle(histo_Xlabels.at(ivar).c_str());
      // h_ratio_SHERPA->SetYTitle("Data / MC");
      // h_ratio_SHERPA->SetTitle("");
      // h_ratio_SHERPA->GetYaxis()->SetLabelSize(0.12);
      // h_ratio_SHERPA->GetYaxis()->SetTitleSize(0.12);
      // h_ratio_SHERPA->GetYaxis()->SetTitleOffset(0.60);
      // h_ratio_SHERPA->GetYaxis()->SetNdivisions(505);
      // h_ratio_SHERPA->GetXaxis()->SetLabelSize(0.12);
      // h_ratio_SHERPA->GetXaxis()->SetTitleSize(0.12);       
      // h_ratio_SHERPA->GetXaxis()->SetTitleOffset(1.36);       
      // }

      TLegend* legend = new TLegend(0.580201,0.561807,0.780702,0.706947);
      legend->SetTextFont(42);
      legend->SetFillStyle(0);
      legend->SetBorderSize(0);  // no border
      legend->SetFillColor(0);   // Legend background should be white
      legend->SetTextSize(0.04); // Increase entry font size!
      legend->AddEntry(h_data,"Data","LPE");
      legend->AddEntry(h_mc,"Pythia MC","LPE");

      TLegend* legend_HERWIG = new TLegend(0.580201,0.561807,0.780702,0.706947);
      legend_HERWIG->SetTextFont(42);
      legend_HERWIG->SetBorderSize(0);  // no border
      legend_HERWIG->SetFillColor(0);   // Legend background should be white
      legend_HERWIG->SetTextSize(0.04); // Increase entry font size!
      legend_HERWIG->AddEntry(h_data,"Data","LPE");
      legend_HERWIG->AddEntry(h_mc_HERWIG,"HERWIG MC","LPE");

      // TLegend* legend_SHERPA = new TLegend(0.580201,0.561807,0.780702,0.706947);
      // legend_SHERPA->SetTextFont(42);
      // legend_SHERPA->SetBorderSize(0);  // no border
      // legend_SHERPA->SetFillColor(0);   // Legend background should be white
      // legend_SHERPA->SetTextSize(0.04); // Increase entry font size!
      // if(!mSname.empty()){
      // legend_SHERPA->AddEntry(h_data,"Data","LPE");
      // legend_SHERPA->AddEntry(h_mc_SHERPA,"SHERPA","LPE");
      // }

      TPaveText *pt = new TPaveText(0.508772,0.851259,0.629073,0.919908,"brNDC");
      pt->SetBorderSize(0);
      pt->SetFillColor(0);
      pt->SetTextSize(0.04);
      pt->SetTextFont(72);
      pt->AddText("ATLAS");
    
      TPaveText *pt2 = new TPaveText(0.617794,0.850768,0.738095,0.921053,"brNDC");
      pt2->SetBorderSize(0);
      pt2->SetFillColor(0);
      pt2->SetTextSize(0.04);
      pt2->SetTextFont(42);
      pt2->AddText("Internal");
    
      TPaveText *pt3 = new TPaveText(0.602757,0.713959,0.723058,0.864335,"brNDC");
      pt3->SetBorderSize(0);
      pt3->SetFillColor(0);
      pt3->SetTextSize(0.04);
      pt3->SetTextFont(42);
      pt3->AddText(text1.c_str());
      if(ijet==2) pt3->AddText("Subleading jet");
      //pt3->AddText("p_{T}^{jet}/|#eta|^{jet} and p_{T}^{jet}/N_{trk}^{jet} reweighting applied");

      // draw canvas
      std::string canvas_name = "c_" + histo_var.at(ivar) + jet_index + suffix; 
      std::string pad1_name = "pad1_" + histo_var.at(ivar) + jet_index + suffix; 
      std::string pad2_name = "pad2_" + histo_var.at(ivar) + jet_index + suffix; 
      TCanvas *c = new TCanvas(canvas_name.c_str(), "", 800, 900);
      TPad* pad1 = new TPad(pad1_name.c_str(),"", 0, 0.3, 1, 1.0);
      pad1->SetBottomMargin(0.1);
      TPad* pad2 = new TPad(pad2_name.c_str(),"", 0, 0.01, 1, 0.35);
      pad2->SetBottomMargin(0.35);
      pad1->Draw();
      pad2->Draw();

      pad1->cd();
      if(flag_logy) pad1->SetLogy();
      if(flag_logx) pad1->SetLogx();

      h_mc->Draw("LPE");
      h_data->Draw("LPESAME");
      h_data->Draw("AXISSAME");
      legend->Draw();
      pt->Draw();
      pt2->Draw();
      pt3->Draw();

      pad2->cd();
      if(flag_logx) pad2->SetLogx();
      h_ratio->Draw("LPE");
      h_ratio->Draw("AXISSAME");

      std::string canvas_HERWIG_name = "c_HERWIG_" + histo_var.at(ivar) + jet_index + suffix; 
      std::string pad1_HERWIG_name = "pad1_HERWIG_" + histo_var.at(ivar) + jet_index + suffix; 
      std::string pad2_HERWIG_name = "pad2_HERWIG_" + histo_var.at(ivar) + jet_index + suffix; 
      TCanvas *c_HERWIG = new TCanvas(canvas_HERWIG_name.c_str(), "", 800, 900);
      TPad* pad1_HERWIG = new TPad(pad1_HERWIG_name.c_str(),"", 0, 0.3, 1, 1.0);
      pad1_HERWIG->SetBottomMargin(0.1);
      TPad* pad2_HERWIG = new TPad(pad2_HERWIG_name.c_str(),"", 0, 0.01, 1, 0.35);
      pad2_HERWIG->SetBottomMargin(0.35);
      pad1_HERWIG->Draw();
      pad2_HERWIG->Draw();

      pad1_HERWIG->cd();
      if(flag_logy) pad1_HERWIG->SetLogy();
      if(flag_logx) pad1_HERWIG->SetLogx();

      h_mc_HERWIG->Draw("LPE");
      h_data->Draw("LPESAME");
      h_data->Draw("AXISSAME");
      legend_HERWIG->Draw();
      pt->Draw();
      pt2->Draw();
      pt3->Draw();

      pad2_HERWIG->cd();
      if(flag_logx) pad2_HERWIG->SetLogx();
      h_ratio_HERWIG->Draw("LPE");
      h_ratio_HERWIG->Draw("AXISSAME");

      // TCanvas *c_SHERPA;
      // std::string canvas_SHERPA_name = "c_SHERPA_" + histo_var.at(ivar) + jet_index + suffix; 
      // if(!mSname.empty()){
      //  std::string pad1_SHERPA_name = "pad1_SHERPA_" + histo_var.at(ivar) + jet_index + suffix; 
      // std::string pad2_SHERPA_name = "pad2_SHERPA_" + histo_var.at(ivar) + jet_index + suffix; 
      // c_SHERPA = new TCanvas(canvas_SHERPA_name.c_str(), "", 800, 900);
      // TPad* pad1_SHERPA = new TPad(pad1_SHERPA_name.c_str(),"", 0, 0.3, 1, 1.0);
      // pad1_SHERPA->SetBottomMargin(0.1);
      // TPad* pad2_SHERPA = new TPad(pad2_SHERPA_name.c_str(),"", 0, 0.01, 1, 0.35);
      // pad2_SHERPA->SetBottomMargin(0.35);
      // pad1_SHERPA->Draw();
      // pad2_SHERPA->Draw();

      // pad1_SHERPA->cd();
      // if(flag_logy) pad1_SHERPA->SetLogy();
      // if(flag_logx) pad1_SHERPA->SetLogx();

      // h_mc_SHERPA->Draw("LPE");
      // h_data->Draw("LPESAME");
      // h_data->Draw("AXISSAME");
      // legend_SHERPA->Draw();
      // pt->Draw();
      // pt2->Draw();
      // pt3->Draw();

      // pad2_SHERPA->cd();
      // if(flag_logx) pad2_SHERPA->SetLogx();
      // h_ratio_SHERPA->Draw("LPE");
      // h_ratio_SHERPA->Draw("AXISSAME");
      // }

      f_output->cd();
      c->Write();
      c->SaveAs((outfolder+"/pdf/"+canvas_name+".pdf").c_str());
      c_HERWIG->Write();
      c_HERWIG->SaveAs((outfolder+"/pdf/"+canvas_HERWIG_name+".pdf").c_str());
      // if(!mSname.empty()){
      // c_SHERPA->Write();
      // c->SaveAs((outfolder+"/pdf/"+canvas_SHERPA_name+".pdf").c_str());
      // }
    }

    std::cout << "OK" << std::endl ;

  }

  f_output->Write();
  f_output->Close();
}




