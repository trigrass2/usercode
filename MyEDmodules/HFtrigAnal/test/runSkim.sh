#!/bin/bash

if (( ${#LOCALRT} < 4 ))
then
    if (( ${#HCALDAQ_SW_LOC} > 3  && ${#HCAL_CMSSW_RELEASE} > 3 ))
    then
	pushd $HCALDAQ_SW_LOC/src/$HCAL_CMSSW_RELEASE/src >/dev/null
	eval `scramv1 runtime -sh`
	popd >/dev/null
    fi
fi

if (( ${#LOCALRT} < 4 ))
then
    echo Please setup your runtime environment!
    exit
fi

if [[ -e ./skim_setup.rc ]] 
then
    source ./skim_setup.rc
fi

ARG1=$1
RUNSPEC=`printf "${FORMAT}" $*`
INCLUDEFILE="${PREFIX}${RUNSPEC}${SUFFIX}"
OUTPUTFILE=../results/${RUNSPEC}-hfttskim.root

echo Including file $INCLUDEFILE
echo Writing file $OUTPUTFILE

if (( ${#EVENTLIMIT} == 0 )) 
then
    EVENTLIMIT="-1";
fi

#### common head part of Config File
### create the file
CFGFILE=/tmp/runCMSSWskim_${USER}_${ARG1}.py
cat > ${CFGFILE}<<EOF
import FWCore.ParameterSet.Config as cms

process = cms.Process("HFTRIGSKIM")
process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(${EVENTLIMIT})
)

process.MessageLogger.cerr.INFO.limit = 500
process.MessageLogger.cerr.FwkReport.reportEvery = 100

EOF

### Mode-dependent part

if [[ "$MODE" == "RAW" ]]
    then
cat >> ${CFGFILE}<<EOF
process.load("L1TriggerConfig.L1GtConfigProducers.L1GtConfig_cff")
process.load("L1TriggerConfig.L1ScalesProducers.L1MuTriggerScalesConfig_cff")
process.load("L1TriggerConfig.L1ScalesProducers.L1MuTriggerPtScaleConfig_cff")
process.load("L1TriggerConfig.L1ScalesProducers.L1MuGMTScalesConfig_cff")
process.load("L1TriggerConfig.GMTConfigProducers.L1MuGMTParametersConfig_cff")

process.gtDigis = cms.EDProducer( "L1GlobalTriggerRawToDigi",
     DaqGtInputTag = cms.InputTag( "source" ),
     DaqGtFedId = cms.untracked.int32( 813 ),
     ActiveBoardsMask = cms.uint32( 0x101 ),
     UnpackBxInEvent = cms.int32( 1 )
)
EOF
elif [[ "$MODE" == "RECO" ]]
    then
echo Assume reco products in input.
else
  echo Unknown mode '$MODE'
  exit
fi    

#### common tail part of Config File
cat >> ${CFGFILE}<<EOF99

process.load("${INCLUDEFILE}")

process.trigFilt = cms.EDFilter("HcalTechTriggerFilt",
    gtDigiLabel     = cms.untracked.InputTag("gtDigis"),
    techTriggerBits = cms.vint32(9)
)

process.out = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('p')
    ),
    fileName = cms.untracked.string('file:${OUTPUTFILE}')
)

EOF99

if [[ "$MODE" == "RECO" ]] 
then
cat >> ${CFGFILE}<<EOF999
process.p = cms.Path(process.trigFilt)
process.o = cms.EndPath(process.out)
EOF999
elif [[ "$MODE" == "RAW" ]] 
then
cat >> ${CFGFILE}<<EOF999
process.p = cms.Path(process.gtDigis*process.trigFilt)
process.o = cms.EndPath(process.out)
EOF999
fi

# run cmsRun
cmsRun ${CFGFILE}
