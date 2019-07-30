#include <FastLED.h>
#define BRIGHTNESS  64
#define COLOR_ORDER GRB

/*======================= colorPallete Declorations =======================*/
#define DATA_PIN1    3
#define NUM_LEDS1    7
#define BRIGHTNESS  64
#define COLOR_ORDER GRB
CRGB strip1[NUM_LEDS1];
#define UPDATES_PER_SECOND 100
CRGBPalette16 currentPalette;
TBlendType    currentBlending;
extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

const int buttonPin1 = 2;
int buttonState1 = 0;

/*======================= noisePlusPalette Declorations =======================*/
#define DATA_PIN2 5
#define LED_TYPE    WS2811
const uint8_t kMatrixWidth = 16;
const uint8_t kMatrixHeight = 16;
#define MAX_DIMENSION ((kMatrixWidth>kMatrixHeight) ? kMatrixWidth : kMatrixHeight)
#define NUM_LEDS2 (kMatrixWidth * kMatrixHeight)
const bool    kMatrixSerpentineLayout = true;
uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;
  if ( kMatrixSerpentineLayout == false) {
    i = (y * kMatrixWidth) + x;
  }
  if ( kMatrixSerpentineLayout == true) {
    if ( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (kMatrixWidth - 1) - x;
      i = (y * kMatrixWidth) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * kMatrixWidth) + x;
    }
  }
  return i;
}

CRGB strip2[kMatrixWidth * kMatrixHeight];
static uint16_t x;
static uint16_t y;
static uint16_t z;
uint16_t speed = 20; // a nice starting speed, mixes well with a scale of 100
uint16_t scale = 311;
uint8_t noise[MAX_DIMENSION][MAX_DIMENSION];

const int buttonPin2 = 4;
int buttonState2 = 0;


/*======================= Arcade Buttons Declorations =======================*/
const int buttonPin3 = 7;
int buttonState3 = 0;
const int buttonPin4 = 8;
int buttonState4 = 0;
const int buttonPin5 = 10;
int buttonState5 = 0;
const int buttonPin6 = 11;
int buttonState6 = 0;
const int buttonPin7 = 12;
int buttonState7 = 0;

/*======================= light Potentiometer Blue Declorations =======================*/
#define DATA_PIN3 6
#define NUM_LEDS3    7
CRGB strip3[NUM_LEDS3];
int potPin1 = A0;
int valOne = 0;

/*======================= light Potentiometer Pink Declorations =======================*/
#define DATA_PIN4 9
#define NUM_LEDS4    7
CRGB strip4[NUM_LEDS4];
int potPin2 = A1;
int val2 = 0;

void setup() {
  Serial.begin(9600);
  delay( 3000 ); // power-up safety delay
  FastLED.setBrightness(  BRIGHTNESS );


  /*======================= colorPallette Declorations =======================*/
  FastLED.addLeds<NEOPIXEL, DATA_PIN1>(strip1, NUM_LEDS1).setCorrection( TypicalLEDStrip );
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;

  /*======================= noiseFunction Declorations =======================*/
  LEDS.addLeds<LED_TYPE, DATA_PIN2, RGB>(strip2 , NUM_LEDS2);
  LEDS.setBrightness(96);
  x = random16();
  y = random16();
  z = random16();

  /*======================= potentiometer Declorations =======================*/
  FastLED.addLeds<NEOPIXEL, DATA_PIN3>(strip3, NUM_LEDS3);
  FastLED.addLeds<NEOPIXEL, DATA_PIN4>(strip4, NUM_LEDS4);

  /*======================= Button Setups =======================*/
  pinMode(buttonPin1, INPUT_PULLUP);  // colorPallette
  pinMode(buttonPin2, INPUT_PULLUP);  // noiseFunction

  // arcade buttons
  pinMode(buttonPin3, INPUT_PULLUP);  // clear
  pinMode(buttonPin4, INPUT_PULLUP);  // red
  pinMode(buttonPin5, INPUT_PULLUP);  // blue
  pinMode(buttonPin6, INPUT_PULLUP);  // green
  pinMode(buttonPin7, INPUT_PULLUP);  // yellow

  /*======================= pot Setup =======================*/
  pinMode(potPin1, INPUT);
  pinMode(potPin2, INPUT);
}

