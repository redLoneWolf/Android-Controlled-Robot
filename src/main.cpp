#include <Arduino.h>

#include <parameters.h>
#include <parser.h>

boolean connected = false;
char PREAMBLE = '$';

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
  if(state==1){
    
      connected = false;
  }else{
      connected = true;
  }


  // write_i8(102);
  // write_i8(103);
  // write_i8(104);
}

void motor(){
  uint8_t m1 = read_i8();
  uint8_t m2 = read_i8();
  uint8_t m3 = read_i8();
  uint8_t m4 = read_i8();

  if(m1==255 && m2==255  && m3==255 && m4==255){
      write_i8(MOTOR);
      connected = false;
  }else{
      write_i8(MOTOR);
      connected = true;

  }
}

void checkCommand(Command command);

void checkConnected(){
  if (connected)
    {
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else{
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
    write_i8(ALREADY_CONNECTED);
  }
  else
  {
    connected = true;
    write_i8(HANDSHAKE);
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