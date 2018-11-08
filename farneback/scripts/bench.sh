#!/bin/bash
# $1 file with first image entries
# $2 file with second image entries
# $3 number of loops for time measurements

algorithm='farneback'

dataPath="/data"
resultPath=${dataPath}"/results/"${algorithm}

# echo -e ${3}

nBeforeEntries=$(wc -l < ${1})
nAfterEntries=$(wc -l < ${2})

# echo ${nBeforeEntries}
# echo ${nAfterEntries}
if [ ${nBeforeEntries} != ${nAfterEntries} ]; then
  echo 'Number of entries do not equals in before and after lists'
fi

mkdir -p ${resultPath}/flow
mkdir -p ${resultPath}/colorflow
echo '#elapsed,percent,systime,usrtime,elapsed' > ${resultPath}/time.csv

for n in $(seq 1 ${3})
do
  echo -e "Loop: " $n
  resultCounter=0
  paste ${1} $2 | while IFS="$(printf '\t')" read -r imageBefore imageAfter
  do
    echo -e "Data: $[${resultCounter} +1]/${nBeforeEntries}"
    # printf $imageBefore" "$imageAfter
    /usr/bin/time -a -o ${resultPath}/time.csv -f "%E,%P,%S,%U,%e" /tmp/farneback --image_before=${dataPath}/${imageBefore} --image_after=${dataPath}/${imageAfter} --output_flow=${resultPath}/flow/${resultCounter}.flo --pyr_scale=0.5 --levels=3 --winsize=15 --iterations=3 --poly_n=5 --poly_sigma=1.2 
    /tmp/color_flow -quiet ${resultPath}/flow/${resultCounter}.flo ${resultPath}/colorflow/${resultCounter}.png > /dev/null
    resultCounter=$[$resultCounter +1]
  done
done

echo 'Done'
