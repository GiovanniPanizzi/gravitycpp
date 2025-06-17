#ifndef GLOBALS_H
#define GLOBALS_H
#include "stats.h"

#pragma once
extern int screenWidth;
extern int screenHeight;
extern float scale;
enum objectType {PLANET, METEORITE, ENTITY, TOOL};
extern bool running;
extern Vec2 cameraPosition;
extern Angle cameraAngle;
extern int state1;

#endif