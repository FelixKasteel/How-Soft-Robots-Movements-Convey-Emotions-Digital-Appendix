/*
  Using a Pololu A4988 or StepStick Stepper Motor Driver with FabScan-Shield.
*/
//driver: 1 | 2 | 3  | 4
#define EN_PIN1   2  //2 | 5 | 11 | A0
#define STEP_PIN1 3  //3 | 6 | 12 | A1
#define DIR_PIN1  4  //4 | 7 | 13 | A2
#define EN_PIN2   5  //2 | 5 | 11 | A0
#define STEP_PIN2 6  //3 | 6 | 12 | A1
#define DIR_PIN2  7  //4 | 7 | 13 | A2
#define EN_PIN3   11  //2 | 5 | 11 | A0
#define STEP_PIN3 12  //3 | 6 | 12 | A1
#define DIR_PIN3  13  //4 | 7 | 13 | A2
#define MS_PIN   28 //A5 //HIGH for 16 microstepping, LOW for no microstepping

void setup(){
  pinMode(EN_PIN1, OUTPUT);
  digitalWrite(EN_PIN1, HIGH); //deactivate driver (LOW active)
  pinMode(DIR_PIN1, OUTPUT);
  digitalWrite(DIR_PIN1, HIGH); //LOW or HIGH
  pinMode(STEP_PIN1, OUTPUT);
  digitalWrite(STEP_PIN1, LOW);
  digitalWrite(EN_PIN1, LOW); //deactivate driver (LOW active)

  pinMode(EN_PIN2, OUTPUT);
  digitalWrite(EN_PIN2, HIGH); //deactivate driver (LOW active)
  pinMode(DIR_PIN2, OUTPUT);
  digitalWrite(DIR_PIN2, HIGH); //LOW or HIGH
  pinMode(STEP_PIN2, OUTPUT);
  digitalWrite(STEP_PIN2, LOW);
  digitalWrite(EN_PIN2, LOW); //activate driver

  pinMode(EN_PIN3, OUTPUT);
  digitalWrite(EN_PIN3, HIGH); //deactivate driver (LOW active)
  pinMode(DIR_PIN3, OUTPUT);
  digitalWrite(DIR_PIN3, HIGH); //LOW or HIGH
  pinMode(STEP_PIN3, OUTPUT);
  digitalWrite(STEP_PIN3, LOW);
  digitalWrite(EN_PIN3, LOW); //activate driver

  pinMode(MS_PIN, OUTPUT);
  digitalWrite(MS_PIN, HIGH); //microstepping
}

#define STEPS_PER_REVOLUTION 200
#define LENGTH 400
#define PITCH 1.25

#define GEAR_RATIO 4 // 30/8

#define CW LOW
#define CCW HIGH
#define AWAY LOW
#define TOWARDS HIGH
#define UP LOW
#define DOWN HIGH

#define XW 10
#define RW 10
#define TW 10

void action(){

  //transitionUnified(0, -20, 30, 1 , 10);

  translate(0000, 3);

  //tilt(-30, 10);
  //tilt(-30, 10);
  //tilt(60, 20);
  //tilt(-120, 20);
  //tilt(60, 20);
  //rotate(30, 10);

}

void loop(){
//  action();
//  while(1){}

}

void transitionUnified(int16_t x, int16_t r, int16_t t, uint16_t divisions, uint16_t wait){
  transition(
    (int16_t)((float)x/PITCH*200),
    (int16_t)((float)r*200/360),
    (int16_t)((float)t*200/360*4),
    divisions, wait);
}

void transition(int16_t x, int16_t r, int16_t t, uint16_t divisions, uint16_t wait){
  //moved
  int16_t xm = 0;
  int16_t rm = 0;
  int16_t tm = 0;

  //delta
  float dx = ((float)x) / divisions;
  float dr = ((float)r) / divisions;
  float dt = ((float)t) / divisions;

  for(uint16_t i = 0; i < divisions + 1; i++){
    //target
    int16_t xt = (uint16_t)(dx * i);
    int16_t rt = (uint16_t)(dr * i);
    int16_t tt = (uint16_t)(dt * i);

    //move
    int16_t mx = xt - xm;
    int16_t mr = rt - rm;
    int16_t mt = tt - tm;
    xm += mx;
    rm += mr;
    tm += mt;

    unsigned long pre = millis();
    translate(mx, XW);
    rotate(mr, RW);
    tilt(mt, TW);

    //delay(wait - (millis() - pre));
  }

}

void translate(int16_t s, uint8_t t){
  operate(s > 0 ? AWAY : TOWARDS, s, t, DIR_PIN1, STEP_PIN1);
}

void rotate(int16_t s, uint8_t t){
  operate(s > 0 ? CW : CCW, s, t, DIR_PIN3, STEP_PIN3);
}

void tilt(int16_t s, uint8_t t){
  operate(s > 0 ? UP : DOWN, s, t, DIR_PIN2, STEP_PIN2);
}

void operate(uint8_t d, int16_t s, uint8_t t, uint8_t dir_pin, uint8_t step_pin){
  digitalWrite(dir_pin, d);
  if(s < 0) s = -s;
  for(int16_t i = 0; i < s; i++){
    digitalWrite(step_pin, HIGH);
    delay(t);
    digitalWrite(step_pin, LOW);
    delay(t);
  }
}
      
