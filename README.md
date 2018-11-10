# opticalflow

This assumes that you have amd64 arch, docker-ce installed, and your user is a part of the docker groups. 

Easy checks:

`docker run hello-world`

Groups:

`groups`

you should see docker. If you are not a part of the docker group, simply pre-pend sudo command to all docker commands.

Have image data ready in a folder 'data'. Create a list of the filenames for the before images and after images. Example of this may be seen in 'data/'

To run single benchmark looping 10 times use the following cases:

Lucas-Kanade (default param values)

`docker run -v ${PWD}/data:/data bjornborg/lucaskanade 10`

If you want to loop it 100 times or any non negative arbitrary number, replace 10 with that number.
Adding additional params is possible, in this case: gridstep=8, k=128, sigma=0.05

`docker run -v ${PWD}/data:/data bjornborg/lucaskanade 10 8 128 0.05`


Farnebäck (default param values)

`docker run -v ${PWD}/data:/data bjornborg/farneback 10`

or with additional params: pyr_scale=0.5, levels=3, winsize=15, iterations=3, poly_n=5, poly_sigma=1.2 

`docker run -v ${PWD}/data:/data bjornborg/farneback 10 0.5 3 15 3 5 1.2`

Simple flow (default param values)

`docker run -v ${PWD}/data:/data bjornborg/simpleflow 10`

or with additional params: layers=3, averaging_block_size=2, max_flow=4, sigma_dist=4.1, sigma_color=25.5, postprocess_window=18, sigma_dist_fix=55.0, sigma_color_fix=25.5, occ_thr=0.35, upscale_averaging_radius=18, upscale_sigma_dist=55.0, upscale_sigma_color=25.5, speed_up_thr=10.0

`docker run -v PWD}/data:/data bjornborg/farneback 10 3 2 4 4.1 25.5 18 55.0 25.5 0.35 18 55.0 25.5 10.0`

All results are generated in ${PWD}/data/results. It is root owned folder. Simply chown or copy the folder if you want to do postprocessing.

There are some included scripts for building and running the benchmarks. For building simply run

`./buildDocker.sh`

For running all benchmarks with 100 loops on standard (defaulted settings) run

`./benchAll.sh ${PWD}/data 100`

If you want to include GPU benchmark cases

`./benchAll.sh ${PWD}/data 100 -g`
