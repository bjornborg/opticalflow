#!/bin/bash

docker build -f Dockerfile.cudaopencv4contrib.amd64 -t bjornborg/cudaopencv4contrib:${1} .

if [[ $* == *-p* ]]; then  
  docker push bjornborg/cudaopencv4contrib:${1} &
fi

projects=( flownet2 deepflow dense-inverse-search dualtvl1 farneback lucas-kanade pcaflow simpleflow )

for optflow in "${projects[@]}"
do 
  cd ${optflow}
  docker build --build-arg FROM_TAG=${1} -f Dockerfile.amd64 -t bjornborg/${optflow}:${1} .
  if [[ $* == *-p* ]]; then 
    docker push bjornborg/${optflow}:${1} &
  fi
  cd ..
done
wait
echo "Build done"