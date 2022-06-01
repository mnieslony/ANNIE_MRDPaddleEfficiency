void compute_correction(const char* infile = "MC/MC_0_999_mrdefficiency.root", bool is_data=true, const char* outfile="MRDCorrectionFactors.dat"){

  TFile *f = new TFile(infile,"READ");

  TH1D *h_eff_hist;
  TEfficiency *h_eff;

  if (is_data) h_eff = (TEfficiency*) f->Get("eff_chankey");
  else h_eff_hist = (TH1D*) f->Get("eff_chankey");
  
  ofstream outfile(outfile);

  if (is_data){
    for (int i=0; i< h_eff->GetCopyPassedHisto()->GetNbinsX(); i++){
      std::cout <<"i: "<<i<<", chankey: "<<h_eff->GetCopyPassedHisto()->GetXaxis()->GetBinCenter(i+1)-0.5<<", efficiency: "<<h_eff->GetEfficiency(i+1)<<", error(eff): "<<h_eff->GetEfficiencyErrorUp(i+1) - h_eff->GetEfficiency(i+1)<<std::endl;
      outfile << h_eff->GetCopyPassedHisto()->GetXaxis()->GetBinCenter(i+1)-0.5 << "    " << h_eff->GetEfficiency(i+1) << "    " << h_eff->GetEfficiencyErrorUp(i+1)-h_eff->GetEfficiency(i+1) << std::endl;
    }
  } else {
    for (int i=0; i< h_eff_hist->GetNbinsX(); i++){
      std::cout <<"i: "<<i<<", chankey: "<<h_eff_hist->GetXaxis()->GetBinCenter(i+1)-0.5<<", efficiency: "<<h_eff_hist->GetBinContent(i+1)<<", factor: "<<0.95/h_eff_hist->GetBinContent(i+1)<<std::endl;
      outfile << h_eff_hist->GetXaxis()->GetBinCenter(i+1)-0.5 << "    " << h_eff->GetBinContent(i+1) << "    " << 0.95/h_eff_hist->GetBinContent(i+1) << std::endl;
    }
  }

}
