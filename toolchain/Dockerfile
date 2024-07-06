FROM alpine:3.20

ADD toolchain-build.sh /opt/build.sh
RUN /opt/build.sh

ENV PATH "/opt/toolchain:$PATH"
ENV ETHER "true"
ENV BUILDDIR "/opt"

WORKDIR /opt
