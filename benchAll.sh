#!/bin/bash
#$1 datapath
#$2 num loops

projects=( deepflow dense-inverse-search dualtvl1 farneback lucas-kanade pcaflow simpleflow )


for optflow in "${projects[@]}"
do 
  echo -e "Running ${optflow}"
  docker run -v ${1}}/data:/data bjornborg/${optflow}:latest ${2}
done

if [[ $* == *-g* ]]; then
  echo "Using gpu on ${1}" 
  docker run --runtime=nvidia ${1}:/data bjornborg/flownet2 -g 0 -n FlowNet2-KITTI ${2}
fi