#include "define.h"
#include <math.h>

int getColorInfo(int object);
int getCrashInfo(int object);
int getDamageHitInfo(int object);
int getAttackHitInfo(int object);
float getGravityMove(PD* pd);
float getParabolaTopTime(PD* pd);

int getColorInfo(int object){
	return object%10;
}
int getCrashInfo(int object){
	return (object/10)%10;
}
int getDamageHitInfo(int object){
	return (object/100)%10;
}
int getAttackHitInfo(int object){
	return (object/1000)%10;
}
float getGravityMove(PD* pd){
	return (ACCEL/2)*pd->fallTime*pd->fallTime+pd->v0*pd->fallTime+pd->vA;
}
float getParabolaTopTime(PD* pd){
	return (-pd->v0+sqrt(pd->v0*pd->v0))/((ACCEL/2)*2);
}

