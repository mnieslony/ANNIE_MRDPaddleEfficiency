// Small macro to plot frequency values for the MRD paddle histograms
// Steps
// 1) Get the frequency of (clustered hits) from TimeClustering*root file
// 2) Convert chankey --> electronics channel
// 3) Fill the result in electronics histograms and save them
// 4) Save histograms in a ROOT-file
// Macro is going to be implemented as a separate tool in the ToolAnalysis framework later

void plot_frequency(const char *inputfile="TimeClustering_R3108.root", const char* outputfile="MRDChannelFrequency_R3108_Cosmic.root", const char* label="MRDChannelFrequency_R3108"){


	int num_slots = 24;
	int num_channels = 32;

	std::stringstream file_in,file_out;
	file_in << "timeclustering/"<<inputfile;
	file_out << "frequency_rootfiles/"<<outputfile;

	TFile *f = new TFile(file_in.str().c_str(),"READ");
	TFile *out_file = new TFile(file_out.str().c_str(),"RECREATE");
	TH1D *freq_chankey = new TH1D("freq_chankey","Frequency channelkey",310,26,336);
	TH2Poly *freq_top = new TH2Poly("freq_top","Frequency - Top View",1.6,3.,-2,2.);
	TH2Poly *freq_side = new TH2Poly("freq_side","Frequency - Side View",1.6,3.,-2.,2.);
	TH2D *freq_crate1 = new TH2D("freq_crate1","Frequency - Rack 7",num_slots,0,num_slots,num_channels,0,num_channels);
	TH2D *freq_crate2 = new TH2D("freq_crate2","Frequency - Rack 8",num_slots,0,num_slots,num_channels,0,num_channels);
	TH1D *freq_chankey_cluster = new TH1D("freq_chankey_cluster","Frequency channelkey (clustered)",310,26,336);
	TH2Poly *freq_top_cluster = new TH2Poly("freq_top_cluster","Frequency - Top View (clustered)",1.6,3.,-2,2.);
	TH2Poly *freq_side_cluster = new TH2Poly("freq_side_cluster","Frequency - Side View (clustered)",1.6,3.,-2.,2.);
	TH2D *freq_crate1_cluster = new TH2D("freq_crate1_cluster","Frequency - Rack 7 (clustered)",num_slots,0,num_slots,num_channels,0,num_channels);
	TH2D *freq_crate2_cluster = new TH2D("freq_crate2_cluster","Frequency - Rack 8 (clustered)",num_slots,0,num_slots,num_channels,0,num_channels);
	TH2D *freq_crate1_multi = new TH2D("freq_crate1_multi","Frequency - Rack 7 (multi-hit)",num_slots,0,num_slots,num_channels,0,num_channels);
	TH2D *freq_crate2_multi = new TH2D("freq_crate2_multi","Frequency - Rack 8 (multi-hit)",num_slots,0,num_slots,num_channels,0,num_channels);

	std::vector<TBox*> vector_box_inactive;
	freq_chankey->GetXaxis()->SetTitle("chankey");
	freq_chankey->GetYaxis()->SetTitle("frequency");	
	freq_chankey->SetStats(0);
	freq_top->GetXaxis()->SetTitle("z [m]");
	freq_top->GetYaxis()->SetTitle("x [m]");
	freq_side->GetXaxis()->SetTitle("z [m]");
	freq_side->GetYaxis()->SetTitle("y [m]");
	freq_top->GetZaxis()->SetTitle("#");
	freq_side->GetZaxis()->SetTitle("#");
	freq_top->SetStats(0);
	freq_side->SetStats(0);

	freq_crate1->SetStats(0);
	freq_crate1->GetXaxis()->SetNdivisions(num_slots);
	freq_crate1->GetYaxis()->SetNdivisions(num_channels);
	for (int i_label=0;i_label<int(num_channels);i_label++){
		std::stringstream ss_slot, ss_ch;
		ss_slot<<(i_label+1);
		ss_ch<<(i_label);
		std::string str_ch = "ch "+ss_ch.str();
		freq_crate1->GetYaxis()->SetBinLabel(i_label+1,str_ch.c_str());
		if (i_label<num_slots){
			std::string str_slot = "slot "+ss_slot.str();
			freq_crate1->GetXaxis()->SetBinLabel(i_label+1,str_slot.c_str());
		}
	}
	freq_crate1->LabelsOption("v");
	freq_crate2->SetStats(0);
	freq_crate2->GetXaxis()->SetNdivisions(num_slots);
	freq_crate2->GetYaxis()->SetNdivisions(num_channels);
	for (int i_label=0;i_label<int(num_channels);i_label++){
		std::stringstream ss_slot, ss_ch;
		ss_slot<<(i_label+1);
		ss_ch<<(i_label);
		std::string str_ch = "ch "+ss_ch.str();
		freq_crate2->GetYaxis()->SetBinLabel(i_label+1,str_ch.c_str());
		if (i_label<num_slots){
			std::string str_slot = "slot "+ss_slot.str();
			freq_crate2->GetXaxis()->SetBinLabel(i_label+1,str_slot.c_str());
    		}
  	}
	freq_crate2->LabelsOption("v");
	freq_chankey_cluster->GetXaxis()->SetTitle("chankey");
	freq_chankey_cluster->GetYaxis()->SetTitle("frequency");	
	freq_chankey_cluster->SetStats(0);
	freq_top_cluster->GetXaxis()->SetTitle("z [m]");
	freq_top_cluster->GetYaxis()->SetTitle("x [m]");
	freq_side_cluster->GetXaxis()->SetTitle("z [m]");
	freq_side_cluster->GetYaxis()->SetTitle("y [m]");
	freq_top_cluster->GetZaxis()->SetTitle("#");
	freq_side_cluster->GetZaxis()->SetTitle("#");
	freq_top_cluster->SetStats(0);
	freq_side_cluster->SetStats(0);


	freq_crate1_cluster->SetStats(0);
	freq_crate1_cluster->GetXaxis()->SetNdivisions(num_slots);
	freq_crate1_cluster->GetYaxis()->SetNdivisions(num_channels);
	for (int i_label=0;i_label<int(num_channels);i_label++){
		std::stringstream ss_slot, ss_ch;
		ss_slot<<(i_label+1);
		ss_ch<<(i_label);
		std::string str_ch = "ch "+ss_ch.str();
		freq_crate1_cluster->GetYaxis()->SetBinLabel(i_label+1,str_ch.c_str());
		if (i_label<num_slots){
			std::string str_slot = "slot "+ss_slot.str();
			freq_crate1_cluster->GetXaxis()->SetBinLabel(i_label+1,str_slot.c_str());
		}
	}
	freq_crate1_cluster->LabelsOption("v");
	freq_crate2_cluster->SetStats(0);
	freq_crate2_cluster->GetXaxis()->SetNdivisions(num_slots);
	freq_crate2_cluster->GetYaxis()->SetNdivisions(num_channels);
	for (int i_label=0;i_label<int(num_channels);i_label++){
		std::stringstream ss_slot, ss_ch;
		ss_slot<<(i_label+1);
		ss_ch<<(i_label);
		std::string str_ch = "ch "+ss_ch.str();
		freq_crate2_cluster->GetYaxis()->SetBinLabel(i_label+1,str_ch.c_str());
		if (i_label<num_slots){
			std::string str_slot = "slot "+ss_slot.str();
			freq_crate2_cluster->GetXaxis()->SetBinLabel(i_label+1,str_slot.c_str());
    		}
  	}
	freq_crate2_cluster->LabelsOption("v");	
	freq_crate1_multi->SetStats(0);
	freq_crate1_multi->GetXaxis()->SetNdivisions(num_slots);
	freq_crate1_multi->GetYaxis()->SetNdivisions(num_channels);
	for (int i_label=0;i_label<int(num_channels);i_label++){
		std::stringstream ss_slot, ss_ch;
		ss_slot<<(i_label+1);
		ss_ch<<(i_label);
		std::string str_ch = "ch "+ss_ch.str();
		freq_crate1_multi->GetYaxis()->SetBinLabel(i_label+1,str_ch.c_str());
		if (i_label<num_slots){
			std::string str_slot = "slot "+ss_slot.str();
			freq_crate1_multi->GetXaxis()->SetBinLabel(i_label+1,str_slot.c_str());
		}
	}
	freq_crate1_multi->LabelsOption("v");
	freq_crate2_multi->SetStats(0);
	freq_crate2_multi->GetXaxis()->SetNdivisions(num_slots);
	freq_crate2_multi->GetYaxis()->SetNdivisions(num_channels);
	for (int i_label=0;i_label<int(num_channels);i_label++){
		std::stringstream ss_slot, ss_ch;
		ss_slot<<(i_label+1);
		ss_ch<<(i_label);
		std::string str_ch = "ch "+ss_ch.str();
		freq_crate2_multi->GetYaxis()->SetBinLabel(i_label+1,str_ch.c_str());
		if (i_label<num_slots){
			std::string str_slot = "slot "+ss_slot.str();
			freq_crate2_multi->GetXaxis()->SetBinLabel(i_label+1,str_slot.c_str());
    		}
  	}
	freq_crate2_multi->LabelsOption("v");	
	TCanvas *canvas_elec = new TCanvas("canvas_elec","Canvas Electronics Space",900,600);
	TCanvas *canvas_elec_cluster = new TCanvas("canvas_elec_cluster","Canvas Electronics Space",900,600);
	TCanvas *canvas_elec_multi = new TCanvas("canvas_elec_multi","Canvas Electronics Space",900,600);

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

	ifstream geometry_file("db/mrdgeometry_noheader_09-29-20.dat");
	while(!geometry_file.eof()){
		geometry_file >> temp_channel >> temp_orientation >> temp_half >> temp_side >> temp_xmin >> temp_xmax >> temp_ymin >> temp_ymax >> temp_zmin >> temp_zmax >> temp_elec_crate >> temp_elec_slot >> temp_elec_ch; 
		if (geometry_file.eof()) break;
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

	ifstream inactive_file("db/inactive_channels.dat");
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
					freq_side->AddBin(zmin[i_layer]-enlargeBoxes,map_ch_ymin[channels_start[i_layer]+i_ch],zmax[i_layer]+enlargeBoxes,map_ch_ymax[channels_start[i_layer]+i_ch]);
					freq_side_cluster->AddBin(zmin[i_layer]-enlargeBoxes,map_ch_ymin[channels_start[i_layer]+i_ch],zmax[i_layer]+enlargeBoxes,map_ch_ymax[channels_start[i_layer]+i_ch]);
				}else {
					freq_side->AddBin(zmin[i_layer]-enlargeBoxes+shiftSecRow,map_ch_ymin[channels_start[i_layer]+i_ch],zmax[i_layer]+shiftSecRow+enlargeBoxes,map_ch_ymax[channels_start[i_layer]+i_ch]);
					freq_side_cluster->AddBin(zmin[i_layer]-enlargeBoxes+shiftSecRow,map_ch_ymin[channels_start[i_layer]+i_ch],zmax[i_layer]+shiftSecRow+enlargeBoxes,map_ch_ymax[channels_start[i_layer]+i_ch]);
				}	
			} else {
				if (i_ch < channels_per_layer[i_layer]/2.){
					freq_top->AddBin(zmin[i_layer]-enlargeBoxes,map_ch_xmin[channels_start[i_layer]+i_ch],zmax[i_layer]+enlargeBoxes,map_ch_xmax[channels_start[i_layer]+i_ch]);
					freq_top_cluster->AddBin(zmin[i_layer]-enlargeBoxes,map_ch_xmin[channels_start[i_layer]+i_ch],zmax[i_layer]+enlargeBoxes,map_ch_xmax[channels_start[i_layer]+i_ch]);
				} else {
					freq_top->AddBin(zmin[i_layer]-enlargeBoxes+shiftSecRow,map_ch_xmin[channels_start[i_layer]+i_ch],zmax[i_layer]+shiftSecRow+enlargeBoxes,map_ch_xmax[channels_start[i_layer]+i_ch]);
					freq_top_cluster->AddBin(zmin[i_layer]-enlargeBoxes+shiftSecRow,map_ch_xmin[channels_start[i_layer]+i_ch],zmax[i_layer]+shiftSecRow+enlargeBoxes,map_ch_xmax[channels_start[i_layer]+i_ch]);
				}
			
			}
		}
	}

    TH1D *chankey_freq = (TH1D*) f->Get("hist_chankey");
    TH1D *chankey_freq_cluster = (TH1D*) f->Get("hist_chankey_cluster_cosmic");
    TH1D *chankey_freq_multi = (TH1D*) f->Get("hist_chankey_multi");

    for (int i_ch=0; i_ch < chankey_freq->GetXaxis()->GetNbins(); i_ch++){

    	int i= i_ch;
    	int i_bin = i_ch+26;
    	double freq = chankey_freq->GetBinContent(i_bin+1);
    	double freq_cluster = chankey_freq_cluster->GetBinContent(i_bin+1);
    	double freq_multi = chankey_freq_multi->GetBinContent(i_bin+1);
    	double chankey = chankey_freq->GetBinCenter(i_bin+1);
    	freq_chankey->SetBinContent(i+1,freq);
    	freq_chankey_cluster->SetBinContent(i+1,freq_cluster);

		double xcenter = 0.5*(map_ch_xmin[channels_start[0]+i]+map_ch_xmax[channels_start[0]+i]);
		double ycenter = 0.5*(map_ch_ymin[channels_start[0]+i]+map_ch_ymax[channels_start[0]+i]);
		double zcenter = 0.5*(map_ch_zmin[channels_start[0]+i]+map_ch_zmax[channels_start[0]+i]);
		if (map_ch_half[channels_start[0]+i]==1) zcenter+=shiftSecRow;

		if (map_ch_orientation[channels_start[0]+i]==0) {
			freq_side->Fill(zcenter,ycenter,freq);
			freq_side_cluster->Fill(zcenter,ycenter,freq_cluster);
		}
		else {
			freq_top->Fill(zcenter,xcenter,freq);
			freq_top_cluster->Fill(zcenter,xcenter,freq_cluster);
		}

		if (map_ch_Crate[channels_start[0]+i]==7) {
			freq_crate1->SetBinContent(map_ch_Slot[channels_start[0]+i],map_ch_Channel[channels_start[0]+i]+1,freq);
			freq_crate1_cluster->SetBinContent(map_ch_Slot[channels_start[0]+i],map_ch_Channel[channels_start[0]+i]+1,freq_cluster);
			freq_crate1_multi->SetBinContent(map_ch_Slot[channels_start[0]+i],map_ch_Channel[channels_start[0]+i]+1,freq_multi);
		}
		else {
			freq_crate2->SetBinContent(map_ch_Slot[channels_start[0]+i],map_ch_Channel[channels_start[0]+i]+1,freq);
			freq_crate2_cluster->SetBinContent(map_ch_Slot[channels_start[0]+i],map_ch_Channel[channels_start[0]+i]+1,freq_cluster);
			freq_crate2_multi->SetBinContent(map_ch_Slot[channels_start[0]+i],map_ch_Channel[channels_start[0]+i]+1,freq_multi);
		}

	}

