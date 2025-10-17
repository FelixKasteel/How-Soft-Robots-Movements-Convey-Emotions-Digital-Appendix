#include <Arduino.h>
#include <Button.h>
#include <ShiftRegister74HC595.h>

#define EMERGENCY_STOP_PIN 2

#define ENABLE_PIN 12
#define STEP_PIN 3
#define DIRECTION_PIN 4

#define RUN_BUTTON_PIN 6

#define FREQUENCY_SWITCH_PIN 7
#define AMPLITUDE_SWITCH_PIN 8
#define SHAPE_SWITCH_PIN A0
#define MATERIAL_SWITCH_PIN A4

#define PUSH_BUTTON_PIN 5
#define PULL_BUTTON_PIN A5

#define DATA_PIN A1  //DS
#define STORE_PIN A2 //STCP
#define SHIFT_PIN A3 //SHCP

#define CW HIGH
#define CCW LOW

#define SILICONE 0
#define FABRIC 1
#define LEATHER 2
#define METAL 3

#define BEND 0
#define WAVE 1
#define TWIST 2
#define ELONGATE 3

#define FULL 0
#define HALF 1
#define BASE 2

bool motorEnabled = true;

int currentMaterial = 0;
int currentShape = 0;
int currentFrequency = 0;
int currentAmplitude = 0;

int shortestDelay = 15;

float highFrequency = (float)1 / 3;
float lowFrequency = (float)1 / 6;
float noneFrequency = (float)1 / 3;

float frequencies[3] = {highFrequency, lowFrequency, noneFrequency};

int stepsPerMililiter = 92 * 16;

uint32_t getAnimationDuration(){
  int maxAnimationDurationInSeconds = 15;
  int maxLowFrequencyPeriods = maxAnimationDurationInSeconds / (1 / lowFrequency);
  return toMicroseconds(1 / lowFrequency * maxLowFrequencyPeriods);
}

uint32_t animationDurationInMicroseconds = getAnimationDuration();

int displacements[4][4][3] = {
    //Silicone
    {
        //Bend
        {
            //Full, Half, Base
            38, 29, 15},
        //Wave
        {
            //Full, Half, Base
            35, 28, 10}, // 25, 18
        //Twist
        {
            //Full, Half, Base
            36, 27, 10}, //26, 17
        //Elongate
        {
            //Full, Half, Base
            42, 35, 15}}, // 27, 20
    //Fabric
    {
        //Bend
        {
            //Full, Half, Base
            38, 30, 15},
        //Wave
        {
            //Full, Half, Base
            38, 29, 10}, //28, 19
        //Twist
        {
            //Full, Half, Base
            36, 27, 10}, //26, 17
        //Elongate
        {
            //Full, Half, Base
            45, 37, 20}}, // 25, 17
    //Leather
    {
        //Bend
        {
            //Full, Half, Base
            46, 35, 20},
        //Wave
        {
            //Full, Half, Base
            45, 35, 15},
        //Twist
        {
            //Full, Half, Base
            52, 35, 20},
        //Elongate
        {
            //Full, Half, Base
            50, 40, 22}}, // 30, 20
    //Metal
    {
        //Bend
        {
            //Full, Half, Base
            41, 32, 18},
        //Wave
        {
            //Full, Half, Base
            40, 31, 10},
        //Twist
        {
            //Full, Half, Base
            42, 30, 15},
        //Elongate
        {
            //Full, Half, Base
            48, 42, 20}}}; //28, 22

int getDelay(int displacement, int baseDisplacement, float frequency)
{
  float periodDuration = 1.0 / frequency;
  long animationDisplacement = displacement - baseDisplacement;
  uint32_t stepsPerDierection = (uint32_t)animationDisplacement * stepsPerMililiter;
  uint32_t stepsPerPeriode = (uint32_t)stepsPerDierection * 2;
  float delayPerStep = toMicroseconds((periodDuration / stepsPerPeriode));
  int delay = delayPerStep / 2;
  return delay;
}

unsigned long toMicroseconds(float seconds)
{
  return seconds * 1000000;
}

float toSeconds(unsigned long microseconds)
{
  return 0.000001 * microseconds;
}

