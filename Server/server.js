var port = 3000;
var guestId = 0;
var playerList = new Array();

var io = require("socket.io").listen(port, () => {

});

io.sockets.on("connection", (client) => {
    var roomName = null;
    var playerName = "손님"+(++guestId);
    client.emit('nickName', playerName);
    playerList.forEach(x => { client.emit('join', x ); });
    io.sockets.emit('join', playerName);
    playerList.push(playerName);
    
    client.on('message', (data) => {
        if(roomName != null)
            io.sockets.in(roomName).emit('message', {sender: playerName, message: data });
        else
            io.sockets.emit('message', {sender: playerName, message: data });
    });

    client.on('createRoom', (data) => {
        io.sockets.emit('createRoom', {roomName: data, builder: playerName });
        roomName = data;
        client.join(data);
    });

    client.on('nameChange', (data) => {
        if(data.length < 2 | data.length > 12)
            alertMessage("이름은 2글자 이상 12글자 이하여야 합니다.");
        else {
            data.replace(/&/gi, "&#38;");
            data.replace(/#/gi, "&#35;");
            data.replace(/&&#3538;/gi, "&#38;");
            data.replace(/</gi, "&lt;");
            data.replace(/>/, "&gt;");
            // data.replace(/\(/gi, "&#40;");
            // data.replace(/\)/gi, "&#41;");
            data.replace(" ", "nbsp;");
            data.replace("=", "&#61;");

            if(playerList.includes(data)) {
                alertMessage("이미 존재하는 이름입니다.");
            }else {
                playerList[playerList.indexOf(playerName)] = data;
                io.sockets.emit('nickChange', { oldName: playerName, newName: data });
                playerName = data;
                client.emit('nickName', data);
            }
        }
    });

    client.on('disconnect', () => {
        //접속해제
        //방 존재시 끉기 및 quit 실행
        io.sockets.emit('quit', playerName);
        playerList.splice(playerList.indexOf(playerName), 1);
    });

    

    var alertMessage = (msg) => {
        client.emit('alertMessage', msg);
    };
});

setInterval(function() {
    console.log("wakeUP");
}, 600000);

//https://velog.io/@ju_h2/Node.js-socket.io로-실시간-채팅-구현하기
//https://m.blog.naver.com/tkddlf4209/221833759786