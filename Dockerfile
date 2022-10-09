FROM nvidia/cuda:11.7.1-devel-ubuntu20.04

ENV NVIDIA_VISIBLE_DEVICES all
ENV NVIDIA_DRIVER_CAPABILITIES all

ENV VULKAN_SDK_VERSION 1.3.224.1

RUN apt-get update \
  && apt-get install -y -qq --no-install-recommends \
    libglvnd0 \
    libgl1 \
    libglx0 \
    libegl1 \
    libgles2 \
    libxcb1-dev \
    wget \
    vulkan-utils \
    && rm -rf /var/lib/apt/lists/*

RUN wget -q --show-progress \
    --progress=bar:force:noscroll \
    https://sdk.lunarg.com/sdk/download/latest/linux/vulkan_sdk.tar.gz \
    -O /tmp/vulkansdk-linux-x86_64-${VULKAN_SDK_VERSION}.tar.gz \ 
    && echo "Installing Vulkan SDK ${VULKAN_SDK_VERSION}" \
    && mkdir -p /opt/vulkan \
    && tar -xf /tmp/vulkansdk-linux-x86_64-${VULKAN_SDK_VERSION}.tar.gz -C /opt/vulkan \
    && mkdir -p /usr/local/include/ && cp -ra /opt/vulkan/${VULKAN_SDK_VERSION}/x86_64/include/* /usr/local/include/ \
    && mkdir -p /usr/local/lib && cp -ra /opt/vulkan/${VULKAN_SDK_VERSION}/x86_64/lib/* /usr/local/lib/ \
    && cp -a /opt/vulkan/${VULKAN_SDK_VERSION}/x86_64/lib/libVkLayer_*.so /usr/local/lib \
    && mkdir -p /usr/local/share/vulkan/explicit_layer.d \
    && cp /opt/vulkan/${VULKAN_SDK_VERSION}/x86_64/etc/vulkan/explicit_layer.d/VkLayer_*.json /usr/local/share/vulkan/explicit_layer.d \
    && mkdir -p /usr/local/share/vulkan/registry \
    && cp -a /opt/vulkan/${VULKAN_SDK_VERSION}/x86_64/share/vulkan/registry/* /usr/local/share/vulkan/registry \
    && cp -a /opt/vulkan/${VULKAN_SDK_VERSION}/x86_64/bin/* /usr/local/bin \
    && ldconfig \
    && rm /tmp/vulkansdk-linux-x86_64-${VULKAN_SDK_VERSION}.tar.gz && rm -rf /opt/vulkan


RUN apt-get update \
  && apt-get install -y -qq --no-install-recommends \
    libxext6 \
    libx11-6 \
  && rm -rf /var/lib/apt/lists/*


# WORKDIR /project

# RUN apt-get update \
#   && apt-get install -y -qq --no-install-recommends \
#     vulkan-tools libvulkan-dev \
#     vulkan-validationlayers-dev spirv-tools \
#     libglfw3-dev libglm-dev \
#     && rm -rf /var/lib/apt/lists/*

# RUN apt-get update \
#   && apt-get install -y -qq --no-install-recommends \
#     libxxf86vm-dev libxi-dev libxrandr-dev \
#     && rm -rf /var/lib/apt/lists/*

#RUN apt-get update \
#  && apt-get install -y -qq --no-install-recommends \
#    wget \
#    && rm -rf /var/lib/apt/lists/*

#RUN wget -O /tmp/install.tgz https://storage.googleapis.com/shaderc/artifacts/prod/graphics_shader_compiler/shaderc/linux/continuous_clang_release/387/20220907-102225/install.tgz

#RUN tar zxf /tmp/install.tgz -C /tmp

#RUN rm /tmp/install.tgz

#RUN cp /tmp/install/bin/glslc /usr/local/bin/glslc

#RUN rm -rf /tmp/install

#RUN wget -qO - http://packages.lunarg.com/lunarg-signing-key-pub.asc | apt-key add -

#RUN wget -qO /etc/apt/sources.list.d/lunarg-vulkan-focal.list http://packages.lunarg.com/vulkan/lunarg-vulkan-focal.list

#RUN apt update

#RUN DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt install -y vulkan-sdk
# COPY . .

# RUN make

CMD [ "vulkaninfo"]

#CMD [ "./main.out"]