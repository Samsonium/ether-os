#!/bin/bash -e

./tc emu & remote-viewer vnc://localhost:5900 && docker kill $(docker ps | awk 'NR==2 {print $1}')
