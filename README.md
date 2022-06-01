# ANNIE_MRDPaddleEfficiency

Simple ROOT scripts to create plots of the Muon Range Detector efficiencies in ANNIE based on outputs produced with the `MRDPaddleEfficiencyPreparer` and `MRDPaddleEfficiencyCalc` tools in the `ToolAnalysis` repository https://github.com/ANNIEsoft/ToolAnalysis.

The different scripts do the following:
* `compute_correction_factors.C`: Compute efficiency correction factors based on Monte Carlo simulations as inputs for the efficiency calculations performed on data.
* `convert_eff_to_csv_dat.C`: Convert efficiency information stored in histograms in the input root files into `csv` and `dat` formats.
* `plot_frequency.C`: Plots paddle hits frequencies based on `TimeClustering*root` files
* `plot_mrd_comparison.C`: Plots comparisons of MRD paddle efficiencies for different run periods. The available run periods are defined in `db/MRD_Efficiency_DB.csv`.
* `plot_mrd_plots.C`: Saves pdf version of the MRD paddle efficiency plots from the ROOT input files.
* `time_evolution_eff.C`: Plots time evolution plots of efficiencies as a function of the date. Categorisation of channels into stable, metastable, and unstable.
