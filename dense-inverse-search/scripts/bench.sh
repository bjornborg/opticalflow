#!/bin/bash
# $1 number of loops for time measurements
# $2 preset

echo "Installing perf tool specifically for your kernel"
apt-get -qq update &&  apt-get -qq install -y \
        linux-tools-`uname -r`

algorithm='dense-inverse-search'



dataPath="/data"
resultPath=${dataPath}"/results/"${algorithm}

# echo -e ${3}

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
    perf stat /tmp/${algorithm} --image_before=${dataPath}/${imageBefore} --image_after=${dataPath}/${imageAfter} --output_flow=${flowOutputPath}/${outputName}.flo --preset=${2:-medium} 2>&1 >/dev/null | tail -n 2 | head -n 1 | sed 's/ \+//' | sed 's/,/./' | sed 's/ seconds time elapsed//' >> ${resultPath}/time.csv
    /tmp/color_flow -quiet ${flowOutputPath}/${outputName}.flo ${colorFlowOutpuPath}/${outputName}.png > /dev/null
    resultCounter=$[$resultCounter +1]
  done
done

echo 'Done'
