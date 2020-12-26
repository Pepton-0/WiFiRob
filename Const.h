#pragma once
class Const
{
public:
    const char* ssid = "AG45-A24";
    const char* password = "ZY53357542011";
    const std::string index PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html id="TouchArea">
  <head>
    <meta charset="UTF-8" />
    <meta
      name="viewport"
      content="width=device-width,initial-scale=1.0,minimum-scale=1.0,maximum-scale=1.0,user-scalable=no"/>
    <title>Controller</title>
    <style>
      html {
        background-color: rgb(162, 196, 230);
        width: 100%;
        height: 100%;
        position: relative;
    }
    #sample1,
    #sample2 {
      position: absolute;
      top: 0;
      left: 0;
    }
    #Text {
      text-align: center;
      font-family: "Avenir";
      user-select: none;
    }
    </style>
    <link rel="icon" href="data:,">
  </head>
  <body>
    <canvas id="sample1"></canvas>
    <canvas id="sample2"></canvas>
    <h1 id="Text">CONNECTED</h1>
    <script>
      var gateway = `ws://${window.location.hostname}/ws`;
      var websocket;
      el_TouchArea = document.getElementById("TouchArea");
      el_sample1 = document.getElementById("sample1");
      el_sample2 = document.getElementById("sample2");
      el_Text = document.getElementById("Text");
      el_sample1.width = el_TouchArea.offsetWidth;
      el_sample1.height = el_TouchArea.offsetHeight;
      el_sample2.width = el_TouchArea.offsetWidth;
      el_sample2.height = el_TouchArea.offsetHeight;
      const canvas1 = el_sample1.getContext("2d");
      const canvas2 = el_sample2.getContext("2d");
      window.addEventListener('load', onLoad);
      function initWebSocket() {
        websocket = new WebSocket(gateway);
        websocket.onclose   = onClose;
      }
      function onClose(event) {
        console.log('Connection closed');
        setTimeout(initWebSocket, 2000);
      }
      function onLoad(event) {
        initWebSocket();
        drawstart();
      }
      function drawstart(){
        el_TouchArea.addEventListener("touchstart", draw);
        el_TouchArea.addEventListener("touchmove", redraw, { passive: false });
        el_TouchArea.addEventListener("touchend", clear);
      }
      function draw(e) {
        prePosX = e.changedTouches[0].clientX;
        prePosY = e.changedTouches[0].clientY;
        canvas1.beginPath();
        canvas1.arc(prePosX, prePosY, 40, 0, Math.PI * 2, false);
        canvas1.fillStyle = "rgba(100, 100, 100,0.8)";
        canvas1.fill();
        canvas2.beginPath();
        canvas2.arc(prePosX, prePosY, 20, 0, Math.PI * 2, false);
        canvas2.fillStyle = "rgba(255, 255, 255, 0.75)";
        canvas2.fill();
      }
      function redraw(e) {
        e.preventDefault();
        coordX = e.changedTouches[0].clientX - prePosX;
        coordY = -(e.changedTouches[0].clientY - prePosY);
        if (coordX * coordX + coordY * coordY < 1600) {
          posX = e.changedTouches[0].clientX;
          posY = e.changedTouches[0].clientY;
          el_Text.innerHTML = "STOP";
        } else {
          direction();
          posX = prePosX + (40 * coordX) / Math.sqrt(coordX * coordX + coordY * coordY);
          posY = prePosY - (40 * coordY) / Math.sqrt(coordX * coordX + coordY * coordY);
        }
        canvas2.clearRect(0, 0, el_sample2.width, el_sample2.height);
        canvas2.beginPath();
        canvas2.arc(posX, posY, 20, 0, Math.PI * 2, false);
        canvas2.fillStyle = "rgba(255, 255, 255, 0.75)";
        canvas2.fill();
      }
      function direction() {
        if(coordY > coordX && coordY > - coordX){
          el_Text.innerHTML = "FORWARD";
          websocket.send('FORWARD');
        }else if (coordY < coordX && coordY > - coordX){
          el_Text.innerHTML = "TURN</br>RIGHT";
          websocket.send('RIGHT');
        }else if(coordY < - coordX && coordY < coordX){
          el_Text.innerHTML = "BACKWARD";
          websocket.send('BACKWARD');
        }else if(coordY > coordX && coordY < - coordX){
          el_Text.innerHTML = "TURN</br>LEFT";
          websocket.send('TL');
        }
      }
      function clear() {
        canvas1.clearRect(0, 0, el_sample1.width, el_sample1.height);
        canvas2.clearRect(0, 0, el_sample2.width, el_sample2.height);
        el_Text.innerHTML = "STOP";
        websocket.send('STOP');
      }
    </script>
  </body>
</html>
)rawliteral";
};