#!/bin/bash

export SCRAM_ARCH slc6_amd64_gcc491
cd $HOME/public/Zbb/CMSSW_7_4_11/src/DYJetsMY 
eval `scramv1 runtime -sh`
