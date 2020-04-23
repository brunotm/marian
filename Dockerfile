FROM ubuntu:18.04 AS builder


ENV MARIANPATH /marian
ENV INTEL_KEY_URL https://apt.repos.intel.com/intel-gpg-keys/GPG-PUB-KEY-INTEL-SW-PRODUCTS-2019.PUB
ENV INTEL_REPO_URL https://apt.repos.intel.com/mkl


RUN apt update \
    && apt install -y curl ca-certificates gnupg \
    && curl -sS $INTEL_KEY_URL | apt-key add - \
    && echo deb $INTEL_REPO_URL all main > /etc/apt/sources.list.d/intel-mkl.list \
    && apt update \
    && apt install -y \
        git \
        cmake \
        build-essential \
        libboost-all-dev \
        libprotobuf10 \
        protobuf-compiler \
        libprotobuf-dev \
        openssl \
        libssl-dev \
        libgoogle-perftools-dev \
        intel-mkl-64bit-2019.5-075 \
        && rm -rf /var/lib/apt/lists/*

WORKDIR /
RUN git clone --single-branch --branch brunotm.mmap-threads https://github.com/brunotm/marian

WORKDIR $MARIANPATH/build
RUN cmake \
    -DCOMPILE_CPU=ON \
    -DCOMPILE_CUDA=FALSE \
    -DCOMPILE_SERVER=ON \
    -DUSE_STATIC_LIBS=ON \
    $MARIANPATH \
    && make -j 4

FROM ubuntu:18.04
WORKDIR /app/marian
COPY --from=builder /marian/build/marian* /app/marian/

