#!/bin/bash

projects=( deepflow dense-inverse-search dualtvl1 farneback lucas-kanade pcaflow simpleflow flownet2 )

for optflow in "${projects[@]}"
do 
  cd ${optflow}
  docker build -f Dockerfile.amd64 -t bjornborg/${optflow}:latest .
  # docker push bjornborg/${optflow}:latest &
  cd ..
done