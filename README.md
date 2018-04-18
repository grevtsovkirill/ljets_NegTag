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
Ouput will be available