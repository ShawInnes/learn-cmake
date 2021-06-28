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
                    unzip \
                    tar \
                    ca-certificates \
                    libboost-all-dev \
                    && \
    apt-get autoclean && \
    apt-get autoremove && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

RUN update-alternatives --install /usr/bin/python python /usr/bin/python3 1
RUN pip3 install conan
RUN conan profile update settings.compiler.libcxx=libstdc++11 default