out_file->cd();
freq_chankey->Write();
freq_chankey_cluster->Write();
freq_top->Write();
freq_side->Write();
freq_top_cluster->Write();
freq_side_cluster->Write();

TCanvas *ctemp = new TCanvas("ctemp","Temporary canvas",900,600);
ctemp->cd();
freq_top->Draw("colz");
std::stringstream ss_spatial;
ss_spatial << "frequency_rootfiles/" << label << "_TopView.pdf";
ctemp->SaveAs(ss_spatial.str().c_str());

ctemp->Clear();
freq_side->Draw("colz");
ss_spatial.str("");
ss_spatial << "frequency_rootfiles/" << label << "_SideView.pdf";
ctemp->SaveAs(ss_spatial.str().c_str());

ctemp->Clear();
freq_top_cluster->Draw("colz");
ss_spatial.str("");
ss_spatial << "frequency_rootfiles/" << label << "_Cluster_TopView.pdf";
ctemp->SaveAs(ss_spatial.str().c_str());

ctemp->Clear();
freq_side_cluster->Draw("colz");
ss_spatial.str("");
ss_spatial << "frequency_rootfiles/" << label << "_Cluster_SideView.pdf";
ctemp->SaveAs(ss_spatial.str().c_str());

canvas_elec->cd();
canvas_elec->Divide(2,1);
TPad *p1 = (TPad*) canvas_elec->cd(1);
p1->SetGrid();
freq_crate1->Draw("colz");
int inactive_crate1 = 0;
int inactive_crate2 = 0;
for (int i_slot=0;i_slot<num_slots;i_slot++){
	if (active_channel[0][i_slot]==0){
		TBox *box_inactive = new TBox(i_slot,0,i_slot+1,num_channels);
		vector_box_inactive.push_back(box_inactive);
		box_inactive->SetFillStyle(3004);
		box_inactive->SetFillColor(1);
		inactive_crate1++;
	}
}
for (int i_ch = 0; i_ch < inactive_ch_crate1.size(); i_ch++){
	TBox *box_inactive = new TBox(inactive_slot_crate1.at(i_ch)-1,inactive_ch_crate1.at(i_ch),inactive_slot_crate1.at(i_ch),inactive_ch_crate1.at(i_ch)+1);
	vector_box_inactive.push_back(box_inactive);
	box_inactive->SetFillStyle(3004);
	box_inactive->SetFillColor(1);
	inactive_crate1++;
}

