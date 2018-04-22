## Istructions for NegTag with calo jets


##### NtupleDumper
Grab variables from CxAOD inputs.
Rely on ```../config/conf.hpp``` and ```../config/subTagger.hpp```
Need to specify ```.slices``` for mc and ```.periods``` for data, to be read from files specified in ```run.ch```
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
rt 'calculate_jetptetareweighting.C("data1516_FlavourTagging_Nominal_0.root","mc_a_FlavourTagging_Nominal_0.root")'