int delays[4][4][3][2] = {
    //SILICONE
    {
        {{getDelay(displacements[SILICONE][BEND][FULL], displacements[SILICONE][BEND][BASE], highFrequency),
          getDelay(displacements[SILICONE][BEND][HALF], displacements[SILICONE][BEND][BASE], highFrequency)},
         {getDelay(displacements[SILICONE][BEND][FULL], displacements[SILICONE][BEND][BASE], lowFrequency),
          getDelay(displacements[SILICONE][BEND][HALF], displacements[SILICONE][BEND][BASE], lowFrequency)},
         {getDelay(displacements[SILICONE][BEND][FULL], displacements[SILICONE][BEND][BASE], noneFrequency),
          getDelay(displacements[SILICONE][BEND][HALF], displacements[SILICONE][BEND][BASE], noneFrequency)}},

        {{getDelay(displacements[SILICONE][WAVE][FULL], displacements[SILICONE][WAVE][BASE], highFrequency),
          getDelay(displacements[SILICONE][WAVE][HALF], displacements[SILICONE][WAVE][BASE], highFrequency)},
         {getDelay(displacements[SILICONE][WAVE][FULL], displacements[SILICONE][WAVE][BASE], lowFrequency),
          getDelay(displacements[SILICONE][WAVE][HALF], displacements[SILICONE][WAVE][BASE], lowFrequency)},
         {getDelay(displacements[SILICONE][WAVE][FULL], displacements[SILICONE][WAVE][BASE], noneFrequency),
          getDelay(displacements[SILICONE][WAVE][HALF], displacements[SILICONE][WAVE][BASE], noneFrequency)}},

        {{getDelay(displacements[SILICONE][TWIST][FULL], displacements[SILICONE][TWIST][BASE], highFrequency),
          getDelay(displacements[SILICONE][TWIST][HALF], displacements[SILICONE][TWIST][BASE], highFrequency)},
         {getDelay(displacements[SILICONE][TWIST][FULL], displacements[SILICONE][TWIST][BASE], lowFrequency),
          getDelay(displacements[SILICONE][TWIST][HALF], displacements[SILICONE][TWIST][BASE], lowFrequency)},
         {getDelay(displacements[SILICONE][TWIST][FULL], displacements[SILICONE][TWIST][BASE], noneFrequency),
          getDelay(displacements[SILICONE][TWIST][HALF], displacements[SILICONE][TWIST][BASE], noneFrequency)}},

        {{getDelay(displacements[SILICONE][ELONGATE][FULL], displacements[SILICONE][ELONGATE][BASE], highFrequency),
          getDelay(displacements[SILICONE][ELONGATE][HALF], displacements[SILICONE][ELONGATE][BASE], highFrequency)},
         {getDelay(displacements[SILICONE][ELONGATE][FULL], displacements[SILICONE][ELONGATE][BASE], lowFrequency),
          getDelay(displacements[SILICONE][ELONGATE][HALF], displacements[SILICONE][ELONGATE][BASE], lowFrequency)},
         {getDelay(displacements[SILICONE][ELONGATE][FULL], displacements[SILICONE][ELONGATE][BASE], noneFrequency),
          getDelay(displacements[SILICONE][ELONGATE][HALF], displacements[SILICONE][ELONGATE][BASE], noneFrequency)}}},
    //FABRIC
    {
        {{getDelay(displacements[FABRIC][BEND][FULL], displacements[FABRIC][BEND][BASE], highFrequency),
          getDelay(displacements[FABRIC][BEND][HALF], displacements[FABRIC][BEND][BASE], highFrequency)},
         {getDelay(displacements[FABRIC][BEND][FULL], displacements[FABRIC][BEND][BASE], lowFrequency),
          getDelay(displacements[FABRIC][BEND][HALF], displacements[FABRIC][BEND][BASE], lowFrequency)},
         {getDelay(displacements[FABRIC][BEND][FULL], displacements[FABRIC][BEND][BASE], noneFrequency),
          getDelay(displacements[FABRIC][BEND][HALF], displacements[FABRIC][BEND][BASE], noneFrequency)}},

        {{getDelay(displacements[FABRIC][WAVE][FULL], displacements[FABRIC][WAVE][BASE], highFrequency),
          getDelay(displacements[FABRIC][WAVE][HALF], displacements[FABRIC][WAVE][BASE], highFrequency)},
         {getDelay(displacements[FABRIC][WAVE][FULL], displacements[FABRIC][WAVE][BASE], lowFrequency),
          getDelay(displacements[FABRIC][WAVE][HALF], displacements[FABRIC][WAVE][BASE], lowFrequency)},
         {getDelay(displacements[FABRIC][WAVE][FULL], displacements[FABRIC][WAVE][BASE], noneFrequency),
          getDelay(displacements[FABRIC][WAVE][HALF], displacements[FABRIC][WAVE][BASE], noneFrequency)}},

        {{getDelay(displacements[FABRIC][TWIST][FULL], displacements[FABRIC][TWIST][BASE], highFrequency),
          getDelay(displacements[FABRIC][TWIST][HALF], displacements[FABRIC][TWIST][BASE], highFrequency)},
         {getDelay(displacements[FABRIC][TWIST][FULL], displacements[FABRIC][TWIST][BASE], lowFrequency),
          getDelay(displacements[FABRIC][TWIST][HALF], displacements[FABRIC][TWIST][BASE], lowFrequency)},
         {getDelay(displacements[FABRIC][TWIST][FULL], displacements[FABRIC][TWIST][BASE], noneFrequency),
          getDelay(displacements[FABRIC][TWIST][HALF], displacements[FABRIC][TWIST][BASE], noneFrequency)}},

        {{getDelay(displacements[FABRIC][ELONGATE][FULL], displacements[FABRIC][ELONGATE][BASE], highFrequency),
          getDelay(displacements[FABRIC][ELONGATE][HALF], displacements[FABRIC][ELONGATE][BASE], highFrequency)},
         {getDelay(displacements[FABRIC][ELONGATE][FULL], displacements[FABRIC][ELONGATE][BASE], lowFrequency),
          getDelay(displacements[FABRIC][ELONGATE][HALF], displacements[FABRIC][ELONGATE][BASE], lowFrequency)},
         {getDelay(displacements[FABRIC][ELONGATE][FULL], displacements[FABRIC][ELONGATE][BASE], noneFrequency),
          getDelay(displacements[FABRIC][ELONGATE][HALF], displacements[FABRIC][ELONGATE][BASE], noneFrequency)}}},

    //LEATHER
    {
        {{getDelay(displacements[LEATHER][BEND][FULL], displacements[LEATHER][BEND][BASE], highFrequency),
          getDelay(displacements[LEATHER][BEND][HALF], displacements[LEATHER][BEND][BASE], highFrequency)},
         {getDelay(displacements[LEATHER][BEND][FULL], displacements[LEATHER][BEND][BASE], lowFrequency),
          getDelay(displacements[LEATHER][BEND][HALF], displacements[LEATHER][BEND][BASE], lowFrequency)},
         {getDelay(displacements[LEATHER][BEND][FULL], displacements[LEATHER][BEND][BASE], noneFrequency),
          getDelay(displacements[LEATHER][BEND][HALF], displacements[LEATHER][BEND][BASE], noneFrequency)}},

        {{getDelay(displacements[LEATHER][WAVE][FULL], displacements[LEATHER][WAVE][BASE], highFrequency),
          getDelay(displacements[LEATHER][WAVE][HALF], displacements[LEATHER][WAVE][BASE], highFrequency)},
         {getDelay(displacements[LEATHER][WAVE][FULL], displacements[LEATHER][WAVE][BASE], lowFrequency),
          getDelay(displacements[LEATHER][WAVE][HALF], displacements[LEATHER][WAVE][BASE], lowFrequency)},
         {getDelay(displacements[LEATHER][WAVE][FULL], displacements[LEATHER][WAVE][BASE], noneFrequency),
          getDelay(displacements[LEATHER][WAVE][HALF], displacements[LEATHER][WAVE][BASE], noneFrequency)}},

        {{getDelay(displacements[LEATHER][TWIST][FULL], displacements[LEATHER][TWIST][BASE], highFrequency),
          getDelay(displacements[LEATHER][TWIST][HALF], displacements[LEATHER][TWIST][BASE], highFrequency)},
         {getDelay(displacements[LEATHER][TWIST][FULL], displacements[LEATHER][TWIST][BASE], lowFrequency),
          getDelay(displacements[LEATHER][TWIST][HALF], displacements[LEATHER][TWIST][BASE], lowFrequency)},
         {getDelay(displacements[LEATHER][TWIST][FULL], displacements[LEATHER][TWIST][BASE], noneFrequency),
          getDelay(displacements[LEATHER][TWIST][HALF], displacements[LEATHER][TWIST][BASE], noneFrequency)}},

        {{getDelay(displacements[LEATHER][ELONGATE][FULL], displacements[LEATHER][ELONGATE][BASE], highFrequency),
          getDelay(displacements[LEATHER][ELONGATE][HALF], displacements[LEATHER][ELONGATE][BASE], highFrequency)},
         {getDelay(displacements[LEATHER][ELONGATE][FULL], displacements[LEATHER][ELONGATE][BASE], lowFrequency),
          getDelay(displacements[LEATHER][ELONGATE][HALF], displacements[LEATHER][ELONGATE][BASE], lowFrequency)},
         {getDelay(displacements[LEATHER][ELONGATE][FULL], displacements[LEATHER][ELONGATE][BASE], noneFrequency),
          getDelay(displacements[LEATHER][ELONGATE][HALF], displacements[LEATHER][ELONGATE][BASE], noneFrequency)}}},

    //METAL
    {
        {{getDelay(displacements[METAL][BEND][FULL], displacements[METAL][BEND][BASE], highFrequency),
          getDelay(displacements[METAL][BEND][HALF], displacements[METAL][BEND][BASE], highFrequency)},
         {getDelay(displacements[METAL][BEND][FULL], displacements[METAL][BEND][BASE], lowFrequency),
          getDelay(displacements[METAL][BEND][HALF], displacements[METAL][BEND][BASE], lowFrequency)},
         {getDelay(displacements[METAL][BEND][FULL], displacements[METAL][BEND][BASE], noneFrequency),
          getDelay(displacements[METAL][BEND][HALF], displacements[METAL][BEND][BASE], noneFrequency)}},

        {{getDelay(displacements[METAL][WAVE][FULL], displacements[METAL][WAVE][BASE], highFrequency),
          getDelay(displacements[METAL][WAVE][HALF], displacements[METAL][WAVE][BASE], highFrequency)},
         {getDelay(displacements[METAL][WAVE][FULL], displacements[METAL][WAVE][BASE], lowFrequency),
          getDelay(displacements[METAL][WAVE][HALF], displacements[METAL][WAVE][BASE], lowFrequency)},
         {getDelay(displacements[METAL][WAVE][FULL], displacements[METAL][WAVE][BASE], noneFrequency),
          getDelay(displacements[METAL][WAVE][HALF], displacements[METAL][WAVE][BASE], noneFrequency)}},

        {{getDelay(displacements[METAL][TWIST][FULL], displacements[METAL][TWIST][BASE], highFrequency),
          getDelay(displacements[METAL][TWIST][HALF], displacements[METAL][TWIST][BASE], highFrequency)},
         {getDelay(displacements[METAL][TWIST][FULL], displacements[METAL][TWIST][BASE], lowFrequency),
          getDelay(displacements[METAL][TWIST][HALF], displacements[METAL][TWIST][BASE], lowFrequency)},
         {getDelay(displacements[METAL][TWIST][FULL], displacements[METAL][TWIST][BASE], noneFrequency),
          getDelay(displacements[METAL][TWIST][HALF], displacements[METAL][TWIST][BASE], noneFrequency)}},

        {{getDelay(displacements[METAL][ELONGATE][FULL], displacements[METAL][ELONGATE][BASE], highFrequency),
          getDelay(displacements[METAL][ELONGATE][HALF], displacements[METAL][ELONGATE][BASE], highFrequency)},
         {getDelay(displacements[METAL][ELONGATE][FULL], displacements[METAL][ELONGATE][BASE], lowFrequency),
          getDelay(displacements[METAL][ELONGATE][HALF], displacements[METAL][ELONGATE][BASE], lowFrequency)},
         {getDelay(displacements[METAL][ELONGATE][FULL], displacements[METAL][ELONGATE][BASE], noneFrequency),
          getDelay(displacements[METAL][ELONGATE][HALF], displacements[METAL][ELONGATE][BASE], noneFrequency)}}},
};

