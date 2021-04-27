#!/bin/sh

mkdir -p '/var/time_log'
cp 'cntr.py' '/var/time_log/'
chmod -R 0777 '/var/time_log'

cp 'internettime.service' '/etc/systemd/system/'
