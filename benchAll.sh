#!/bin/bash

projects=( deepflow dense-inverse-search dualtvl1 farneback lucas-kanade pcaflow simpleflow flownet2 )

for optflow in "${projects[@]}"
do 
  echo -e "Running ${optflow}"
  docker run -v ${PWD}/data:/data bjornborg/${optflow}:latest 3
done