#!/bin/sh

counter=0

while : ; do
    echo "$counter: a little something"
    counter=$((counter+1)) 
    sleep 3
done
