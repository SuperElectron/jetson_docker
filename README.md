---

# Jetson Docker with Ridgerun Plugins

---
## Jetson Xavier dependencies

- Nvidia NGC requires a package for docker login

```bash
sudo apt-get -y install gnupg2 pass
```

- and docker may complain about secrets if you haven't completed docker's post-installation instructions ...

```bash
sudo groupadd docker
sudo usermod -aG docker $USER
newgrp docker 
sudo chown "$USER":"$USER" /home/"$USER"/.docker -R
sudo chmod g+rwx "$HOME/.docker" -R
docker run hello-world
```
<if this fails, install docker>


- sync gpu clocks to enable max performance for accelerated plug ins
```bash
sudo nvpmodel -m 0
sudo jetson_clocks
```

- ensure you have the accelerated plugins on your jetson

```bash
ls -al /usr/lib/aarch64-linux-gnu/gstreamer-1.0/libgstnvvideo4linux2.so
-rw-r--r-- 1 root root 254552 Feb 19 08:46 /usr/lib/aarch64-linux-gnu/gstreamer-1.0/libgstnvvideo4linux2.so
```


## Running the project

- login to Nvidia NGC

```bash
make docker_login
```

- ensure that you have videos in 'jetson_docker/videos' and include the filenames in 'simulated_rtsp.py'
```bash
jetson_docker $ tree -I 'ridgerun_plugins'
.
├── docker-compose.yml
├── README.md
├── videos
│   ├── Tvideo1.ts
│   └── video2.ts
└── video_server
    |── ridgerun_plugins
    ├── Dockerfile
    ├── __init__.py
    ├── requirements.txt
    └── server
        ├── bus_call.py
        ├── __init__.py
        ├── logs
        │   └── readme.txt
        ├── setup_logging.py
        ├── simulated_rtsp.py
        └── utils.py

```  

- spin up the project

```bash
make start
```

- start the video server

```bash
make run
```

- view the output

```bash
gst-play-1.0 rtsp://172.22.0.4:8554/eo
gst-play-1.0 rtsp://172.22.0.4:9000/ir
```

- shut down and clean up (this will clean images, network, and more!)
```bash
make stop
make clean
```
---

# changing the IP address
- stop the project
```bash
make stop
```

- make modifications to the address in `macvlan_setup.sh` (--ip-range 192.168.80.128/28)
- the argument is the ethernet port ('eth0' on Nvidia Jetson Xavier)
```bash
./macvlan_setup.sh 'eth0'
```

- if you have a custom network, adjust `--net=external` and `--ip` as needed for `docker run`.  Do not include `docker network create` if you do this.
```bash
docker network create --driver=bridge --subnet=172.22.0.0/24 media_server
docker run -it -d --rm --name video_server --net=jetsondocker_media_server --runtime nvidia  -w /video_server -v /tmp/.X11-unix/:/tmp/.X11-unix -v $(pwd)/video_server:/video_server/ -v $(pwd)/videos:/vid -p "8554:8554" -p "9000:9000" --ip "172.22.0.4" nvcr.io/nvidia/deepstream-l4t:5.1-21.02-samples
```

- alternatively, use this if you've set up `macvlan_setup.sh`
```bash
make start_macvlan
```

---

- good luck!
