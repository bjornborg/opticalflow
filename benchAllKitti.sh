#!/bin/bash
#$1 datapath
#$2 num loops

projects=( deepflow dense-inverse-search farneback lucas-kanade pcaflow simpleflow dualtvl1 )


for optflow in "${projects[@]}"
do 
  echo -e "Running ${optflow}"
  docker run --privileged -v ${1}:/data bjornborg/${optflow}:latest ${2}
done

if [[ $* == *-g* ]]; then
  echo "Using gpu on ${1}" 
  echo -e "Running flownet2"
  docker run --privileged --runtime=nvidia -v ${1}:/data bjornborg/flownet2 -g 0 -n FlowNet2-KITTI ${2}
fi