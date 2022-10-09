#/bin/sh

docker run --gpus all -it --rm --env="DISPLAY" --volume="/tmp/.X11-unix:/tmp/.X11-unix:rw" --env="QT_X11_NO_MITSHM=1" -v $(pwd):/tmp/repo vulkan-test:latest /bin/bash /tmp/repo/scripts/compile.sh