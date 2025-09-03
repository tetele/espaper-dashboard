ARG BUILD_BASE_VERSION=2025.7.4


FROM ghcr.io/esphome/esphome:${BUILD_BASE_VERSION} AS base

RUN git config --system --add safe.directory "*"

RUN apt-get update && apt-get install -y libsdl2-dev libsodium-dev build-essential git

WORKDIR /workspaces