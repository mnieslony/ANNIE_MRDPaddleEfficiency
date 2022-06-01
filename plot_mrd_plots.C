void read_in_db(std::string filename, std::map<std::string,std::map<std::string,std::string>> &my_map){

  fstream fin;
  
  // Open an existing file
  fin.open(filename.c_str(), ios::in);
  
  // Read the Data from the file
  // as String Vector
  vector<string> row;
  string line, word, temp;

  bool first_row = true;
  vector<string> labels;

  while (!fin.eof()) {
  
    row.clear();
  
    // read an entire row and
    // store it in a string variable 'line'
    getline(fin, line);
    if (fin.eof() || (line=="")) break;

  
    // used for breaking words
    stringstream s(line);
    //std::cout <<"line: "<<line<<std::endl;
  
    // read every column data of a row and
    // store it in a string variable, 'word'
    while (getline(s, word, ',')) {
  
      // add all the column data
      // of a row to a vector
      row.push_back(word);

    }
  
    if (first_row) {
      for (int i_row = 0; i_row < (row.size()); i_row++){
        labels.push_back(row.at(i_row));
      }
      first_row = false;
    } else {
      std::string key = row.at(0);
      std::map<std::string,std::string> single_map;
      for (int i_row=1; i_row < (row.size()); i_row++){
        single_map.emplace(labels.at(i_row),row.at(i_row));
      }
      my_map.emplace(key,single_map);
    }
  }

}

void read_in_geom(std::string filename, std::map<int,std::vector<int>> &my_map_chkey_tdc, std::map<std::vector<int>,int> &my_map_tdc_chkey){

  fstream fin;
  
  // Open an existing file
  fin.open(filename.c_str(), ios::in);
  
  // Read the Data from the file
  // as String Vector
  vector<string> row;
  string line, word, temp;

  bool first_row = true;
  vector<string> labels;

  while (!fin.eof()) {
  
    row.clear();
  
    // read an entire row and
    // store it in a string variable 'line'
    getline(fin, line);
    //if (fin.eof() || (line=="")) break;

  
    // used for breaking words
    stringstream s(line);
    //std::cout <<"line: "<<line<<std::endl;
  
    // read every column data of a row and
    // store it in a string variable, 'word'
    while (getline(s, word, ',')) {
  
      // add all the column data
      // of a row to a vector
      row.push_back(word);

    }
  
    if (first_row) {
      for (int i_row = 0; i_row < (row.size()); i_row++){
        labels.push_back(row.at(i_row));
      }
      first_row = false;
    } else {
      int chkey;
      int tdc_crate, tdc_slot, tdc_ch;
      for (int i_row=0; i_row < (row.size()); i_row++){
        if (labels.at(i_row)=="detector_num") chkey = stoi(row.at(i_row));
        else if (labels.at(i_row)=="rack") tdc_crate = stoi(row.at(i_row));
        else if (labels.at(i_row)=="TDC_slot") tdc_slot = stoi(row.at(i_row));
        else if (labels.at(i_row)=="TDC_channel") tdc_ch = stoi(row.at(i_row));
      }
      std::vector<int> tdc={tdc_crate,tdc_slot,tdc_ch};
      my_map_chkey_tdc.emplace(chkey,tdc);
      my_map_tdc_chkey.emplace(tdc,chkey);
    }
  }

}