for (int i_slot=0;i_slot<num_slots;i_slot++){
	if (active_channel[1][i_slot]==0){
		TBox *box_inactive = new TBox(i_slot,0,i_slot+1,num_channels);
		vector_box_inactive.push_back(box_inactive);
		box_inactive->SetFillColor(1);
		box_inactive->SetFillStyle(3004);
		inactive_crate2++;
	}
}
for (int i_ch = 0; i_ch < inactive_ch_crate2.size(); i_ch++){
	TBox *box_inactive = new TBox(inactive_slot_crate2.at(i_ch)-1,inactive_ch_crate2.at(i_ch),inactive_slot_crate2.at(i_ch),inactive_ch_crate2.at(i_ch)+1);
	vector_box_inactive.push_back(box_inactive);
	box_inactive->SetFillStyle(3004);
	box_inactive->SetFillColor(1);
	inactive_crate2++;
}
for (int i_ch=0; i_ch < inactive_crate1; i_ch++){
	vector_box_inactive.at(i_ch)->Draw("same");
}
p1->Update();
TPad *p2 = (TPad*) canvas_elec->cd(2);
p2->SetGrid();
freq_crate2->Draw("colz");
for (int i_ch = 0; i_ch < inactive_crate2; i_ch++){
	vector_box_inactive.at(inactive_crate1+i_ch)->Draw("same");
}
p2->Update();
canvas_elec->Write();

