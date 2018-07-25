#!/bin/bash
rm results.log
for i in Dockerfile-*[^~]; do
    docker build --pull -f $i .
    if [ $? -eq 0 ]; then
        echo "$i PASSED" >> results.log
    else
        echo "$i FAILED" >> results.log
    fi
done
docker container prune -f
cat results.log
