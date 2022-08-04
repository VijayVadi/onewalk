/*
  central.ino

  This program uses the ArduinoBLE library to set-up an Arduino Nano 33 BLE
  as a central device and looks for a specified service and characteristic in a 
  peripheral device.
*/

#include <ArduinoBLE.h>
#include <Arduino_APDS9960.h>

#define ECHOPIN 7
#define TRIGPIN 8

const char* deviceServiceUuid = "19b10000-e8f2-537e-4f6c-d104768a1214";
const char* deviceServiceCharacteristicUuid = "19b10001-e8f2-537e-4f6c-d104768a1214";


int distance;
int triggerDistance = 10;
bool connected;

void setup() {
  Serial.begin(9600);
  pinMode(ECHOPIN, INPUT);
  pinMode(TRIGPIN, OUTPUT);

  if (!BLE.begin()) {
    Serial.println("* Starting Bluetooth® Low Energy module failed!");
    while (1);
  }
  
  BLE.setLocalName("Nano 33 BLE (Central)"); 
  BLE.advertise();

  connected = false;
}

void loop() {

  if (!connected) {
    connectToPeripheral();
  }
  
  digitalWrite(TRIGPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(20);
  digitalWrite(TRIGPIN, LOW);
  distance = pulseIn(ECHOPIN, HIGH)/58;
  if (distance > triggerDistance) {
                   
  }
  delay(50);
  
}

void connectToPeripheral(){
  BLEDevice peripheral;
  
  Serial.println("- Discovering peripheral device...");

  do
  {
    BLE.scanForUuid(deviceServiceUuid);
    peripheral = BLE.available();
  } while (!peripheral);
  
  if (peripheral) {
    Serial.println("* Peripheral device found!");
    Serial.print("* Device MAC address: ");
    Serial.println(peripheral.address());
    Serial.print("* Device name: ");
    Serial.println(peripheral.localName());
    Serial.print("* Advertised service UUID: ");
    Serial.println(peripheral.advertisedServiceUuid());
    Serial.println(" ");
    BLE.stopScan();
    controlPeripheral(peripheral);
    connected = false;
  }
}

void controlPeripheral(BLEDevice peripheral) {
  Serial.println("- Connecting to peripheral device...");

  if (peripheral.connect()) {
    Serial.println("* Connected to peripheral device!");
    Serial.println(" ");
    connected = true;
  } else {
    Serial.println("* Connection to peripheral device failed!");
    Serial.println(" ");
    connected = false;
    return;
  }
  
  while (peripheral.connected()) {

    Serial.println("connected (p)");
    delay(2500);
  
  }
  Serial.println("- Peripheral device disconnected!");
  connected = false;
}
