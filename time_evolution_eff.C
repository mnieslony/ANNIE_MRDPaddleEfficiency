#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

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
      for (int i_label=0; i_label < labels.size(); i_label++) std::cout <<labels.at(i_label)<<std::endl;
      for (int i_row=0; i_row < (row.size()); i_row++){
        std::cout <<"labels: "<<labels.at(i_row)<<std::endl;
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

uint64_t timestamp_from_date(std::string str_date){

  uint64_t timestamp;
  std::string epoch_start = "1970/1/1";
  boost::posix_time::ptime Epoch(boost::gregorian::from_string(epoch_start));
  boost::posix_time::ptime ptime_string(boost::posix_time::time_from_string(str_date));
  boost::posix_time::time_duration duration;
  duration = boost::posix_time::time_duration(ptime_string - Epoch);
  timestamp = (duration.total_milliseconds())/1000;   //Convert milliseconds to seconds

  return timestamp;
}

void time_evolution_eff(const char *configuration_file = "TimeEvolutionConfig"){

  //load run information
  std::map<std::string,std::map<std::string,std::string>> run_db;
  read_in_db("db/MRD_Efficiency_DB.csv",run_db);

  //load geometry information
  std::map<int,std::vector<int>> geom_chkey_tdc;
  std::map<std::vector<int>,int> geom_tdc_chkey;
  //read_in_geom("db/FullMRDGeometry_09_29_20.csv",geom_chkey_tdc,geom_tdc_chkey);

  std::cout <<"2"<<std::endl;
  //Load configfile information
  std::stringstream ss_filename_config;
  ss_filename_config << "config/" <<configuration_file;

  std::vector<std::string> filenames, names, labels, start_dates, end_dates;
  std::vector<uint64_t> starttime, endtime, meantime;

  ifstream input_file(ss_filename_config.str().c_str());
  std::string temp_string;
  while (!input_file.eof()){
    getline(input_file,temp_string);
    if (input_file.eof() || (temp_string=="")) break;
    if (run_db.count(temp_string)>0){
      filenames.push_back(run_db[temp_string]["Filename"]);
      names.push_back(run_db[temp_string]["Legend"]);
      labels.push_back(temp_string);
      start_dates.push_back(run_db[temp_string]["TimeStart"]);
      end_dates.push_back(run_db[temp_string]["TimeEnd"]);
      starttime.push_back(timestamp_from_date(run_db[temp_string]["TimeStart"]));
      endtime.push_back(timestamp_from_date(run_db[temp_string]["TimeEnd"]));
    } else {
      std::cout <<"Specified label "<<temp_string<<" not found in MRD Efficiency run DB!"<<std::endl;
    }
   if (input_file.eof()) break;
  }
  input_file.close();

  for (int i_time=0; i_time < starttime.size(); i_time++){
    meantime.push_back(0.5*(starttime.at(i_time)+endtime.at(i_time)));
  }
  
  // ---------------------------------------
  // ----------OUTPUT FILE------------------
  // ---------------------------------------

  std::stringstream ss_output_filename;
  ss_output_filename << "efficiency_timeevolution/MRDEff_TimeEvolution_"<<labels.at(0)<<"_"<<labels.at(labels.size()-1)<<".root";
  TFile *output = new TFile(ss_output_filename.str().c_str(),"RECREATE");

  std::stringstream ss_output_csv;
  ss_output_csv << "efficiency_timeevolution/MRDEff_TimeEvolution_"<<labels.at(0)<<"_"<<labels.at(labels.size()-1)<<".csv";
  ofstream output_csv(ss_output_csv.str().c_str());
  output_csv << "RunPeriod,StartDate,EndDate,";
  for (int i_mrd=52;i_mrd<306; i_mrd++){
    output_csv << i_mrd<<",";
  }
  output_csv << std::endl;

  std::stringstream ss_output_pdf, ss_output_pdf_open, ss_output_pdf_close;
  ss_output_pdf << "efficiency_timeevolution/MRDEff_TimeEvolution_"<<labels.at(0)<<"_"<<labels.at(labels.size()-1)<<".pdf";
  ss_output_pdf_open << "efficiency_timeevolution/MRDEff_TimeEvolution_"<<labels.at(0)<<"_"<<labels.at(labels.size()-1)<<".pdf(";
  ss_output_pdf_close << "efficiency_timeevolution/MRDEff_TimeEvolution_"<<labels.at(0)<<"_"<<labels.at(labels.size()-1)<<".pdf)";

  std::stringstream ss_unstable_output_pdf, ss_unstable_output_pdf_open, ss_unstable_output_pdf_close;
  ss_unstable_output_pdf << "efficiency_timeevolution/MRDEff_TimeEvolution_Unstable_"<<labels.at(0)<<"_"<<labels.at(labels.size()-1)<<".pdf";
  ss_unstable_output_pdf_open << "efficiency_timeevolution/MRDEff_TimeEvolution_Unstable_"<<labels.at(0)<<"_"<<labels.at(labels.size()-1)<<".pdf(";
  ss_unstable_output_pdf_close << "efficiency_timeevolution/MRDEff_TimeEvolution_Unstable_"<<labels.at(0)<<"_"<<labels.at(labels.size()-1)<<".pdf)";

  std::stringstream ss_metastable_output_pdf, ss_metastable_output_pdf_open, ss_metastable_output_pdf_close;
  ss_metastable_output_pdf << "efficiency_timeevolution/MRDEff_TimeEvolution_Metastable_"<<labels.at(0)<<"_"<<labels.at(labels.size()-1)<<".pdf";
  ss_metastable_output_pdf_open << "efficiency_timeevolution/MRDEff_TimeEvolution_Metastable_"<<labels.at(0)<<"_"<<labels.at(labels.size()-1)<<".pdf(";
  ss_metastable_output_pdf_close << "efficiency_timeevolution/MRDEff_TimeEvolution_Metastable_"<<labels.at(0)<<"_"<<labels.at(labels.size()-1)<<".pdf)";

  std::stringstream ss_stable_output_pdf, ss_stable_output_pdf_open, ss_stable_output_pdf_close;
  ss_stable_output_pdf << "efficiency_timeevolution/MRDEff_TimeEvolution_Stable_"<<labels.at(0)<<"_"<<labels.at(labels.size()-1)<<".pdf";
  ss_stable_output_pdf_open << "efficiency_timeevolution/MRDEff_TimeEvolution_Stable_"<<labels.at(0)<<"_"<<labels.at(labels.size()-1)<<".pdf(";
  ss_stable_output_pdf_close << "efficiency_timeevolution/MRDEff_TimeEvolution_Stable_"<<labels.at(0)<<"_"<<labels.at(labels.size()-1)<<".pdf)";

  //-----------------------------------------
  // -----------OUTPUT DEFINITIONS-----------
  // ----------------------------------------

  TH2F *eff_diff = new TH2F("eff_diff","Efficiency differences - Channelkey",254,52,306,100,-1,1);
  eff_diff->GetXaxis()->SetTitle("Channelkey");
  eff_diff->GetYaxis()->SetTitle("Efficiency difference");

  TH1F *eff_diff_combined = new TH1F("eff_diff_combined","Efficiency differences (combined) - Channelkey",254,52,306);
  eff_diff_combined->GetXaxis()->SetTitle("Channelkey");
  eff_diff_combined->GetYaxis()->SetTitle("Efficiency diff combined");

  TH1F *eff_diff_1d = new TH1F("eff_diff_1d","Efficiency differences - Overall",100,-1,1);
  eff_diff_1d->GetXaxis()->SetTitle("Efficiency difference");
  eff_diff_1d->GetYaxis()->SetTitle("#");

  TH2F *eff_diff_run = new TH2F("eff_diff_run","Efficiency differences - Channelkey",filenames.size(),0,filenames.size(),254,52,306);
  //eff_diff_run->GetXaxis()->SetTitle("Run Period");
  eff_diff_run->GetYaxis()->SetTitle("Channelkey");

  TH2F *eff_run = new TH2F("eff_run","Efficiency - Channelkey",filenames.size(),0,filenames.size(),254,52,306);
  //eff_diff_run->GetXaxis()->SetTitle("Run Period");
  eff_diff_run->GetYaxis()->SetTitle("Channelkey");

  TH2F *eff_time = new TH2F("eff_time","Efficiency over time",filenames.size(),0,filenames.size(),200,0,1);
  //eff_time->GetXaxis()->SetTitle("Run cycle");
  eff_time->GetYaxis()->SetTitle("Efficiency #varepsilon");

  TH2F *eff_time_active = new TH2F("eff_time_active","Efficiency over time (active channels)",filenames.size(),0,filenames.size(),200,0,1);
  //eff_time_active->GetXaxis()->SetTitle("Run cycle");
  eff_time_active->GetYaxis()->SetTitle("Efficiency #varepsilon");

  TH2F *eff_time_10 = new TH2F("eff_time_10","Efficiency over time",filenames.size(),0,filenames.size(),10,0,1);
  //eff_time_10->GetXaxis()->SetTitle("Run cycle");
  eff_time_10->GetYaxis()->SetTitle("Efficiency #varepsilon");

  TH1F *inactive_ch = new TH1F("inactive_ch","Inactive channels",filenames.size(),0,filenames.size());
  //inactive_ch->GetXaxis()->SetTitle("Run cycle");
  inactive_ch->GetYaxis()->SetTitle("# inactive channels");

  for (int i_file=0; i_file < filenames.size(); i_file++){
    inactive_ch->GetXaxis()->SetBinLabel(i_file+1,labels.at(i_file).c_str());
    eff_time_10->GetXaxis()->SetBinLabel(i_file+1,labels.at(i_file).c_str());
    eff_time_active->GetXaxis()->SetBinLabel(i_file+1,labels.at(i_file).c_str());
    eff_time->GetXaxis()->SetBinLabel(i_file+1,labels.at(i_file).c_str());
    eff_diff_run->GetXaxis()->SetBinLabel(i_file+1,labels.at(i_file).c_str());
  }

  //CHANNELKEY range
  int mrd_start = 52;
  int mrd_stop = 305;

  //CHANNEL-wise time evolution graphs
  std::vector<TGraphAsymmErrors*> h_eff_timeevolution;
  std::vector<TGraphAsymmErrors*> h_eff_timeevolution_date;
  for (int i_hist = 0; i_hist<=mrd_stop-mrd_start; i_hist++){
    std::stringstream ss_hist, ss_hist_title;
    ss_hist <<"h_eff_time_chankey"<<mrd_start+i_hist;
    ss_hist_title<<"Efficiency Chankey "<<mrd_start+i_hist;
    TGraphAsymmErrors *gr = new TGraphAsymmErrors();
    gr->SetName(ss_hist.str().c_str());
    gr->SetTitle(ss_hist_title.str().c_str());
    h_eff_timeevolution.push_back(gr);

    std::stringstream ss_hist2, ss_hist2_title;
    ss_hist2 << "h_eff_time2_chankey"<<mrd_start+i_hist;
    ss_hist2_title << "Efficiency Chankey "<<mrd_start+i_hist;
    TGraphAsymmErrors *gr2 = new TGraphAsymmErrors();
    gr2->SetName(ss_hist2.str().c_str());
    gr2->SetTitle(ss_hist2_title.str().c_str());
    h_eff_timeevolution_date.push_back(gr2);
  }

  //AVERAGE time evolution graphs
  std::vector<double> avg_eff, avg_eff_error, avg_eff_active, avg_eff_active_error;
  std::vector<std::vector<double>> eff_file;
  for (int i_file=0; i_file < filenames.size(); i_file++){
    std::vector<double> single_eff;
    eff_file.push_back(single_eff);
  }

  //Different channel categories
  //Stable channels: ±5% variation
  //Meta-stable channels: ±10% variation
  //Unstable channels: >10% variation
  std::vector<int> stable_chkeys;
  std::vector<int> metastable_chkeys;
  std::vector<int> unstable_chkeys;
  std::map<int,std::vector<double>> stable_channels_chkey;
  std::map<int,std::vector<double>> metastable_channels_chkey;
  std::map<int,std::vector<double>> unstable_channels_chkey;

  for (unsigned int i_file = 0; i_file < filenames.size(); i_file++){
    
    std::cout <<"Looping through file # "<<i_file+1<<"/ "<<filenames.size()<<", filename: "<<filenames.at(i_file)<<std::endl;
    std::stringstream ss_filename;
    ss_filename << "efficiency_rootfiles/combined/"<<filenames.at(i_file);

    TFile *f = new TFile(ss_filename.str().c_str(),"READ");
    TEfficiency *h_eff = (TEfficiency*) f->Get("eff_chankey_corrected");

    output_csv << labels.at(i_file)<<", "<<starttime.at(i_file)<<", "<<endtime.at(i_file)<<", ";

    std::cout <<"stuff, ";

    double avg = 0;
    double avg_active = 0;
    std::vector<double> vec_eff;
    std::vector<double> vec_eff_active;
    int inactive = 0;
    int active_ch=0;

    for (int i=0; i< h_eff->GetTotalHistogram()->GetNbinsX(); i++){

      int chankey = h_eff->GetTotalHistogram()->GetXaxis()->GetBinCenter(i+1)-0.5;
      double eff = h_eff->GetEfficiency(i+1);
      double eff_error_low = h_eff->GetEfficiencyErrorLow(i+1);
      double eff_error_up = h_eff->GetEfficiencyErrorUp(i+1);

      //std::cout <<"i: "<<i<<", chankey: "<<chankey<<", efficiency: "<<eff<<", efficiency error (low): "<<eff_error_low<<", efficiency error (up): "<<eff_error_up<<std::endl;
      if(chankey >=52 && chankey <=305) {
        output_csv << eff <<",";
        h_eff_timeevolution.at(chankey-mrd_start)->SetPoint(i_file,i_file+0.5,eff);
        h_eff_timeevolution.at(chankey-mrd_start)->SetPointError(i_file,0,0,eff_error_low,eff_error_up);
        h_eff_timeevolution_date.at(chankey-mrd_start)->SetPoint(i_file,meantime.at(i_file),eff);
        h_eff_timeevolution_date.at(chankey-mrd_start)->SetPointError(i_file,meantime.at(i_file)-starttime.at(i_file),endtime.at(i_file)-meantime.at(i_file),eff_error_low,eff_error_up);
        avg += eff;
        vec_eff.push_back(eff);
        eff_time->Fill(i_file,eff);
        eff_time_10->Fill(i_file,eff);
        if (eff > 0.10) {
          eff_time_active->Fill(i_file,eff);
          avg_active += eff;
          active_ch++;
          vec_eff_active.push_back(eff);
        }
        else inactive_ch->Fill(i_file);
        eff_file.at(i_file).push_back(eff);
      }
    }
    output_csv << std::endl;
    avg /= (mrd_stop-mrd_start+1);
    avg_active /= active_ch;
    double std_avg = 0;
    double std_avg_active = 0;
    for (int i_eff = 0; i_eff < (vec_eff.size()); i_eff++){
      std_avg += pow((vec_eff.at(i_eff)-avg),2);
    }
    for (int i_eff = 0; i_eff < (vec_eff_active.size()); i_eff++){
      std_avg_active += pow((vec_eff_active.at(i_eff)-avg_active),2);
    }
    std_avg /= (mrd_stop-mrd_start);
    avg_eff.push_back(avg);
    //avg_eff_error.push_back(sqrt(std_avg)/(mrd_stop-mrd_start+1));
    avg_eff_error.push_back(sqrt(std_avg));
    std_avg_active /= active_ch;
    avg_eff_active.push_back(avg_active);
    avg_eff_active_error.push_back(std_avg_active);
    f->Close();
    delete f;
  }

  for (int i_f = 1; i_f < eff_file.size(); i_f++){
    for (int i_eff=0; i_eff < eff_file.at(i_f).size(); i_eff++){
      eff_diff->Fill(i_eff+mrd_start,eff_file.at(i_f).at(i_eff)-eff_file.at(i_f-1).at(i_eff));
      eff_diff_1d->Fill(eff_file.at(i_f).at(i_eff)-eff_file.at(i_f-1).at(i_eff));
      eff_diff_combined->SetBinContent(i_eff+1,eff_diff_combined->GetBinContent(i_eff+1)+fabs(eff_file.at(i_f).at(i_eff)-eff_file.at(i_f-1).at(i_eff)));
      eff_diff_run->SetBinContent(i_f+1,i_eff+1,eff_file.at(i_f).at(i_eff)-eff_file.at(i_f-1).at(i_eff));
      eff_run->SetBinContent(i_f+1,i_eff+1,eff_file.at(i_f).at(i_eff));
    }
  }

  for (int i_ch=mrd_start; i_ch < mrd_stop+1; i_ch++){
    bool is_stable=true;
    bool is_metastable=false;
    bool is_unstable=false;
    for (int i_x=0; i_x < eff_diff_run->GetXaxis()->GetNbins(); i_x++){
      double eff_difference = eff_diff_run->GetBinContent(i_x+1,i_ch-mrd_start+1);
      if (fabs(eff_difference) > 0.05){
        is_stable=false;
        is_metastable=true;
        if (fabs(eff_difference) > 0.10){
          is_unstable=true;
        }
      }
    }
    if (is_unstable){
      unstable_chkeys.push_back(i_ch);
    } else if (is_metastable){
      metastable_chkeys.push_back(i_ch);
    } else if (is_stable){
      stable_chkeys.push_back(i_ch);
    }
  }

  output->cd();
  TMultiGraph *multi = new TMultiGraph();
  TMultiGraph *multi_active = new TMultiGraph();
  TMultiGraph* multi2 = new TMultiGraph();
  TMultiGraph* multi2_active = new TMultiGraph();

  //std::cout <<"h_eff_timeevolution.size(): "<<h_eff_timeevolution.size()<<std::endl;
  TCanvas *c = new TCanvas("c","Canvas",900,600);
  for (unsigned int i_hist = 0; i_hist < h_eff_timeevolution.size(); i_hist++){

    h_eff_timeevolution.at(i_hist)->GetXaxis()->SetTitle("Date");
    h_eff_timeevolution.at(i_hist)->GetYaxis()->SetTitle("Efficiency #varepsilon");
    h_eff_timeevolution.at(i_hist)->GetYaxis()->SetRangeUser(0.,1.1);

    h_eff_timeevolution.at(i_hist)->SetLineColor(i_hist+800);
    h_eff_timeevolution.at(i_hist)->SetMarkerColor(i_hist+800);
    h_eff_timeevolution.at(i_hist)->SetFillColor(i_hist+800);
    h_eff_timeevolution.at(i_hist)->SetFillStyle(3001);
    h_eff_timeevolution.at(i_hist)->Write();
    multi->Add(h_eff_timeevolution.at(i_hist));
    h_eff_timeevolution_date.at(i_hist)->GetXaxis()->SetTitle("Date");
    h_eff_timeevolution_date.at(i_hist)->GetYaxis()->SetTitle("Efficiency #varepsilon");
    h_eff_timeevolution_date.at(i_hist)->GetYaxis()->SetRangeUser(0.,1.1);
    h_eff_timeevolution_date.at(i_hist)->SetLineColor(i_hist+800);
    h_eff_timeevolution_date.at(i_hist)->SetMarkerColor(i_hist+800);
    h_eff_timeevolution_date.at(i_hist)->SetFillColor(i_hist+800);
    h_eff_timeevolution_date.at(i_hist)->SetFillStyle(3001);
    h_eff_timeevolution_date.at(i_hist)->GetXaxis()->SetTimeDisplay(1);
    h_eff_timeevolution_date.at(i_hist)->GetXaxis()->SetTimeOffset(0);
    h_eff_timeevolution_date.at(i_hist)->Write();
    c->cd();
    c->Clear();
    h_eff_timeevolution_date.at(i_hist)->Draw("ALP");
    
    if (i_hist==0) c->Print(ss_output_pdf_open.str().c_str(),"pdf");
    else if (i_hist != (h_eff_timeevolution.size()-1)) c->Print(ss_output_pdf.str().c_str(),"pdf");
    else c->Print(ss_output_pdf_close.str().c_str(),"pdf");

    for (int i_unstable=0; i_unstable < unstable_chkeys.size(); i_unstable++){
      if (i_hist+52 == unstable_chkeys.at(i_unstable)){
        if (i_unstable==0) c->Print(ss_unstable_output_pdf_open.str().c_str(),"pdf");
        else if (i_unstable != unstable_chkeys.size()-1) c->Print(ss_unstable_output_pdf.str().c_str(),"pdf");
        else c->Print(ss_unstable_output_pdf_close.str().c_str(),"pdf");
      }
    }
    for (int i_metastable=0; i_metastable < metastable_chkeys.size(); i_metastable++){
      if (i_hist+52 == metastable_chkeys.at(i_metastable)){
        if (i_metastable==0) c->Print(ss_metastable_output_pdf_open.str().c_str(),"pdf");
        else if (i_metastable != metastable_chkeys.size()-1) c->Print(ss_metastable_output_pdf.str().c_str(),"pdf");
        else c->Print(ss_metastable_output_pdf_close.str().c_str(),"pdf");
      }
    }
    for (int i_stable=0; i_stable < stable_chkeys.size(); i_stable++){
      if (i_hist+52 == stable_chkeys.at(i_stable)){
        if (i_stable==0) c->Print(ss_stable_output_pdf_open.str().c_str(),"pdf");
        else if (i_stable != stable_chkeys.size()-1) c->Print(ss_stable_output_pdf.str().c_str(),"pdf");
        else c->Print(ss_stable_output_pdf_close.str().c_str(),"pdf");
      }
    }
    multi2->Add(h_eff_timeevolution_date.at(i_hist));
  }

  //multi->GetXaxis()->SetTitle("Run period");
  multi->GetYaxis()->SetTitle("Efficiency");
  multi2->GetYaxis()->SetTitle("Efficiency #varepsilon");
  multi2->GetXaxis()->SetTimeDisplay(1);
  multi2->GetXaxis()->SetTimeOffset(0);

  TGraphErrors *gr_avg = new TGraphErrors();
  TGraphErrors *gr_avg_active = new TGraphErrors();

  for (int i_avg=0; i_avg < (int) avg_eff.size(); i_avg++){
    gr_avg->SetPoint(i_avg,i_avg+0.5,avg_eff.at(i_avg));
    gr_avg->SetPointError(i_avg,0,avg_eff_error.at(i_avg));
    gr_avg_active->SetPoint(i_avg,i_avg+0.5,avg_eff_active.at(i_avg));
    gr_avg_active->SetPointError(i_avg,0,avg_eff_active_error.at(i_avg));
  }
  gr_avg->SetName("gr_avg");
  gr_avg->SetTitle("Average efficiency time evolution");
  gr_avg->GetXaxis()->SetTitle("Run numbers");
  gr_avg->GetYaxis()->SetTitle("Efficiency #varepsilon");
  gr_avg->GetYaxis()->SetRangeUser(0.,1.1);
  gr_avg->Write();

  gr_avg_active->SetName("gr_avg_active");
  gr_avg_active->SetTitle("Average efficiency time evolution (active channels)");
  gr_avg_active->GetXaxis()->SetTitle("Run numbers");
  gr_avg_active->GetYaxis()->SetTitle("Efficiency #varepsilon");
  gr_avg_active->GetYaxis()->SetRangeUser(0.,1.1);
  gr_avg_active->Write();

  eff_diff->Write();
  eff_diff_1d->Write();
  eff_diff_run->Write();
  eff_diff_combined->Write();
  eff_run->Write();
  eff_time->Write();
  eff_time_10->Write();
  eff_time_active->Write();
  multi->Write("multi");
  multi2->Write("multi2");
  inactive_ch->Write();
  output->Close();
  delete output;

  std::cout <<"Metastable channels: "<<std::endl;
  for (int i_meta =0; i_meta < metastable_chkeys.size(); i_meta++){
    std::cout << metastable_chkeys.at(i_meta)<<" , ";
  }
  std::cout << std::endl;
  std::cout <<"Unstable channels: "<<std::endl;
  for (int i_un =0; i_un < unstable_chkeys.size(); i_un++){
    std::cout << unstable_chkeys.at(i_un)<<" , ";
  }
  std::cout << std::endl;


}
