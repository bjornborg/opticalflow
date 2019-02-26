#!/bin/bash

##
# Author: Nikolaus Mayer
# Author: Björnborg Nguyen
##
# -g gpu id
# -n network
# $1 number of loops for time measurements


# echo "Installing perf tool specifically for your kernel"
# apt-get -qq update &&  apt-get -qq install -y \
#         linux-tools-`uname -r`

## Fail if any command fails (use "|| true" if a command is ok to fail)
set -e
## Treat unset variables as error
set -u

## Exit with error code
fun__die () {
  exit `false`;
}

## Print usage help
fun__print_usage () {
  printf "###################################################################\n";
  printf "#                           FlowNet 2.0                           #\n";
  printf "###################################################################\n";
  printf "add '-g 0' for using gpu with id 0\n";
  printf "add '-n FlowNet2' for using a pretrained net. E.g. '-n FlowNet2-KITTI'\n";
  printf "example with 100 loops: ./bench.sh -g 0 -n FlowNet2-KITTI 100 ";
  printf "Available 'network' values:\n";
  printf "  FlowNet2\n";
  printf "  FlowNet2-c\n";
  printf "  FlowNet2-C\n";
  printf "  FlowNet2-cs\n";
  printf "  FlowNet2-CS\n";
  printf "  FlowNet2-css\n";
  printf "  FlowNet2-CSS\n";
  printf "  FlowNet2-css-ft-sd\n";
  printf "  FlowNet2-CSS-ft-sd\n";
  printf "  FlowNet2-s\n";
  printf "  FlowNet2-S\n";
  printf "  FlowNet2-SD\n";
  printf "  FlowNet2-ss\n";
  printf "  FlowNet2-SS\n";
  printf "  FlowNet2-sss\n";
  printf "  FlowNet2-KITTI\n";
  printf "  FlowNet2-Sintel\n";
}

## Parameters (some hardcoded, others user-settable)
GPU_IDX=0;
# CONTAINER="flownet2";
NETWORK="";
# VERBOSITY=0;


## Parse arguments into parameters
while getopts g:n:h OPTION; do
  case "${OPTION}" in
    g) GPU_IDX=$OPTARG;;
    n) NETWORK=$OPTARG;;
    # v) VERBOSITY=`expr $VERBOSITY + 1`;;
    h) fun__print_usage; exit `:`;;
    [?]) fun__print_usage; fun__die;;
  esac
done
shift `expr $OPTIND - 1`;



## Check and use "-n" input argument
BASEDIR="/tmp/flownet2/models";
WORKDIR="/tmp/flownet2/scripts";
case "${NETWORK}" in
  "FlowNet2")           ;;
  "FlowNet2-c")         ;;
  "FlowNet2-C")         ;;
  "FlowNet2-cs")        ;;
  "FlowNet2-CS")        ;;
  "FlowNet2-css")       ;;
  "FlowNet2-CSS")       ;;
  "FlowNet2-css-ft-sd") ;;
  "FlowNet2-CSS-ft-sd") ;;
  "FlowNet2-s")         ;;
  "FlowNet2-S")         ;;
  "FlowNet2-SD")        ;;
  "FlowNet2-ss")        ;;
  "FlowNet2-SS")        ;;
  "FlowNet2-sss")       ;;
  "FlowNet2-KITTI")     ;;
  "FlowNet2-CSS-Sintel")    ;;
  *) echo -e "Unknown network: ${NETWORK} (run with -h to print available networks)";
     fun__die;;
esac
WEIGHTS="${BASEDIR}/${NETWORK}/${NETWORK}_weights.caffemodel*";
DEPLOYPROTO="${BASEDIR}/${NETWORK}/${NETWORK}_deploy.prototxt.template";

## (Debug output)
echo -e "Using GPU:       ${GPU_IDX}";
echo -e "Running network: ${NETWORK}";
echo -e "Working dir:     ${WORKDIR}";
# fun__debug_printf "First input:     ${FIRST_INPUT}";
# fun__debug_printf "Second input:    ${SECOND_INPUT}";
# fun__debug_printf "Output:          ${OUTPUT}";

cd /tmp/flownet2

export CAFFE_PATH="/tmp/flownet2"
export RELEASE_PATH="$CAFFE_PATH/build" 
export PYTHONPATH="$CAFFE_PATH/python"
export LD_LIBRARY_PATH="$RELEASE_PATH/lib:$LD_LIBRARY_PATH"
export PATH="$RELEASE_PATH/tools:$RELEASE_PATH/scripts:$PATH"
export CAFFE_BIN="$RELEASE_PATH/tools/caffe"
algorithm='flownet2'

dataPath="/data"
resultPath=${dataPath}"/results/"${algorithm}

nBeforeEntries=$(wc -l < /data/first_image.txt)
nAfterEntries=$(wc -l < /data/second_image.txt)

# echo ${nBeforeEntries}
# echo ${nAfterEntries}
if [ ${nBeforeEntries} != ${nAfterEntries} ]; then
  echo 'Number of entries do not equals in before and after lists'
fi

mkdir -p ${resultPath}/flow
mkdir -p ${resultPath}/colorflow
echo '#elapsed time [seconds]' > ${resultPath}/time.csv

for n in $(seq 1 ${1})
do
  echo -e "Loop: " $n
  resultCounter=0
  paste /data/first_image.txt /data/second_image.txt | while IFS="$(printf '\t')" read -r imageBefore imageAfter
  do
    echo -e "Data: $[${resultCounter} +1]/${nBeforeEntries}"
    # printf $imageBefore" "$imageAfter
    flowOutputPath=${dataPath}/results/${algorithm}/flow/$(dirname ${imageBefore})
    colorFlowOutpuPath=${dataPath}/results/${algorithm}/colorflow/$(dirname ${imageBefore})
    mkdir -p ${flowOutputPath}
    mkdir -p ${colorFlowOutpuPath}
    outputName=$(basename ${imageBefore} | sed 's/\.[^.]*$//')
    # echo ${dataPath}/${imageBefore} 
    # echo ${dataPath}/${imageAfter}
    # echo ${flowOutputPath}/${outputName}.flo
    # python /tmp/flownet2/scripts/run-flownet-docker.py --gpu ${GPU_IDX} ${WEIGHTS} ${DEPLOYPROTO} ${dataPath}/${imageBefore} ${dataPath}/${imageAfter} ${flowOutputPath}/${outputName}.flo
    perf stat python /tmp/flownet2/scripts/run-flownet-docker.py --gpu ${GPU_IDX} ${WEIGHTS} ${DEPLOYPROTO} ${dataPath}/${imageBefore} ${dataPath}/${imageAfter} ${flowOutputPath}/${outputName}.flo 2>&1 >/dev/null | tail -n 6 | head -n 1 | sed 's/ \+//' | sed 's/,/./' | sed 's/ seconds time elapsed//' >> ${resultPath}/time.csv
    /tmp/color_flow -quiet ${flowOutputPath}/${outputName}.flo ${colorFlowOutpuPath}/${outputName}.png > /dev/null
    resultCounter=$[$resultCounter +1]
  done
done

echo 'Done'

## Bye!
exit `:`;