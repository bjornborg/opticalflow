#!/bin/bash
#$1 datapath
#$2 num loops

projects=( deepflow dense-inverse-search dualtvl1 farneback lucas-kanade pcaflow simpleflow )

dataBases=( clean final )
dataCases=( ambush_1 ambush_3 bamboo_3 cave_3 market_1 market_4 mountain_2 PERTURBED_market_3 PERTURBED_shaman_1 temple_1 tiger wall)

for base in "${dataBases[@]}"
do
  for case in "${dataCases[@]}"
  do
    for optflow in "${projects[@]}"
    do 
      echo -e "Running ${optflow}"
      docker run -v ${1}/${base}/${case}:/data bjornborg/${optflow}:latest ${2}
    done
    if [[ $* == *-g* ]]; then
      echo "Using gpu on ${1}" 
      echo -e "Running flownet2"
      docker run --runtime=nvidia -v ${1}/${base}/${case}:/data bjornborg/flownet2 -g 0 -n FlowNet2-Sintel ${2}
    fi
  done
done
