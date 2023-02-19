let gateway = `ws://${window.location.hostname}/ws`;
window.addEventListener("load", onLoad);
let websocket, rcv, name;

function initWebSocket() {
    console.log("Trying to open a WebSocket connection...");
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
}

function onOpen(event) {
    console.log("Connection opened");
    sndFirstReq();
    document.getElementsByClassName("time")[0].innerHTML =
        document.getElementsByClassName("time")[1].innerHTML =
            new Date().getHours() +
            ":" +
            (new Date().getMinutes() < 10 ? "0" : "") +
            new Date().getMinutes();
}

function onClose(event) {
    console.log("Connection closed");
    setTimeout(initWebSocket, 2000);
}

function scrollLast() {
    let objDiv = document.getElementById("conv");
    objDiv.scrollTop = objDiv.scrollHeight;
}

function sndFirstReq() {
    name = prompt("Enter Your Name (Visible to others)");
    let time =
        new Date().getHours() +
        ":" +
        (new Date().getMinutes() < 10 ? "0" : "") +
        new Date().getMinutes();
    const info = {
        name,
        time,
    };
    console.log(info);
    websocket.send(JSON.stringify(info));
}

function onMessage(event) {
    rcv = JSON.parse(event.data);
    console.log(rcv);
    if (rcv.name && rcv.name != name && rcv.msg && rcv.time) {
        if (rcv.time == "NA")
            rcv.time =
                new Date().getHours() +
                ":" +
                (new Date().getMinutes() < 10 ? "0" : "") +
                new Date().getMinutes();
        else {
            document.getElementById(
                "conv"
            ).innerHTML += `<div class='text left'>
                <div class='name'>${rcv.name}</div>
                <div class='txt'>${rcv.msg}</div>
                <div class='time'>${rcv.time}</div>
                </div>`;
        }
    } else if (rcv.name && rcv.name != name && !rcv.msg && rcv.time) {
        document.getElementById("conv").innerHTML +=
            "<div class='join'>" +
            rcv.name +
            " joined the chat at " +
            rcv.time +
            "</div>";
    } else if (!rcv.name && rcv.msg && !rcv.time) {
        document.getElementById(
            "conv"
        ).innerHTML += `<div class='join'>${rcv.msg} are(is) in the chat!!</div>`;
    } else if (rcv.name && rcv.name != name && !rcv.msg) {
        let time =
            new Date().getHours() +
            ":" +
            (new Date().getMinutes() < 10 ? "0" : "") +
            new Date().getMinutes();
        document.getElementById("conv").innerHTML +=
            "<div class='join leave'>" +
            rcv.name +
            " left the chat at " +
            time +
            "</div>";
    }
    setTimeout(scrollLast, 500);
}

function onLoad(event) {
    initWebSocket();
    initButton();
}

function initButton() {
    document.getElementById("in").addEventListener("keypress", (event) => {
        if (event.key == "Enter") {
            document.getElementById("send").click();
        }
    });
}

async function sndMsg() {
    const info = {
        name,
        msg: document.getElementById("in").value,
        time: "",
    };
    if (info.msg) {
        info.time =
            new Date().getHours() +
            ":" +
            (new Date().getMinutes() < 10 ? "0" : "") +
            new Date().getMinutes();

        document.getElementById("in").value = "";
        document.getElementById(
            "conv"
        ).innerHTML += `<div class='text right'>
        <div class='txt'>${info.msg}</div>
        <div class='time'>${info.time}</div>
        </div>`;
        websocket.send(JSON.stringify(info));
    }
    setTimeout(scrollLast, 500);
}
