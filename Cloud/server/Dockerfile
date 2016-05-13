FROM daocloud.io/node:0.10-onbuild
RUN mkdir -p /usr/src/app
WORKDIR /usr/src/app
COPY . /usr/src/app
ENTRYPOINT node main.js
EXPOSE 8888
EXPOSE 80