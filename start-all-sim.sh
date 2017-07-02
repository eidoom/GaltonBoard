#!/bin/bash
for i in pi@10.10.10.{11..16}
do 
	ssh $i /home/pi/src/royalsoc/galtonsim &
done
