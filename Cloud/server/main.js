var net = require('net');
var fs = require('fs');
var http= require('http'), io= require('socket.io');

var HOST = '0.0.0.0';
var PORT = 8888;
var global_new = false;
var gsock;

var server= http.createServer(function(req, res){
  // 发送HTML的headers和message
  res.writeHead(200,{ 'Content-Type': 'text/html' });
  res.end('<h1>Hello Socket Lover!</h1>');
});
server.listen(80);
var socket= io.listen(server);
// 添加一个连接监听器
socket.on('connection', function(client){
  console.log('CLIENT Connected');
  client.on('message',function(event){
    console.log('Received message from client!',event);
    gsock.write(event);
  });
  client.on('disconnect',function(){
    console.log('Server has disconnected');
  });
});

net.createServer(function(sock) {

    // 我们获得一个连接 - 该连接自动关联一个socket对象
    console.log('CONNECTED: ' +
        sock.remoteAddress + ':' + sock.remotePort);
    gsock=sock;
    // 为这个socket实例添加一个"data"事件处理函数
    sock.on('data', function(odata) {
        odata = odata.toString();
        var data = odata.split(":");
        if(data[0]=='N'){
            //写入文件
            data.shift();
            console.log('DATA ' + sock.remoteAddress + ": \n  Motor Status: " + data[0]+"\n  Motor Speed: "+data[1]+"\n  Tempature: "+data[2]+"\n  Humidity: "+data[3]+"\n  AQI: "+data[4]);
            socket.emit("notice",data);
            data = JSON.stringify(data);
            fs.writeFile('data.dat',data, function (err) {
                if (err) throw err;
                console.log('Saved!'); //文件被保存
            });
        }else{
            console.log('DATA ' + sock.remoteAddress + ': ' + odata);
        }
        sock.write('G');
        process.env.TZ = "Asia/Shanghai";
        var curDate = new Date();
        var hour=curDate.getHours();
        var min=curDate.getMinutes();
        var sec=curDate.getSeconds();
        sock.write(hour+':'+min+':'+sec);
    });

    // 为这个socket实例添加一个"close"事件处理函数
    sock.on('close', function(data) {
        console.log('CLOSED: ' +
            sock.remoteAddress + ' ' + sock.remotePort);
    });

}).listen(PORT, HOST);

console.log('Server listening on ' + HOST +':'+ PORT);