#include "../AtlasStyle/AtlasStyle.C"

void plot_HFfrac()
{

  SetAtlasStyle();
  gStyle->SetPaintTextFormat("2.2f%");
  gStyle->SetNumberContours(6);

  // input
  std::string version = "";

  std::string file_ldg_input_name = version + "template_fit.root.noCorrections";
  //std::string file_ldg_data_input_name = version + "template_fit.root.pTetaCorrections";
  std::string file_ldg_data_input_name = version + "template_fit.root.noCorrections";

  std::string file_sldg_input_name = version + "template_fit.root.noCorrections";
  std::string file_sldg_data_input_name = version + "template_fit.root.noCorrections";

  // ldg
  TFile *f_ldg = TFile::Open(file_ldg_input_name.c_str());

  TH2D *h1_fracc = (TH2D*)f_ldg->Get("h_fracc_mc");
  TH2D *h1_fracb = (TH2D*)f_ldg->Get("h_fracb_mc");

  TH2D *h1_fracc_HERWIG = (TH2D*)f_ldg->Get("h_fracc_mc_HERWIG");
  TH2D *h1_fracb_HERWIG = (TH2D*)f_ldg->Get("h_fracb_mc_HERWIG");

  TH2D *h1_fracc_var = (TH2D*)f_ldg->Get("h_fracc_var");
  TH2D *h1_fracb_var = (TH2D*)f_ldg->Get("h_fracb_var");

  // ldg - datafit / data + MC uncertainties
  TFile *f_ldg_data = TFile::Open(file_ldg_data_input_name.c_str());

  TH2D *h1_fracc_data = (TH2D*)f_ldg_data->Get("h_fracc_data");
  TH2D *h1_fracb_data = (TH2D*)f_ldg_data->Get("h_fracb_data");

  TH2D *h1_chi2 = (TH2D*)f_ldg_data->Get("h_chi2");

  // sldg
  TFile *f_sldg = TFile::Open(file_sldg_input_name.c_str());

  TH2D *h2_fracc = (TH2D*)f_sldg->Get("h_fracc_mc");
  TH2D *h2_fracb = (TH2D*)f_sldg->Get("h_fracb_mc");

  TH2D *h2_fracc_HERWIG = (TH2D*)f_sldg->Get("h_fracc_mc_HERWIG");
  TH2D *h2_fracb_HERWIG = (TH2D*)f_sldg->Get("h_fracb_mc_HERWIG");

  TH2D *h2_fracc_var = (TH2D*)f_sldg->Get("h_fracc_var");
  TH2D *h2_fracb_var = (TH2D*)f_sldg->Get("h_fracb_var");

  // sldg - datafit / data + MC uncertainties
  TFile *f_sldg_data = TFile::Open(file_sldg_data_input_name.c_str());

  TH2D *h2_fracc_data = (TH2D*)f_sldg_data->Get("h_fracc_data");
  TH2D *h2_fracb_data = (TH2D*)f_sldg_data->Get("h_fracb_data");

  TH2D *h2_chi2 = (TH2D*)f_sldg_data->Get("h_chi2");

  h1_fracc->Scale(100.);
  h1_fracb->Scale(100.);
  h1_fracc_HERWIG->Scale(100.);
  h1_fracb_HERWIG->Scale(100.);
  h1_fracc_var->Scale(100.);
  h1_fracb_var->Scale(100.);
  h1_fracc_data->Scale(100.);
  h1_fracb_data->Scale(100.);

  h2_fracc->Scale(100.);
  h2_fracb->Scale(100.);
  h2_fracc_HERWIG->Scale(100.);
  h2_fracb_HERWIG->Scale(100.);
  h2_fracc_var->Scale(100.);
  h2_fracb_var->Scale(100.);
  h2_fracc_data->Scale(100.);
  h2_fracb_data->Scale(100.);

  // output
  TFile *f_out = new TFile("fracHF.root","RECREATE");

  // X Title
  h1_fracc->SetXTitle("p_{T}^{jet} [GeV]");
  h1_fracb->SetXTitle("p_{T}^{jet} [GeV]");
  h1_fracc_HERWIG->SetXTitle("p_{T}^{jet} [GeV]");
  h1_fracb_HERWIG->SetXTitle("p_{T}^{jet} [GeV]");
  h1_fracc_var->SetXTitle("p_{T}^{jet} [GeV]");
  h1_fracb_var->SetXTitle("p_{T}^{jet} [GeV]");
  h1_fracc_data->SetXTitle("p_{T}^{jet} [GeV]");
  h1_fracb_data->SetXTitle("p_{T}^{jet} [GeV]");

  h2_fracc->SetXTitle("p_{T,2}^{jet} [GeV]");
  h2_fracb->SetXTitle("p_{T,2}^{jet} [GeV]");
  h2_fracc_HERWIG->SetXTitle("p_{T}^{jet} [GeV]");
  h2_fracb_HERWIG->SetXTitle("p_{T}^{jet} [GeV]");
  h2_fracc_var->SetXTitle("p_{T,2}^{jet} [GeV]");
  h2_fracb_var->SetXTitle("p_{T,2}^{jet} [GeV]");
  h2_fracc_data->SetXTitle("p_{T,2}^{jet} [GeV]");
  h2_fracb_data->SetXTitle("p_{T,2}^{jet} [GeV]"); 

  h1_chi2->SetXTitle("p_{T}^{jet} [GeV]");
  h2_chi2->SetXTitle("p_{T,2}^{jet} [GeV]");

  // Y Title
  h1_fracc->SetYTitle("#eta^{jet}");
  h1_fracb->SetYTitle("#eta^{jet}");
  h1_fracc_HERWIG->SetYTitle("#eta^{jet}");
  h1_fracb_HERWIG->SetYTitle("#eta^{jet}");
  h1_fracc_var->SetYTitle("#eta^{jet}");
  h1_fracb_var->SetYTitle("#eta^{jet}");
  h1_fracc_data->SetYTitle("#eta^{jet}");
  h1_fracb_data->SetYTitle("#eta^{jet}");

  h2_fracc->SetYTitle("#eta_{2}^{jet}");
  h2_fracb->SetYTitle("#eta_{2}^{jet}");
  h2_fracc_HERWIG->SetYTitle("#eta_{2}^{jet}");
  h2_fracb_HERWIG->SetYTitle("#eta_{2}^{jet}");
  h2_fracc_var->SetYTitle("#eta_{2}^{jet}");
  h2_fracb_var->SetYTitle("#eta_{2}^{jet}");
  h2_fracc_data->SetYTitle("#eta_{2}^{jet}");
  h2_fracb_data->SetYTitle("#eta_{2}^{jet}"); 

  h1_chi2->SetYTitle("#eta^{jet}");
  h2_chi2->SetYTitle("#eta_{2}^{jet}");

  // Z Title
  h1_fracc->SetZTitle("c-fraction (PYTHIA)");
  h1_fracb->SetZTitle("b-fraction (PYTHIA)");
  h1_fracc_HERWIG->SetZTitle("c-fraction in % (HERWIG++)");
  h1_fracb_HERWIG->SetZTitle("b-fraction in % (HERWIG++)");
  h1_fracc_var->SetZTitle("c-fraction diff. in % (fit vs Pythia)");
  h1_fracb_var->SetZTitle("b-fraction diff. in % (fit vs Pythia)");
  h1_fracc_data->SetZTitle("c-fraction (fit)");
  h1_fracb_data->SetZTitle("b-fraction (fit)");

  h2_fracc->SetZTitle("c-fraction (PYTHIA)");
  h2_fracb->SetZTitle("b-fraction (PYTHIA)");
  h2_fracc_HERWIG->SetZTitle("c-fraction in % (HERWIG++)");
  h2_fracb_HERWIG->SetZTitle("b-fraction in % (HERWIG++)");
  h2_fracc_var->SetZTitle("c-fraction diff. in % (fit vs Pythia)");
  h2_fracb_var->SetZTitle("b-fraction diff. in % (fit vs Pythia)");
  h2_fracc_data->SetZTitle("c-fraction (fit)");
  h2_fracb_data->SetZTitle("b-fraction (fit)"); 

  h1_chi2->SetZTitle("#chi^{2}/N_{dof}");
  h2_chi2->SetZTitle("#chi^{2}/N_{dof}");

  // Y Title offset
  h1_fracc->GetYaxis()->SetTitleOffset(1.0);
  h1_fracb->GetYaxis()->SetTitleOffset(1.0);
  h1_fracc_HERWIG->GetYaxis()->SetTitleOffset(1.0);
  h1_fracb_HERWIG->GetYaxis()->SetTitleOffset(1.0);
  h1_fracc_var->GetYaxis()->SetTitleOffset(1.0);
  h1_fracb_var->GetYaxis()->SetTitleOffset(1.0);
  h1_fracc_data->GetYaxis()->SetTitleOffset(1.0);
  h1_fracb_data->GetYaxis()->SetTitleOffset(1.0);

  h2_fracc->GetYaxis()->SetTitleOffset(1.0);
  h2_fracb->GetYaxis()->SetTitleOffset(1.0);
  h2_fracc_HERWIG->GetYaxis()->SetTitleOffset(1.0);
  h2_fracb_HERWIG->GetYaxis()->SetTitleOffset(1.0);
  h2_fracc_var->GetYaxis()->SetTitleOffset(1.0);
  h2_fracb_var->GetYaxis()->SetTitleOffset(1.0);
  h2_fracc_data->GetYaxis()->SetTitleOffset(1.0);
  h2_fracb_data->GetYaxis()->SetTitleOffset(1.0); 

  h1_chi2->GetYaxis()->SetTitleOffset(1.0);
  h2_chi2->GetYaxis()->SetTitleOffset(1.0);

  // Z Title offset
  h1_fracc->GetZaxis()->SetTitleOffset(0.8);
  h1_fracb->GetZaxis()->SetTitleOffset(0.8);
  h1_fracc_HERWIG->GetZaxis()->SetTitleOffset(0.8);
  h1_fracb_HERWIG->GetZaxis()->SetTitleOffset(0.8);
  h1_fracc_var->GetZaxis()->SetTitleOffset(0.8);
  h1_fracb_var->GetZaxis()->SetTitleOffset(0.8);
  h1_fracc_data->GetZaxis()->SetTitleOffset(0.8);
  h1_fracb_data->GetZaxis()->SetTitleOffset(0.8);

  h2_fracc->GetZaxis()->SetTitleOffset(0.8);
  h2_fracb->GetZaxis()->SetTitleOffset(0.8);
  h2_fracc_HERWIG->GetZaxis()->SetTitleOffset(0.8);
  h2_fracb_HERWIG->GetZaxis()->SetTitleOffset(0.8);
  h2_fracc_var->GetZaxis()->SetTitleOffset(0.8);
  h2_fracb_var->GetZaxis()->SetTitleOffset(0.8);
  h2_fracc_data->GetZaxis()->SetTitleOffset(0.8);
  h2_fracb_data->GetZaxis()->SetTitleOffset(0.8); 

  h1_chi2->GetZaxis()->SetTitleOffset(0.8);
  h2_chi2->GetZaxis()->SetTitleOffset(0.8);

  // Leading for data and PYTHIA
  h1_fracc->SetBarOffset(0.2);
  h1_fracb->SetBarOffset(0.2);
  h1_fracc_HERWIG->SetBarOffset(0.2);
  h1_fracb_HERWIG->SetBarOffset(0.2);
  h1_fracc_var->SetBarOffset(0.2);
  h1_fracb_var->SetBarOffset(0.2);
  h1_fracc_data->SetBarOffset(0.2);
  h1_fracb_data->SetBarOffset(0.2);
  h1_chi2->SetBarOffset(0.2);

  h2_fracc->SetBarOffset(-0.2);
  h2_fracb->SetBarOffset(-0.2);
  h2_fracc_HERWIG->SetBarOffset(-0.2);
  h2_fracb_HERWIG->SetBarOffset(-0.2);
  h2_fracc_var->SetBarOffset(-0.2);
  h2_fracb_var->SetBarOffset(-0.2);
  h2_fracc_data->SetBarOffset(-0.2);
  h2_fracb_data->SetBarOffset(-0.2);
  h2_chi2->SetBarOffset(-0.2);

  h1_fracc->SetMarkerSize(1.5);
  h1_fracb->SetMarkerSize(1.5);
  h1_fracc_HERWIG->SetMarkerSize(1.5);
  h1_fracb_HERWIG->SetMarkerSize(1.5);
  h1_fracc_var->SetMarkerSize(1.5);
  h1_fracb_var->SetMarkerSize(1.5);
  h1_fracc_data->SetMarkerSize(1.5);
  h1_fracb_data->SetMarkerSize(1.5);
  h1_chi2->SetMarkerSize(1.5);

  h2_fracc->SetMarkerSize(1.5);
  h2_fracb->SetMarkerSize(1.5);
  h2_fracc_HERWIG->SetMarkerSize(1.5);
  h2_fracb_HERWIG->SetMarkerSize(1.5);
  h2_fracc_var->SetMarkerSize(1.5);
  h2_fracb_var->SetMarkerSize(1.5);
  h2_fracc_data->SetMarkerSize(1.5);
  h2_fracb_data->SetMarkerSize(1.5);
  h2_chi2->SetMarkerSize(1.5);

  h2_fracc->SetMarkerColor(kRed+1);
  h2_fracb->SetMarkerColor(kRed+1);
  h2_fracc_HERWIG->SetMarkerColor(kRed+1);
  h2_fracb_HERWIG->SetMarkerColor(kRed+1);
  h2_fracc_var->SetMarkerColor(kRed+1);
  h2_fracb_var->SetMarkerColor(kRed+1);
  h2_fracc_data->SetMarkerColor(kRed+1);
  h2_fracb_data->SetMarkerColor(kRed+1);
  h2_chi2->SetMarkerColor(kRed+1);

  TCanvas *c1_fracc = new TCanvas("c1_fracc","",1200,600);
  c1_fracc->SetLogx();
  h1_fracc->Draw("TEXTCOLZE");
  h2_fracc->Draw("TEXTESAME");
  c1_fracc->Write();

  TCanvas *c1_fracb = new TCanvas("c1_fracb","",1200,600);
  c1_fracb->SetLogx();
  h1_fracb->Draw("TEXTCOLZE");
  h2_fracb->Draw("TEXTESAME");
  c1_fracb->Write();

  TCanvas *c1_fracc_HERWIG = new TCanvas("c1_fracc_HERWIG","",1200,600);
  c1_fracc_HERWIG->SetLogx();
  h1_fracc_HERWIG->Draw("TEXTCOLZ");
//  h2_fracc_HERWIG->Draw("TEXTSAME");
  c1_fracc_HERWIG->Write();

  TCanvas *c1_fracb_HERWIG = new TCanvas("c1_fracb_HERWIG","",1200,600);
  c1_fracb_HERWIG->SetLogx();
  h1_fracb_HERWIG->Draw("TEXTCOLZ");
//  h2_fracb_HERWIG->Draw("TEXTSAME");
  c1_fracb_HERWIG->Write();

  TCanvas *c1_fracc_var = new TCanvas("c1_fracc_var","",1200,600);
  c1_fracc_var->SetLogx();
  h1_fracc_var->Draw("TEXTCOLZ");
  h2_fracc_var->Draw("TEXTSAME");
  c1_fracc_var->Write();

  TCanvas *c1_fracb_var = new TCanvas("c1_fracb_var","",1200,600);
  c1_fracb_var->SetLogx();
  h1_fracb_var->Draw("TEXTCOLZ");
  h2_fracb_var->Draw("TEXTSAME");
  c1_fracb_var->Write();

  TCanvas *c1_fracc_data = new TCanvas("c1_fracc_data","",1200,600);
  c1_fracc_data->SetLogx();
  h1_fracc_data->Draw("TEXTCOLZE");
  h2_fracc_data->Draw("TEXTESAME");
  c1_fracc_data->Write();

  TCanvas *c1_fracb_data = new TCanvas("c1_fracb_data","",1200,600);
  c1_fracb_data->SetLogx();
  h1_fracb_data->Draw("TEXTCOLZE");
  h2_fracb_data->Draw("TEXTESAME");
  c1_fracb_data->Write();

  TCanvas *c1_chi2 = new TCanvas("c1_chi2","",1200,600);
  c1_chi2->SetLogx();
  h1_chi2->Draw("TEXTCOLZ");
  h2_chi2->Draw("TEXTSAME");
  c1_chi2->Write();

}