void plot_mrd_plots(const char *configuration_file = "PlotMRDConfig"){

  std::map<std::string,std::map<std::string,std::string>> run_db;
  read_in_db("db/MRD_Efficiency_DB.csv",run_db);

  std::map<int,std::vector<int>> geom_chkey_tdc;
  std::map<std::vector<int>,int> geom_tdc_chkey;
  read_in_geom("db/FullMRDGeometry_09_29_20.csv",geom_chkey_tdc,geom_tdc_chkey);

  std::stringstream ss_filename;
  ss_filename << "config/" <<configuration_file;

  std::vector<std::string> filenames, names, labels;

  ifstream input_file(ss_filename.str().c_str());
  std::string temp_string;
  while (!input_file.eof()){
    getline(input_file,temp_string);
    if (input_file.eof() || (temp_string=="")) break;
    if (run_db.count(temp_string)>0){
      filenames.push_back(run_db[temp_string]["Filename"]);
      names.push_back(run_db[temp_string]["Legend"]);
      labels.push_back(temp_string);
    } else {
      std::cout <<"Specified label "<<temp_string<<" not found in MRD Efficiency run DB!"<<std::endl;
    }
    if (input_file.eof()) break;
  }
  input_file.close();

  TLegend *leg =new TLegend(0.15,0.2,0.45,0.45);
  TCanvas *c1 = new TCanvas("c","Canvas",900,600);

  for (int i_file= 0; i_file < filenames.size(); i_file++){

    c1->cd();
    c1->Clear();
    std::cout <<"i_file: "<<i_file<<std::endl;
    std::stringstream ss_filename;
    ss_filename << "efficiency_rootfiles/combined/"<<filenames.at(i_file);
    TFile *f = new TFile(ss_filename.str().c_str(),"READ");
    TEfficiency *eff = (TEfficiency*) f->Get("eff_chankey_corrected");
    eff->Draw();
    std::stringstream ss_title;
    ss_title << "MRD Efficiency Channelkeys "<<labels.at(i_file);
    eff->SetTitle(ss_title.str().c_str());
    eff->Paint("");
    std::string current_label = labels.at(i_file);
    std::stringstream plotname;
    plotname << "efficiency_plots/MRD_Efficiency_Chankeys/MRD_Efficiency_Chankeys_"<<current_label<<".pdf";
    c1->SaveAs(plotname.str().c_str());

    c1->Clear();
    TH2Poly *top_side = (TH2Poly*) f->Get("eff_top_side");
    for (int i_bin=0; i_bin < top_side->GetNumberOfBins()+5; i_bin++){
      double content = top_side->GetBinContent(i_bin+1);
      if (content < 0.1) top_side->SetBinContent(i_bin+1,-0.1);
    }
    top_side->Draw("colz");
    top_side->GetZaxis()->SetRange(0,1.);
    ss_title.str("");
    ss_title << "MRD Efficiency - Spatial view "<<labels.at(i_file);
    top_side->SetTitle(ss_title.str().c_str());
    plotname.str("");
    plotname << "efficiency_plots/MRD_Efficiency_Spatial/MRD_Efficiency_Spatial_"<<current_label<<".pdf";
    c1->SaveAs(plotname.str().c_str());
    c1->Clear();

    TCanvas *canvas_elec = (TCanvas*)f->Get("canvas_elec");
    canvas_elec->Draw();
    plotname.str("");
    plotname << "efficiency_plots/MRD_Efficiency_Electronics/MRD_Efficiency_Electronics_"<<current_label<<".pdf";
    canvas_elec->SaveAs(plotname.str().c_str()); 

    c1->cd();
    TH1F *hist_eff_corr = new TH1F("hist_eff_corr","MRD efficiencies",100,0,1);
    for (int i_bin=0; i_bin < eff->GetTotalHistogram()->GetXaxis()->GetNbins(); i_bin++){
      if (eff->GetEfficiency(i_bin+1)>0.01) hist_eff_corr->Fill(eff->GetEfficiency(i_bin+1));
    }
    hist_eff_corr->SetStats(0);
    hist_eff_corr->SetLineWidth(2);
    double mean_corr = hist_eff_corr->GetMean();
    double rms_corr = hist_eff_corr->GetStdDev();
    std::cout <<"Average efficiency (corrected): "<<mean_corr<<std::endl;
    std::cout <<"Average efficiency (RMS, corrected): "<<rms_corr<<std::endl;
    hist_eff_corr->Draw();
    ss_title.str("");
    ss_title << "MRD Efficiency histogram - "<<labels.at(i_file);
    hist_eff_corr->SetTitle(ss_title.str().c_str());

    plotname.str("");
    plotname << "efficiency_plots/MRD_Efficiency_Histogram/MRD_Efficiency_Histogram_"<<current_label<<".pdf";
    c1->SaveAs(plotname.str().c_str());
    c1->Clear();

    f->Close();
    delete f;

  }

}
