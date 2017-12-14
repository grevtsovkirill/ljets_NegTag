#!/usr/bin/python

import os, math, sys

isData = True 
is16   = True
is15   = True
path_list = ["/eos/atlas/atlascerngroupdisk/perf-flavtag/calib/negtag/tag21_data1516_mc16_v01-00/ntuples/"]



if isData                        : path = path_list[0] 
if isData==False and is16        : 
  path = path_list[0] + "mc16/Pythia/"
  path_H = path_list[0] 
if isData==False and is16==False : 
  path = path0 + "mc15c_bpu/"
  path_H = path0 + "mc15c_HERWIG_bpu/"

data_list_15 = [
"data15/user.kgrevtso.data15_13TeV.00266904.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/", 
"data15/user.kgrevtso.data15_13TeV.00266919.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00267073.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00267152.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00267162.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00267167.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00267638.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00267639.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00270441.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00270448.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00270588.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00270806.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00270816.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00270949.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00270953.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00271048.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00271298.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00271388.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00271421.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00271516.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00271595.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00271744.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00276073.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00276147.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00276161.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00276183.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00276189.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00276212.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00276245.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00276262.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00276329.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00276330.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00276336.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00276416.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00276511.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00276689.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00276731.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00276778.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00276790.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00276952.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00276954.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00278727.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00278748.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00278880.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00278912.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00278968.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00278970.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00279169.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00279259.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00279279.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00279284.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00279345.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00279515.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00279598.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00279685.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00279764.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00279813.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00279867.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00279928.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00279932.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00279984.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00280231.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00280273.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00280319.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00280368.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00280423.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00280464.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00280500.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00280520.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00280614.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00280673.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00280753.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00280853.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00280862.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00280950.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00280977.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00281070.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00281074.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00281075.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00281130.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00281143.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00281317.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00281381.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00281385.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00281411.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00282625.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00282631.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00282712.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00282784.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00282992.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00283074.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00283155.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00283270.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00283429.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00283608.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00283780.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00284006.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00284154.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00284213.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00284285.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00284420.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00284427.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00284473.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/",
"data15/user.kgrevtso.data15_13TeV.00284484.DAOD_FTAG1.FTNtupCalib.NegTag_data15_p3180_FTAG1_Ntuple.r9264_p3083_p3180_tuple.root/"
]

data_list_16 = [
## period A
"data16/user.kgrevtso.data16_13TeV.00297730.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00298595.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00298609.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00298633.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00298687.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00298690.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00298771.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00298773.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00298862.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00298967.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00299055.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00299144.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00299147.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00299184.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00299241.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00299243.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00299288.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00299343.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00299584.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/"
## end of period A
## period B
"data16/user.kgrevtso.data16_13TeV.00300345.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00300415.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00300418.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00300487.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00300540.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00300571.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00300600.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00300655.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00300687.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00300784.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00300800.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/"
## end of period B
## period C
## end of period C
## period D
## end of period D
## period E
## end of period E
## period F
"data16/user.kgrevtso.data16_13TeV.00304243.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00304308.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00304337.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00304409.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00304431.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00304494.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/"
## end of period F
## period G
"data16/user.kgrevtso.data16_13TeV.00305380.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/", 
"data16/user.kgrevtso.data16_13TeV.00305543.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00305571.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00305618.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00305671.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00305674.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00305723.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00305727.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00305735.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00305777.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00305811.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00305920.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00306269.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00306278.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00306310.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00306384.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00306419.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00306442.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00306448.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00306451.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/" 
## end of period G
## period I
"data16/user.kgrevtso.data16_13TeV.00307126.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00307195.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00307259.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00307306.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00307354.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00307358.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00307394.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00307454.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00307514.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00307539.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00307569.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00307601.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00307619.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00307656.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00307710.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00307716.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00307732.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00307861.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00307935.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00308047.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00308084.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00309375.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/"
## end of period I
## period K
"data16/user.kgrevtso.data16_13TeV.00309375.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00309390.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00309440.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00309516.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00309640.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00309674.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00309759.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/"
## end of period K
## period L
"data16/user.kgrevtso.data16_13TeV.00310015.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00310247.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00310249.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00310341.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00310370.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00310405.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00310468.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00310473.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00310634.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00310691.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00310738.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00310809.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00310863.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00310872.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00310969.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00311071.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00311170.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00311244.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00311287.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00311321.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00311365.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00311402.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00311473.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/",
"data16/user.kgrevtso.data16_13TeV.00311481.DAOD_FTAG1.FTNtupCalib.NegTag_data17_FTAG1_testNtuple_v01.r9264_p3083_p3261_tuple.root/"
## end of period L
]

