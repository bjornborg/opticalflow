#!/bin/bash
# $1 number of loops for time measurements

algorithm='deepflow'

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
    outputName=$(basename ${imageBefore} | sed 's/\.[^.]*$//')
    perf stat /tmp/${algorithm} --image_before=${dataPath}/${imageBefore} --image_after=${dataPath}/${imageAfter} --output_flow=${resultPath}/flow/${outputName}.flo 2>&1 >/dev/null | tail -n 2 | head -n 1 | sed 's/ \+//' | sed 's/,/./' | sed 's/ seconds time elapsed//' >> ${resultPath}/time.csv
    # /usr/bin/time -a -o ${resultPath}/time.csv -f "%E,%P,%S,%U,%e" /tmp/${algorithm} --image_before=${dataPath}/${imageBefore} --image_after=${dataPath}/${imageAfter} --output_flow=${resultPath}/flow/${resultCounter}.flo
    /tmp/color_flow -quiet ${resultPath}/flow/${outputName}.flo ${resultPath}/colorflow/${outputName}.png > /dev/null
    resultCounter=$[$resultCounter +1]
  done
done

echo 'Done'
