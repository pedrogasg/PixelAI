#/bin/sh

docker build -t vulkan-test .

docker run --gpus all -e NVIDIA_DISABLE_REQUIRE=1 -e DISPLAY --device /dev/dri --net host \
   -v /etc/vulkan/icd.d/nvidia_icd.json:/etc/vulkan/icd.d/nvidia_icd.json \
   -v /etc/vulkan/implicit_layer.d/nvidia_layers.json:/etc/vulkan/implicit_layer.d/nvidia_layers.json \
   -it --rm  vulkan-test:latest

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