std::stringstream ss_elec;
ss_elec << "frequency_rootfiles/" << label << "_Elec.pdf";
canvas_elec->SaveAs(ss_elec.str().c_str());

canvas_elec_cluster->cd();
canvas_elec_cluster->Divide(2,1);
TPad *p1_cluster = (TPad*) canvas_elec_cluster->cd(1);
p1_cluster->SetGrid();
freq_crate1_cluster->Draw("colz");
inactive_crate1 = 0;
inactive_crate2 = 0;
for (int i_slot=0;i_slot<num_slots;i_slot++){
	if (active_channel[0][i_slot]==0){
		TBox *box_inactive = new TBox(i_slot,0,i_slot+1,num_channels);
		vector_box_inactive.push_back(box_inactive);
		box_inactive->SetFillStyle(3004);
		box_inactive->SetFillColor(1);
		inactive_crate1++;
	}
}
for (int i_ch = 0; i_ch < inactive_ch_crate1.size(); i_ch++){
	TBox *box_inactive = new TBox(inactive_slot_crate1.at(i_ch)-1,inactive_ch_crate1.at(i_ch),inactive_slot_crate1.at(i_ch),inactive_ch_crate1.at(i_ch)+1);
	vector_box_inactive.push_back(box_inactive);
	box_inactive->SetFillStyle(3004);
	box_inactive->SetFillColor(1);
	inactive_crate1++;
}

