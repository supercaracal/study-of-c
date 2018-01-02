FROM debian:latest

RUN apt-get -q -y update && apt-get -q -y install xinetd

COPY ./daytime /etc/xinetd.d/
COPY ./echo    /etc/xinetd.d/

EXPOSE 7
EXPOSE 13

CMD ["/usr/sbin/xinetd", "-d"]
