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
Will check ranges of historgrams and types of taggers into ```../config/subTagger.hpp``` and check that input file is in ```.slices/.periods```
