FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y g++ make && \
    apt-get clean

WORKDIR /app

COPY . .

RUN make

CMD ["make", "test"]
