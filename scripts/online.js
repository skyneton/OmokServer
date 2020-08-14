var playerName = "";

var playerNameChange = (_playerName) => {
    playerName = _playerName;
    document.getElementById('myName').innerHTML = playerName;
}

var addPlayerList = (name) => {
    document.getElementsByClassName('playerList')[0].innerHTML += "<p class='playerListItem' id='playerListItem_"+name+"'>"+name+"</p>";
}

var removePlayerList = (name) => {
    var removeItem = document.getElementById("playerListItem_"+name);
    if(removeItem != null)
        document.getElementsByClassName('playerList')[0].removeChild(removeItem);
}

var playerListNameChange = (oldName, newName) => {
    var oldItem = document.getElementById("playerListItem_"+oldName);
    oldItem.id = "playerListItem_"+newName;
    oldItem.innerHTML = newName;
}

var nameChangePacketSend = (newName) => {
    socket.emit('nameChange', newName);
}