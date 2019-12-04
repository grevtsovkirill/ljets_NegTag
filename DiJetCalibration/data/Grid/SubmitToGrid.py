#!/usr/bin/env python
import TopExamples.grid
import filesR21
import os
import grid
import shutil
import sys

configDir = os.getenv('WorkDir_DIR') + '/data/DiJetCalibration/'
configFiles = os.listdir(configDir)
for configFile in configFiles:
    print configFile
    fullFileName = os.path.join(configDir, configFile)
    print fullFileName
    if os.path.isfile(fullFileName) and os.path.realpath('.') != os.path.realpath(configDir):
        shutil.copy(fullFileName,'.');
config=grid.Config()
#config = TopExamples.grid.Config()
config.CMake = True
config.code          = 'top-xaod'
config.gridUsername  = 'perf-flavtag'
config.groupProduction= 'True'
#config.excludedSites = 'ANALY_CONNECT,ANALY_CONNECT_SHORT'#'ANALY_GOEGRID'
config.noSubmit      =  False # True #
config.memory	     = ''
config.otherOptions='--forceStaged'
config.mergeType     = 'Default' #'None', 'Default' or 'xAOD'
#config.destSE        = 'DESY-HH_LOCALGROUPDISK' 
config.nGBPerJob='4'
config.maxFileSize='20000000000000'
config.extFile='.root,.so'

subsuf="05-12"
v_subsuf="_v0"
# for systematics
#baseSuffix = subsuf+"_mc16a"+v_subsuf
baseSuffix = subsuf+"_data15"+v_subsuf
config.suffix = baseSuffix
names = ["data15",]
config.settingsFile = "Dijet.txt"
samples = grid.Samples(names)
grid.submit(config, samples)
