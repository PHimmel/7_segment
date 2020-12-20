/*
   To control a 4 digit 7-segment display using light levels.
*/

// VARIABLES for pins used for communication with the display

const int A = 11;
// analog pin set for GPIO
const int B = A5;
const int C = 4;
const int D = 6;
const int E = 7;
const int F = 10;
const int G = 3;
// decimal point
const int DP = 5;

// pins to control individual digits
const int D1 = 12; //
const int D2 = 9; //
const int D3 = 8; //
const int D4 = 2; //

// VARIABLES for pins for extended functionality

// analog pin for light readings
const int LIGHT = A3;
// analog pin for button
const int BUTTON = A0;
// general use digital pin
const int D13 = 13;

//  VARIABLES for general-use

// for alternating patterns
bool alternate = true;

// for light level positional ints
int ones;
int tens;
int hundreds;
int thousands;

// number of times light reading is displayed (else too fast for viewing)
int repeater = 60;

void setup() {
  // set state of display communication pins
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(DP, OUTPUT);

  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);

  // start with display in off state
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH);

  // set extended function pins
  pinMode(LIGHT, INPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(D13, OUTPUT);

  //Serial.begin(9600); // for debugging
}

void loop() {

  // analog pins reads int 0 - 1023.
  // int sensitivity sets the upper-limit for the button trigger
  int sensitivity = 50;
  int button = analogRead(BUTTON);
  //shift_position(false); // nice background pattern/s

  // trigger ---> future: use DI/O 2,3 for interrupting processes
  if (button <= sensitivity) {
    delay(150);

    while (true) {

      digitalWrite(D13, HIGH);
      light_level();
      show_numbers(ones, tens, hundreds, thousands);

      Serial.println(button); // debug

      // retrieve new light value
      button = analogRead(BUTTON);
      if (button <= sensitivity) {

        // kill display
        clear_seg(100);
        digitalWrite(D13, LOW);

        // retrieve new light value
        button = analogRead(BUTTON);

        // pull-up resistor reads HIGH(or int ~1023) as default
        // wait until button pressed again
        while (button >= 900) {
          button = analogRead(BUTTON);
          delay(150);
          continue;

        } // while (button pressed and program in 'off' state)
      } // if (button pressed)
    } // while (true)
  } // if (first time button pressed [loop starts in off state])
} // loop()

void show_numbers(int one, int ten, int hundred, int thousand) {

  int sleep = 3;
  if (thousand != 0) {
    display_number(1, thousand);
    delay(sleep);
    on_off(0);
  }

  if (thousand != 0 || hundred != 0) {
    display_number(2, hundred);
    delay(sleep);
    on_off(0);
  }

  if (thousand != 0 || hundred != 0 || ten != 0) {
    display_number(3, ten);
    delay(sleep);
    on_off(0);
  }

  display_number(4, one);
  delay(sleep);
  on_off(0);
}
void clear_seg(int sleep) {
  delay(sleep);
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH);
  delay(sleep);
}
/* alternates between left-to-right and right-to-left display

  for left-to-right movement only:

  i = 0
  remove abs()
*/
void shift_position(bool pattern) {
  // display single character function
  zero();
  //one(1);
  //two(1);
  //three(1);
  //four(1);
  //five(1);
  //six(1);
  //seven(1);
  //eight(1);
  //nine(1);
  // dash
  //digitalWrite(G, HIGH);

  // delay duration for how long each digit stays on
  int sleep = 200;

  for (int i = -3; i < 4; i++) {
    //light_level();
    switch (abs(i)) {
      case 0 :
        {
          digitalWrite(D1, LOW);
          delay(sleep);
          digitalWrite(D1, HIGH);
          break;
        }
      case 1: {
          digitalWrite(D2, LOW);
          delay(sleep);
          digitalWrite(D2, HIGH);
          break;
        }
      case 2 : {
          digitalWrite(D3, LOW);
          delay(sleep);
          digitalWrite(D3, HIGH);
          break;
        }
      case 3 : {
          digitalWrite(D4, LOW);
          delay(sleep);
          digitalWrite(D4, HIGH);
          //light_level();
          i = -3;
          if (pattern) {
            change_pattern();
          }
          break;
        }
      default : {
          break;
        }

    } // switch statement
  } // for loop
} // function block

