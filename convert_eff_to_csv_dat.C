void convert_eff_to_csv_dat(const char* filename){

  std::stringstream eff_file;
  eff_file << "efficiency_rootfiles/combined/"<<filename<<".root";
  std::cout <<"eff_file: "<<eff_file.str()<<std::endl;
  TFile *f = new TFile(eff_file.str().c_str(),"READ");
  std::cout <<"Get Hist"<<std::endl;
  TEfficiency *h_eff = (TEfficiency*) f->Get("eff_chankey_corrected");
   
  std::cout <<"Define csv output file"<<std::endl;
  std::stringstream out_file_csv;
  out_file_csv << "efficiency_csv/" << filename <<".csv";
  ofstream outfile(out_file_csv.str().c_str());
  outfile << "chankey,efficiency,error" << std::endl;

  std::stringstream out_file_dat;
  out_file_dat << "efficiency_csv/"<<filename <<".dat";
  ofstream outfiledat(out_file_dat.str().c_str());
  outfiledat << "chankey,efficiency,errorlow,errorup" << std::endl;

  std::cout <<"Loop through hist"<<std::endl;
  for (int i=0; i< h_eff->GetCopyTotalHisto()->GetNbinsX(); i++){

    std::cout <<"i: "<<i<<", chankey: "<<h_eff->GetCopyTotalHisto()->GetXaxis()->GetBinCenter(i+1)-0.5<<", efficiency: "<<h_eff->GetEfficiency(i+1)<<", error low (eff): "<<h_eff->GetEfficiencyErrorLow(i+1)<<", error high (eff): "<<h_eff->GetEfficiencyErrorUp(i+1)<<std::endl;
    outfile << h_eff->GetCopyTotalHisto()->GetXaxis()->GetBinCenter(i+1)-0.5 << "," << h_eff->GetEfficiency(i+1) << "," << h_eff->GetEfficiencyErrorLow(i+1) << "," << h_eff->GetEfficiencyErrorUp(i+1) << std::endl;
    outfiledat << h_eff->GetCopyPassedHisto()->GetXaxis()->GetBinCenter(i+1)-0.5 << "    " << h_eff->GetEfficiency(i+1) << "    " << h_eff->GetEfficiencyErrorLow(i+1) << "    " << h_eff->GetEfficiencyErrorUp(i+1) << std::endl;

  }
  outfile.close();
  outfiledat.close();

}
