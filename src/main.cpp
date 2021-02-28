#include <Arduino.h>
#include <constants.h>


int touchdelay;

#include <touch.h>
#include <patterns.h>


void setup() { 
   #ifdef debug	
	Serial.begin(57600);
	Serial.println("Fesetting");
	#endif
	setupPatterns();
	setup_touch();
}


void loop() { 
	int pattern;
	static int brightLevel = 2;

	#ifdef debug
	  //Serial.print("x");
	#endif
	delay(20);

	if(touchdelay >=TOUCH_CYCLES){ // 1 equals approximately to 50ms
		read_touch();
		pattern = getTouchPattern();
		brightLevel = getTouchBrightLevel();
		touchdelay = 0;
		setBrightness(brightLevel);
	}else{
		touchdelay += 1;
	}

	displayPattern(pattern);

}