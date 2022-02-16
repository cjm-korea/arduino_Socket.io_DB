const express = require('express');
const app = express();
const http = require('http');
const server = http.createServer(app);

const { Server } = require("socket.io");
const io = new Server(server);

var mysql      = require('mysql');
var connection = mysql.createConnection({
  host     : 'localhost',
  user     : 'root',
  password : 'choi2350',
  database : 'testdb1'
});
connection.connect();

// localhost:3000으로 방문 시 index.html로 라우팅
app.get('/', (req, res) => {
  res.sendFile(__dirname + '/index.html');
});

var isRequest = false;

var request = setInterval(() => {
  if(isRequest) {
    io.emit('request', 9999);
    var sql = 'INSERT INTO project VALUES(\'arduino\',' + temp + ')'
    connection.query(sql, function(err, rows, fields){
      if(err){
        console.log(err);
      } else {
        console.log(rows);
      }
    });
  }
}, 3000);

var temp;

// socket이 connection 상태일때
io.on('connection', (socket) => {
  socket.on('join',(nick) => {
    if(nick == "Arduino!") {
      isRequest = true;
      request;
      console.log("Arduino is connected");
    }else {

    }
  });

  // 아두이노에서 데이터를 받는 부분
  socket.on('request', (msg) => {
    temp = msg;
    console.log(temp);
  });
  
  socket.on('disconnect', (nick) => {
    console.log(`${nick} disconnected`);
    if (nick == "Arduino!") {
      isRequest = false;
    } else {

    }
  });
});



// // socket이 connection 상태일때
// io.on('connection', (socket) => {
//   console.log('connect!');
//   // 3초에 한 번씩 Server에서 아두이노로 데이터를 보내라고 요청하는 메세지를 보냄
//   var request = setInterval(function () {
//     io.emit('request', 9999);
//   },3000);

//   // 그럼 아두이노가 보낸 데이터를 DB, WEB으로 보냄
//   socket.on('request', (msg) => {
//     console.log(msg);
//   });

//   socket.on('chat message', (msg) => {
//     // 메세지를 받으면 다시 메세지를 보낸다
//     // io.emit('chat message', msg);
//     console.log('message: ' + msg);
//   });
// socket.on('disconnect', () => {
//   clearInterval(request);
//   console.log('user disconnected');
// });
// });

// server는 localhost:3000
server.listen(3000, () => {
  console.log('listening on *:3000');
});

// connection.end();
