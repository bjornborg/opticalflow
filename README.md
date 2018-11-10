# opticalflow

This assumes that you have amd64 arch, docker-ce installed, and your user is a part of the docker groups. 

Easy checks:

`docker run hello-world`

Groups:

`groups`

you should see docker. If you are not a part of the docker group, simply pre-pend sudo command to all docker commands.

Have image data ready in a folder 'data'. Create a list of the filenames for the before images and after images. Example of this may be seen in 'data/'

To run the benchmark looping 10 times use

Lucas-Kanade

`docker run -v ${PWD}/data:/data bjornborg/lucaskanade 10`

Farnebäck

`docker run -v ${PWD}/data:/data bjornborg/farneback 10`

Simple flow

`docker run -v ${PWD}/data:/data bjornborg/simpleflow 10`

If you want to loop it 100 times or any non negative arbitrary number, replace 10 with that number.
All results are generated in ${PWD}/data/results. It is root owned folder. Simply chown or copy the folder if you want to do postprocessing.
