#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET -1
#define sw1 2
#define sw2 3
#define buzzer 11

Adafruit_SSD1306 OLED(OLED_RESET);

int x = 64, y = 16;
int p1_pos = 11;
int p2_pos = 11;
int p1_move = 1;
int p2_move = 1;
int bx = 1; //ball move x
int by = 1; //ball move y
int ballState_x = 0;
int ballState_y = 0;
long timeDebounce1 = 0;
long timeDebounce2 = 0;
bool is_p1_movedown = true;
bool is_p2_movedown = true;
int p1_count = 0;
int p2_count = 0;

void Hello() {
  OLED.clearDisplay();
  OLED.setTextColor(WHITE);
  OLED.setCursor(10,0);
  OLED.setTextSize(2);
  OLED.println("The");
  OLED.setCursor(10,15);
  OLED.println("Feel!");
  OLED.display();
  delay(1500);
}

void score() {
  OLED.setTextColor(WHITE);
  OLED.setTextSize(1);
  OLED.setCursor(4, 4);
  OLED.println("PY 1");
  OLED.setCursor(100, 4);
  OLED.println("PY 2");
}

int point(int p1_count, int p2_count) {
  OLED.setTextColor(WHITE);
  OLED.setTextSize(2);
  OLED.setCursor(11, 15);
  OLED.println(p1_count);
  OLED.setCursor(106, 15);
  OLED.println(p2_count);
}

void gameover() {
  OLED.clearDisplay();
  OLED.setTextColor(WHITE);
  OLED.setCursor(10,4);
  OLED.setTextSize(2);
  OLED.println("Game Over");
  if(p1_count > p2_count) {
    OLED.setCursor(28,24);
    OLED.setTextSize(1);
    OLED.println("Player1 Win!"); 
  }
  else if(p1_count < p2_count) {
    OLED.setCursor(28,24);
    OLED.setTextSize(1);
    OLED.println("Player2 Win!");
  }
  OLED.display();
}

void soundBounce() {
  tone(11, 500, 25);
}

void soundPoint() {
  tone(11, 250, 50);
}

void setup() {
  Serial.begin(115200);
  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  pinMode(sw1, INPUT_PULLUP); //left
  pinMode(sw2, INPUT_PULLUP); //right
  pinMode(buzzer, OUTPUT);
  randomSeed(analogRead(0));
  bx = random(-1, 2);
  do {
    bx = random(-1, 2);
  } while(bx == 0);
  by = random(-1, 2);
  do {
    by = random(-1, 2);
  } while(by == 0);
  // x = random(60, 68), y = random(4, 28); //ball position
  Hello();
}

void loop() {
  OLED.clearDisplay();
  if(p1_count == 5 || p2_count == 5) {
    gameover();
    delay(3000);
    p1_count = 0, p2_count = 0;
    x = 64, y = 16;
    p1_pos = 11, p2_pos = 11;
    bx = random(-1, 2);
    do {
      bx = random(-1, 2);
    } while(bx == 0);
    by = random(-1, 2);
    do {
      by = random(-1, 2);
    } while(by == 0);
  }
  else {
    OLED.fillCircle(x += bx, y += by, 2, WHITE);
    OLED.drawRect(32, 0, 64, 32, WHITE);
    //draw player
    OLED.fillRect(36, p1_pos += p1_move, 2, 10, WHITE);
    OLED.fillRect(90, p2_pos += p2_move, 2, 10, WHITE);
    
    point(p1_count, p2_count);
    
    //player1 move
    if (digitalRead(sw1) == LOW) {
      if (millis() - timeDebounce1 >= 100) {
        p1_move += 1;
        if (is_p1_movedown) {
          p1_move = 1;
          if (p1_pos == 20) {
            is_p1_movedown = false;
          }
        }
        else {
          p1_move = -1;
          if (p1_pos == 2) {
            is_p1_movedown = true;
          }
        }
        timeDebounce1 = millis();
      }
    }
    else {
      p1_move = 0;
    }

    //player2 move
    if (digitalRead(sw2) == LOW) {
      if (millis() - timeDebounce2 >= 100) {
        p2_move += 1;
        if (is_p2_movedown) {
          p2_move = 1;
          if (p2_pos == 20) {
            is_p2_movedown = false;
          }
        }
        else {
          p2_move = -1;
          if (p2_pos == 2) {
            is_p2_movedown = true;
          }
        }
        timeDebounce2 = millis();
      }
    }
    else {
      p2_move = 0;
    }

    // //check ball collisions
      //check vertical walls
    if (x == 35) {
      bx *= -1;
      x += bx + bx;
      p2_count+=1;
      soundPoint();
      delay(200);
      x = 64, y = 16;
    }
    if (x == 93) {
      bx *= -1;
      x += bx + bx;
      p1_count+=1;
      soundPoint();
      delay(200);
      x = 64, y = 16;
    }
      //check horizontal walls
    if (y == 2 || y == 29) { //estimate 3
      by *= -1;
      y += by + by;
    }
      //check collision player1
    if (x == 38 && y >= p1_pos && y <= p1_pos + 10) {
      bx *= -1;
      x += bx + bx;
      soundBounce();
    }
      //check collision player2
    if (x == 88 && y >= p2_pos && y <= p2_pos + 10) {
      bx *= -1;
      x += bx + bx;
      soundBounce();
    }
    
    score();
  }

  OLED.display();
}
