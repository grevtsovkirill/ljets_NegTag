

void TriggerBinPlotter() {
	



	TFile *jz0 = new TFile("../HistoMaker/HistoFiles/ConHistoFilemcJZ0W.root");
	TFile *jz1 = new TFile("../HistoMaker/HistoFiles/ConHistoFilemcJZ1W.root");
	TFile *jz2 = new TFile("../HistoMaker/HistoFiles/ConHistoFilemcJZ2W.root");
	TFile *jz3 = new TFile("../HistoMaker/HistoFiles/ConHistoFilemcJZ3W.root");
	TFile *jz4 = new TFile("../HistoMaker/HistoFiles/ConHistoFilemcJZ4W.root");
	TFile *jz5 = new TFile("../HistoMaker/HistoFiles/ConHistoFilemcJZ5W.root");


	TH1F *hj15pt = (TH1F*) jz0->Get("hpT");
	TH1F *hj25pt = (TH1F*) jz1->Get("hpT");
	TH1F *hj60pt = (TH1F*) jz2->Get("hpT");
    	TH1F *hj110pt = (TH1F*) jz3->Get("hpT");
	TH1F *hj175pt = (TH1F*) jz4->Get("hpT");
	TH1F *hj380pt = (TH1F*) jz5->Get("hpT");

	TCanvas *c = new TCanvas("c", "canvas", 800, 800);

	//TPad *pad1 = new TPad("pad1", "Leading Calo Jet p_{T}, back to back", 0, 0.3, 1, 1.0);
	//pad1->SetBottomMargin(0);
	//pad1->SetLogy();
	//pad1->SetLogx();
	//pad1->Draw();
	c->SetLogy();
	c->SetLogx();
	c->Draw();
	c->cd();
	//calopt_mc->Scale(calopt_d->Integral()/calopt_mc->Integral());
	
	hj15pt->SetAxisRange(10, 10000000000000, "Y");
	hj15pt->SetAxisRange(0, 4000, "X");
	
	hj15pt->Draw("hist");
	hj25pt->Draw("same hist");
	hj60pt->Draw("same hist");
    hj110pt->Draw("same hist");
    hj175pt->Draw("same hist");
    hj380pt->Draw("same hist");

	auto legendc = new TLegend(0.10,0.7, 0.15, 0.9);
	legendc->AddEntry(hj15pt, "jz0" , "f" );
	legendc->AddEntry(hj25pt, "jz1" , "f" );
    legendc->AddEntry(hj60pt, "jz2" , "f" );
    legendc->AddEntry(hj110pt, "jz3" , "f" );
    legendc->AddEntry(hj175pt, "jz4" , "f" );
    legendc->AddEntry(hj380pt, "jz5" , "f" );
	legendc->SetTextSize(0.02);
	legendc->Draw();
	
	
	hj15pt->SetLineColor(kCyan-9);
	hj15pt->SetLineWidth(2);
	//hj15pt->SetFillColorAlpha(kCyan-9, 0.25);
        hj25pt->SetLineColor(kOrange);
        hj25pt->SetLineWidth(2);
        //hj25pt->SetFillColorAlpha(kOrange, 0.25);
        hj60pt->SetLineColor(kPink);
        hj60pt->SetLineWidth(2);
        //hj60pt->SetFillColorAlpha(kViolet+10, 0.25);
        hj110pt->SetLineColor(kGreen+2);
        hj110pt->SetLineWidth(2);
        //hj110pt->SetFillColorAlpha(kGreen+2, 0.25);
        hj175pt->SetLineColor(kRed+2);
        hj175pt->SetLineWidth(2);
        //hj175pt->SetFillColorAlpha(kRed+2, 0.25);
        hj380pt->SetLineColor(kBlue-4);
        hj380pt->SetLineWidth(2);
        //hj380pt->SetFillColorAlpha(kBlue-4, 0.25);	
        hj15pt->SetTitle("Track Jet p_{T} In Different MC Slices");
        hj15pt->SetYTitle("Number of Events");
        hj15pt->SetXTitle("p_{T} [GeV]");
        hj15pt->GetYaxis()->SetTitleSize(20);
        hj15pt->GetYaxis()->SetTitleFont(43);
        hj15pt->GetYaxis()->SetTitleOffset(1.55);
        hj15pt->GetXaxis()->SetTitleSize(20);
        hj15pt->GetXaxis()->SetTitleFont(43);
        hj15pt->GetXaxis()->SetTitleOffset(4.);
        hj15pt->GetXaxis()->SetLabelFont(43);
        hj15pt->GetXaxis()->SetLabelSize(15);


c->SaveAs(("Control_plots/jpg/mcSliceComparison.jpg"));









}


 