for (int i_slot=0;i_slot<num_slots;i_slot++){
	if (active_channel[1][i_slot]==0){
		TBox *box_inactive = new TBox(i_slot,0,i_slot+1,num_channels);
		vector_box_inactive.push_back(box_inactive);
		box_inactive->SetFillColor(1);
		box_inactive->SetFillStyle(3004);
		inactive_crate2++;
	}
}
for (int i_ch = 0; i_ch < inactive_ch_crate2.size(); i_ch++){
	TBox *box_inactive = new TBox(inactive_slot_crate2.at(i_ch)-1,inactive_ch_crate2.at(i_ch),inactive_slot_crate2.at(i_ch),inactive_ch_crate2.at(i_ch)+1);
	vector_box_inactive.push_back(box_inactive);
	box_inactive->SetFillStyle(3004);
	box_inactive->SetFillColor(1);
	inactive_crate2++;
}
for (int i_ch=0; i_ch < inactive_crate1; i_ch++){
	vector_box_inactive.at(i_ch)->Draw("same");
}
p1_cluster->Update();
TPad *p2_cluster = (TPad*) canvas_elec_cluster->cd(2);
p2_cluster->SetGrid();
freq_crate2_cluster->Draw("colz");
for (int i_ch = 0; i_ch < inactive_crate2; i_ch++){
	vector_box_inactive.at(inactive_crate1+i_ch)->Draw("same");
}
p2_cluster->Update();
canvas_elec_cluster->Write();
std::stringstream ss_elec_cluster;
ss_elec_cluster << "frequency_rootfiles/" << label << "_ElecCluster.pdf";
canvas_elec_cluster->SaveAs(ss_elec_cluster.str().c_str());

