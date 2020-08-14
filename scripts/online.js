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

var chatPacketSend = (data) => {
    socket.emit('message', data);
}

var chattingMessageGet = (sender, message) => {
    var chatItem = document.createElement("div");
    chatItem.className = "chat_list_item"

    var name = document.createElement("span");
    name.innerHTML = sender;
    name.className = "chat_sender";

    var msg = document.createElement("span");
    msg.innerHTML = message;
    msg.className = "chat_message";

    var time = document.createElement("span");
    time.innerHTML = new Date().format("hh:mm:ss a/p");
    time.className = "chat_time"

    chatItem.appendChild(name);
    chatItem.appendChild(msg);
    chatItem.appendChild(time);

    
    var chatBox = document.getElementsByClassName('chattingBox')[0];
    chatBox.appendChild(chatItem);
    chatBox.scrollTop = chatBox.scrollHeight;
}

Date.prototype.format = function(f) {
    if(!this.valueOf()) return "";

    var d = this;

    return f.replace(/(yyyy|yy|MM|dd|hh|mm|ss|a\/p)/gi, function($1) {
        switch($1) {
            case "yyyy": return d.getFullYear();
            case "yy": return (d.getFullYear() % 1000).zf(2);
            case "MM": return (d.getMonth() + 1).zf(2);
            case "dd": return d.getDate().zf(2);
            case "HH": return d.getHours().zf(2);
            case "hh": return ((h=d.getHours() % 12) ? h:12).zf(2) * 1;
            case "mm": return d.getMinutes().zf(2);
            case "ss": return d.getSeconds().zf(2);
            case "a/p": return d.getHours() < 12 ? "AM":"PM";
            default: return $1;
        }
    });
};

String.prototype.string = function(len) { var s = '', i = 0; while(i++ < len) { s += this; } return s; };
String.prototype.zf = function(len) { return "0".string(len - this.length) + this; };
Number.prototype.zf = function(len) { return this.toString().zf(len); };