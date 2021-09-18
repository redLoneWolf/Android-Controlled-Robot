// Thanks to Antonin RAFFIN https://medium.com/@araffin/simple-and-robust-computer-arduino-serial-communication-f91b95596788
// code copied from https://github.com/araffin/cpp-arduino-serial/tree/2075fd8f203483af9c82caffba4bdbc84024ea63
#include<parser.h>

void wait_for_bytes(int num_bytes, unsigned long timeout)
{
  unsigned long startTime = millis();
  //Wait for incoming bytes or exit if timeout
  while ((Serial.available() < num_bytes) && (millis() - startTime < timeout))
  {
  }
}

Command read_order()
{
  return (Command)read_i32();
}

// NOTE : Serial.readBytes is SLOW
// this one is much faster, but has no timeout
void read_signed_bytes(int8_t *buffer, size_t n)
{
  size_t i = 0;
  int c;
  while (i < n)
  {
    c = Serial.read();
    if (c < 0)
      break;
    *buffer++ = (int8_t)c; // buffer[i] = (int8_t)c;
    i++;
  }
}

int8_t read_i8()
{
  wait_for_bytes(1, 100); // Wait for 1 byte with a timeout of 100 ms
  return (int8_t)Serial.read();
}

int16_t read_i16()
{
  int8_t buffer[2];
  wait_for_bytes(2, 100); // Wait for 2 bytes with a timeout of 100 ms
  read_signed_bytes(buffer, 2);
  return (((int16_t)buffer[0]) & 0xff) | (((int16_t)buffer[1]) << 8 & 0xff00);
}

int32_t read_i32()
{
  int8_t buffer[4];
  wait_for_bytes(4, 200); // Wait for 4 bytes with a timeout of 200 ms
  read_signed_bytes(buffer, 4);
  return (((int32_t)buffer[0]) & 0xff) | (((int32_t)buffer[1]) << 8 & 0xff00) | (((int32_t)buffer[2]) << 16 & 0xff0000) | (((int32_t)buffer[3]) << 24 & 0xff000000);
}

void write_order(enum Command command)
{
//   uint8_t *Order = (uint8_t *)&command;
  write_i32(command);
}

void write_i8(int8_t num)
{
  Serial.write(num);
}

float readFloat(){

    int8_t buffer[4];
    for (int i = 0; i <4; i++)
    {
      buffer[i] = read_i8();
    }
    

    float x = *(float *)&buffer;
    return x;
}

void write_float(float num)
{

  Serial.write((byte*)&num, sizeof(float));

}
void write_i16(int16_t num)
{
  int8_t buffer[2] = {(int8_t)(num & 0xff), (int8_t)(num >> 8)};
  Serial.write((uint8_t *)&buffer, 2 * sizeof(int8_t));
}

void write_i32(int32_t num)
{
  int8_t buffer[4] = {(int8_t)(num & 0xff), (int8_t)(num >> 8 & 0xff), (int8_t)(num >> 16 & 0xff), (int8_t)(num >> 24 & 0xff)};
  Serial.write((uint8_t *)&buffer, 4 * sizeof(int8_t));
}