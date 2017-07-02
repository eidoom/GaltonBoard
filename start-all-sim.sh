#!/bin/bash
for i in 10.10.10.{11..16}
do 
	ssh $i /home/pi/src/royalsoc/galtonsim &
done
