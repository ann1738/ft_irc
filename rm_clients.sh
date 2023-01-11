#!/bin/bash
​
# Get a list of all containers that ran from the 'irssi' image
containers=$(docker ps -a --format "{{.ID}}" --filter ancestor=irssi)
​
# Loop through the list of containers and stop and remove each one
for container in $containers; do
  docker rm -f $container
done