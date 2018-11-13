#!/bin/bash
# $1 number of loops for time measurements
# $2 gridstep
# $3 k
# $4 sigma

algorithm='lucas-kanade'

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
    perf stat /tmp/${algorithm} --image_before=${dataPath}/${imageBefore} --image_after=${dataPath}/${imageAfter} --output_flow=${resultPath}/flow/${resultCounter}.flo --gridstep=${2:-8} --k=${3:-128}  --sigma=${4:-0.05} 2>&1 >/dev/null | tail -n 2 | head -n 1 | sed 's/ \+//' | sed 's/,/./' | sed 's/ seconds time elapsed//' >> ${resultPath}/time.csv
    /tmp/color_flow -quiet ${resultPath}/flow/${resultCounter}.flo ${resultPath}/colorflow/${resultCounter}.png > /dev/null
    resultCounter=$[$resultCounter +1]
  done
done

echo 'Done'
