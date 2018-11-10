#!/bin/bash
# $1 number of loops for time measurements
# $2 tau=0.25
# $3 lambda=0.15
# $4 theta=0.3
# $5 nscales=5
# $6 warps=5
# $7 epsilon=0.01
# $8 innnerIterations=30
# $9 outerIterations=10
# $10 scaleStep=0.8
# $11 gamma=0.0
# $12 medianFiltering=5
algorithm='dualtvl1'



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
echo '#elapsed,percent,systime,usrtime,elapsed' > ${resultPath}/time.csv

for n in $(seq 1 ${1})
do
  echo -e "Loop: " $n
  resultCounter=0
  paste /data/first_image.txt /data/second_image.txt | while IFS="$(printf '\t')" read -r imageBefore imageAfter
  do
    echo -e "Data: $[${resultCounter} +1]/${nBeforeEntries}"
    # printf $imageBefore" "$imageAfter
    /usr/bin/time -a -o ${resultPath}/time.csv -f "%E,%P,%S,%U,%e" /tmp/${algorithm} --image_before=${dataPath}/${imageBefore} --image_after=${dataPath}/${imageAfter} --output_flow=${resultPath}/flow/${resultCounter}.flo --tau=${2:-0.25} --lambda=${3:-0.15} --theta=${4:-0.3} --nscales=${5:-5} --warps=${6:-5} --epsilon=${7:-0.01} --innnerIterations=${8:-30} --outerIterations=${9:-10} --scaleStep=${10:-0.8} --gamma=${11:-0.0}0 --medianFiltering=${12:-5}
    /tmp/color_flow -quiet ${resultPath}/flow/${resultCounter}.flo ${resultPath}/colorflow/${resultCounter}.png > /dev/null
    resultCounter=$[$resultCounter +1]
  done
done

echo 'Done'
