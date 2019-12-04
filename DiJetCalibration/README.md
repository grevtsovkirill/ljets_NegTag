## Introduction
The ZJetCalibration package runs a basic Di-jet selection in AnalysisTop and saves various additional variables to the output ntuple required for the NegTag light flavour jet calibration

### Setup environment
```
setupATLAS
lsetup git
```

### Make work area
```
mkdir WorkArea
cd WorkArea
```

### Checkout only the relevant directory
```
git init
git remote add -f origin https://:@gitlab.cern.ch:8443/atlas-ftag-calibration/ljets_NegTag.git
git config core.sparseCheckout true
echo DiJetCalibration >> .git/info/sparse-checkout
git pull origin calojets_r21_withAT
```

### Setup the release
```
asetup AnalysisTop,21.2.51,here 
```
always look for updates!!!

### Build everything
```
mkdir build run
cd build
cmake ../
make
source x86_64-slc6-gcc62-opt/setup.sh
```

### Run
```
cd ../run
top-xaod ../DiJetCalibration/scripts/Dijet.txt input_test.txt
```

### Submit to grid
Copy folder ```data/Grid``` to your run folder, change datalists in ```filesR21.py```.
There is README with instruction how to setup environment for production role (if you have one).
Submit files with :
```
python SubmitToGrid.py
```