#!/bin/bash
#$1 datapath
#$2 num loops
#$3 docker image tag

projects=( deepflow dense-inverse-search farneback lucas-kanade pcaflow simpleflow dualtvl1 )

for optflow in "${projects[@]}"
do 
  echo -e "Running ${optflow}"
  # docker run --runtime=nvidia --privileged -v /usr/bin/perf:/usr/bin/perf -v ${1}:/data bjornborg/${optflow}:latest ${2}
  docker run --privileged -v ${1}:/data bjornborg/${optflow}:${3} ${2}

done