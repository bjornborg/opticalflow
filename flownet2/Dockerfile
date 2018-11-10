## Note: Our Caffe version does not work with CuDNN 6
FROM nvidia/cuda:8.0-cudnn5-devel-ubuntu16.04

## Put everything in some subfolder
WORKDIR "/tmp"

## The build context contains some files which make the raw FlowNet2
## repo fit for Docker
COPY FN2_Makefile.config ./
COPY FN2_run-flownet-docker.py ./

## 1. Install packages
## 2. Download and install CUDA driver
## 3. Download and build DispNet/FlowNet Caffe distro
## 4. Remove some now unused packages and clean up (for a smaller image)
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        module-init-tools \
        build-essential \
        ca-certificates \
        wget \
        git \
        libatlas-base-dev \
        libboost-all-dev \
        libgflags-dev \
        libgoogle-glog-dev \
        libhdf5-serial-dev \
        libleveldb-dev \
        liblmdb-dev \
        libopencv-dev \
        libprotobuf-dev \
        libsnappy-dev \
        protobuf-compiler \
        python-dev \
        python-numpy \
        python-scipy \
        python-protobuf \
        python-pillow \
        python-skimage \
        time \
        axel
RUN git clone --depth=1 https://github.com/lmb-freiburg/flownet2 && \
    cp ./FN2_Makefile.config ./flownet2/Makefile.config && \
    cp ./FN2_run-flownet-docker.py ./flownet2/scripts/run-flownet-docker.py && \
    cd flownet2 && \
    rm -rf .git && \
    cd models && \
    wget -q https://lmb.informatik.uni-freiburg.de/resources/binaries/flownet2/flownet2-models-kitti.tar.gz && \
    wget -q https://lmb.informatik.uni-freiburg.de/resources/binaries/flownet2/flownet2-models-sintel.tar.gz && \
    wget -q https://lmb.informatik.uni-freiburg.de/resources/binaries/flownet2/flownet2-models.tar.gz  && \
    tar xvzf flownet2-models.tar.gz && \
    tar xvzf flownet2-models-kitti.tar.gz && \
    tar xvzf flownet2-models-sintel.tar.gz && \
    rm -rf *.tar.gz && \
    cd .. && \
    make -j`nproc` && \
    make -j`nproc` pycaffe 

RUN wget -q http://vision.middlebury.edu/flow/code/flow-code.zip && \
    unzip flow-code.zip && \
    cd flow-code/imageLib && \
    make && \
    cd .. && \
    make && \
    cp color_flow /tmp

RUN  apt-get remove -y \
        module-init-tools \
        build-essential \
        ca-certificates \
        git \
        wget \
        axel && \
    apt-get autoremove -y && \
    apt-get autoclean -y && \
    rm -rf /var/lib/apt/lists/*

ADD scripts/bench.sh /flownet2/
ENTRYPOINT ["/flownet2/bench.sh"]