uint32_t steps[4][4][2] = {
    //SILICONE
    {
        {(uint32_t)displacements[SILICONE][BEND][FULL] * stepsPerMililiter, (uint32_t)displacements[SILICONE][BEND][HALF] * stepsPerMililiter},
        {(uint32_t)displacements[SILICONE][WAVE][FULL] * stepsPerMililiter, (uint32_t)displacements[SILICONE][WAVE][HALF] * stepsPerMililiter},
        {(uint32_t)displacements[SILICONE][TWIST][FULL] * stepsPerMililiter, (uint32_t)displacements[SILICONE][TWIST][HALF] * stepsPerMililiter},
        {(uint32_t)displacements[SILICONE][ELONGATE][FULL] * stepsPerMililiter, (uint32_t)displacements[SILICONE][ELONGATE][HALF] * stepsPerMililiter},
    },
    //FABRIC
    {
        {(uint32_t)displacements[FABRIC][BEND][FULL] * stepsPerMililiter, (uint32_t)displacements[FABRIC][BEND][HALF] * stepsPerMililiter},
        {(uint32_t)displacements[FABRIC][WAVE][FULL] * stepsPerMililiter, (uint32_t)displacements[FABRIC][WAVE][HALF] * stepsPerMililiter},
        {(uint32_t)displacements[FABRIC][TWIST][FULL] * stepsPerMililiter, (uint32_t)displacements[FABRIC][TWIST][HALF] * stepsPerMililiter},
        {(uint32_t)displacements[FABRIC][ELONGATE][FULL] * stepsPerMililiter, (uint32_t)displacements[FABRIC][ELONGATE][HALF] * stepsPerMililiter},
    },
    //FABRIC
    {
        {(uint32_t)displacements[LEATHER][BEND][FULL] * stepsPerMililiter, (uint32_t)displacements[LEATHER][BEND][HALF] * stepsPerMililiter},
        {(uint32_t)displacements[LEATHER][WAVE][FULL] * stepsPerMililiter, (uint32_t)displacements[LEATHER][WAVE][HALF] * stepsPerMililiter},
        {(uint32_t)displacements[LEATHER][TWIST][FULL] * stepsPerMililiter, (uint32_t)displacements[LEATHER][TWIST][HALF] * stepsPerMililiter},
        {(uint32_t)displacements[LEATHER][ELONGATE][FULL] * stepsPerMililiter, (uint32_t)displacements[LEATHER][ELONGATE][HALF] * stepsPerMililiter},
    },
    //METAL
    {
        {(uint32_t)displacements[METAL][BEND][FULL] * stepsPerMililiter, (uint32_t)displacements[METAL][BEND][HALF] * stepsPerMililiter},
        {(uint32_t)displacements[METAL][WAVE][FULL] * stepsPerMililiter, (uint32_t)displacements[METAL][WAVE][HALF] * stepsPerMililiter},
        {(uint32_t)displacements[METAL][TWIST][FULL] * stepsPerMililiter, (uint32_t)displacements[METAL][TWIST][HALF] * stepsPerMililiter},
        {(uint32_t)displacements[METAL][ELONGATE][FULL] * stepsPerMililiter, (uint32_t)displacements[METAL][ELONGATE][HALF] * stepsPerMililiter},
    },
};

