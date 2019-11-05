#!/bin/bash
#$1 datapath
#$2 num loops
#$3 docker image tag


projects=( deepflow dense-inverse-search farneback lucas-kanade pcaflow simpleflow dualtvl1 )

for optflow in "${projects[@]}"
do 
  echo -e "Running ${optflow}"
  docker run --gpus all --privileged -v ${1}:/data bjornborg/${optflow}:${3} ${2}
done

if [[ $* == *-g* ]]; then
  echo -e "Running flownet2"  
  docker run --gpus all --cap-add SYS_ADMIN -v ${1}:/data bjornborg/flownet2:${3} -g 0 -n FlowNet2-KITTI ${2}
fi