void light_level() {
  int level;
  
  // defined globally
  if (repeater <= 0) {
    level = analogRead(LIGHT);
    repeater = 30;
  }
  ones = level % 10;
  tens = (level % 100 - ones) / 10;
  hundreds = (level % 1000 - tens - ones) / 100;
  thousands = (level % 10000 - hundreds - tens - ones) / 1000;

  repeater--;
  
  // debugging 
  // D1
  Serial.print("Thousands: ");
  Serial.println(thousands);
  // D2
  Serial.print("Hundreds: ");
  Serial.println(hundreds);
  // D3
  Serial.print("Tens: ");
  Serial.println(tens);
  //D4
  Serial.print("Ones: ");
  Serial.println(ones);
  Serial.println("");

}
void change_pattern() {

  alternate = !(alternate);
  if (alternate) {
    zero();
    digitalWrite(G, LOW);
    digitalWrite(DP, LOW);
  }
  else {
    digitalWrite(G, HIGH);
    digitalWrite(DP, HIGH);
    zero();
  }
}

void display_number(int position_, int number) {
  switch (position_) {
    case 1 : {
        digitalWrite(D1, LOW);
        digitalWrite(D2, HIGH);
        digitalWrite(D3, HIGH);
        digitalWrite(D4, HIGH);
        break;
      }
    case 2 : {
        digitalWrite(D2, LOW);
        digitalWrite(D1, HIGH);
        digitalWrite(D3, HIGH);
        digitalWrite(D4, HIGH);
        break;
      }
    case 3 : {
        digitalWrite(D3, LOW);
        digitalWrite(D2, HIGH);
        digitalWrite(D1, HIGH);
        digitalWrite(D4, HIGH);
        break;
      }
    case 4 : {
        digitalWrite(D4, LOW);
        digitalWrite(D2, HIGH);
        digitalWrite(D3, HIGH);
        digitalWrite(D1, HIGH);
        break;
      }
    default : break;
  }
  switch (number) {
    case 0 : {
        zero();
        break;
      }
    case 1 : {
        one();
        break;
      }
    case 2 : {
        two();
        break;
      }
    case 3 : {
        three();
        break;
      }
    case 4 : {
        four();
        break;
      }
    case 5 : {
        five();
        break;
      }
    case 6 : {
        six();
        break;
      }
    case 7 : {
        seven();
        break;
      }
    case 8 : {
        eight();
        break;
      }
    case 9 : {
        nine();
        break;
      }
    default : break;
  }
}

// toggle every segment
void on_off(int state) {
  digitalWrite(A, state);
  digitalWrite(B, state);
  digitalWrite(C, state);
  digitalWrite(D, state);
  digitalWrite(E, state);
  digitalWrite(F, state);
  digitalWrite(G, state);
  digitalWrite(DP, state);
}

// FUNCTIONS FOR EACH NUMBER
void zero() {
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
  digitalWrite(E, HIGH);
  digitalWrite(F, HIGH);
}

void one() {
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
}

void two() {
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(D, HIGH);
  digitalWrite(E, HIGH);
  digitalWrite(G, HIGH);
}

void three() {
  digitalWrite(C, HIGH);
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(D, HIGH);
  digitalWrite(G, HIGH);
}

void four() {
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(F, HIGH);
  digitalWrite(G, HIGH);
}

void five() {
  digitalWrite(A, HIGH);
  digitalWrite(F, HIGH);
  digitalWrite(D, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(G, HIGH);
}

void six() {
  digitalWrite(F, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
  digitalWrite(E, HIGH);
  digitalWrite(G, HIGH);
}

void seven() {
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
}

void eight() {
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
  digitalWrite(E, HIGH);
  digitalWrite(F, HIGH);
  digitalWrite(G, HIGH);
}

void nine() {
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(F, HIGH);
  digitalWrite(G, HIGH);
}