data_list = data_list_15
if is16 and is15==False : data_list = data_list_16
if is16 and is15        : data_list = data_list_15 + data_list_16

mc_list_15 = [
# not produced
]

mc_list_16 = [
"group.perf-flavtag.mc16_13TeV.361020.DAOD_FTAG1.FTNtupCalib.NegTag_nosys.e3569_s3126_r9364_r9315_p3260_tuple.root/",
"group.perf-flavtag.mc16_13TeV.361021.DAOD_FTAG1.FTNtupCalib.NegTag_nosys.e3569_s3126_r9364_r9315_p3260_tuple.root/",
"group.perf-flavtag.mc16_13TeV.361022.DAOD_FTAG1.FTNtupCalib.NegTag_nosys.e3668_s3126_r9364_r9315_p3260_tuple.root/",
"group.perf-flavtag.mc16_13TeV.361023.DAOD_FTAG1.FTNtupCalib.NegTag_nosys.e3668_s3126_r9364_r9315_p3260_tuple.root/",
"group.perf-flavtag.mc16_13TeV.361024.DAOD_FTAG1.FTNtupCalib.NegTag_nosys.e3668_s3126_r9364_r9315_p3260_tuple.root/",
"group.perf-flavtag.mc16_13TeV.361025.DAOD_FTAG1.FTNtupCalib.NegTag_nosys.e3668_s3126_r9364_r9315_p3260_tuple.root/",
"group.perf-flavtag.mc16_13TeV.361026.DAOD_FTAG1.FTNtupCalib.NegTag_nosys.e3569_s3126_r9364_r9315_p3260_tuple.root/",
"group.perf-flavtag.mc16_13TeV.361027.DAOD_FTAG1.FTNtupCalib.NegTag_nosys.e3668_s3126_r9364_r9315_p3260_tuple.root/",
"group.perf-flavtag.mc16_13TeV.361028.DAOD_FTAG1.FTNtupCalib.NegTag_nosys.e3569_s3126_r9364_r9315_p3260_tuple.root/",
"group.perf-flavtag.mc16_13TeV.361029.DAOD_FTAG1.FTNtupCalib.NegTag_nosys.e3569_s3126_r9364_r9315_p3260_tuple.root/",
"group.perf-flavtag.mc16_13TeV.361030.DAOD_FTAG1.FTNtupCalib.NegTag_nosys.e3569_s3126_r9364_r9315_p3260_tuple.root/",
"group.perf-flavtag.mc16_13TeV.361031.DAOD_FTAG1.FTNtupCalib.NegTag_nosys.e3569_s3126_r9364_r9315_p3260_tuple.root/",
"group.perf-flavtag.mc16_13TeV.361032.DAOD_FTAG1.FTNtupCalib.NegTag_nosys.e3668_s3126_s3136_r9364_r9315_p3260_tuple.root/" 
]

mc_list = mc_list_15
if is16 : mc_list = mc_list_16

mcHERWIG_list_15 = [
# not produced
]

