#!/bin/sh
source functions.sh
###########################################################
## CONFIGURE JOB ####
###########################################################

## What cycle do you want to run.  
cycle="Analyzer"
## Which stream is being run egamma/muon
stream="egamma"
## How many cores should the job use
njobs=2
## How much data are you running/ for MC this helpw weight the events
data_lumi="A"
### name output location : by default it is ${LQANALYZER_DIR}/data/output, but you can change it 
outputdir=${LQANALYZER_DIR}/data/output/

## How many events between log messages (default = 10000)     
#logstep=1000 
#### WHAT SAMPLES TO RUN >> THIS SHOULD CORRESPOND TO FIRST COLUMN IN txt/datasets.txt
declare -a periods=( "A")

############################################################
################# Do not change anything after this line
############################################################

if [ -z ${LQANALYZER_DIR} ]
    then 
    setupLQANALYZER
fi

stream=$(makeParseVariable 's' ${stream})
njobs=$(makeParseVariable 'j' ${njobs})
cycle=$(makeParseVariable 'c' ${cycle})
data_lumi=$(makeParseVariable 'd' ${data_lumi})
outputdir=$(makeParseVariable 'O' ${outputdir})


################
#submit
for i in ${periods[@]}
  do
  python ${LQANALYZER_DIR}/python/localsubmit.py -p ${i} ${stream} ${njobs} ${cycle} ${logstep} ${data_lumi} ${outputdir}
done

################
echo ""
echo "Done"