uint32_t getPause(int totalDisplacement, int baseDisplacement, float frequency)
{
  int shapeDisplacement = totalDisplacement - baseDisplacement;
  uint32_t steps = (uint32_t)shapeDisplacement * stepsPerMililiter;
  uint32_t movementDuration = (steps * 2 * getDelay(totalDisplacement, baseDisplacement, frequency) * 2);
  uint32_t pause = animationDurationInMicroseconds - movementDuration;
  return pause;
}

uint32_t pauses[4][4][3][2] = {
    //SILICONE
    {
        {{0, 0},
         {0, 0},
         {getPause(displacements[SILICONE][BEND][FULL], displacements[SILICONE][BEND][BASE], noneFrequency), getPause(displacements[SILICONE][BEND][HALF], displacements[SILICONE][BEND][BASE], noneFrequency)}},
        {{0, 0},
         {0, 0},
         {getPause(displacements[SILICONE][WAVE][FULL], displacements[SILICONE][WAVE][BASE], noneFrequency), getPause(displacements[SILICONE][WAVE][HALF], displacements[SILICONE][BEND][BASE], noneFrequency)}},
        {{0, 0},
         {0, 0},
         {getPause(displacements[SILICONE][TWIST][FULL], displacements[SILICONE][TWIST][BASE], noneFrequency), getPause(displacements[SILICONE][TWIST][HALF], displacements[SILICONE][TWIST][BASE], noneFrequency)}},
        {{0, 0},
         {0, 0},
         {getPause(displacements[SILICONE][ELONGATE][FULL], displacements[SILICONE][ELONGATE][BASE], noneFrequency), getPause(displacements[SILICONE][ELONGATE][HALF], displacements[SILICONE][ELONGATE][BASE], noneFrequency)}},
    },
    //FABRIC
    {
        {{0, 0},
         {0, 0},
         {getPause(displacements[FABRIC][BEND][FULL], displacements[FABRIC][BEND][BASE], noneFrequency), getPause(displacements[FABRIC][BEND][HALF], displacements[FABRIC][BEND][BASE], noneFrequency)}},
        {{0, 0},
         {0, 0},
         {getPause(displacements[FABRIC][WAVE][FULL], displacements[FABRIC][WAVE][BASE], noneFrequency), getPause(displacements[FABRIC][WAVE][HALF], displacements[FABRIC][BEND][BASE], noneFrequency)}},
        {{0, 0},
         {0, 0},
         {getPause(displacements[FABRIC][TWIST][FULL], displacements[FABRIC][TWIST][BASE], noneFrequency), getPause(displacements[FABRIC][TWIST][HALF], displacements[FABRIC][TWIST][BASE], noneFrequency)}},
        {{0, 0},
         {0, 0},
         {getPause(displacements[FABRIC][ELONGATE][FULL], displacements[FABRIC][ELONGATE][BASE], noneFrequency), getPause(displacements[FABRIC][ELONGATE][HALF], displacements[FABRIC][ELONGATE][BASE], noneFrequency)}},
    },
    //LEATHER
    {
        {{0, 0},
         {0, 0},
         {getPause(displacements[LEATHER][BEND][FULL], displacements[LEATHER][BEND][BASE], noneFrequency), getPause(displacements[LEATHER][BEND][HALF], displacements[LEATHER][BEND][BASE], noneFrequency)}},
        {{0, 0},
         {0, 0},
         {getPause(displacements[LEATHER][WAVE][FULL], displacements[LEATHER][WAVE][BASE], noneFrequency), getPause(displacements[LEATHER][WAVE][HALF], displacements[LEATHER][BEND][BASE], noneFrequency)}},
        {{0, 0},
         {0, 0},
         {getPause(displacements[LEATHER][TWIST][FULL], displacements[LEATHER][TWIST][BASE], noneFrequency), getPause(displacements[LEATHER][TWIST][HALF], displacements[LEATHER][TWIST][BASE], noneFrequency)}},
        {{0, 0},
         {0, 0},
         {getPause(displacements[LEATHER][ELONGATE][FULL], displacements[LEATHER][ELONGATE][BASE], noneFrequency), getPause(displacements[LEATHER][ELONGATE][HALF], displacements[LEATHER][ELONGATE][BASE], noneFrequency)}},
    },
    //METAL
    {
        {{0, 0},
         {0, 0},
         {getPause(displacements[METAL][BEND][FULL], displacements[METAL][BEND][BASE], noneFrequency), getPause(displacements[METAL][BEND][HALF], displacements[METAL][BEND][BASE], noneFrequency)}},
        {{0, 0},
         {0, 0},
         {getPause(displacements[METAL][WAVE][FULL], displacements[METAL][WAVE][BASE], noneFrequency), getPause(displacements[METAL][WAVE][HALF], displacements[METAL][BEND][BASE], noneFrequency)}},
        {{0, 0},
         {0, 0},
         {getPause(displacements[METAL][TWIST][FULL], displacements[METAL][TWIST][BASE], noneFrequency), getPause(displacements[METAL][TWIST][HALF], displacements[METAL][TWIST][BASE], noneFrequency)}},
        {{0, 0},
         {0, 0},
         {getPause(displacements[METAL][ELONGATE][FULL], displacements[METAL][ELONGATE][BASE], noneFrequency), getPause(displacements[METAL][ELONGATE][HALF], displacements[METAL][ELONGATE][BASE], noneFrequency)}},
    },
};

