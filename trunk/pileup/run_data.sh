#!/bin/sh

export LD_LIBRARY_PATH=PileupReweighting/StandAlone:$LD_LIBRARY_PATH

./d_ana -d ../run/data/user.*/*.root*
mv prw_output.root prwroo/prw_data.root
ls -l prwroo/prw_data.root