canvas_elec_multi->cd();
canvas_elec_multi->Divide(2,1);
TPad *p1_multi = (TPad*) canvas_elec_multi->cd(1);
p1_multi->SetGrid();
freq_crate1_multi->Draw("colz");
inactive_crate1 = 0;
inactive_crate2 = 0;
for (int i_slot=0;i_slot<num_slots;i_slot++){
	if (active_channel[0][i_slot]==0){
		TBox *box_inactive = new TBox(i_slot,0,i_slot+1,num_channels);
		vector_box_inactive.push_back(box_inactive);
		box_inactive->SetFillStyle(3004);
		box_inactive->SetFillColor(1);
		inactive_crate1++;
	}
}
for (int i_ch = 0; i_ch < inactive_ch_crate1.size(); i_ch++){
	TBox *box_inactive = new TBox(inactive_slot_crate1.at(i_ch)-1,inactive_ch_crate1.at(i_ch),inactive_slot_crate1.at(i_ch),inactive_ch_crate1.at(i_ch)+1);
	vector_box_inactive.push_back(box_inactive);
	box_inactive->SetFillStyle(3004);
	box_inactive->SetFillColor(1);
	inactive_crate1++;
}

for (int i_slot=0;i_slot<num_slots;i_slot++){
	if (active_channel[1][i_slot]==0){
		TBox *box_inactive = new TBox(i_slot,0,i_slot+1,num_channels);
		vector_box_inactive.push_back(box_inactive);
		box_inactive->SetFillColor(1);
		box_inactive->SetFillStyle(3004);
		inactive_crate2++;
	}
}
for (int i_ch = 0; i_ch < inactive_ch_crate2.size(); i_ch++){
	TBox *box_inactive = new TBox(inactive_slot_crate2.at(i_ch)-1,inactive_ch_crate2.at(i_ch),inactive_slot_crate2.at(i_ch),inactive_ch_crate2.at(i_ch)+1);
	vector_box_inactive.push_back(box_inactive);
	box_inactive->SetFillStyle(3004);
	box_inactive->SetFillColor(1);
	inactive_crate2++;
}
for (int i_ch=0; i_ch < inactive_crate1; i_ch++){
	vector_box_inactive.at(i_ch)->Draw("same");
}
p1_multi->Update();
TPad *p2_multi = (TPad*) canvas_elec_multi->cd(2);
p2_multi->SetGrid();
freq_crate2_multi->Draw("colz");
for (int i_ch = 0; i_ch < inactive_crate2; i_ch++){
	vector_box_inactive.at(inactive_crate1+i_ch)->Draw("same");
}
p2_multi->Update();
canvas_elec_multi->Write();

std::stringstream ss_elec_multi;
ss_elec_multi << "frequency_rootfiles/" << label << "_Elec_Multi.pdf";
canvas_elec_multi->SaveAs(ss_elec_multi.str().c_str());

out_file->Close();
delete out_file;





}
