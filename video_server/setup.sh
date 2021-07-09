#!/bin/bash

# Setup when using docker run ...
# EXAMPLE USAGE: sudo -H ./setup.sh

echo " *** starting setup.sh *** "

apt-get update
## install basics
ln -fs /usr/share/zoneinfo/Canada/Pacific /etc/localtime
apt-get install -y build-essential graphviz tzdata python3-pip git pkg-config libcairo2-dev libgirepository1.0-dev
## Create symbolic link for 'docker logs <container-name>'
mkdir -p /logs  && chmod +x /logs
ln -sf /dev/stdout /logs/python.log

apt-get -y autoclean && apt-get -y clean && apt-get -y autoremove
python3 -m pip install --upgrade wheel pip setuptools
python3 -m pip install -r /video_server/requirements.txt

echo " *** Running setup_ridgerun_docker.sh *** "

chmod +x /video_server/ridgerun_plugins/setup_ridgerun_docker.sh
/video_server/ridgerun_plugins/setup_ridgerun_docker.sh

echo " *** Finished setup.sh *** "
