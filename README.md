
[![DOI](https://zenodo.org/badge/123254737.svg)](https://zenodo.org/badge/latestdoi/123254737)

# Optical flow docker benchmark deployment

This assumes that you have amd64 architecture, docker-ce installed, and your user is a part of the docker group. For installation of Docker, visit https://docs.docker.com/install/overview/ . If you want to run nvidia+docker, please visit https://github.com/NVIDIA/nvidia-docker
First, run the following sanity checks to ensure compatibility:

```sh
# Check if you are a part of docker group
groups
# running docker hello world
docker run hello-world
# running nvidia docker hello world
docker run --gpus all --rm nvidia/cuda:9.0-base nvidia-smi
```

## Data
There are some example data in folder 'data/'. The folder contains sequences of images data, two .txt files containing a list of before and after images (to estimate optical flow between).

```sh
# List the example data
ls data/
# see listed before images
cat data/first_image.txt
# see listed after images
cat data/first_image.txt
```
If you want to run the benchmarks on a different data set, simply put the data in the data/ folder and update the .txt files with the names of the images files. Alternatively, replace ${PWD} variable when running the benchmark with the absolute path pointing to the data containing similar structure with two lists containing lists of images names.

### Creating the first_image.txt and second_image.txt lists

```sh
cd data/
ls exampledatabase/* > tmp.txt && head -n -1 tmp.txt > first_image.txt && rm tmp.txt
ls exampledatabase/* > tmp.txt && tail -n +2 tmp.txt > second_image.txt && rm tmp.txt
cd ..

```


## Running the estimators
The benchmark results will have three outputs (.flo files, colored flo images and time.csv files) in the results folder located where the data set is (for example in data/results). For example:
```sh
# Running lucas kanade OF algorithm on the example data
docker run --privileged -v ${PWD}/data:/data bjornborg/lucas-kanade 1
# Or on custom data with the path /myDataPath/customdata
docker run --privileged -v /myDataPath/customdata:/data bjornborg/lucas-kanade 1
```


To run a single benchmark looping 10 or 1000 times use the following cases:

### Deepflow
```sh
# Deepflow (default param values)
docker run --privileged -v ${PWD}/data:/data bjornborg/deepflow 10
# If you want to loop it 1000 times or any non-negative arbitrary number, replace 10 with that number.
# For example 1000 loops 
docker run --privileged -v ${PWD}/data:/data bjornborg/deepflow 1000 
```

### Dense inverse search
```sh
# Dense inverse search with default preset (medium)
docker run --privileged -v ${PWD}/data:/data bjornborg/dense-inverse-search 10 
# with ultrafast preset
docker run --privileged -v ${PWD}/data:/data bjornborg/dense-inverse-search 10 ultrafast
```

### Dual TVL1
```sh
# Dual TVL1
docker run --privileged -v ${PWD}/data:/data bjornborg/dualtvl1 10
# with additional params: tau=0.25, lambda=0.15, theta=0.3, nscales=5, warps=5, epsilon=0.01, innnerIterations=30, outerIterations=10, scaleStep=0.8, gamma=0.0, medianFiltering=5
docker run --privileged -v ${PWD}/data:/data bjornborg/dualtvl1 10 0.25 0.15 0.3 5 5 0.01 30 10 0.8 0.0 5
```

### Farnebäck 
```sh
#(default param values) with 10 loops
docker run --privileged -v ${PWD}/data:/data bjornborg/farneback 10
#or with additional params: pyr_scale=0.5, levels=3, winsize=15, iterations=3, poly_n=5, poly_sigma=1.2 
docker run --privileged -v ${PWD}/data:/data bjornborg/farneback 10 0.5 3 15 3 5 1.2
```


### FlowNet2 (requires gpu)
```sh
# FlowNet2 using gpu (id) 0 and KITTI pretrained network with 10 loops
docker run --gpus all --cap-add SYS_ADMIN -v ${PWD}/data:/data bjornborg/flownet2 -g 0 -n FlowNet2-KITTI 10
# To see other pretrained network run 
docker run --gpus all --cap-add SYS_ADMIN -v ${PWD}/data:/data bjornborg/flownet2 -h
```

### Lucas-Kanade
```sh
# Lucas-Kanade (default param values)
docker run --privileged -v ${PWD}/data:/data bjornborg/lucas-kanade 10
# Adding additional params is possible, in this case: gridstep=8, k=128, sigma=0.05
docker run --privileged -v ${PWD}/data:/data bjornborg/lucas-kanade 10 8 128 0.05
```

### Pcaflow
```sh
# Pca flow (default param values)
docker run --privileged -v ${PWD}/data:/data bjornborg/pcaflow 10
```

### Simple flow 
```sh
# Simple flow (default param values) with 10 loops
docker run --privileged -v ${PWD}/data:/data bjornborg/simpleflow 10
#or with additional params: layers=3, averaging_block_size=2, max_flow=4, sigma_dist=4.1, sigma_color=25.5, postprocess_window=18, sigma_dist_fix=55.0, sigma_color_fix=25.5, occ_thr=0.35, upscale_averaging_radius=18, upscale_sigma_dist=55.0, upscale_sigma_color=25.5, speed_up_thr=10.0
docker run --privileged -v PWD}/data:/data bjornborg/farneback 10 3 2 4 4.1 25.5 18 55.0 25.5 0.35 18 55.0 25.5 10.0
```

All results are generated in '${PWD}/data/results'. It is root owned folder. Simply chown or copy the folder if you want to do postprocessing.

## Building docker images on your local machine

There are some included scripts for building and running the benchmarks. For building simply run
```sh
./buildDocker.sh customdockertag
```

## Running automated benchmarking script
For running benchmarks for all algorithms with 100 loops on defaulted settings run
```sh
# Looping 100 times in the data
./benchAll.sh ${PWD}/data 100

# Using your custom docker build
./benchAll.sh ${PWD}/data 100 customdockertag

# Using my docker hub repo with stable tag
./benchAll.sh ${PWD}/data 100 stable

# If you want to include GPU benchmark cases on the Kitti data, for example, run
./benchAllKitti.sh ${PWD}/data 100 stable -g
```

# Real time kernel

## Ubuntu 18.04
For best consistency of results, install and use a real time linux kernel. There are many real time kernel versions and patches. Most recent versions should be compatible with docker. 

Building the real-time kernel with docker
```sh
cd rt-kernel

docker build -f Dockerfile.rtkernel.amd64 -t builtkernel .

# Copying the debian packages to your host machine 
id=$(docker create builtkernel) && docker cp $id:/root/deb-pkg deb-pkg && docker rm -v $id
```


Install the deb packages
```sh
cd deb-pkg
# install custom kernel
sudo dpkg -i *.deb

# update the grub
sudo update-grub

# reboot in to the rt kernel (during boot up, choose the advanced option and pick the rt-kernel we just built ) 
sudo reboot
```


Make sure to install equivalent real-time nvidia-drivers as well (highly recommend nvidia dkms for easy installation).

If you are using nvidia drivers, it is recommended to use dkms packages for handling driver compatibilities.
```sh
sudo apt install -y nvidia-dkms-390
```

After the reboot, use the following command
```sh
uname -a
```
and you should see 'preempt rt' which are the tags for real-time capabilities for the Linux kernel.

## Arch Linux

See the aur package: https://aur.archlinux.org/pkgbase/linux-rt-lts/ and further install the nvidia-dkms package.

## Further information

For further reading and details to customize your linux kernel, see https://wiki.linuxfoundation.org/realtime/documentation/howto/applications/preemptrt_setup
