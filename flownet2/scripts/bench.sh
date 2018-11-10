#!/bin/bash

##
# Author: Nikolaus Mayer
# Author: Björnborg Nguyen
##
# -g gpu id
# -n network
# $1 number of loops for time measurements


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

## Verbosity-controlled "printf" wrapper for ERROR
# fun__error_printf () {
#   if test $VERBOSITY -ge 0; then
#     printf "%s\n" "$@";
#   fi
# }
# ## Verbosity-controlled "printf" wrapper for DEBUG
# fun__debug_printf () {
#   if test $VERBOSITY -ge 1; then
#     printf "%s\n" "$@";
#   fi
# }

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

## Isolate network inputs
# FIRST_INPUT="";
# SECOND_INPUT="";
# OUTPUT="";
# if test "$#" -ne 3; then
#   fun__error_printf "! Missing input or output arguments";
#   fun__die;
# else
#   FIRST_INPUT="$1";
#   SECOND_INPUT="$2";
#   OUTPUT="$3";
# fi

## Check if input files exist
# if test ! -f "${FIRST_INPUT}"; then
#   fun__error_printf "First input '${FIRST_INPUT}' is unreadable or does not exist.";
#   fun__die;
# fi
# if test ! -f "${SECOND_INPUT}"; then
#   fun__error_printf "Second input '${SECOND_INPUT}' is unreadable or does not exist.";
#   fun__die;
# fi


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
  "FlowNet2-Sintel")    ;;
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
echo '#elapsed,percent,systime,usrtime,elapsed' > ${resultPath}/time.csv

for n in $(seq 1 ${1})
do
  echo -e "Loop: " $n
  resultCounter=0
  paste /data/first_image.txt /data/second_image.txt | while IFS="$(printf '\t')" read -r imageBefore imageAfter
  do
    echo -e "Data: $[${resultCounter} +1]/${nBeforeEntries}"
    # printf $imageBefore" "$imageAfter
    /usr/bin/time -a -o ${resultPath}/time.csv -f "%E,%P,%S,%U,%e" python /tmp/flownet2/scripts/run-flownet-docker.py --gpu ${GPU_IDX} ${WEIGHTS} ${DEPLOYPROTO} ${dataPath}/${imageBefore} ${dataPath}/${imageAfter} ${resultPath}/flow/${resultCounter}.flo
    # /usr/bin/time -a -o ${resultPath}/time.csv -f "%E,%P,%S,%U,%e" /tmp/farneback --image_before=${dataPath}/${imageBefore} --image_after=${dataPath}/${imageAfter} --output_flow=${resultPath}/flow/${resultCounter}.flo --pyr_scale=${4:-0.5} --levels=${5:-3} --winsize=${6:-15} --iterations=${7:-3} --poly_n=${8:-5} --poly_sigma=${9:-1.2} 
    /tmp/color_flow -quiet ${resultPath}/flow/${resultCounter}.flo ${resultPath}/colorflow/${resultCounter}.png > /dev/null
    resultCounter=$[$resultCounter +1]
  done
done

echo 'Done'






## Run docker container
#  - "--device" lines map a specified host GPU into the contained
#  - "-v" allows the container the read from/write to the current $PWD
#  - "-w" executes "cd" in the container (each network has a folder)
## Note: The ugly conditional only switches stdout on/off.
# if test $VERBOSITY -ge 2; then
#   nvidia-docker run \
#     --rm \
#     --volume "${PWD}:/input-output:rw" \
#     --workdir "${WORKDIR}" \
#     -it "$CONTAINER" /bin/bash -c "cd ..; source set-env.sh; cd -; python run-flownet-docker.py --verbose --gpu ${GPU_IDX} ${WEIGHTS} ${DEPLOYPROTO} ${FIRST_INPUT} ${SECOND_INPUT} ${OUTPUT}"
# else
#   nvidia-docker run \
#     --rm \
#     --volume "${PWD}:/input-output:rw" \
#     --workdir "${WORKDIR}" \
#     -it "$CONTAINER" /bin/bash -c "cd ..; source set-env.sh; cd -; python run-flownet-docker.py --gpu ${GPU_IDX} ${WEIGHTS} ${DEPLOYPROTO} ${FIRST_INPUT} ${SECOND_INPUT} ${OUTPUT}"
#     > /dev/null;
# fi

## Bye!
echo -e "Done!";
exit `:`;