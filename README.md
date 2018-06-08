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

##### GetPtEtaWeights
Create histograms of data and MC to compute corresponding reweighting files:
```./run.sh``` - specify data/mc + compaignie (a/d, 1516/17)

Once reweighting files ready, check distributions, for that:
```
rt 'calculate_jetptetareweighting.C("data1516_FlavourTagging_Nominal_0.root","mc_a_FlavourTagging_Nominal_0.root")'
```



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
 - ```-f`` path to input file
Code will check ranges of historgrams and types of taggers into ```../config/subTagger.hpp``` and check that input file exists is in ```.slices/.periods```.
