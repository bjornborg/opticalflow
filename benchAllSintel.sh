#!/bin/bash
#$1 datapath
#$2 num loops

projects=( deepflow dense-inverse-search farneback lucas-kanade pcaflow simpleflow dualtvl1 )

for optflow in "${projects[@]}"
do 
  echo -e "Running ${optflow}"
  docker run --runtime=nvidia --privileged -v /usr/bin/perf:/usr/bin/perf -v ${1}:/data bjornborg/${optflow}:beta ${2}
done
if [[ $* == *-g* ]]; then
  # echo -e "Using gpu on ${1}" 
  echo -e "Running flownet2"
  docker run --runtime=nvidia --privileged -v /usr/bin/perf:/usr/bin/perf -v ${1}:/data bjornborg/flownet2:beta -g 0 -n FlowNet2-CSS-Sintel ${2}
fi