#!/usr/bin/env python
import os, re
import commands
import math, time
import sys

print 
print 'START'
print 
########   YOU ONLY NEED TO FILL THE AREA BELOW   #########
########   customization  area #########
NumberOfJobs= 50 # number of jobs to be submitted
interval = 1 # number files to be processed in a single job, take care to split your file so that you run on all files. The last job might be with smaller number of files (the ones that remain).
dir = "/vols/cms/tstreble/HGC_ntuples/SingleNu_PU200/"
OutputFileNames = dir + "jet_ntuples_merged/ntuple_jet_merged" # base of the output file name, they will be saved in res directory
ScriptName = "scripts/runJets.py" # script to be used with cmsRun
#FileList = "filelist.txt" # list with all the file directories
queue = "8nh" # give bsub queue -- 8nm (8 minutes), 1nh (1 hour), 8nh, 1nd (1day), 2nd, 1nw (1 week), 2nw 
########   customization end   #########

path = os.getcwd()
print
print 'do not worry about folder creation:'
os.system("rm -r " + dir + "tmp")
os.system("mkdir " + dir + "tmp")
print

##### loop for creating and sending jobs #####
for x in range(1, int(NumberOfJobs)+1):
   ##### creates directory and file list for job #######
   os.system("mkdir "+dir+"tmp/"+str(x))
   os.chdir(dir+"tmp/"+str(x))
   #os.system("sed '"+str(1+interval*(x-1))+","+str(interval*x)+"!d' ../../"+FileList+" > list.txt ")
   
   ##### creates jobs #######
   with open('job.sh', 'w') as fout:
      fout.write("#!/bin/sh\n")
      fout.write("echo\n")
      fout.write("echo\n")
      fout.write("echo 'START---------------'\n")
      fout.write("echo 'WORKDIR ' ${PWD}\n")
      fout.write("export SCRAM_ARCH=slc6_amd64_gcc530\n")
      fout.write("export XRD_NETWORKSTACK=IPv4\n")
      fout.write("export X509_USER_PROXY=/home/hep/tstreble/myVoms/x509up_u`id -u`\n")
      fout.write("cd /home/hep/tstreble/CMSSW_9_2_5_patch2/src\n")
      fout.write("eval `scramv1 runtime -sh`\n")
      fout.write("cd /home/hep/tstreble/HGC_macros\n")     
      fout.write("root -b -l <<EOF\n")
      fout.write(".L jet_ntuple_merger.C\n")
      #fout.write("add_jet(\"root://cms-xrd-global.cern.ch//store/user/tstreble/DoubleNuE1Eta14_31/DoubleNu_91X_PU200_17_10_21/171021_160459/0000/ntuple_piPU200_test90X_"+str(x)+".root\",\"hgcalTriggerNtuplizer/HGCalTriggerNtuple\",\"/vols/cms/tstreble/HGC_ntuples/SingleNu_PU200/jet_ntuples/ntuple_jet_"+str(x)+".root\",\"jets\",\""+OutputFileNames+"_"+str(x)+".root\",false);\n")
      #fout.write("add_jet(\"root://xrootd.grid.hep.ph.ic.ac.uk//store/user/tstreble/VBF_HToInvisible_M125_14TeV_powheg_pythia8/VBF_Hinv_90X_PU0_17_10_25/171025_123942/0000/ntuple_piPU200_test90X_"+str(x)+".root\",\"hgcalTriggerNtuplizer/HGCalTriggerNtuple\",\"/vols/cms/tstreble/HGC_ntuples/VBF_Hinv_PU0/jet_ntuples/ntuple_jet_"+str(x)+".root\",\"jets\",\""+OutputFileNames+"_"+str(x)+".root\",false);\n")
      #fout.write("add_jet(\"root://xrootd.grid.hep.ph.ic.ac.uk//store/user/tstreble/VBF_HToInvisible_M125_14TeV_powheg_pythia8/VBF_Hinv_90X_PU200_17_10_25/171025_124148/0000/ntuple_piPU200_test90X_"+str(x)+".root\",\"hgcalTriggerNtuplizer/HGCalTriggerNtuple\",\"/vols/cms/tstreble/HGC_ntuples/VBF_Hinv_PU200/jet_ntuples/ntuple_jet_"+str(x)+".root\",\"jets\",\""+OutputFileNames+"_"+str(x)+".root\",false);\n")
      fout.write("add_jet(\"root://cms-xrd-global.cern.ch//store/user/tstreble/DoubleNuE1Eta14_31/DoubleNu_91X_PU200_17_10_21/171021_160459/0000/ntuple_piPU200_test90X_"+str(x)+".root\",\"hgcalTriggerNtuplizer/HGCalTriggerNtuple\",\""+dir+"jet_ntuples/ntuple_jet_"+str(x)+".root\",\"jets\",\""+OutputFileNames+"_"+str(x)+".root\",false);\n")
      fout.write(".q\n")
      fout.write("EOF\n")
      fout.write("echo 'STOP---------------'\n")
      fout.write("echo\n")
      fout.write("echo\n")
   os.system("chmod 755 job.sh")
   
   ###### sends bjobs ######
   #os.system("bsub -q "+queue+" -o logs job.sh")
   os.system("qsub -cwd -q hep.q -l h_vmem=20G -l h_rt=24:0:0 -l s_rt=23:50:0 job.sh")
   print "job nr " + str(x) + " submitted"
   
   os.chdir("../..")
   
print
print "your jobs:"
os.system("qstat")
print
print 'END'
print
