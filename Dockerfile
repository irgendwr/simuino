FROM debian:buster-slim
LABEL description="Simuino is an Arduino UNO/MEGA Pin Simulator."

RUN apt-get update && \
    apt-get -y upgrade && \
    apt-get -y install ncurses-dev build-essential git && \
    rm -rf /tmp/* /var/tmp/* /var/lib/apt/lists/*

COPY . /simuino

WORKDIR /simuino
RUN g++ -o /simuino/simuino simuino.c -lncurses
RUN chmod +x /simuino/simuino

VOLUME ["/simuino/sketchbook"]
ENTRYPOINT /simuino/simuino
