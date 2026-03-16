#include <math.h>

// LW0 -> Local Word Register 0 -> 0x0000

#define TX_PIN 17
#define RX_PIN 16
#define DE_RE 4
#define ADC_PIN 25

HardwareSerial RS485(2);

uint16_t crc16(uint8_t *buf, int len)
{
  uint16_t crc = 0xFFFF;

  for (int pos = 0; pos < len; pos++)
  {
    crc ^= (uint16_t)buf[pos];

    for (int i = 0; i < 8; i++)
    {
      if (crc & 0x0001)
      {
        crc >>= 1;
        crc ^= 0xA001;
      }
      else
        crc >>= 1;
    }
  }
  return crc;
}

void write_LW0(uint16_t value)
{
  uint8_t frame[8];

  frame[0] = 0x01;  // Slave ID
  frame[1] = 0x06;  // Write single register
  frame[2] = 0x00;
  frame[3] = 0x00;  // LW0 address

  frame[4] = value >> 8;
  frame[5] = value & 0xFF;

  uint16_t crc = crc16(frame, 6);

  frame[6] = crc & 0xFF;
  frame[7] = crc >> 8;

  digitalWrite(DE_RE, HIGH);

  RS485.write(frame, 8);
  RS485.flush();

  digitalWrite(DE_RE, LOW);
}

void setup()
{
  Serial.begin(115200);

  pinMode(DE_RE, OUTPUT);
  digitalWrite(DE_RE, LOW);

  pinMode(ADC_PIN, INPUT);

  RS485.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);

  analogReadResolution(12);
}

void loop()
{
  uint16_t adc = analogRead(ADC_PIN);

  printf("%d\n", adc);

  float voltage = (adc / 4095.0) * 3.3;

  printf("%.2f\n", voltage);

  uint16_t scaledValue = (uint16_t)round(voltage * 10);   // 3.30V → 330

  write_LW0(scaledValue);

  delay(100);
}
