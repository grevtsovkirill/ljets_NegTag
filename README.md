## Istructions for NegTag with calo jets


##### NtupleDumper
Grab variables from CxAOD inputs.
Rely on ```../config/conf.hpp``` and ```../config/subTagger.hpp```
Need to specify ```.slices``` for mc and ```.periods``` for data, to be read from files specified in ```run.sh```
To run:
```
make
./run.sh
```
Specify option -c in run.sh to provide different files for mc16a and mc16d (since same name of slices and same xs).

Output will be available at: ```/NtupleDumper/res```

```./run.sh send2ge_NtupleDumper``` will create pbs jobs used for ```htc.sh``` to submit Condor jobs, like ```./htc.sh d```

Use ```./run.sh send2ge++_NtupleDumper``` if files are too big, this will split per systematics

##### GetPtEtaWeights
Create histograms of data and MC to compute corresponding reweighting files:
```./run.sh``` - specify data/mc + compaignie (a/d, 1516/17)
to submit to condor use ```./run.sh send2ge_Reweighting_noBootstrap```
Once reweighting files ready, check distributions, for that:
```
rt -b 'calculate_jetptetareweighting.C("data1516.root","mc_a.root",1)'
```
- don't forget last option to include non-tree systematics!

Output - ```rew.root``` - (tempopary) - rename by hand to ```rew_+XXX+.root``` where XXX is ```a``` or ```d``` (necessary to follow this convention, as thif files will be red in next step).
And ```rew_XXX_HERWIG.root``` for Herwig.

Also, run ```data_3mc_comparison_mc_w.cpp``` to perform comparison data to mc before any reweihting:

```
rt -b 'data_3mc_comparison_mc_w.cpp("a1516","data1516.root","mc_a_full.root","mc_HERWIG_a.root")' 
```

##### GetNTrackWeights
Create reweighting files for NTracks, taking into account pTeta reweighting from file of previous step.
Use ```run.sh``` to creat pbs files and submit to batch system. First run with ```send2ge_Reweighting_noBootstrap``` option to perform check before submitting whole BS.
Check that pTeta reweighting actually goes in right direction (can be done for nominal only):
``` rt -b 'data_mc_comparison_pteta_rew.cpp("a","data_A.root","mc_a_full.root","mc_HERWIG_a.root")' ```
Then submit rest variations and bootsrap replicas.
After, create reweighting file for Pythia:
```
rt -b 'calculate_jetntrackreweighting.C("data_A.root","mc_a_full.root",1)' 
and for Herwig:
```
rt -b 'calculate_jetntrackreweighting_HERWIG.C("data_A.root","mc_HERWIG_a.root")'
```

Rename reweighting file according to conventions to be read correctly in next step (NtupleReader):
```reweight_ntrack2D_XXX.root``` and ```reweight_ntrack2D_XXX_HERWIG.root```


##### NtupleReader
Look into dumped ntuples, take reweighting results to build histograms for each tagger - nominal and flipped.

```
./NtupleReaderApp -s FlavourTagging_Nominal -c a -m xAOD -f ../NtupleDumper/res/mc16a_smth.root
```
Provided options:
 - ```-s``` systematics variation, nominal, tree variations and scale factors (see ```config/systematics.txt```)
 - ```-d``` required option to run on data
 - ```-c``` compaigne, choose ```a``` for mc16a and data15-16, ```d``` for mc16d and data17
 - ```-m``` stands for mode, and can be xAOD/doJets/doSubLeadJet/nomode/bootstrap - consult ```config/systematics.txt``` for more details (requre modification according available systematic variations)
 - ```-f``` path to input file
 - ```-p``` submission can be split by period/slice
 - ```-split i``` run bootstrap replicas
Code will check ranges of historgrams and types of taggers into ```../config/subTagger.hpp``` and check that input file exists is in ```.slices/.periods```.

##### TemplateFit
Exctract HF fractions

```
rt -b -q 'template_fit.cxx("a",0,X)'
```
where X 1/0 - leading/subleading jet.
Requires FlavourTagging_Nominal,subleadingjet and generator outputs from NtupleReader.
Second option in command - correction - require inputs from Adjusted MC group (not yet available in rel21)

##### CalculateVariables
Last step towards SF is to run for each nominal and systematic variation ```CalculateVariables```.
It takes ```../NtupleReader/res/XXX``` as input, using convention "data_"/"mc_"+"compaigne" (a/d)
First, one need to run ```notrackrew``` systematic (before nominal), because it reads unweighted eps_l MC in order to compute SF.
And then:
```
./CalculateVariables -s FlavourTagging_Nominal -c a
```

Also, this can be done in loop by calling ```./run.sh```  - will loop over all variations listed in ```config/systematics.txt```.


After all necessary variations are in ```raw_systematics```:
 - change Make file to ```rel_syst```
 - recompile 

Run over all variation corresponding uncertainty computation - IMPORTANT - be sure all inputs exists for systematics listed in config:
```
./run_syst.sh 
```
to loop over all "v", or use ```./run_syst.sh XXX``` to run on single option, where ```XXX``` can be:
 - ```sf``` = SF -> mistag rate (data / MC ratio) 
 - ```khf``` = MC HF corrections
 - ```kll``` = MC LF corrections 
 - ```eps_d/eps_neg_d``` = Data nominal/negative tag efficiency

Type (```-t```: rel(onesided)/updown) is readed directly from config.

Outputs will be available at ```rel_systematics/``` .

Last step - run total uncertainty computation for all variations:
```
./run_total.sh XXX
```
Will create final plots in ```tot_systematics/total_XXX_a.root ```, where ```XXX``` one of options sf/kll/khf/eps_d/epd_neg_d. 


