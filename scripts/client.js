var socket = io.connect("http://localhost:3000");

socket.on('connect', () => {
    console.log("소켓 연결 성공");
});

socket.on('disconnect', () => {
    console.log("소켓 끉어짐");
    socket.close();
});

socket.on('connect_error', () => {
    socket.close();
    if(document.getElementById("singlePlay").style.display == "" | document.getElementById("singlePlay").style.display == "none") {
        alert("서버가 다운되었거나 네트워크에 연결되어 있지 않습니다.");
        singleModeChange();
    }
});


socket.on('message', (data) => {
    chattingMessageGet(data.sender, data.message);
});

socket.on('nickName', (data) => {
    playerNameChange(data);
});

socket.on('nickChange', (data) => {
    playerListNameChange(data.oldName, data.newName);
})

socket.on('join', (data) => {
    addPlayerList(data);
});

socket.on('quit', (data) => {
    removePlayerList(data);
});

socket.on('alertMessage', (data) => {
    alert(data);
})