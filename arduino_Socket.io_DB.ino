#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ArduinoJson.h>

#include <WebSocketsClient.h>
#include <SocketIOclient.h>

#include <Hash.h>

ESP8266WiFiMulti WiFiMulti;
SocketIOclient socketIO;

// 수정했음
#define USE_SERIAL Serial

void socketIOEvent(socketIOmessageType_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case sIOtype_DISCONNECT:
      USE_SERIAL.printf("[IOc] Disconnected!\n");
      break;
    case sIOtype_CONNECT:
      USE_SERIAL.printf("[IOc] Connected to url: %s\n", payload);

      // join default namespace (no auto join in Socket.IO V3)
      socketIO.send(sIOtype_CONNECT, "/");
      break;
    case sIOtype_EVENT:
      {
        //        USE_SERIAL.printf("[IOc] get event: %s\n", payload);
        //사물인터넷보드가 메세지를 수신하는 지점!
        String msg = (char*)payload;
        if (msg == "[\"request\",9999]") {
          int temp = random(0,40);
//          USE_SERIAL.println("request is complete");
          // 사물인터넷보드가 데이터를 보내는 로직
          DynamicJsonDocument doc(1024);
          JsonArray array = doc.to<JsonArray>();
          array.add("request");
          array.add(temp);
          String output;
          serializeJson(doc, output);
          socketIO.sendEVENT(output);
          //          USE_SERIAL.println(output);
          //      } else if (msg.indexOf('2') != -1) {    // 사물인터넷보드의 설정을 바꿔줄 로직 ex.super mode, power mode, economy mode
        } else {
          USE_SERIAL.println(msg + 'is wrong');
        }
        break;
      }
    case sIOtype_ACK:
      USE_SERIAL.printf("[IOc] get ack: %u\n", length);
      hexdump(payload, length);
      break;
    case sIOtype_ERROR:
      USE_SERIAL.printf("[IOc] get error: %u\n", length);
      hexdump(payload, length);
      break;
    case sIOtype_BINARY_EVENT:
      USE_SERIAL.printf("[IOc] get binary: %u\n", length);
      hexdump(payload, length);
      break;
    case sIOtype_BINARY_ACK:
      USE_SERIAL.printf("[IOc] get binary ack: %u\n", length);
      hexdump(payload, length);
      break;
  }
}

void setup() {
  pinMode(D1, OUTPUT);
  // 시리얼 모니터 사용 시 115200 사용
  USE_SERIAL.begin(115200);

  //Serial.setDebugOutput(true);
  USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  // disable AP
  if (WiFi.getMode() & WIFI_AP) {
    WiFi.softAPdisconnect(true);
  }

  WiFiMulti.addAP("KT_GiGA_9655", "cfb13dc124");

  //WiFi.disconnect();
  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
  }

  String ip = WiFi.localIP().toString();
  USE_SERIAL.printf("[SETUP] WiFi Connected %s\n", ip.c_str());

  // server address, port and URL
  // 서버의 ip,port,그대로
  socketIO.begin("172.30.1.44", 3000, "/socket.io/?EIO=4");

  // event handler
  socketIO.onEvent(socketIOEvent);
}
//digitalWrite(D1, HIGH);
//digitalWrite(D1, LOW);

void loop() {
  socketIO.loop();
  if (Serial.available()) {
      char c = Serial.read();
      DynamicJsonDocument doc(1024);
      JsonArray array = doc.to<JsonArray>();
  
      if (c == '0') {
        array.add("join");
        array.add("Arduino!");
      } else if (c == '1') {
        array.add("chat message");
        array.add("disconnect");
      } else if (c == '2') {
        array.add("disconnect");
        array.add("Arduino!");
      }
      //만들어 놓은 구조를 바탕으로 JSON Seriallize(직렬화)
      String output;
      serializeJson(doc, output);
  
      //시리얼모니터에 테스트 메세지 출력(전송한 메세지 출력)
      USE_SERIAL.println(output);
  
      //서버로 전송
      socketIO.sendEVENT(output);
    }
  // 아두이노 시리얼에서 입력 후 테스트 할 부분
  //  if (Serial.available()) {
  //    char c = Serial.read();
  //    DynamicJsonDocument doc(1024);
  //    JsonArray array = doc.to<JsonArray>();
  //
  //    if (c == '0') {
  //      // add evnet name
  //      // Hint: socket.on('event_name', ....
  //      array.add("chat message");
  //      array.add("ESP8266BOARD");
  //    } else if (c == '1') {
  //      array.add("chat message");
  //      array.add("disconnect");
  //    } else if (c == '2') {
  //      array.add("chat message");
  //      array.add("잘 전송 되나?");
  //    }
  //    //만들어 놓은 구조를 바탕으로 JSON Seriallize(직렬화)
  //    String output;
  //    serializeJson(doc, output);
  //
  //    //시리얼모니터에 테스트 메세지 출력(전송한 메세지 출력)
  //    USE_SERIAL.println(output);
  //
  //    //서버로 전송
  //    socketIO.sendEVENT(output);
  //  }
}