uint32_t baseSteps[4][4] = {
    //SILICONE
    {(uint32_t)displacements[SILICONE][BEND][BASE] * stepsPerMililiter,
     (uint32_t)displacements[SILICONE][WAVE][BASE] * stepsPerMililiter,
     (uint32_t)displacements[SILICONE][TWIST][BASE] * stepsPerMililiter,
     (uint32_t)displacements[SILICONE][ELONGATE][BASE] * stepsPerMililiter},
    //FABRIC
    {(uint32_t)displacements[FABRIC][BEND][BASE] * stepsPerMililiter,
     (uint32_t)displacements[FABRIC][WAVE][BASE] * stepsPerMililiter,
     (uint32_t)displacements[FABRIC][TWIST][BASE] * stepsPerMililiter,
     (uint32_t)displacements[FABRIC][ELONGATE][BASE] * stepsPerMililiter},
    //LEATHER
    {(uint32_t)displacements[LEATHER][BEND][BASE] * stepsPerMililiter,
     (uint32_t)displacements[LEATHER][WAVE][BASE] * stepsPerMililiter,
     (uint32_t)displacements[LEATHER][TWIST][BASE] * stepsPerMililiter,
     (uint32_t)displacements[LEATHER][ELONGATE][BASE] * stepsPerMililiter},
    //METAL
    {(uint32_t)displacements[METAL][BEND][BASE] * stepsPerMililiter,
     (uint32_t)displacements[METAL][WAVE][BASE] * stepsPerMililiter,
     (uint32_t)displacements[METAL][TWIST][BASE] * stepsPerMililiter,
     (uint32_t)displacements[METAL][ELONGATE][BASE] * stepsPerMililiter},
};

