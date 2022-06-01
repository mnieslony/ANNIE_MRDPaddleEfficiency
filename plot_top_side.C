// Small macro to calculate efficiency values for the MRD paddle histograms
// Steps
// 1) Get the efficiency by dividing the number of observed hits in a paddle by the number of expected hits in a paddle
// 2) Calculate the uncertainty accordingly
// 3) Fill the result in histograms and save them
// 4) Save histograms in a ROOT-file
// Macro is going to be implemented as a separate tool in the ToolAnalysis framework later

void plot_top_side(){


	int num_slots = 24;
	int num_channels = 32;

	//TFile *f = new TFile("mrd_observed_R2018S0p0-p26_morethan1track_mrdefficiency.root","READ");
	TFile *f = new TFile("TimeClustering_R2018p0-p26_newnewnewnew.root","READ");
	TFile *out_file = new TFile("R2018_clusters_frequency_topside.root","RECREATE");
	TH2Poly *eff_top = new TH2Poly("eff_top","Efficiency - Top View",1.6,3.,-2,2.);
	TH2Poly *eff_side = new TH2Poly("eff_side","Efficiency - Side View",1.6,3.,-2.,2.);

	TH1F *eff_chankey = (TH1F*) f->Get("hist_chankey_cluster");

	eff_top->GetXaxis()->SetTitle("z [m]");
	eff_top->GetYaxis()->SetTitle("x [m]");
	eff_side->GetXaxis()->SetTitle("z [m]");
	eff_side->GetYaxis()->SetTitle("y [m]");
	eff_top->GetZaxis()->SetTitle("#epsilon");
	eff_side->GetZaxis()->SetTitle("#epsilon");
	eff_top->SetStats(0);
	eff_side->SetStats(0);
	eff_top->GetZaxis()->SetRangeUser(0.001,10000);
	eff_side->GetZaxis()->SetRangeUser(0.001,10000);

	int chankey_offset = 26;
	int channels_per_layer[11] = {26,30,26,34,26,26,26,30,26,30,26};
	int channels_start[11] = {26,52,82,108,142,168,194,220,250,276,306};
	int bins_start[11] = {1,27,57,83,117,143,169,195,225,251,281};
	double extents[11] = {1.318,1.146,1.318,1.299,1.318,1.318,1.318,1.521,1.318,1.521,1.318};
	double zmin[11] = {1.6798,1.8009,1.929,2.0501,2.1782,2.2993,2.4204,2.5415,2.6626,2.7837,2.9048};
	double zmax[11] = {1.6858,1.8139,1.935,2.0631,2.1842,2.3053,2.4264,2.5475,2.6686,2.7897,2.9108};

	double dummy;
	int temp_channel, temp_orientation, temp_half, temp_side, temp_elec_crate, temp_elec_slot, temp_elec_ch;
	double temp_xmin, temp_xmax, temp_ymin, temp_ymax, temp_zmin, temp_zmax;
	std::map<int,int> map_ch_orientation, map_ch_half, map_ch_side;
	std::map<int,double> map_ch_xmin, map_ch_xmax, map_ch_ymin, map_ch_ymax, map_ch_zmin, map_ch_zmax, map_ch_Crate, map_ch_Slot, map_ch_Channel;

	std::vector<int> active_slots_cr1, active_slots_cr2;
	std::vector<int> inactive_ch_crate1, inactive_slot_crate1, inactive_ch_crate2, inactive_slot_crate2;
	int active_channel[2][24]={{0}};

	ifstream geometry_file("mrdgeometry_noheader.dat");
	while(!geometry_file.eof()){
		geometry_file >> temp_channel >> temp_orientation >> temp_half >> temp_side >> temp_xmin >> temp_xmax >> temp_ymin >> temp_ymax >> temp_zmin >> temp_zmax >> temp_elec_crate >> temp_elec_slot >> temp_elec_ch; 
		if (geometry_file.eof()) break;
		std::cout <<"Reading in channel "<<temp_channel<<std::endl;
		map_ch_orientation.emplace(temp_channel,temp_orientation);
		map_ch_half.emplace(temp_channel,temp_half);
		map_ch_side.emplace(temp_channel,temp_side);
		map_ch_xmin.emplace(temp_channel,temp_xmin);
		map_ch_xmax.emplace(temp_channel,temp_xmax);
		map_ch_ymin.emplace(temp_channel,temp_ymin);
		map_ch_ymax.emplace(temp_channel,temp_ymax);
		map_ch_zmin.emplace(temp_channel,temp_zmin);
		map_ch_zmax.emplace(temp_channel,temp_zmax);
		map_ch_Crate.emplace(temp_channel,temp_elec_crate);
		map_ch_Slot.emplace(temp_channel,temp_elec_slot);
		map_ch_Channel.emplace(temp_channel,temp_elec_ch);
		if (temp_elec_crate == 7) active_slots_cr1.push_back(temp_elec_slot);
		else active_slots_cr2.push_back(temp_elec_slot);
		active_channel[temp_elec_crate-7][temp_elec_slot-1]=1;
	}
	geometry_file.close();

	ifstream inactive_file("inactive_channels.dat");
	while(!inactive_file.eof()){
		inactive_file >> temp_elec_crate >> temp_elec_slot >> temp_elec_ch;
		if (inactive_file.eof()) break;
		if (temp_elec_crate == 7){
			inactive_ch_crate1.push_back(temp_elec_ch);
			inactive_slot_crate1.push_back(temp_elec_slot);
		} else {
			inactive_ch_crate2.push_back(temp_elec_ch);
			inactive_slot_crate2.push_back(temp_elec_slot);
		}
	}
	inactive_file.close();

	double enlargeBoxes = 0.01;
	double shiftSecRow = 0.03;

	for (int i_layer = 0; i_layer < 11; i_layer++){

		for (int i_ch = 0; i_ch < channels_per_layer[i_layer]; i_ch++){

			if (i_layer%2 == 0) {
				if (i_ch < channels_per_layer[i_layer]/2.){
					eff_side->AddBin(zmin[i_layer]-enlargeBoxes,map_ch_ymin[channels_start[i_layer]+i_ch],zmax[i_layer]+enlargeBoxes,map_ch_ymax[channels_start[i_layer]+i_ch]);
				}else {
					eff_side->AddBin(zmin[i_layer]-enlargeBoxes+shiftSecRow,map_ch_ymin[channels_start[i_layer]+i_ch],zmax[i_layer]+shiftSecRow+enlargeBoxes,map_ch_ymax[channels_start[i_layer]+i_ch]);
				}	
			} else {
				if (i_ch < channels_per_layer[i_layer]/2.){
					eff_top->AddBin(zmin[i_layer]-enlargeBoxes,map_ch_xmin[channels_start[i_layer]+i_ch],zmax[i_layer]+enlargeBoxes,map_ch_xmax[channels_start[i_layer]+i_ch]);
				} else {
					eff_top->AddBin(zmin[i_layer]-enlargeBoxes+shiftSecRow,map_ch_xmin[channels_start[i_layer]+i_ch],zmax[i_layer]+shiftSecRow+enlargeBoxes,map_ch_xmax[channels_start[i_layer]+i_ch]);
				}
			
			}
		}
	}

	//Loop over MRD layers & create the histograms
	for (int i_layer = 0; i_layer < 11; i_layer++){

		//If first or last layer, don't calculate efficiencies
		//if (i_layer == 0 || i_layer == 10) continue;

		//Loop over channels within that layer
		for (int i_ch = 0; i_ch < channels_per_layer[i_layer]; i_ch++){

			int chankey = bins_start[i_layer]+i_ch+25;
			std::cout <<"chankey: "<<chankey;

			double bin_content = eff_chankey->GetBinContent(chankey+1);
			std::cout <<", bin content: "<<bin_content<<std::endl;

			double xcenter = 0.5*(map_ch_xmin[channels_start[i_layer]+i_ch]+map_ch_xmax[channels_start[i_layer]+i_ch]);
			double ycenter = 0.5*(map_ch_ymin[channels_start[i_layer]+i_ch]+map_ch_ymax[channels_start[i_layer]+i_ch]);
			double zcenter = 0.5*(map_ch_zmin[channels_start[i_layer]+i_ch]+map_ch_zmax[channels_start[i_layer]+i_ch]);
			if (map_ch_half[channels_start[i_layer]+i_ch]==1) zcenter+=shiftSecRow;

			if (map_ch_orientation[channels_start[i_layer]+i_ch]==0) eff_side->Fill(zcenter,ycenter,bin_content);
			else eff_top->Fill(zcenter,xcenter,bin_content);
		}
		
	}

out_file->cd();
eff_top->Write();
eff_side->Write();

out_file->Close();
delete out_file;





}
