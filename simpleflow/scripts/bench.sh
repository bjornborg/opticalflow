#!/bin/bash
# $1 number of loops for time measurements
# $2 layers
# $3 averaging_block_size
# $4 max_flow
# $5 sigma_dist
# $6 sigma_color
# $7 postprocess_window
# $8 sigma_dist_fix
# $9 sigma_color_fix
# $10 occ_thr
# $11 upscale_averaging_radius
# $12 upscale_sigma_dist
# $13 upscale_sigma_color
# $14 speed_up_thr

algorithm='simpleflow'

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
    /usr/bin/time -a -o ${resultPath}/time.csv -f "%E,%P,%S,%U,%e" /tmp/simpleflow --image_before=${dataPath}/${imageBefore} --image_after=${dataPath}/${imageAfter} --output_flow=${resultPath}/flow/${resultCounter}.flo --layers=${2:-3} --averaging_block_size=${3:-2} --max_flow=${4:-4} --sigma_dist=${5:-4.1} --sigma_color=${6:-25.5} --postprocess_window=${7:-18} --sigma_dist_fix=${8:-55.0} --sigma_color_fix=${9:-25.5} --occ_thr=${10:-0.35} --upscale_averaging_radius=${11:-18} --upscale_sigma_dist=${12:-55.0} --upscale_sigma_color=${13:-25.5} --speed_up_thr=${14:-10.0}

    /tmp/color_flow -quiet ${resultPath}/flow/${resultCounter}.flo ${resultPath}/colorflow/${resultCounter}.png > /dev/null
    
    resultCounter=$[$resultCounter +1]
  done
done

echo 'Done'