void loop() {

  // read colorPallette pin
  buttonState1 = digitalRead(buttonPin1);
  if (buttonState1 == LOW) {
    colorPalette();
    Serial.println("colorPallette");
  }

  // read noiseFunction pin
  buttonState2 = digitalRead(buttonPin2);
  if (buttonState2 == LOW) {
    noiseFunction();
    Serial.println("noiseFunction");
  }

  // read clear pin
  buttonState3 = digitalRead(buttonPin3);
  if (buttonState3 == LOW) {
    digitalWrite(buttonPin3, HIGH);
    Serial.println("clear");
  }

  // read red pin
  buttonState4 = digitalRead(buttonPin4);
  if (buttonState4 == LOW) {
    digitalWrite(buttonPin4, HIGH);
    Serial.println("red");
  }

  // read blue pin
  buttonState5 = digitalRead(buttonPin5);
  if (buttonState5 == LOW) {
    digitalWrite(buttonPin5, HIGH);
    Serial.println("blue");
  }

  // read green pin
  buttonState6 = digitalRead(buttonPin6);
  if (buttonState6 == LOW) {
    digitalWrite(buttonPin6, HIGH);
    Serial.println("green");
  }

  // read yellow pin
  buttonState7 = digitalRead(buttonPin7);
  if (buttonState7 == LOW) {
    digitalWrite(buttonPin7, HIGH);
    Serial.println("yellow");
  }

  ledClimb();
  ledClimb2();

}

/*======================= ledsClimb Blue =======================*/
void ledClimb() {
  int valOne = analogRead(potPin1);
  int numLedsToLight = map(valOne, 0, 1023, 0, NUM_LEDS3);

  // First, clear the existing led values
  FastLED.clear();
  for (int led = 0; led < numLedsToLight; led++) {
    strip3[led] = CRGB::Red;
  }
  FastLED.show();
}

/*======================= ledsClimb2 Pink =======================*/
void ledClimb2() {
  int val2 = analogRead(potPin2);
  int numLedsToLight = map(val2, 0, 1023, 0, NUM_LEDS4);

  // First, clear the existing led values
  FastLED.clear();
  for (int led = 0; led < numLedsToLight; led++) {
    strip4[led] = CRGB::Green;
  }
  FastLED.show();
}

/*======================= noiseFunction Function =======================*/
void fillnoise8() {
  for (int i = 0; i < MAX_DIMENSION; i++) {
    int ioffset = scale * i;
    for (int j = 0; j < MAX_DIMENSION; j++) {
      int joffset = scale * j;
      noise[i][j] = inoise8(x + ioffset, y + joffset, z);
    }
  }
  z += speed;
}

void noiseFunction() {
  static uint8_t ihue = 0;
  fillnoise8();
  for (int i = 0; i < kMatrixWidth; i++) {
    for (int j = 0; j < kMatrixHeight; j++) {
      strip2[XY(i, j)] = CHSV(noise[j][i], 255, noise[i][j]);
    }
  }
  ihue += 1;
  LEDS.show();
}

/*======================= colorPalette Function =======================*/
void colorPalette() {
  ChangePalettePeriodically();
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */
  FillLEDsFromPaletteColors( startIndex);
  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void FillLEDsFromPaletteColors( uint8_t colorIndex) {
  uint8_t brightness = 255;
  for ( int i = 0; i < NUM_LEDS1; i++) {
    strip1[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }
}

void ChangePalettePeriodically()
{
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;

  if ( lastSecond != secondHand) {
    lastSecond = secondHand;
    if ( secondHand ==  0)  {
      currentPalette = RainbowColors_p;
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 10)  {
      currentPalette = RainbowStripeColors_p;
      currentBlending = NOBLEND;
    }
    if ( secondHand == 15)  {
      currentPalette = RainbowStripeColors_p;
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 20)  {
      SetupPurpleAndGreenPalette();
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 25)  {
      SetupTotallyRandomPalette();
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 30)  {
      SetupBlackAndWhiteStripedPalette();
      currentBlending = NOBLEND;
    }
    if ( secondHand == 35)  {
      SetupBlackAndWhiteStripedPalette();
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 40)  {
      currentPalette = CloudColors_p;
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 45)  {
      currentPalette = PartyColors_p;
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 50)  {
      currentPalette = myRedWhiteBluePalette_p;
      currentBlending = NOBLEND;
    }
    if ( secondHand == 55)  {
      currentPalette = myRedWhiteBluePalette_p;
      currentBlending = LINEARBLEND;
    }
  }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
  for ( int i = 0; i < 16; i++) {
    currentPalette[i] = CHSV( random8(), 255, random8());
  }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
  // 'black out' all 16 palette entries...
  fill_solid( currentPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  currentPalette[0] = CRGB::White;
  currentPalette[4] = CRGB::White;
  currentPalette[8] = CRGB::White;
  currentPalette[12] = CRGB::White;

}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
  CRGB purple = CHSV( HUE_PURPLE, 255, 255);
  CRGB green  = CHSV( HUE_GREEN, 255, 255);
  CRGB black  = CRGB::Black;

  currentPalette = CRGBPalette16(
                     green,  green,  black,  black,
                     purple, purple, black,  black,
                     green,  green,  black,  black,
                     purple, purple, black,  black );
}

const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
  CRGB::Red,
  CRGB::Gray, // 'white' is too bright compared to red and blue
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black
};
