FROM node:0.10-onbuild
RUN mkdir -p /usr/src/app
WORKDIR /usr/src/app
RUN npm install socket.io
COPY . /usr/src/app
ENTRYPOINT node main.js
EXPOSE 8888
EXPOSE 80