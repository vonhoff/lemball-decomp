FROM ubuntu:24.04

ARG DEBIAN_FRONTEND=noninteractive
ARG OW_PORTABLE_URL=http://openwatcom.org/ftp/source/ow_portable_v2_stable.zip

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        ca-certificates \
        curl \
        python3 \
        unzip \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /opt

RUN curl -fsSLo /tmp/openwatcom.zip "${OW_PORTABLE_URL}" \
    && unzip -q /tmp/openwatcom.zip -d /opt/openwatcom \
    && rm -f /tmp/openwatcom.zip

ENV WATCOM=/opt/openwatcom
ENV PATH=/opt/openwatcom/binl64:/opt/openwatcom/binl:${PATH}
ENV EDPATH=/opt/openwatcom/eddat
ENV INCLUDE=/opt/openwatcom/h

WORKDIR /work

CMD ["/bin/sh"]
