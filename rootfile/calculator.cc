void calculator(){
	Double_t input_mt[4] = {160, 170, 180, 190};
	Double_t reco_mt[4] = {160.43, 168.83, 179.79, 190.89};
	Double_t reco_mt_error[4] = {2.69, 2.52, 1.46, 1.73};
	 
	TCanvas *c = new TCanvas("Reconstruction", "Reconstruction", 400, 400);
	c->SetLeftMargin(0.15);
	c->cd(1);
	TGraphErrors *g = new TGraphErrors(4, input_mt, reco_mt, 0, reco_mt_error);
	g->SetMarkerStyle(7);
	g->GetXaxis()->SetRangeUser(155,195);
	g->GetYaxis()->SetRangeUser(155,195);
	g->GetXaxis()->SetTitle("input m_{t}(GeV)");
	g->GetXaxis()->CenterTitle();
	g->GetYaxis()->SetTitle("reconstructed m_{t}(GeV)");
	g->GetYaxis()->CenterTitle();
	g->GetYaxis()->SetTitleOffset(2);
	g->SetTitle("top quark mass reconstruction");
	g->Draw("AP");

	//c->SaveAs(mw + "madweight_result" + ".pdf");
	//c->Close();
	
	TF1 *f = new TF1("f", "[0]*x + [1]", input_mt[0]-10, input_mt[3]+10);
	TF1 *fit = new TF1("fit", "[0]*x + [1]", input_mt[0]-10, input_mt[3]+10);
	TF1 *refer = new TF1("refer", "x", input_mt[0] -10 , input_mt[3]+10);
	g->Fit("f","","",input_mt[0], input_mt[3]);	

	Double_t par[2];
	f->GetParameters(&par[0]);
	fit->SetParameters(par);
	fit->Draw("SAME");

	refer->SetLineColor(12);
	refer->Draw("SAME");
	c->SaveAs("reconstruction.pdf");
}
