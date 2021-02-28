#include <Arduino.h>
#include <CapacitiveSensor.h>
#include <touch.h>
#include <constants.h>

CapacitiveSensor   cs_1  = CapacitiveSensor(2,4);        // 10M resistor between pins 2 and sensor
CapacitiveSensor   cs_2  = CapacitiveSensor(2,6);    
CapacitiveSensor   cs_3 = CapacitiveSensor(2,8);    

boolean t1;  // Touch status
boolean t2;
boolean t3;

boolean tr1;  // Trigger
boolean tr2;
boolean tr3;

boolean to1; // old(internal)
boolean to2;
boolean to3;

int patternNumber = 98; // Sunrise
int brightLevel = 2;

void setup_touch(){
/*
   cs_1.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
   cs_2.set_CS_AutocaL_Millis(0xFFFFFFFF);
   cs_3.set_CS_AutocaL_Millis(0xFFFFFFFF);
*/
}

boolean trigger(bool &old){
    if(old){
         return false;
    }
    old = true;
    return true;
}

void read_touch(){
   
    long total1 =  cs_1.capacitiveSensor(30);
    long total2 =  cs_2.capacitiveSensor(30);
    long total3 =  cs_3.capacitiveSensor(30);
//    long total2;
//    long total3;

    long total = max(total1,max(total2,total3));
    if(total1<total){total1=0;};
    if(total2<total){total2=0;};
    if(total3<total){total3=0;};

    int treshold = 100;
    t1 = false;
    t2 = false;
    t3 = false;
    tr1 = false;
    tr2 = false;
    tr3 = false;

    if(total1 > treshold){
        tr1 = trigger(to1);
        t1 = true;
    }else{
        to1=false;
    }
    if(total2 > treshold){
        tr2 = trigger(to2);
        t2 = true;
    }else{
        to2=false;
    }
    if(total3 > treshold){
        tr3 = trigger(to3);
        t3 = true;
    }else{
        to3=false;
    }

    #ifdef debug
    
    Serial.print("\t");
    Serial.print(total1);                  // print sensor output 1
    Serial.print("\t");
    Serial.print(total2);                  // print sensor output 2
    Serial.print("\t");
    Serial.print(total3);                // print sensor output 3
    Serial.print("\n");
    
    #endif
}

int getTouchPattern(){
	// Read the global variables tr<x> to see which touchpad was triggered
	if(tr1==true){
    	patternNumber += 1; 
        patternNumber %= 4;
	}
    if(tr2==true){
        if(patternNumber == 99){
            patternNumber = 98;    
        }else{
            patternNumber = 99;
        }
    }
    return patternNumber;
}

int getTouchBrightLevel(){
	if(tr3==true){
    	brightLevel += 1;
        brightLevel %= 3;
	}
    return brightLevel;
}