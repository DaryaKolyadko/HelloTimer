sudo echo "0" > /sys/hello_world/HelloWorld
sudo rmmod hello_timer
dmesg > info.txt