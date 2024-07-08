FROM alpine:3.6

ADD build-tc.sh /opt/build-tc.sh
RUN /opt/build-tc.sh

ENV PATH "/opt/toolchain:$PATH"
ENV ISETHER "true"
ENV BUILDROOT "/opt/"
WORKDIR /opt
