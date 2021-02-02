#include <Arduino.h>

const int scl = 32;
const int sda = 33;
unsigned long timeUs;

 // 0: idel, 1: data, 2: ack
unsigned int readState = 0;
unsigned int period = 50; // 25 us
unsigned int sampleTime = 0;
unsigned int start_detect[2] = {0, 0};
unsigned int ack_detect[2] = {0, 0};

unsigned int pre_scl = 0;
unsigned int write_flag = 1;
int current_data = 0;
int current_bit = 7;
int current_digit = 0;

float temp = 0;
int detect_start_cnt = 0;

int incomingByte = 0;

void setup() {
  pinMode(scl, INPUT);
  pinMode(sda, INPUT);
  Serial.begin(115200);
}

void printHexByte(int b)
{
  Serial.print("0x");
  Serial.print((b >> 4) & 0xF, HEX);
  Serial.println(b & 0xF, HEX);
}

void printBinary(byte inByte)
{
  for (int b = 7; b >= 0; b--)
  {
    Serial.print(bitRead(inByte, b));
  }
  Serial.print(", ");
}

void loop() {

  // if (Serial.available() > 0) {
  //   // read the incoming byte:
  //   incomingByte = Serial.read();
  //
  //   String text = Serial.readString();
  //   // say what you got:
  //   Serial.print("I received: ");
  //   Serial.println(text);
  //   // Serial.println(incomingByte , HEX);
  // }

  int scl_val = digitalRead(scl);
  int sda_val = digitalRead(sda);
  // Serial.print("scl: ");
  // Serial.println(scl_val);
  // Serial.print("sda: ");
  // Serial.println(sda_val);
  // Serial.print("Time: ");
  unsigned int now = micros();
  // Serial.println(now);

  // === start transmission  ===
  // detect sda = 0 and scl = 1
  // wait for sda = 0 and scl = 0
  // wait for sda = 0 and scl = 1 then detech start transmission
  if(now - sampleTime >= period){
    // Serial.print("now - sampleTime: ");
    // Serial.println(now - sampleTime);
    sampleTime = now;
    if(readState == 0){ // idle state
      if(sda_val == 0 && scl_val == 1){
        start_detect[0] = 1;
      }
      else if(sda_val == 0 && scl_val == 0 && start_detect[0] == 1){
        start_detect[1] = 1;
      }
      else {
        start_detect[0] = 0;
        start_detect[1] = 0;
      }

      if(start_detect[0] == 1  && start_detect[1] == 1){
        readState = 1; // go to data state
        start_detect[0] = 0;
        start_detect[1] = 0;
        detect_start_cnt++;
        // Serial.print("Detect start: ");
        // Serial.println(detect_start_cnt);
      }
    }
    else if(readState == 1){ // state data
      if(scl_val == 1 && pre_scl == 0){
        pre_scl = 1;
        current_data += sda_val << current_bit;
        if(current_bit == 0){ // go to wait for next data
          current_bit = 7;
          readState = 2;
          // delayMicroseconds(5);
          // printBinary(current_data);
          // printHexByte(current_data);

          // if(current_data == 0x3F) Serial.println("0");
          // else if(current_data == 0x06) Serial.println("1");
          // else if(current_data == 0x5B) Serial.println("2");
          // else if(current_data == 0x4F) Serial.println("3");
          // else if(current_data == 0x66) Serial.println("4");
          // else if(current_data == 0x6D) Serial.println("5");
          // else if(current_data == 0x7D) Serial.println("6");
          // else if(current_data == 0x07) Serial.println("7");
          // else if(current_data == 0x7F) Serial.println("8");
          // else if(current_data == 0x6F) Serial.println("9");
          // else if(current_data == 0xBF) Serial.println("0.");
          // else if(current_data == 0x86) Serial.println("1.");
          // else if(current_data == 0xDB) Serial.println("2.");
          // else if(current_data == 0xCF) Serial.println("3.");
          // else if(current_data == 0xE6) Serial.println("4.");
          // else if(current_data == 0xED) Serial.println("5.");
          // else if(current_data == 0xFD) Serial.println("6.");
          // else if(current_data == 0x87) Serial.println("7.");
          // else if(current_data == 0xFF) Serial.println("8.");
          // else if(current_data == 0xEF) Serial.println("9.");


          if(write_flag == 0){ // read thermometer data
              float read_data = 0;
              // printHexByte(current_data);

              if(current_data == 0x3F){     read_data = 0;  } // Serial.println("0");}
              else if(current_data == 0x06){ read_data = 1; } // Serial.println("1");}
              else if(current_data == 0x5B){ read_data = 2; } // Serial.println("2");}
              else if(current_data == 0x4F){ read_data = 3; } // Serial.println("3");}
              else if(current_data == 0x66){ read_data = 4; } // Serial.println("4");}
              else if(current_data == 0x6D){ read_data = 5; } // Serial.println("5");}
              else if(current_data == 0x7D){ read_data = 6; } // Serial.println("6");}
              else if(current_data == 0x07){ read_data = 7; } // Serial.println("7");}
              else if(current_data == 0x7F){ read_data = 8; } // Serial.println("8");}
              else if(current_data == 0x6F){ read_data = 9; } // Serial.println("9");}
              else if(current_data == 0xBF){ read_data = 0; } // Serial.println("0.");}
              else if(current_data == 0x86){ read_data = 1; } // Serial.println("1.");}
              else if(current_data == 0xDB){ read_data = 2; } // Serial.println("2.");}
              else if(current_data == 0xCF){ read_data = 3; } // Serial.println("3.");}
              else if(current_data == 0xE6){ read_data = 4; } // Serial.println("4.");}
              else if(current_data == 0xED){ read_data = 5; } // Serial.println("5.");}
              else if(current_data == 0xFD){ read_data = 6; } // Serial.println("6.");}
              else if(current_data == 0x87){ read_data = 7; } // Serial.println("7.");}
              else if(current_data == 0xFF){ read_data = 8; } // Serial.println("8.");}
              else if(current_data == 0xEF){ read_data = 9; } // Serial.println("9.");}
              // Serial.println(read_data);

              if(current_digit == 2){
                temp += read_data * 10;
                if(temp > 34 && temp < 40){
                  // Serial.print(current_digit);
                  // Serial.print(": ");
                  // Serial.print(read_data);
                  // Serial.print(", ");
                  Serial.println(temp);
                  // client.update_shadow((char*)"meter.temp", temp);

                }
                current_digit = 0;
                temp = 0;
              }
              else if (current_digit == 1){
                temp += read_data;
                // Serial.print(current_digit);
                // Serial.print(": ");
                // Serial.print(read_data);
                // Serial.print(", ");
                // Serial.println(temp);
                current_digit++;
              }
              else {
                temp += read_data * 0.1;
                // Serial.print(current_digit);
                // Serial.print(": ");
                // Serial.print(read_data);
                // Serial.print(", ");
                // Serial.println(temp);
                current_digit++;
              }
          }

          current_data = 0;
        } else { current_bit--; }
      }
      else if(scl_val == 0 && pre_scl == 1){
        pre_scl = 0;
      }
    }
    else if(readState == 2){ // ack state
      if(sda_val == 0 && scl_val == 1){
        ack_detect[0] = 1;
      }
      else if(sda_val == 0 && scl_val == 0 && ack_detect[0] == 1){
        ack_detect[1] = 1;
      }

      if(ack_detect[0] == 1 && ack_detect[1] == 1){
        ack_detect[0] = 0;
        ack_detect[1] = 0;
        if(write_flag == 1){ // go to data state
          readState = 1;
          write_flag = 0;
        } else { // go to idle state
          readState = 0;
          write_flag = 1;
        }
      }
    }

  }
}
