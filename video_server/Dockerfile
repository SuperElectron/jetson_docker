#FROM nvcr.io/nvidia/l4t-base:r32.5.0
FROM nvcr.io/nvidia/deepstream-l4t:5.1-21.02-samples

LABEL maintainer="terrasense <matthew.mccann@terrasense.ca>"
#
#WORKDIR /
#ENV PYTHONPATH /
#
#RUN apt-get update
## install basics
#RUN ln -fs /usr/share/zoneinfo/Canada/Pacific /etc/localtime
#RUN apt-get install -y build-essential graphviz tzdata python3-pip git pkg-config libcairo2-dev libgirepository1.0-dev
## Create symbolic link for 'docker logs <container-name>'
#RUN mkdir -p /logs  && chmod +x /logs
#RUN ln -sf /dev/stdout /logs/python.log
## Set up python
#RUN apt-get -y autoclean && apt-get -y clean && apt-get -y autoremove
#RUN python3 -m pip install --upgrade wheel pip setuptools
#
## copy contents to docker container
#RUN mkdir -p /video_server
#COPY . /video_server
#RUN python3 -m pip install -r /video_server/requirements.txt

## install ridgerun plugins
#RUN chmod +x /video_server/ridgerun_plugins/setup_ridgerun_docker.sh
#RUN /video_server/ridgerun_plugins/setup_ridgerun_docker.sh
