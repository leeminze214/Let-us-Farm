//
// A simple server implementation showing how to:
//  * serve static messages
//  * read GET and POST parameters
//  * handle missing pages / 404s
//

#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <L298N.h>

AsyncWebServer server(80);
L298N motor_FL(16, 17);
L298N motor_FR(5, 18);
L298N motor_BL(15, 2);
L298N motor_BR(0, 4);

const char *ssid = "LetUsFarm";
const char *password = "12345678";

void stop() {
  motor_FL.stop();
  motor_FR.stop();
  motor_BL.stop();
  motor_BR.stop();
}
void forward() {
  motor_FL.forward();
  motor_FR.forward();
  motor_BL.forward();
  motor_BR.forward();
}
void backward() {
  motor_FL.backward();
  motor_FR.backward();
  motor_BL.backward();
  motor_BR.backward();
}
void turn_left() {
  motor_FL.backward();
  motor_FR.forward();
  motor_BL.backward();
  motor_BR.forward();
}
void turn_right() {
  motor_FL.forward();
  motor_FR.backward();
  motor_BL.forward();
  motor_BR.backward();
}

class CommandRequestHandler : public AsyncWebHandler {
public:
  CommandRequestHandler() {}
  virtual ~CommandRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request) {
    return true;
  }

  void handleRequest(AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("text/plain");
    response->print("OK");
    request->send(response);
  }

  void handleBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
    // char data_str[len];
    // strcpy(data_str, (const char *) data);
    // Serial.println(data_str);

    if (!strncmp((const char *) data, "forward", len)) {
      Serial.println("Sending forward!");
      forward();
      delay(500);
      Serial.println("Sending stop!");
      stop();
    }
    else if (!strncmp((const char *) data, "back", len)) {
      Serial.println("Sending backward!");
      backward();
      delay(500);
      Serial.println("Sending stop!");
      stop();
    }
    else if (!strncmp((const char *) data, "left_t", len)) {
      Serial.println("Got left_t!");
      turn_left();
      delay(500);
      Serial.println("Sending stop!");
      stop();
    }
    else if (!strncmp((const char *) data, "right_t", len)) {
      Serial.println("Got right_t!");
      turn_right();
      delay(500);
      Serial.println("Sending stop!");
      stop();
    }
    else if (!strncmp((const char *) data, "cut", len)) {
      Serial.println("Got cut!");
    }
    else if (!strncmp((const char *) data, "arm_left", len)) {
      Serial.println("Got arm_left!");
    }
    else if (!strncmp((const char *) data, "arm_right", len)) {
      Serial.println("Got arm_right!");
    }
    else if (!strncmp((const char *) data, "arm_down", len)) {
      Serial.println("Got arm_down!");
    }
    else if (!strncmp((const char *) data, "arm_up", len)) {
      Serial.println("Got arm_up!");
    }
  }
};

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  uint8_t status = WiFi.waitForConnectResult();
  if (status != WL_CONNECTED) {
    Serial.printf("WiFi Failed!\n");
    Serial.printf("%d\n", status);
    return;
  }

  IPAddress staticIP(192, 168, 137, 3);
  IPAddress gateway(192, 168, 137, 1);
  IPAddress subnet(255, 255, 255, 0);
  IPAddress primaryDNS(1, 1, 1, 1);
  IPAddress secondaryDNS(1, 0, 0, 1);
  WiFi.config(staticIP, gateway, subnet, primaryDNS, secondaryDNS);

  Serial.print("Current ESP32 IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("Gateway (router) IP: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("Subnet Mask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Primary DNS: ");
  Serial.println(WiFi.dnsIP(0));
  Serial.print("Secondary DNS: ");
  Serial.println(WiFi.dnsIP(1));

  // server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
  //   request->setHandler()
  // });

  // // Send a GET request to <IP>/get?message=<message>
  // server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request) {
  //   String message;
  //   if (request->hasParam(PARAM_MESSAGE)) {
  //     message = request->getParam(PARAM_MESSAGE)->value();
  //   } else {
  //     message = "No message sent";
  //   }
  //   request->send(200, "text/plain", "Hello, GET: " + message);
  // });

  // // Send a POST request to <IP>/post with a form field message set to <message>
  // server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
  //   String message;
  //   if (request->hasParam(PARAM_MESSAGE, true)) {
  //     message = request->getParam(PARAM_MESSAGE, true)->value();
  //   } else {
  //     message = "No message sent";
  //   }
  //   request->send(200, "text/plain", "Hello, POST: " + message);
  // });

  server.onNotFound(notFound);
  server.addHandler(new CommandRequestHandler());

  server.begin();

  motor_BL.setSpeed(255);
  motor_BR.setSpeed(255);
  motor_FL.setSpeed(255);
  motor_FR.setSpeed(255);
  stop();
}

void loop() {}