int materialStateCount = 4;
int shapeStateCount = 4;
int frequencyStateCount = 3;
int amplitudeStateCount = 2;

Button materialButton(MATERIAL_SWITCH_PIN);
Button shapeButton(SHAPE_SWITCH_PIN);
Button frequencyButton(FREQUENCY_SWITCH_PIN);
Button amplitudeButton(AMPLITUDE_SWITCH_PIN);
Button runButton(RUN_BUTTON_PIN);
Button pullButton(PULL_BUTTON_PIN);
Button pushButton(PUSH_BUTTON_PIN);

ShiftRegister74HC595<2> ledRegister(DATA_PIN, SHIFT_PIN, STORE_PIN);

void setup()
{
  Serial.begin(9600);

  delay(500);

  // for (int mat = 0; mat < 4; mat++)
  // {
  //   for (int shape = 0; shape < 4; shape++)
  //   {
  //     // for (int freq = 0; freq < 3; freq++)
  //     // {
  //     for (int amp = 0; amp < 2; amp++)
  //     {
  //       getPause(displacements[mat][shape][amp], displacements[mat][shape][BASE], noneFrequency);
  //     }
  //     // }
  //   }
  // }

  int siliconeBendFullShapeDisplacement = displacements[SILICONE][BEND][FULL] - displacements[SILICONE][BEND][BASE];
  int fabricBendFullShapeDisplacement = displacements[FABRIC][BEND][FULL] - displacements[FABRIC][BEND][BASE];
  int leatherBendFullShapeDisplacement = displacements[LEATHER][BEND][FULL] - displacements[LEATHER][BEND][BASE];
  int metalBendFullShapeDisplacement = displacements[METAL][BEND][FULL] - displacements[METAL][BEND][BASE];
  int biggestBendDisplacement = max(siliconeBendFullShapeDisplacement, max(fabricBendFullShapeDisplacement, max(leatherBendFullShapeDisplacement, metalBendFullShapeDisplacement)));

  int siliconeWaveFullShapeDisplacement = displacements[SILICONE][WAVE][FULL] - displacements[SILICONE][WAVE][BASE];
  int fabricWaveFullShapeDisplacement = displacements[FABRIC][WAVE][FULL] - displacements[FABRIC][WAVE][BASE];
  int leatherWaveFullShapeDisplacement = displacements[LEATHER][WAVE][FULL] - displacements[LEATHER][WAVE][BASE];
  int metalWaveFullShapeDisplacement = displacements[METAL][WAVE][FULL] - displacements[METAL][WAVE][BASE];
  int biggestWaveDisplacement = max(siliconeWaveFullShapeDisplacement, max(fabricWaveFullShapeDisplacement, max(leatherWaveFullShapeDisplacement, metalWaveFullShapeDisplacement)));

  int siliconeTwistFullShapeDisplacement = displacements[SILICONE][TWIST][FULL] - displacements[SILICONE][TWIST][BASE];
  int fabricTwistFullShapeDisplacement = displacements[FABRIC][TWIST][FULL] - displacements[FABRIC][TWIST][BASE];
  int leatherTwistFullShapeDisplacement = displacements[LEATHER][TWIST][FULL] - displacements[LEATHER][TWIST][BASE];
  int metalTwistFullShapeDisplacement = displacements[METAL][TWIST][FULL] - displacements[METAL][TWIST][BASE];
  int biggestTwistDisplacement = max(siliconeTwistFullShapeDisplacement, max(fabricTwistFullShapeDisplacement, max(leatherTwistFullShapeDisplacement, metalTwistFullShapeDisplacement)));

  int siliconeElongateFullShapeDisplacement = displacements[SILICONE][ELONGATE][FULL] - displacements[SILICONE][ELONGATE][BASE];
  int fabricElongateFullShapeDisplacement = displacements[FABRIC][ELONGATE][FULL] - displacements[FABRIC][ELONGATE][BASE];
  int leatherElongateFullShapeDisplacement = displacements[LEATHER][ELONGATE][FULL] - displacements[LEATHER][ELONGATE][BASE];
  int metalElongateFullShapeDisplacement = displacements[METAL][ELONGATE][FULL] - displacements[METAL][ELONGATE][BASE];
  int biggestElongateDisplacement = max(siliconeElongateFullShapeDisplacement, max(fabricElongateFullShapeDisplacement, max(leatherElongateFullShapeDisplacement, metalElongateFullShapeDisplacement)));

  unsigned long biggestAnimationDisplacement = max(max(max(biggestBendDisplacement, biggestWaveDisplacement), biggestTwistDisplacement), biggestElongateDisplacement);
  unsigned long maxShapeSteps = (biggestAnimationDisplacement * stepsPerMililiter) * 2;
  unsigned long maxShapeDurationInMicroseconds = maxShapeSteps * shortestDelay * 2;
  float maxShapeDurationInSeconds = toSeconds(maxShapeDurationInMicroseconds);
  Serial.print("maxShapeDurationInSeconds: ");
  Serial.println(maxShapeDurationInSeconds);
  float highestFrequency = 1.0 / maxShapeDurationInSeconds;

  Serial.print("Biggest andimation displacement: ");
  Serial.println(biggestAnimationDisplacement);

  Serial.print("Highest possible frequency: ");
  Serial.println(highestFrequency);

  setupStepper();
  setupSwitches();
  setLedsByCurrentState();

  pinMode(EMERGENCY_STOP_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(EMERGENCY_STOP_PIN), &disableMotor, RISING);
}

