#!/bin/bash

# Example usage
# ./macvlan_setup.sh 'eth0'
# arg passed for target device: panda/enp2s0, nx1/ eth0, nx2/eth0
# run ifconfig on target device to find ethernet name (Jetson Xavier nx is eth0 for 192.168.80.29)
# sudo ifconfig mist promisc
parent=$1

sprintf "** Creating macvlan on device **"
sprintf "** Retrieving ethernet from ifconfig paramaters **"
netstat -rn
echo

ip link add mist link "$parent" type macvlan mode bridge
ip addr add 192.168.80.0/24 dev mist
ip link set mist up
ifconfig mist promisc
ip route add 192.168.80.128/28 dev mist
docker network create -d macvlan -o macvlan_mode=bridge -o parent="$parent" --subnet=192.168.80.0/24 --gateway 192.168.80.1 --ip-range 192.168.80.128/28 --aux-address="ba1=192.168.80.11" --aux-address="ba2=192.168.80.12" --aux-address="ba3=192.168.80.13" --aux-address="jetson=192.168.80.55" --aux-address="jetson2=192.168.80.56" mist_server || continue

