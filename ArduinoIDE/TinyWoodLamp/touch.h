#pragma once
#include <Arduino.h>

boolean trigger(bool &old);

void setup_touch();
void read_touch();
int getTouchPattern();
int getTouchBrightLevel();