mcHERWIG_list_16 = [
"group.perf-flavtag.mc15_13TeV.426040.DAOD_FTAG1.FTNtupCalib.NegTag_v00-07-00.e4410_s2608_r7725_r7676_p3001_tuple.root/",
"group.perf-flavtag.mc15_13TeV.426041.DAOD_FTAG1.FTNtupCalib.NegTag_v00-07-00.e4410_s2608_r7725_r7676_p3001_tuple.root/",
"group.perf-flavtag.mc15_13TeV.426042.DAOD_FTAG1.FTNtupCalib.NegTag_v00-07-00.e4410_s2608_r7725_r7676_p3001_tuple.root/",
"group.perf-flavtag.mc15_13TeV.426043.DAOD_FTAG1.FTNtupCalib.NegTag_v00-07-00.e4410_s2608_r7725_r7676_p3001_tuple.root/",
"group.perf-flavtag.mc15_13TeV.426044.DAOD_FTAG1.FTNtupCalib.NegTag_v00-07-00.e4410_s2608_r7725_r7676_p3001_tuple.root/",
"group.perf-flavtag.mc15_13TeV.426045.DAOD_FTAG1.FTNtupCalib.NegTag_v00-07-00.e4410_s2608_r7725_r7676_p3001_tuple.root/",
"group.perf-flavtag.mc15_13TeV.426046.DAOD_FTAG1.FTNtupCalib.NegTag_v00-07-00.e4410_s2608_r7725_r7676_p3001_tuple.root/",
"group.perf-flavtag.mc15_13TeV.426047.DAOD_FTAG1.FTNtupCalib.NegTag_v00-07-00.e4410_s2608_r7725_r7676_p3001_tuple.root/",
"group.perf-flavtag.mc15_13TeV.426048.DAOD_FTAG1.FTNtupCalib.NegTag_v00-07-00.e4410_s2608_r7725_r7676_p3001_tuple.root/",
"group.perf-flavtag.mc15_13TeV.426049.DAOD_FTAG1.FTNtupCalib.NegTag_v00-07-00.e4410_s2608_r7725_r7676_p3001_tuple.root/",
"group.perf-flavtag.mc15_13TeV.426050.DAOD_FTAG1.FTNtupCalib.NegTag_v00-07-00.e4410_s2608_r7725_r7676_p3001_tuple.root/",
"group.perf-flavtag.mc15_13TeV.426051.DAOD_FTAG1.FTNtupCalib.NegTag_v00-07-00.e4410_s2608_r7772_r7676_p3001_tuple.root/",
"group.perf-flavtag.mc15_13TeV.426052.DAOD_FTAG1.FTNtupCalib.NegTag_v00-07-00.e4410_s2608_r7772_r7676_p3001_tuple.root/"
]

mcHERWIG_list = mcHERWIG_list_15
if is16 : mcHERWIG_list = mcHERWIG_list_16

# current GRL
# data16_13TeV.periodAllYear_DetStatus-v88-pro20-21_DQDefects-00-02-04_PHYS_StandardGRL_All_Good_25ns.xml
ref = [
#--------------------------
["A15",   252604, 267167], # 50ns, not in GRL nor data15 list
["A15",   267638, 267639], # 50ns, not in GRL nor data15 list
["B15",   267358, 267599], # 50ns, not in GRL nor data15 list
["C15",   270441, 272531], # 50ns, not in GRL -> can stop to use period C samples
#["I15",   281662, 282482], # not in GRL -> Runs with beta*=90
["D15",   276073, 276954], #  51.6461 pb-1 (GRL starts at 276262)
["E15",   278727, 279928], #  435.605 pb-1
["F15",   279932, 280422], #  301.693 pb-1
["G15",   280423, 281075], #  724.399 pb-1
["H15",   281130, 281411], #  264.362 pb-1
["J15",   282625, 284484], #  1435.25 pb-1
# total_15 = 3212.96 pb-1
#--------------------------
#["H16",   305359, 305359], # low mu run not use
["A16",   297730, 300287],  #  552.738 pb-1 -> run<297730 not in GRL
["B16",   300345, 300908],  # 1949.59  pb-1
["C16",   301912, 302393],  # 2872.72  pb-1
["D16",   302737, 303560],  # 4626.62  pb-1
["E16",   303638, 303892],  # 1481.01  pb-1
["F16",   303943, 304494],  # 3398.89  pb-1
["G16",   305291, 306714],  # 3827.95  pb-1
["I16",   307124, 308084],  # 5802.86  pb-1
["K16",   309311, 309759],  # 2198.23  pb-1
["L16",   310015, 311481],  # 6151.19  pb-1
# -> total_16 = 32861.6 pb-1 (Sum=32861.798)
#--------------------------
# MC samples - Pythia
["JZ0W",  361020, 361020],
["JZ1W",  361021, 361021],
["JZ2W",  361022, 361022],
["JZ3W",  361023, 361023],
["JZ4W",  361024, 361024],
["JZ5W",  361025, 361025],
["JZ6W",  361026, 361026],
["JZ7W",  361027, 361027],
["JZ8W",  361028, 361028],
["JZ9W",  361029, 361029],
["JZ10W", 361030, 361030],
["JZ11W", 361031, 361031],
["JZ12W", 361032, 361032],
#--------------------------
# MC samples - HERWIG++
["JZ0W_H",  426040, 426040],
["JZ1W_H",  426041, 426041],
["JZ2W_H",  426042, 426042],
["JZ3W_H",  426043, 426043],
["JZ4W_H",  426044, 426044],
["JZ5W_H",  426045, 426045],
["JZ6W_H",  426046, 426046],
["JZ7W_H",  426047, 426047],
["JZ8W_H",  426048, 426048],
["JZ9W_H",  426049, 426049],
["JZ10W_H", 426050, 426050],
["JZ11W_H", 426051, 426051],
["JZ12W_H", 426052, 426052],
]

