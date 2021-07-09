
docker_login:
	@echo "Username: $oauthtoken"
	@echo "Password: ZTZwNWZva20wNDEwNWhibGU3N25nOXUzNW46MTY4ZGYzOGItNWQ3NS00YzMwLWJhZDAtMTg5NDI1NGY4NTk2"
	docker login nvcr.io


start:
	docker network create -d macvlan -o parent=enp2s0 --subnet=192.168.20.0/24 --gateway 192.168.20.1 --ip-range 192.168.20.128/28 --aux-address="root=192.168.20.50" --aux-address="jetson1=192.168.20.55" --aux-address="jetson2=192.168.20.56"  media_server
	docker run -it -d --rm --name video_server --net=jetsondocker_media_server --runtime nvidia  -w /video_server -v /tmp/.X11-unix/:/tmp/.X11-unix -v $(pwd)/video_server:/video_server/ -v $(pwd)/videos:/vid -p "8554:8554" -p "9000:9000" --ip "172.22.0.4" nvcr.io/nvidia/deepstream-l4t:5.1-21.02-samples

run:
	@echo "enter this: 'python3 /video_server/server/simulated_rtsp.py'"
	docker exec -it video_server bash

start_macvlan:
	./macvlan_setup.sh 'eth0'
	docker run -it -d --rm --name video_server --net=jetsondocker_media_server --runtime nvidia  -w /video_server -v /tmp/.X11-unix/:/tmp/.X11-unix -v $(pwd)/video_server:/video_server/ -v $(pwd)/videos:/vid -p "8554:8554" -p "9000:9000" --ip "192.168.80.55" nvcr.io/nvidia/deepstream-l4t:5.1-21.02-samples
	@echo "enter this: 'python3 /video_server/server/simulated_rtsp.py'"
	docker exec -it video_server bash

stop:
	docker-compose down --rmi all --volumes --remove-orphans

clean:
	@ echo 'RUN: pruning containers, volumes, and networks'
	docker network rm mist_server || continue
	docker system prune -a  || continue
	docker volume prune || continue
	docker network prune || continue
	@echo "\n\nChecking docker containers"
	docker ps
stop_clean: stop clean
