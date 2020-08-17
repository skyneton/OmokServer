var port = 3000;
var guestId = 0;
var playerList = new Array();
var roomList = new Array();

var io = require("socket.io").listen(port, () => { });

io.sockets.on("connection", (client) => {
    client.ready = false;
    client.roomName = null;
    client.playerName = "손님"+(++guestId);
    client.emit('nickName', client.playerName);
    client.join("connection_main_players_room");
    playerList.forEach(x => { client.emit('join', x ); });
    io.sockets.emit('join', client.playerName);
    playerList.push(client.playerName);
    for(var key in roomList) {
        var x = roomList[key];
        client.emit('createRoom', {'roomName': x.roomName, 'admin': x.admin, 'len': x.len, 'password': (x.password != "") } );
    }
    
    
    client.on('message', (data) => {
        if(client.roomName != null)
            io.sockets.in(client.roomName).emit('message', {'sender': client.playerName, 'message': splitTags(data) });
        else
            io.sockets.in("connection_main_players_room").emit('message', {'sender': client.playerName, 'message': splitTags(data) });
    });

    client.on('clientStatusChange', () => {
        if(client.roomName != null) {
            if(roomList[client.roomName].admin == client.playerName) {
                var readyCheck = roomList[client.roomName].clients.some(x => {
                    if(!x.ready) {
                        alertMessage("준비되지 않은 플레이어가 있습니다.");
                        return true;
                    }
                });

                if(!readyCheck) {
                    //게임 시작
                    io.sockets.in(client.roomName).emit('gameStart');
                    roomList[client.roomName].DuringGame = true;
                    roomList[client.roomName].clients[roomList[client.roomName].NowTurn].emit("myTurnNow");
                }

            }else {
                client.ready = !client.ready;
                client.emit('myReadyStatusChange', client.ready);
                io.sockets.in(client.roomName).emit('readyStatusChange', {'playerName': client.playerName, 'status': client.ready });
            }
        }
    });

    client.on('boardClick', (data) => {
        if(client.roomName != null && roomList[client.roomName].DuringGame) {
            if(roomList[client.roomName].clients.indexOf(client) == roomList[client.roomName].NowTurn) {
                if(data.y >= 15 || data.y < 0 || data.x >= 15 || data.x < 0) return;

                if(!roomList[client.roomName].gameBoard[data.y][data.x]) {
                    roomList[client.roomName].gameBoard[data.y][data.x] = roomList[client.roomName].NowTurn+1;
                    io.sockets.in(client.roomName).emit('boardClick', { 'y': data.y, 'x': data.x, 'turn': roomList[client.roomName].NowTurn });


                    switch(gameClearCheck(roomList[client.roomName].gameBoard, data.x, data.y)) {
                        case 1:
                            io.sockets.in(client.roomName).emit("gameEnd");

                            roomList[client.roomName].clients.forEach(x => {
                                if(x.playerName == client.playerName)
                                    alertMessage("승리하였습니다.", x);
                                else
                                    alertMessage("패배하였습니다.", x);
                                
                                if(roomList[client.roomName].admin == x.playerName)
                                    x.ready = true;
                                else {
                                    x.ready = false;
                                    x.emit('myReadyStatusChange', x.ready);
                                    io.sockets.in(client.roomName).emit('readyStatusChange', {'playerName': x.playerName, 'status': x.ready });
                                }
                            });

                            roomList[client.roomName].NowTurn = 0;
                            roomList[client.roomName].DuringGame = false;
                            break;

                        case 2:
                            io.sockets.in(client.roomName).emit("gameEnd");
                            alertMessage("무승부", io.sockets.in(client.roomName));

                            roomList[client.roomName].NowTurn = 0;
                            roomList[client.roomName].DuringGame = false;
                            break;

                        default:
                            roomList[client.roomName].clients[roomList[client.roomName].NowTurn].emit("otherTurnNow");
        
                            roomList[client.roomName].NowTurn = TurnChange(roomList[client.roomName].NowTurn);
                            
                            roomList[client.roomName].clients[roomList[client.roomName].NowTurn].emit("myTurnNow");
                            break;
                    }
                }
            }
        }
    });

    client.on('createRoom', (packet) => {
        var data = packet.roomName;
        var password = packet.password;
        if(data.length < 1 | data.length > 12) {
            alertMessage("방 이름은 1글자 이상 12글자 이하여야 합니다.");
            return;
        }
        
        data = splitTags(data);

        if(client.roomName != null) {
            if(roomList[client.roomName].admin == client.playerName && (roomList[data] == null || data == client.roomName)) {
                io.sockets.emit('changeRoom', { 'oldName': client.roomName, 'newName': data, 'password' : (password != "") });
                if(roomList[data] == null) {
                    roomList[data] = { 'DuringGame': false, 'NowTurn': 0, 'gameBoard': roomList[client.roomName].gameBoard, 'roomName': data, 'admin': client.playerName, 'password': password, 'len': roomList[client.roomName].len, 'clients': roomList[client.roomName].clients };
                    delete roomList[client.roomName];
                    
                    roomList[data].clients.forEach(x => { x.join(data); });
                    roomList[data].clients.forEach(x => { x.leave(client.roomName); });
                    roomList[data].clients.forEach(x => { x.roomName = data; });
                    io.sockets.in(client.roomName).emit("roomDataChanged", client.roomName );
                }else {
                    roomList[data].password = password;
                }
                client.emit("roomAdmin", {"roomName": client.roomName, "password": password});
                client.ready = true;

                return;
            }else {
                if(roomList[data] == null) {
                    io.sockets.in(client.roomName).emit('leavePlayer', client.playerName);
                    client.leave(client.roomName);
                    client.emit("leaveRoom");
                    io.sockets.emit('leaveRoomPlayer', client.roomName);
                    roomList[client.roomName].len--;
                    roomList[client.roomName].clients.splice(roomList[client.roomName].clients.indexOf(client), 1);

                    if(roomList[client.roomName].len <= 0) {
                        delete roomList[client.roomName];
                        io.sockets.emit('deleteRoom', client.roomName);
                    }
                }
            }
        }
        
        if(roomList[data] == null) {
            io.sockets.emit('createRoom', { 'roomName': data, 'admin': client.playerName, 'password': (password != "") });
            roomList[data] = { 'DuringGame': false, 'NowTurn': 0, 'gameBoard': createBoard(), 'roomName': data, 'admin': client.playerName, 'password': password, 'len': 1, 'clients': new Array() };
            
            client.roomName = data;
            
            client.leave("connection_main_players_room");

            client.join(client.roomName);
            client.emit("joinRoom", client.roomName);
            client.ready = false;
            client.emit('myReadyStatusChange', client.ready);
            io.sockets.emit('joinRoomPlayer', client.roomName);
            io.sockets.in(data).emit("joinPlayer", client.playerName);
            client.emit("roomAdmin", {"roomName": client.roomName, "password": password});
            client.ready = true;
            roomList[client.roomName].clients.push(client);
        }else
            alertMessage("이미 존재하는 방 이름 입니다.");
    });

    client.on('leaveRoom', () => {
        if(client.roomName != null) {
            if(roomList[client.roomName].DuringGame) {
                io.sockets.in(client.roomName).emit("gameEnd");
                
                roomList[client.roomName].clients.forEach(x => {
                    if(x.playerName != client.playerName)
                        alertMessage("플레이어가 중도 퇴장하여 게임이 종료되었습니다.", x);
                });

                roomList[client.roomName].NowTurn = 0;
                roomList[client.roomName].DuringGame = false;
            }
            client.join("connection_main_players_room");

            io.sockets.in(client.roomName).emit('leavePlayer', client.playerName);
            client.leave(client.roomName);
            client.emit("leaveRoom");
            io.sockets.emit('leaveRoomPlayer', client.roomName);
            roomList[client.roomName].clients.splice(roomList[client.roomName].clients.indexOf(client), 1);

            roomList[client.roomName].len--;
            if(roomList[client.roomName].len <= 0) {
                delete roomList[client.roomName];
                io.sockets.emit('deleteRoom', client.roomName);
            }else if(roomList[client.roomName].admin == client.playerName) {
                var cl = roomList[client.roomName].clients[0];
                cl.emit("roomAdmin", {"roomName": client.roomName, "password": roomList[client.roomName].password});
                cl.ready = true;
                roomList[client.roomName].admin = cl.playerName;
            }

            client.roomName = null;
        }
    });

    client.on('joinRoom', (packet) => {
        if(roomList[packet.roomName].password != "" && roomList[packet.roomName].password != packet.password && client.roomName != packet.roomName) {
            client.emit('roomPasswdPls', packet.roomName);
        }else if(client.roomName != packet.roomName) {
            if(roomList[packet.roomName].len >= 2) {
                alertMessage("해당 방의 인원이 가득 찼습니다.");
                return;
            }
            client.leave("connection_main_players_room");

            if(client.roomName != null) {
                io.sockets.in(client.roomName).emit('leavePlayer', client.playerName);
                client.leave(client.roomName);
                client.emit("leaveRoom");
                io.sockets.emit('leaveRoomPlayer', client.roomName);
                roomList[client.roomName].clients.splice(roomList[client.roomName].clients.indexOf(client), 1);

                roomList[client.roomName].len--;
                if(roomList[client.roomName].len <= 0) {
                    delete roomList[client.roomName];
                    io.sockets.emit('deleteRoom', client.roomName);
                }else if(roomList[client.roomName].admin == client.playerName) {
                    var cl = io.sockets.in(client.roomName).sockets[0];
                    cl.emit("roomAdmin", {"roomName": client.roomName, "password": roomList[client.roomName].password});
                    cl.ready = true;
                    roomList[client.roomName].admin = cl.playerName;
                }
            }

            client.roomName = packet.roomName;

            client.join(client.roomName);
            client.emit("joinRoom", client.roomName);
            client.ready = false;
            client.emit('myReadyStatusChange', client.ready);
            roomList[client.roomName].clients.forEach(x => { client.emit("roomPlayerList", { 'playerName': x.playerName, 'ready': x.ready }); });
            roomList[client.roomName].len++;
            io.sockets.emit('joinRoomPlayer', client.roomName);
            io.sockets.in(client.roomName).emit("joinPlayer", client.playerName);
            roomList[client.roomName].clients.push(client);
        }
    });

    client.on('nameChange', (data) => {
        if(data.length < 2 | data.length > 12)
            alertMessage("이름은 2글자 이상 12글자 이하여야 합니다.");
        else {

            data = splitTags(data);

            if(playerList.includes(data)) {
                alertMessage("이미 존재하는 이름입니다.");
            }else {

                if(client.roomName != null) {
                    if(roomList[client.roomName].admin == client.playerName)
                        roomList[client.roomName].admin = data;
                }
                
                playerList[playerList.indexOf(client.playerName)] = data;
                io.sockets.emit('nickChange', { 'oldName': client.playerName, 'newName': data });
                client.playerName = data;
                client.emit('nickName', data);
            }
        }
    });

    client.on('disconnect', () => {
        //접속해제
        //방 존재시 끉기 및 quit 실행
        if(client.roomName != null) {
            if(roomList[client.roomName].DuringGame) {
                io.sockets.in(client.roomName).emit("gameEnd");
                alertMessage("플레이어가 중도 퇴장하여 게임이 종료되었습니다.", io.sockets.in(client.roomName));
                roomList[client.roomName].NowTurn = 0;
                roomList[client.roomName].DuringGame = false;
            }
            io.sockets.emit('leaveRoomPlayer', client.roomName);
            io.sockets.in(client.roomName).emit('leavePlayer', client.playerName);
            client.leave(client.roomName);
            roomList[client.roomName].clients.splice(roomList[client.roomName].clients.indexOf(client), 1);

            roomList[client.roomName].len--;
            if(roomList[client.roomName].len <= 0) {
                delete roomList[client.roomName];
                io.sockets.emit('deleteRoom', client.roomName);
            }else if(roomList[client.roomName].admin == client.playerName) {
                var cl = roomList[client.roomName].clients[0];
                cl.emit("roomAdmin", {"roomName": client.roomName, "password": roomList[client.roomName].password});
                cl.ready = true;
                roomList[client.roomName].admin = cl.playerName;
            }
        }

        io.sockets.emit('quit', client.playerName);
        playerList.splice(playerList.indexOf(client.playerName), 1);
    });

    

    var alertMessage = (msg, cl = client) => {
        cl.emit('alertMessage', msg);
    };
    

    var splitTags = (data) => {
        data = data.toString();
        data.replace(/&/gi, "&#38;");
        data.replace(/#/gi, "&#35;");
        data.replace(/&&#3538;/gi, "&#38;");
        data.replace(/</gi, "&lt;");
        data.replace(/>/, "&gt;");
        // data.replace(/\(/gi, "&#40;");
        // data.replace(/\)/gi, "&#41;");
        data.replace(" ", "nbsp;");
        data.replace("=", "&#61;");

        return data;
    };
});

var createBoard = () => {
    var board = new Array();
    for(var y = 0; y < 15; y++) {
        board[y] = new Array();
        for(var x = 0; x < 15; x++) {
            board[y][x] = 0;
        }
    }

    return board;
}

var TurnChange = (turn) => {
    if(turn == 0)
        turn = 1;
    else
        turn = 0;
    
    return turn;
}


var gameClearCheck = ((board, x, y) => {
    { //오른위에서 내려오는 대각선.
        var check = 1;

        var _y = y;
        var _x = x;
        for(var i = 0; i < 5; i++) {
            _y -= 1;
            _x += 1;
            if(_y >= 15 || _y < 0 || _x >= 15 || _x < 0) break;
            if(board[_y][_x] != 0 && board[_y][_x] == board[y][x]) {
                check++;
            }else break;
        }

        _y = y;
        _x = x;
        for(var i = 0; i < 5; i++) {
            _y += 1;
            _x -= 1;
            if(_y >= 15 || _y < 0 || _x >= 15 || _x < 0) break;
            if(board[_y][_x] != 0 && board[_y][_x] == board[y][x]) {
                check++;
            }else break;
        }

        if(check >= 5) return 1;
    }
    
    { //왼쪽위에서 내려오는 대각선
        var check = 1;

        var _y = y;
        var _x = x;
        for(var i = 0; i < 5; i++) {
            _y -= 1;
            _x -= 1;
            if(_y >= 15 || _y < 0 || _x >= 15 || _x < 0) break;
            if(board[_y][_x] != 0 && board[_y][_x] == board[y][x]) {
                check++;
            }else break;
        }

        _y = y;
        _x = x;
        for(var i = 0; i < 5; i++) {
            _y += 1;
            _x += 1;
            if(_y >= 15 || _y < 0 || _x >= 15 || _x < 0) break;
            if(board[_y][_x] != 0 && board[_y][_x] == board[y][x]) {
                check++;
            }else break;
        }

        if(check >= 5) return 1;
    }
    
    { //수직방향
        var check = 1;

        var _y = y;
        for(var i = 0; i < 5; i++) {
            _y -= 1;
            if(_y >= 15 || _y < 0) break;
            if(board[_y][x] != 0 && board[_y][x] == board[y][x]) {
                check++;
            }else break;
        }

        _y = y;
        for(var i = 0; i < 5; i++) {
            _y += 1;
            if(_y >= 15 || _y < 0) break;
            if(board[_y][x] != 0 && board[_y][x] == board[y][x]) {
                check++;
            }else break;
        }

        if(check >= 5) return 1;
    }
    
    { //수평방향
        var check = 1;

        var _x = x;
        for(var i = 0; i < 5; i++) {
            _x -= 1;
            if(_x >= 15 || _x < 0) break;
            if(board[y][_x] != 0 && board[y][_x] == board[y][x]) {
                check++;
            }else break;
        }

        _x = x;
        for(var i = 0; i < 5; i++) {
            _x += 1;
            if(_x >= 15 || _x < 0) break;
            if(board[y][_x] != 0 && board[y][_x] == board[y][x]) {
                check++;
            }else break;
        }

        if(check >= 5) return 1;
    }

    for(var y = 0; y < 15; y++) {
        for(var x = 0; x < 15; x++) {
            if(board[y][x] == 0)
                return 0;
        }
    }
    
    return 2;
});

setInterval(function() {
    io.emit('wakeUP');
}, 600000);

//https://velog.io/@ju_h2/Node.js-socket.io로-실시간-채팅-구현하기
//https://m.blog.naver.com/tkddlf4209/221833759786