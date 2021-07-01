FROM ubuntu:focal
ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=Australia/Brisbane
RUN apt-get update -y && \
    apt-get install -y tzdata
RUN apt-get install -y --no-install-recommends \
                    git \
                    curl \
                    gcc-9 \
                    g++ \
                    clang-10 \
                    python3 \
                    python3-pip \
                    build-essential \
                    cmake \
                    doxygen \
                    unzip \
                    tar \
                    ca-certificates \
                    libboost-all-dev \
                    wget \
                    && \
    apt-get autoclean && \
    apt-get autoremove && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

RUN update-alternatives --install /usr/bin/python python /usr/bin/python3 1

RUN git clone --progress --verbose https://github.com/raspberrypi/tools.git --depth=1 pitools

ENV BUILD_FOLDER /build

WORKDIR ${BUILD_FOLDER}

