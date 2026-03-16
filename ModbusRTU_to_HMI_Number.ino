#define TX_PIN 17
#define RX_PIN 16
#define DE_RE 4

HardwareSerial RS485(2);

uint16_t crc16(uint8_t *buf, int len)
{
  uint16_t crc = 0xFFFF;

  for (int pos = 0; pos < len; pos++) {
    crc ^= (uint16_t)buf[pos];

    for (int i = 0; i < 8; i++) {
      if (crc & 0x0001) {
        crc >>= 1;
        crc ^= 0xA001;
      }
      else
        crc >>= 1;
    }
  }
  return crc;
}

void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(50);

  pinMode(DE_RE, OUTPUT);
  digitalWrite(DE_RE, LOW);

  RS485.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);

  Serial.println("Enter float value for HMI:");
}

void write_LW0(uint16_t value)
{
  uint8_t frame[8];

  frame[0] = 0x01;   // slave ID
  frame[1] = 0x06;   // write register
  frame[2] = 0x00;
  frame[3] = 0x00;   // LW0 = address 1
  frame[4] = value >> 8;
  frame[5] = value & 0xFF;

  uint16_t crc = crc16(frame,6);

  frame[6] = crc & 0xFF;
  frame[7] = crc >> 8;

  Serial.print("Sending: ");

  for(int i=0;i<8;i++)
  {
    Serial.print("0x");
    if(frame[i]<16) Serial.print("0");
    Serial.print(frame[i],HEX);
    Serial.print(" ");
  }

  Serial.println();

  digitalWrite(DE_RE, HIGH);

  RS485.write(frame,8);
  RS485.flush();

  digitalWrite(DE_RE, LOW);
}

void loop()
{
  if (Serial.available())
  {
    String inputString = Serial.readStringUntil('\n');   // read full line
    inputString.trim();                                  // remove spaces

    float input = inputString.toFloat();

    Serial.print("Float entered: ");
    Serial.println(input);

    uint16_t scaledValue = (uint16_t)round(input * 10.0);

    Serial.print("Scaled value sent to HMI: ");
    Serial.println(scaledValue);

    write_LW0(scaledValue);

    delay(100);

    Serial.print("Response: ");

    unsigned long start = millis();

    while (millis() - start < 200)
    {
      if (RS485.available())
      {
        uint8_t b = RS485.read();
        Serial.print("0x");
        if (b < 16) Serial.print("0");
        Serial.print(b, HEX);
        Serial.print(" ");
      }
    }

    Serial.println();
  }
}
