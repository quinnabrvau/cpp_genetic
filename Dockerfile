FROM ubuntu
RUN apt-get update
RUN apt-get install -y gcc-8 g++-8 cmake make
RUN apt-get install -y git
