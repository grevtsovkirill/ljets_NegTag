#!/bin/bash

export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh

lsetup root

root -l -b -q 'template_fit.cxx(0,1,'$1')'
root -l -b -q 'template_fit.cxx(1,1,'$1')'
root -l -b -q 'template_fit.cxx(0,0,'$1')'
root -l -b -q 'template_fit.cxx(1,0,'$1')'
