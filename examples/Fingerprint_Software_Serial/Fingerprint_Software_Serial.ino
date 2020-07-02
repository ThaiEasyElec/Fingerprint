#include "Fingerprint.h"
#include "SoftwareSerial.h"
#define RST 4

SoftwareSerial ser(8,9);

FINGERPRINT fingerprint;

void setup() {
  Serial.begin(115200);
  ser.begin(19200);
  fingerprint.init(ser);

  Serial.println("Fingerprint Test");
  pinMode(RST, OUTPUT);
  digitalWrite(RST, LOW);
  delay(1000);
  digitalWrite(RST, HIGH);
  delay(1000);

  while(fingerprint.countAllUser() != 3000)
  {
    Serial.println("PLESE CHECK SENSORS AND CONNECTION");
  }
  Serial.println("");
  
  Serial.println("1 countAllUser");
  Serial.println("2 countUser");
  Serial.println("3 QueryModelSN");
  Serial.println("4 SetModelSN");
  Serial.println("5 DeleteUser");
  Serial.println("6 DeleteAllUser");
  Serial.println("7 AddFingerPrint");
  Serial.println("8 Compare");
}

void loop() {
  while (Serial.available()) {
    char ch = Serial.read();
    if (ch == '1')
    {
      Serial.print("AllUser = ");
      Serial.print(fingerprint.countAllUser());
      Serial.println(" account");
    }
    else if (ch == '2')
    {
      Serial.print("use = ");
      Serial.print(fingerprint.countUser());
      Serial.println(" account");
    }
    else if (ch == '3')
    {
      Serial.print("SerialNumber of Module (HEX) = ");
      Serial.println(fingerprint.QueryModelSN(), HEX);
    }
    else if (ch == '4')
    {
      Serial.print("ModifySNnumber(HEX) 0x01, 0x02, 0x03 ");
      Serial.println(fingerprint.ModifySNnumber(0x01, 0x02, 0x03), HEX);
    }
    else if (ch == '5')
    {
      Serial.println("Delete ID: 3000");
      if (fingerprint.DeleteUser(3000) == ACK_SUCCESS)
      {
        Serial.println("Delete Success");
      }
      else
      {
        Serial.println("Delete False");
      }
    }
    else if (ch == '6')
    {
      Serial.println("Delete All User");
      if (fingerprint.DeleteAllUser() == ACK_SUCCESS)
      {
        Serial.println("Delete All Success");
      }
      else
      {
        Serial.println("Delete All False");
      }
    }
    else if (ch == '7')
    {
      Serial.println("AddFingerprint ID 3000");
      if (fingerprint.AddFingerprint(3000, 1) == ACK_SUCCESS)
      {
        Serial.println("AddFingerprint Success");
      }
      else
      {
        Serial.println("AddFingerprint False");
      }
    }
    else if (ch == '8')
    {
      Serial.println("Compare Please Insert Finger");
      while (1)
      {
        if (fingerprint.Compare() > 0)
        {
          Serial.print("Found ID: ");
          Serial.println(fingerprint.Compare());
        }
      }
    }
  }
}
