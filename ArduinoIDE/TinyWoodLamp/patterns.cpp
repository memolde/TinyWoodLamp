#include <Adafruit_NeoPixel.h>
#include "constants.h"
#include "patterns.h"

Adafruit_NeoPixel LEDS;
static uint16_t hue = 0;
static byte brightness = 0;
static float slowhue = 0;

void setBrightness(int level){
	#ifdef debug
	    Serial.print("\t");
    	Serial.print(level); 
	#endif

	switch (level)
	{
	case 0:
		brightness = 30;
		break;
	case 1:
		brightness = 120;
		break;
	case 2:
		brightness = 255;
		break;
	default:
		break;
	}
	LEDS.setBrightness(brightness);
}

void setupPatterns(){
	LEDS = Adafruit_NeoPixel(NUM_LEDS, DATA_PIN, NEO_GRBW + NEO_KHZ800);
	LEDS.begin();
	LEDS.setBrightness(brightness);
	LEDS.show();
}

void displayPattern(int patternNumber){

		switch (patternNumber)
		{
		case 0:
			pattern1();
			break;
		case 1:
			pattern2();
			break;
		case 2:
			pattern3();
			break;
		case 3:
			pattern4();
			break;
		case 98:
			pattern6();
			break;
		case 99:
			pattern7();
			break;
		
		default:
			break;
		}
}


void setBar(int number, uint32_t color){
    LEDS.setPixelColor(number, color);	
}

void pixelh(int num, uint16_t huein,uint16_t offset){
	LEDS.setPixelColor(num,LEDS.gamma32(LEDS.ColorHSV(huein+offset*100,255,255)));
}
void pixelhc(int num, uint16_t huein,uint16_t offset,int white){
	LEDS.setPixelColor(num,LEDS.gamma32(LEDS.ColorHSV(huein+offset*100,255,255)+(white*16777216)));
}
void pixelc(int num, int red, int green, int blue, int white){
	LEDS.setPixelColor(num,LEDS.gamma32(LEDS.Color(red,green,blue,white)));
	LEDS.setPixelColor(num,LEDS.Color(red,green,blue,white));
}

void pattern1(){
	pixelh(0,hue,00);
	pixelh(1,hue,20);
	pixelh(2,hue,40);
	pixelh(3,hue,60);
	pixelh(4,hue,60);
	pixelh(5,hue,40);
	pixelh(6,hue,20);
	pixelh(7,hue,00);
	LEDS.show(); 
	hue += 30;
}
void pattern2(){
	for(int i = 0; i < NUM_LEDS; i++) {
		pixelhc(i,hue,i*72,100);
	}
	LEDS.show(); 
	hue += 10;
}


void pattern3(){
	// A Bar is moving right and left
	int bar = hue / 4;
	for(int i = 0; i < NUM_LEDS; i++) {
		if( ( (bar) %14 == i%8) || ( (bar+7) %14 == (7-i)%8) ) {
			pixelh(i,hue,200);
//			setBar(i,LEDS.ColorHSV(hue+150,255,255));
		}else{
			setBar(i,LEDS.Color(0,0,0,0));
		}
	}
	LEDS.show(); 
	hue += 1;
//	hue = hue % 242; // 12*21
}

void pattern4(){
	// Red and white alternating
	//Red white red white
	int r1;
	int r2;
	int w1;
	int w2;

	float val;
	val = (float)hue*2.0;
	if (val > 65535){
       val = 131071-val;
	}
	val = (float)LEDS.sine8(hue/256)*256.0;
	r1 = ((65535.0-val) * (65535.0-val))/16777216.0L;
	w1 = (val * val)/16777216.0L;
	r2 = w1;
	w2 = r1;

	pixelc(0,r1,0,0,w1);
	pixelc(2,r1,0,0,w1);
	pixelc(4,r1,0,0,w1);
	pixelc(6,r1,0,0,w1);

	pixelc(1,r2,0,0,w2);
	pixelc(3,r2,0,0,w2);
	pixelc(5,r2,0,0,w2);
	pixelc(7,r2,0,0,w2);
	LEDS.show(); 

	hue +=50;
}

void pattern5(){
	/*
	// A Bar is moving through the lamp
//	uint8_t off = -45;
	for(int i = 0; i < NUM_LEDS; i++) {
		if((hue/8)%7==i%7){
			setBar(i,CHSV(hue+150,255,255));
		}else{
			setBar(i,CRGB(0,0,0));
		}
		setBetween(i,CRGB(0,0,0));
	}
	FastLED.show(); 
	hue += 1;
	*/
}

void sunrise(int bar,float h){

	h = max(h,0);
	float hsqrt = ( float(sqrt( h )));
	int red   = ( hsqrt * hsqrt );//* float(16.0) );
//	int downred = ( hsqrt * 16.0 );
	int green = ( h * hsqrt ) / 256 * 16.0;
	int blue  = ( h * h ) / 256.0;
	int white = blue;
	if(h>130){
		red -= (h-130);
		green -= (h-130);
		blue -= (h-130);
	}

	setBar(bar,LEDS.Color(red,green,blue,white));
}

void pattern6(){
	// Sunrise. Afterwards stay as it is.
	float floathue = min(slowhue/6,230);
	sunrise(0,floathue-35);
	sunrise(1,floathue-20);
	sunrise(2,floathue-8);
	sunrise(3,floathue);
	sunrise(4,floathue);
	sunrise(5,floathue-8);
	sunrise(6,floathue-20);
	sunrise(7,floathue-35);
	if(slowhue < 2000){
		slowhue += 10;
	}
	LEDS.show(); 
}
void pattern7(){
	// Sunrise. Afterwards stay as it is.
	float floathue = min(slowhue/6,230);
	sunrise(0,floathue-35);
	sunrise(1,floathue-20);
	sunrise(2,floathue-8);
	sunrise(3,floathue);
	sunrise(4,floathue);
	sunrise(5,floathue-8);
	sunrise(6,floathue-20);
	sunrise(7,floathue-35);
	if(slowhue > 0){
		slowhue -= 20;
	}
	if(slowhue > 1380){ //1150
		slowhue = 1380;
	}
	LEDS.show(); 
}
