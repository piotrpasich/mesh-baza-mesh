#include "painlessMesh.h"
#include <ArduinoJson.h>

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

void sendMessage() {}; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  digitalWrite(BUILTIN_LED, HIGH);
  Serial.println(msg.c_str());
  digitalWrite(BUILTIN_LED, LOW);
}

void setup() {
  Serial.begin(115200);

  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  digitalWrite(BUILTIN_LED, HIGH);

  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
  delay(2000);
  Serial.println("HELLO");
  digitalWrite(BUILTIN_LED, LOW);
}

String message = "";

void loop() {
  while (Serial.available())
  {
    digitalWrite(BUILTIN_LED, HIGH);
    message = Serial.readString();
    int found = message.indexOf('}');
    if (found!=-1) {
      mesh.sendBroadcast( message.substring(0, found+1) );  
    }
    digitalWrite(BUILTIN_LED, LOW);
  }

  mesh.update();
}
