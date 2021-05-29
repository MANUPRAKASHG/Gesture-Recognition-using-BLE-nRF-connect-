#include <ArduinoBLE.h>           // Bluetooth Library
#include <Arduino_LSM9DS1.h>      // Magnetometer Sensor Library

// Initalizing global variables for sensor data to pass onto BLE
String p, t, m, flex_sensor_State;

// BLE Service Name
BLEService customService("180C");

// BLE Characteristics
// Syntax: BLE<DATATYPE>Characteristic <NAME>(<UUID>, <PROPERTIES>, <DATA LENGTH>)
BLEStringCharacteristic ble_flex_sensor_a("2A56", BLERead | BLENotify, 13);
BLEStringCharacteristic ble_flex_sensor_b("2A57", BLERead | BLENotify, 13);
BLEStringCharacteristic ble_flex_sensor_c("2A58", BLERead | BLENotify, 20);
BLEStringCharacteristic ble_flex_sensor_e("2A59", BLERead | BLENotify, 20);

// Function prototype
void readValues();

void setup()
{

    IMU.begin();
    Serial.begin(9600);
    while (!Serial);
    if (!BLE.begin())
    {
        Serial.println("BLE failed to Initiate");
        delay(500);
        while (1);
    }

    // Setting BLE Name
    BLE.setLocalName("Analog Sensors Deta");
    
    // Setting BLE Service Advertisment
    BLE.setAdvertisedService(customService);
    
    // Adding characteristics to BLE Service Advertisment
    customService.addCharacteristic(ble_flex_sensor_a);
    customService.addCharacteristic(ble_flex_sensor_b);
    customService.addCharacteristic(ble_flex_sensor_c);
    customService.addCharacteristic(ble_flex_sensor_e);

    // Adding the service to the BLE stack
    BLE.addService(customService);

    // Start advertising
    BLE.advertise();
    Serial.println("Bluetooth device is now active, waiting for connections...");
}

void loop()
{
    // Variable to check if cetral device is connected
    BLEDevice central = BLE.central();
    if (central)
    {
        Serial.print("Connected to central: ");
        Serial.println(central.address());
        while (central.connected())
        {
            delay(200);
            
            // Read values from sensors
            readValues();

            // Writing sensor values to the characteristic
            ble_flex_sensor_a.writeValue(p);
            ble_flex_sensor_b.writeValue(t);
            ble_flex_sensor_c.writeValue(m);
             
            

            // Displaying the sensor values on the Serial Monitor
            Serial.println("Reading Sensors");
            Serial.println(p);
            Serial.println(t);
            Serial.println(m);
            Serial.println("\n");
            delay(1000);
        }
    }
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
}

void readValues()
{
    // Reading raw sensor values from three sensors
    float x, y, z;
    float flex_sensor_a = analogRead(A0)/950;
    float flex_sensor_b = analogRead(A1)/950;
    float flex_sensor_c = analogRead(A2)/950;
    //String ;

    // Saving sensor values into a user presentable way with units
    p = String(flex_sensor_a);
    t = String(flex_sensor_b);
    m = String(flex_sensor_c);

    if((flex_sensor_a==1) && (flex_sensor_b==0) && (flex_sensor_c==0) ){
     ble_flex_sensor_e.writeValue("Need Water");
    }
    
   if((flex_sensor_a==0) && (flex_sensor_b==1) && (flex_sensor_c==0) ){
     ble_flex_sensor_e.writeValue("Call to Doctor");
    }
    
    if((flex_sensor_a==0) && (flex_sensor_b==0) && (flex_sensor_c==1) ){
     ble_flex_sensor_e.writeValue("I am Hungry");
    }

    if((flex_sensor_a==1) && (flex_sensor_b==1) && (flex_sensor_c==0) ){
     ble_flex_sensor_e.writeValue("Need Water");
    }
    
    if((flex_sensor_a==1) && (flex_sensor_b==0) && (flex_sensor_c==1) ){
     ble_flex_sensor_e.writeValue("Thak you");
    }
    
     if((flex_sensor_a==0) && (flex_sensor_b==1) && (flex_sensor_c==1) ){
     ble_flex_sensor_e.writeValue("Sorry");
    }
    
    if((flex_sensor_a==0) && (flex_sensor_b==0) && (flex_sensor_c==0) ){
     ble_flex_sensor_e.writeValue("Welcome");
    }
    
    if((flex_sensor_a==1) && (flex_sensor_b==1) && (flex_sensor_c==1) ){
     ble_flex_sensor_e.writeValue("Be Happy");
    } 
      
  }