if isData : the_list = data_list
else : the_list = mc_list

#--------------------------
run_list = []

# make list of runs
for i_list in the_list :
 n_point = 0
 str_run = ""

 for i_char in i_list :
  if i_char == "." : n_point += 1
  if n_point == 3 and i_char != "." : str_run += i_char

 # get rid of the first two 0 for data
 if isData : str_run = str_run[2:]
 #print str_run
 run_list.append( int(str_run) )

# do it for HERWIG++ if MC
if isData==False : 
  
  the_list_H = mcHERWIG_list
  run_list_H = []
  
  # make list of runs
  for i_list in the_list_H :
   n_point = 0
   str_run = ""
  
   for i_char in i_list :
    if i_char == "." : n_point += 1
    if n_point == 3 and i_char != "." : str_run += i_char
   #print str_run
   run_list_H.append( int(str_run) )

#--------------------------
# recreate file
if isData and is16 and is15      : filename0="data_files_15and16.txt"
if isData and is16==False        : filename0="data_files_15.txt"
if isData and is15==False        : filename0="data_files_16.txt"
if isData==False and is16==False : filename0="mc_files_15c_bpu.txt"
if isData==False and is16        : filename0="mc_files_15c_cpu.txt"
f = open(filename0, 'w')

# order list of runs
run_list.sort()
# print list in order
# loop on runs
index_2=1
the_ref_prev = ""  
the_ref = ""  
for i_run in run_list :

 the_ref_prev = the_ref  
 the_ref = ""  

 # identify slice/dataPeriod
 for i_ref in ref :
  if i_run >= i_ref[1] and i_run <= i_ref[2] : the_ref = i_ref[0]

 # print each file with its ref  
 for i_data in the_list :
  if i_data.find( str(i_run) ) >= 0 :
   for root, directories, files in os.walk(path+i_data):
    index=0
    for filename in files:
     string="  "+path+i_data+filename
     if the_ref!=the_ref_prev and index==0 : 
       if(index_2!=1) : f.write("\n")
       string =the_ref+string
       index += 1
     print string
     index_2=0
     f.write(string)

f.write("\n")
f.close()

#--------------------------
# recreate file for HERWIG
if isData==False:

  if is16==False : filename1="mcHERWIG_files_15c_bpu.txt"
  if is16        : filename1="mcHERWIG_files_15c_cpu.txt"
  f_H = open(filename1, 'w')
  
  # order list of runs
  run_list_H.sort()
  # print list in order
  # loop on runs
  index_2=1
  the_ref_prev = ""  
  the_ref = ""  
  for i_run in run_list_H :
  
   the_ref_prev = the_ref 
   the_ref = ""  
  
   # identify slice/dataPeriod
   for i_ref in ref :
    if i_run >= i_ref[1] and i_run <= i_ref[2] : the_ref = i_ref[0]
  
   # print each file with its ref  
   for i_data in the_list_H :
    if i_data.find( str(i_run) ) >= 0 :
     for root, directories, files in os.walk(path_H+i_data):
      index=0
      for filename in files:
       string="  "+path_H+i_data+filename
       if the_ref!=the_ref_prev and index==0 : 
         if(index_2!=1) : f_H.write("\n")
         string =the_ref+string
         index += 1
       print string
       index_2=0
       f_H.write(string)
  
  f_H.write("\n")
  f_H.close()


