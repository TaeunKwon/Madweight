void plot(TString tmass, TString name, Int_t number){
	TString home = "/afs/cern.ch/user/k/ktaeun/private/rootfile/";

	Double_t *likelihood_sum;
	Double_t *mass;
	Double_t *lh_error;
	int n;
	for (Int_t j=0; j<number; ++j){
		TString num;
		num.Form("%d",j);
		TString fname = "mt" + tmass + "_" + name + num;
		TFile *rf = TFile::Open(home + "data/" + fname + ".root");
		if (rf ==NULL) continue;	
		TGraphErrors *g = (TGraphErrors *) rf->Get("MEM graph");	
		n = (int) g->GetN();	

		Double_t *likelihood;
		Double_t *lh_error_temp;
		likelihood = g->GetY();
		lh_error_temp = g->GetEY();
		for (int i=0; i<n; ++i){
			if (j==0){
				likelihood_sum = g->GetY();
				lh_error = g->GetEY();
				break;
			}
			likelihood_sum[i] += likelihood[i];
			lh_error[i] = TMath::Sqrt(lh_error_temp[i]*lh_error_temp[i] + lh_error[i]*lh_error[i]);
		}
		mass = g->GetX();
	
	}
	double min = likelihood_sum[0];
	int min_ind = 0;
	for (int i =0; i< n;++i){
		cout<<likelihood_sum[i]<<" +- "<<lh_error[i]<<endl;
		if (likelihood_sum[i]<min){
			min = likelihood_sum[i];
			min_ind = i;
		}
	}
	TGraphErrors *g_sum = new TGraphErrors(n, &mass[0], &likelihood_sum[0], 0, lh_error);
	TCanvas *c = new TCanvas("MEM","MEM",500,400);
	g_sum->SetMarkerStyle(7);
	g_sum->GetXaxis()->SetTitle("m_{t}(GeV)");
	g_sum->GetXaxis()->CenterTitle();
	g_sum->GetYaxis()->SetTitle("-ln(L)");
	g_sum->GetYaxis()->CenterTitle();
	g_sum->GetYaxis()->SetTitleOffset(2);
	g_sum->SetTitle("Logarithmic likelihood for different assumptions of m_{t}");
	c->SetLeftMargin(0.15);
	c->cd(1);
	g_sum->Draw("AP");
		
	TF1 *f = new TF1("f", "[0]*(x-[1])^2 + [2]", mass[0], mass[n-1]);
	TF1 *tot = new TF1("total", "[0]*(x-[1])^2 + [2]", mass[0], mass[n-1]);

	f->SetParameters(0.01, mass[min_ind],likelihood_sum[min_ind]); 
	g_sum->Fit("f","","",mass[min_ind-1], mass[min_ind+1]);

	Double_t par[3];
	f->GetParameters(&par[0]);
	Double_t syst_err = f->GetParError(1);
	
	tot->SetParameters(par);
	tot->Draw("SAME");
	c->SaveAs(home + "results/" + name + ".pdf");
	
	Double_t stat_err = TMath::Sqrt((0.5)/par[0]);

	ofstream result_mt;
	TString txt_ = home + "results/mt" + tmass + ".txt";
	const char *txt = (const char*) txt_;
	cout<<txt<<endl;
	result_mt.open(txt, ios::out | ios::app);
	cout<<Form("t mass is %.2f +- %.2f(stat) +- %.2f(syst) GeV", par[1], stat_err, syst_err)<<endl;
	result_mt.close();
}

