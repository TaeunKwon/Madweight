void analyze_event(TString sample_mass, TString dir, TString name){
	TString mw = "/afs/cern.ch/user/k/ktaeun/private/MG5_aMC_v2_6_3_2/";
	ifstream param_in(mw + dir + "/Cards/MadWeight_card.dat");
	ifstream mw_out(mw + dir + "/Events/"+ name + "/weights.out");
	
	vector<double> tmass;
	
	istringstream ss;	
		
	string line;
	while(getline(param_in, line)){
		ss.str(line);
		int index;
		int temp_tmass;
		ss>>index>>temp_tmass;
		ss.clear();
		if(index!=13) continue;
		tmass.push_back(temp_tmass);
	}	
	
	vector<int> param;
	vector<double> weight;
	vector<double> w_error;

	// Skip first two lines
	for (int i=0;i<2;++i){
		getline(mw_out, line);
	}
	
	int id = -1;
	int event_id = -2;
	while(getline(mw_out, line)){

		ss.str(line);
		
		int temp_param;
		double temp_weight;
		double temp_w_error;
		string trash;

		ss>>event_id>>temp_param>>trash>>temp_weight>>temp_w_error;
		ss.clear();
		if (id == event_id) continue;		
		if ((temp_weight == 0) && (weight.size() != 0) && (weight.at(weight.size()-1) != 0)){
			for (int i=1; i<temp_param;++i){
				weight.at(weight.size()-i) = 0;
				w_error.at(w_error.size()-i) = 0;
			}
			id = event_id;
		}
		param.push_back(temp_param);
		weight.push_back(temp_weight);
		w_error.push_back(temp_w_error);

	}
	mw_out.close();

	ifstream cs_in(mw + "MG_NORM/Events/scan_run_[28-76].txt");
	
	vector<double> cs_;
	vector<double> mass;
	getline(cs_in, line);
	while(getline(cs_in, line)){
		double temp_cs;
		double temp_mass;
		string trash;
		ss.str(line);
		ss>>trash>>temp_mass>>temp_cs;
		ss.clear();

		cs_.push_back(temp_cs);
		mass.push_back(temp_mass);
	}
	cs_in.close();
	
	/*ifstream cs_in2(mw + "cross_section_top2.txt");

	vector<double> cs_error_;
	getline(cs_in2, line);
	while(getline(cs_in2, line)){	
		double temp_cs_err;
		string trash;
		ss.str(line);
		ss>>trash>>trash>>trash>>temp_cs_err;
		ss.clear();
	
		cs_error_.push_back(temp_cs_err);
	}
	cs_in2.close();
*/
	vector<double> cs;
//	vector<double> cs_error;
	int j=0;
	for (int i=0; i<cs_.size(); ++i){
		if (abs(mass.at(i) - tmass.at(j))<0.01){
			cs.push_back(cs_.at(i));
//			cs_error.push_back(cs_error_.at(i));
			++j;
//			cout<<cs_.at(i)<<", "<<cs_error_.at(i)<<endl;
			if (j==tmass.size()) break;
		}
	}



	int n = (int)cs.size();
	double likelyhood[n];
	double lh_error[n];

	//cout<<n<<", "<<param.size()<<endl;
	for (int i=0; i< n;++i){
		likelyhood[i] = 0;
		lh_error[i] = 0;
		//cout<<i<<"th :"<<cs.at(i)<<endl;
	}	
	//Calculate normalized likelyhood
	for (int i=0; i<(int)param.size();++i){
		if (weight.at(i) == 0) continue;
		//cout<<"param : "<<param.at(i)<<", crossx : "<<TMath::Log(cs.at(param.at(i)-1))<<endl;
		likelyhood[param.at(i)-1]+=-TMath::Log(weight.at(i)) + TMath::Log(cs.at(param.at(i)-1));
		lh_error[param.at(i)-1]=TMath::Sqrt(lh_error[param.at(i)-1]*lh_error[param.at(i)-1] + (w_error.at(i)/weight.at(i))*(w_error.at(i)/weight.at(i)));// + (cs_error.at(param.at(i)-1)/cs.at(param.at(i)-1))*(cs_error.at(param.at(i)-1)/cs.at(param.at(i)-1)));
	}

	for (int i=0; i<n; ++i){
		cout<<likelyhood[i]<<" +- "<<lh_error[i]<<endl;
	}
	
	TFile output("/afs/cern.ch/user/k/ktaeun/private/rootfile/data/mt" + sample_mass + "_" + dir + ".root","RECREATE");
	
	TGraphErrors *g = new TGraphErrors(n, &tmass[0], likelyhood, 0, lh_error);
	g->SetMarkerStyle(7);
	g->GetXaxis()->SetTitle("m_{t}(MeV)");
	g->GetXaxis()->CenterTitle();
	g->GetYaxis()->SetTitle("-ln(L)");
	g->GetYaxis()->CenterTitle();
	g->GetYaxis()->SetTitleOffset(2);
	g->SetTitle("Logarithmic likelihood for different assumptions of m_{t}");
	
	g->SetName("MEM graph");
	g->Write();
	output.Write();
	output.Close();
		
}

