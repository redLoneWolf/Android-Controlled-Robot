#include <Arduino.h>
#include <MotorDriver.h>
#include <parameters.h>
#include <parser.h>

boolean connected = false;
char PREAMBLE = '$';

uint8_t serialBufferRX[64];
uint8_t count = 0;
MotorDriver m;

void setup()
{

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  while (!Serial)
    ;
}

void led()
{
  int8_t state = read_i8();

  // write_i8(state);
  if (state == 1)
  {

    connected = false;
  }
  else
  {
    connected = true;
  }

  // write_i8(102);
  // write_i8(103);
  // write_i8(104);
}

void sendHeader(int8_t size, Command command)
{
  write_i8((byte)PREAMBLE); // $

  write_i8(size); // size

  write_i8((byte)command); // command
}

void sendPacket(int8_t size, Command command, int8_t data)
{
  sendHeader(size, command);

  int t = 0;
  while (t <= count)
  {
    write_i8(serialBufferRX[t]);
    t++;
  }

  // switch (size)
  // {
  // case 1:
  //   write_i8(data);
  //   break;

  // case 2:
  //   write_i16(data);
  //   break;

  // case 4:
  //   write_i32(data);
  //   break;

  // default:
  //   break;
  // }
  // command
}

void writeMotor(int motorNo, int val)
{

  Directions dir;
  if (val == -1)
  {
    dir = Brake;
  }
  else if (val == 0)
  {
    dir = Release;
   
  }
  else if (val > 0) //forward if positive value
  {
    dir = Forward;
  }

  else if (val < -1) // backward if negative value
  {
    dir = Backward;
  }
  m.motor(motorNo, dir, abs(val));
}
unsigned long previousMillis = 0;
const long interval = 500;
void motor()
{
  int16_t motorsVals[4];

  for (size_t i = 0; i < 4; i++)
  {
    motorsVals[i] = read_i16();
  }


  for (size_t i = 1; i <= 4; i++)
  {
    writeMotor(i, motorsVals[i - 1]);
  }


}
void checkCommand(Command command);

void checkConnected()
{
  if (connected)
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
  }
}
void loop()
{

  checkConnected();

  if (Serial.available() > 0)
  {
    if ((byte)read_i8() == (byte)PREAMBLE)
    {
      int8_t size = read_i8();
      if (size > 0)
      {
        Command command = (Command)read_i8();
        checkCommand(command);
      }
    }
  }
}

void checkHandshake()
{
  if (connected)
  {
    // write_i8(ALREADY_CONNECTED);
    while (count <= 3)
    {
      serialBufferRX[count] = 125;
      count++;
    }
    sendPacket(count, ALREADY_CONNECTED, 1);

    // write_float(12.3);
  }
  else
  {
    connected = true;
    // write_i8(HANDSHAKE);

    while (count <= 3)
    {
      serialBufferRX[count] = 125;
      count++;
    }
    sendPacket(count, HANDSHAKE, 1);
    // write_float(12.2);
  }
}

void checkCommand(Command command)
{

  switch (command)
  {
  case HANDSHAKE:
    checkHandshake();
    break;

  case LED:
    led();
    break;

  case MOTOR:
    motor();
    break;

  default:
    write_i8(ERROR);
    break;
  }
}