void setupStepper()
{
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, LOW); //deactivate driver
  motorEnabled = false;
  pinMode(DIRECTION_PIN, OUTPUT);
  digitalWrite(DIRECTION_PIN, CW);
  pinMode(STEP_PIN, OUTPUT);
  digitalWrite(STEP_PIN, LOW);
  digitalWrite(ENABLE_PIN, HIGH); //activate driver
  motorEnabled = true;
}

void setupSwitches()
{
  materialButton.begin();
  shapeButton.begin();
  frequencyButton.begin();
  amplitudeButton.begin();
  runButton.begin();
  pullButton.begin();
  pushButton.begin();
}

void setLedsByCurrentState()
{
  setRegisterLeds();
}

void setRegisterLeds()
{
  int firstMaterialRegister = 0;
  int turnedOnMaterialIndex = (currentMaterial + firstMaterialRegister);
  int firstShapeRegister = 4;
  int turnedOnShapeIndex = (currentShape + firstShapeRegister);
  int materialLedRegisterValue = 1 << turnedOnMaterialIndex;
  int shapeLedRegisterValue = 1 << turnedOnShapeIndex;
  uint8_t firstRegisterValue = uint8_t(materialLedRegisterValue + shapeLedRegisterValue);

  int firstFrequencyRegister = 3;
  int turnedOnFrequencyIndex = (currentFrequency + firstFrequencyRegister);
  int firstAmplitudeRegister = 6;
  int turnedOnAmplitudeIndex = (currentAmplitude + firstAmplitudeRegister);
  int frequencyLedRegisterValue = 1 << turnedOnFrequencyIndex;
  int amplitudeLedRegisterValue = 1 << turnedOnAmplitudeIndex;
  uint8_t secondRegisterValue = uint8_t(frequencyLedRegisterValue + amplitudeLedRegisterValue);

  uint8_t registerValues[] = {firstRegisterValue, secondRegisterValue};

  ledRegister.setAll(registerValues);
}

void disableMotor()
{
  Serial.println("STOP");
  digitalWrite(ENABLE_PIN, LOW);
  motorEnabled = false;
}

void loop()
{
  if (materialButton.released())
  {
    currentMaterial = (currentMaterial + 1) % materialStateCount;
  }
  if (shapeButton.released())
  {
    currentShape = (currentShape + 1) % shapeStateCount;
  }
  if (frequencyButton.released())
  {
    currentFrequency = (currentFrequency + 1) % frequencyStateCount;
  }
  if (amplitudeButton.released())
  {
    currentAmplitude = (currentAmplitude + 1) % amplitudeStateCount;
  }
  setLedsByCurrentState();
  if (runButton.released())
  {
    executeConfiguration();
  }

  if (pushButton.read())
  {
    digitalWrite(DIRECTION_PIN, CCW);
    for (int x = 0; x < 100; x++)
    {
      performStep(shortestDelay);
    }
  }

  else if (pullButton.read())
  {
    digitalWrite(DIRECTION_PIN, CW);
    for (int x = 0; x < 100; x++)
    {
      performStep(shortestDelay);
    }
  }
}

void executeConfiguration()
{
  displayExecutionStarted();
  actuateRobot();
  displayExecutionEnded();
}

void displayExecutionStarted()
{
  delay(100);
  turnOffAllLeds();
  delay(100);
  setLedsByCurrentState();
}

void turnOffAllLeds()
{
  uint8_t registerValues[] = {B00000000, B00000000};
  ledRegister.setAll(registerValues);
}

