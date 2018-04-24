enable=$1
hostname=`cat /etc/hostname`
for cpu in `seq 0 $(($enable-1))`; do
	echo "$hostname: Switching *ON* cpu #$cpu"
	echo 1 | sudo tee /sys/devices/system/cpu/cpu$cpu/online
done;

for cpu in `seq $enable $(($(ls /sys/devices/system/cpu/cpu[0-9]* | grep :$ | wc -l)-1))`; do
	echo "$hostname: Switching *OFF* cpu #$cpu"
	echo 0 | sudo tee /sys/devices/system/cpu/cpu$cpu/online
done;
