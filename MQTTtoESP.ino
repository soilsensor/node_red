#include <WiFi.h> //ใช้ในการเชื่อมต่อ WiFi
#include <PubSubClient.h> //ใช้ในการสื่อสารกับโปโตคอล mqtt

#define ssid "............" //ssid หรือชื่อของ WiFi ที่ต้องการเชื่อมต่อ
#define password "kkkkoooo" //password หรือรหัสของ WiFi ที่ต้องการเชื่อมต่อ

const char* mqtt_server = "io.adafruit.com"; //ชื่อ Broker ที่ต้องการเชื่อมต่อ
const int mqtt_Port = 1883; //port ของโปโตคอล mqtt

WiFiClient espClient; //ทำการสร้าง Client ของ WiFi
PubSubClient client(espClient); //ทำการสร้าง Client ของ mqtt 

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);//ทำการเชื่อมต่อกับ WiFi
  while (WiFi.status() != WL_CONNECTED) //ทำการตรวจสอบ WiFi หากไม่สามารถเชื่อมต่อ WiFi ได้จะทำการวนลูป
  { delay(1000); 
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  setupMQTT();
}

//สร้างฟังก์ชันในการเชื่อมต่อ broker และทำการเลียกใช้ฟังก์ชั่น callback 
void setupMQTT() {
  client.setServer(mqtt_server, mqtt_Port); //ทำการเชื่อมต่อกับ broker
  client.setCallback(callback); //เรียกใช้ฟังชั่น callback โดยที่ในวงเล็บจำเป็นต้องเป็นชื่อเดียวกันกับฟังชั้นที่จะใช้ในการ Callback ในบรรทัดที่
}

//สร้างฟังก์ชั่นในการตรวจสอบ WiFi  
void reconnectWiFi() {
  while (WiFi.status() != WL_CONNECTED) //ทำการตรวจสอบ WiFi หากไม่สามารถเชื่อมต่อ WiFi ได้จะทำการวนลูป
  { delay(1000);
    Serial.println("Connecting to WiFi...");
  }
}

//สร้างฟังก์ชั่น callback ในการรับข้อมูลที่ส่งกลับมาจาก broker
//ฟังก์ชั่น callback จะมีข้อมูลที่ถูกส่งกลับมาอยู่ด้วยการสามข้อมูลได้แก่ topic คือชื่อของข้อมูลที่ส่งกลับมา payload คือข้อมูลที่ถูกส่งกลับมา length คือจำนวนตัวอักษรของข้อมูลที่ถูกส่งกลับมา
void callback(char* topic, byte* payload, unsigned int length) 
 {
  String message; //สร้างตัวแปรเพื่อเก็บข้อมูล payload
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] = ");
  for (int i = 0; i < length; i++) { //สร้างลูป For เพื่อทำการทำการเก็บข้อมูล paylode ลงในตัวแปร massage       
    message += (char)payload[i];
  }
  Serial.println(message);
  }

 //สร้างฟังก์ชัน ในการตรวจสองการเชื่อมต่อกับโปโตคอล mqtt
  void reconnectMQTT() {
    //สร้างเงื่อนไขเมื่อเชื่อมต่อ mqtt ไม่ได้
  while (!client.connected()) {
    Serial.print("connect mqtt....");
    Serial.println(".");
    if (client.connect("Npru_Test_mqttIn",Kiattisak_naria,aio_QUwV28TwhMyxea3b4sEEQFPLk5yG)) //ทำการเชื่อมต่อ mqtt โดยที่ในวงเล็บคือชื่อที่สามารถตั้งได้ตามต้องการโดยที่ชื่อที่ตั้งไม่ควรซ้ำกันในจำนวนมาก
    { Serial.println("connect");
      client.subscribe("TestData1"); //ทำการติดตามข้อมูลหรือ subscribe โดยในวงเล็บต้องเป็นชื่อเดียวกันกับ topic ของ mqtt out ที่ต้องการรับข้อมูลกลับมา
      client.subscribe("TestData2");
      }
      else {
      Serial.print("MQTT connection failed, rc=");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

void loop() {
  reconnectWiFi(); //
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop(); //เมื่อเรียกใช้ฟังก์ชั่น callback จำเป็นต้องมีฟังก์ชั่น client.loop
}
