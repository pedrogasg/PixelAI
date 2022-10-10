#/bin/sh

docker build -t vulkan-test .

docker run --gpus all -e NVIDIA_DISABLE_REQUIRE=1 -e DISPLAY --device /dev/dri --net host -e NVIDIA_DRIVER_CAPABILITIES=all \
   -v /usr/share/vulkan/icd.d/nvidia_icd.json:/etc/vulkan/icd.d/nvidia_icd.json \
   -v /usr/share/vulkan/implicit_layer.d/nvidia_layers.json:/etc/vulkan/implicit_layer.d/nvidia_layers.json \
   -v /usr/share/glvnd/egl_vendor.d/10_nvidia.json:/usr/share/glvnd/egl_vendor.d/10_nvidia.json \
   -it --rm vulkan-test:latest

##  -v /usr/share/glvnd/egl_vendor.d/10_nvidia.json:/usr/share/glvnd/egl_vendor.d/10_nvidia.json \

# docker run --gpus all \
#    -e NVIDIA_DISABLE_REQUIRE=1 \
#    -v $HOME/.Xauthority:/root/.Xauthority \
#    -e DISPLAY -e NVIDIA_DRIVER_CAPABILITIES=all --device /dev/dri --net host \
#    -v /etc/vulkan/icd.d/nvidia_icd.json:/etc/vulkan/icd.d/nvidia_icd.json \
#    -v /etc/vulkan/implicit_layer.d/nvidia_layers.json:/etc/vulkan/implicit_layer.d/nvidia_layers.json \
#    -v /usr/share/glvnd/egl_vendor.d/10_nvidia.json:/usr/share/glvnd/egl_vendor.d/10_nvidia.json \
#    -it nvidia/vulkan:1.3-470 \ 
#    bash

# docker run --gpus all --net=host \
#    -e NVIDIA_DISABLE_REQUIRE=1 \
#    -e DISPLAY -e NVIDIA_DRIVER_CAPABILITIES=all \
#    -it nvidia/vulkan:1.3-470 /bin/bash

# docker run --gpus all \
#    -e NVIDIA_DISABLE_REQUIRE=1 \
#    -v $HOME/.Xauthority:/root/.Xauthority \
#    -e DISPLAY -e NVIDIA_DRIVER_CAPABILITIES=all --device /dev/dri --net host \
#    -v /usr/share/vulkan/icd.d/nvidia_icd.json:/etc/vulkan/icd.d/nvidia_icd.json \
#    -v /usr/share/vulkan/implicit_layer.d/nvidia_layers.json:/etc/vulkan/implicit_layer.d/nvidia_layers.json \
#    -v /usr/share/glvnd/egl_vendor.d/10_nvidia.json:/usr/share/glvnd/egl_vendor.d/10_nvidia.json \
#    -it nvidia/vulkan:1.3-470 \bin\bash