void actuateRobot()
{
  uint32_t shapeSteps = steps[currentMaterial][currentShape][currentAmplitude];
  Serial.print("Steps per direction: ");
  Serial.println(shapeSteps);

  uint32_t currentBaseSteps = baseSteps[currentMaterial][currentShape];
  Serial.print("Base steps: ");
  Serial.println(currentBaseSteps);

  Serial.print("Animation steps per direction: ");
  uint32_t animationSteps = shapeSteps - currentBaseSteps;
  Serial.println(shapeSteps - currentBaseSteps);

  uint32_t currentDelay = delays[currentMaterial][currentShape][currentFrequency][currentAmplitude];
  Serial.print("Delay: ");
  Serial.println(currentDelay);

  uint32_t currentPause = pauses[currentMaterial][currentShape][currentFrequency][currentAmplitude];
  Serial.print("Pause: ");
  Serial.println(currentPause);

  uint32_t stepDuration = (uint32_t)currentDelay * 2;

  uint32_t microsecondsPerDirection = animationSteps * stepDuration;
  Serial.print("µs per direction: ");
  Serial.println(microsecondsPerDirection);

  uint32_t microsecondsPerPeriod = microsecondsPerDirection * 2;
  Serial.print("µs per period: ");
  Serial.println(microsecondsPerPeriod);

  int stepLosingPreventionDelay = 0;

  uint32_t periods = animationDurationInMicroseconds / (microsecondsPerPeriod + 2 * stepLosingPreventionDelay);
  if (currentFrequency == 2)
  {
    periods = 1;
  }
  Serial.print("Periods: ");
  Serial.println(periods);

  uint32_t basePositionDuration = (currentBaseSteps * shortestDelay * 2);
  Serial.print("Base Position Duration: ");
  Serial.println(toSeconds(basePositionDuration));

  uint32_t actualAnimationDuration = (periods * (shapeSteps - currentBaseSteps) * currentDelay * 2 * 2) + currentPause * periods;
  Serial.print("Animation Duration: ");
  Serial.println(toSeconds(actualAnimationDuration));

  uint32_t offsetPause = 0;
  if (currentFrequency != 2)
  {
    uint32_t remainingAnimationTime = animationDurationInMicroseconds - actualAnimationDuration;
    Serial.print("Offset: ");
    Serial.println(toSeconds(remainingAnimationTime));
    offsetPause = (remainingAnimationTime / periods) / 2;
  }

  Serial.print("Pause: ");
  Serial.println(toSeconds(currentPause));

  Serial.print("Offset pause: ");
  Serial.println(offsetPause);

  Serial.print("Animation duration with offset pause: ");
  Serial.println(toSeconds(actualAnimationDuration + offsetPause * periods * 2));

  if (!motorEnabled)
  {
    Serial.println("Not actuating robot due to inactive motor. Restart controller to enable again.");
    return;
  }

  digitalWrite(DIRECTION_PIN, CCW);
  int preventStallingFactor = 2;
  int basePositionDelay = shortestDelay * preventStallingFactor;
  executeRotation(currentBaseSteps, basePositionDelay);

  int periodDurations[periods] = {};
  int directionDurations[periods][3] = {};
  for (uint32_t period = 0; period < periods; period++)
  {
    int start = millis();
    digitalWrite(DIRECTION_PIN, CCW);
    executeRotation(shapeSteps - currentBaseSteps, currentDelay);

    delay(stepLosingPreventionDelay);

    int outwardEnd = millis();
    delay(currentPause / 1000);
    delay(offsetPause / 1000);
    int returnStart = millis();

    digitalWrite(DIRECTION_PIN, CW);
    executeRotation(shapeSteps - currentBaseSteps, currentDelay);

    delay(stepLosingPreventionDelay);
    delay(offsetPause / 1000);
    int end = millis();
    periodDurations[period] = end - start;
    directionDurations[period][0] = outwardEnd - start;
    directionDurations[period][1] = returnStart - outwardEnd;
    directionDurations[period][2] = end - returnStart;
  }

  executeRotation(currentBaseSteps, basePositionDelay);

  int realAnimationDuration = 0;
  for (uint32_t period = 0; period < periods; period++)
  {
    Serial.print("Duration ");
    Serial.print(period + 1);
    Serial.print(". Period: ");
    Serial.print(periodDurations[period]);
    Serial.println("ms.");
    realAnimationDuration += periodDurations[period];
  }

  Serial.print("Total real duration: ");
  Serial.print((float)realAnimationDuration / 1000);
  Serial.println("s");

  for (uint32_t period = 0; period < periods; period++)
  {
    Serial.print("Direction durations ");
    Serial.print(period + 1);
    Serial.print(". Period: ");
    Serial.print(directionDurations[period][0]);
    Serial.print("ms \t");
    Serial.print(directionDurations[period][1]);
    Serial.print("ms \t");
    Serial.print(directionDurations[period][2]);
    Serial.println("ms");
  }
}

void executeRotation(uint32_t steps, int delay)
{
  unsigned long stepStart = micros();
  for (uint32_t x = 0; x < steps; x++)
  {
    performStep(delay);
  }
  unsigned long stepEnd = micros();
  Serial.println((float)(stepEnd - stepStart) / steps);
}

void performStep(int delay)
{
  // digitalWrite(STEP_PIN, HIGH);
  PORTD = PORTD | B00001000; //Set STEP_PIN to HIGH 11111111
  delayMicroseconds(delay);
  // digitalWrite(STEP_PIN, LOW);
  PORTD = PORTD & B11110111; //Set STEP_PIN to LOW
  delayMicroseconds(delay);
}

void displayExecutionEnded()
{
  displayExecutionStarted();
  displayExecutionStarted();
}
