var socket = io.connect('http://localhost:80');

// 添加一个连接监听器
socket.on('connect',function() {
  console.log('Client has connected to the server!');
  $("#status_tag").html("正常");
  $("#status_tag").addClass("am-badge-success");
  $("#status_tag").removeClass("am-badge-danger");

});

// 添加一个连接监听器
socket.on('notice',function(data) {
  if (data[0]=='0') {
    $("#motorstatus_tag").html("停转");
    $("#motorstatus_tag").removeClass("am-badge-success");
    $("#motorstatus_tag").addClass("am-badge-danger");
    $("#motor_switch").attr('checked', false);
  } else if (data[0]=='1') {
    $("#motorstatus_tag").html("转动中");
    $("#motorstatus_tag").addClass("am-badge-success");
    $("#motorstatus_tag").removeClass("am-badge-danger");
    $("#motor_switch").attr('checked', true);
  }else{
    $("#motorstatus_tag").html("未知");
    $("#motorstatus_tag").removeClass("am-badge-success");
    $("#motorstatus_tag").removeClass("am-badge-danger");
    $("#motor_switch").attr('checked', false);
  }
  $("#motorspeed_tag").html(data[1]);
  if (parseInt(data[1])>=90) {
      $("#motorspeed_tag_badge").removeClass("am-badge-success");
      $("#motorspeed_tag_badge").addClass("am-badge-danger");
  } else {
      $("#motorspeed_tag_badge").addClass("am-badge-success");
      $("#motorspeed_tag_badge").removeClass("am-badge-danger");
  }
  $("#tempature_tag").html(data[2]);
  $("#humidity_tag").html(data[3]);
  $("#aqi_tag").html(data[4]);
  if (parseInt(data[4])>300) {
    $("#aqi_tag_badge").css('background-color', '#660000');
    $("#airlevel_tag").html('严重污染');
    $("#airlevel_tag").css('background-color', '#660000');
  } else if (parseInt(data[4])>200) {
    $("#aqi_tag_badge").css('background-color', '#990099');
    $("#airlevel_tag").html('重度污染');
    $("#airlevel_tag").css('background-color', '#990099');
  } else if (parseInt(data[4])>150) {
    $("#aqi_tag_badge").css('background-color', '#FF0000');
    $("#airlevel_tag").html('中度污染');
    $("#airlevel_tag").css('background-color', '#FF0000');
  } else if (parseInt(data[4])>100) {
    $("#aqi_tag_badge").css('background-color', '#FF6600');
    $("#airlevel_tag").html('轻度污染');
    $("#airlevel_tag").css('background-color', '#FF6600');
  } else if (parseInt(data[4])>50) {
    $("#aqi_tag_badge").css('background-color', '#CCFF00');
    $("#airlevel_tag").html('良');
    $("#airlevel_tag").css('background-color', '#CCFF00');
  } else {
    $("#aqi_tag_badge").css('background-color', '#66CC00');
    $("#airlevel_tag").html('优');
    $("#airlevel_tag").css('background-color', '#66CC00');
  }
});

// 添加一个关闭连接的监听器
socket.on('disconnect',function() {
  console.log('The client has disconnected!');
  $("#status_tag").html("断开");
  $("#status_tag").removeClass("am-badge-success");
  $("#status_tag").addClass("am-badge-danger");
});

// 通过Socket发送一条消息到服务器
function sendMessageToServer(message) {
  socket.send(message);
}

$(function() {
    $("#motor_switch").click(function(event) {
        sendMessageToServer("M");
    });
    $("#ring_test").click(function(event) {
        sendMessageToServer("M");
    });
    $("#up_speed").click(function(event) {
        sendMessageToServer("U");
    });
    $("#down_speed").click(function(event) {
        sendMessageToServer("D");
    });
});