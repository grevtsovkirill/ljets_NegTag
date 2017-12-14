#!/bin/sh

export LD_LIBRARY_PATH=PileupReweighting/StandAlone:$LD_LIBRARY_PATH

./d_ana ../run/mc/user.*/*.root*
mv prw_output.root prwroo/prw_mc.root
ls -l prwroo/prw_mc.root
