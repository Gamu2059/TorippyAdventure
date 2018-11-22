#include "define.h"
#include <math.h>

int getColorInfo(int);
char getGradationInfo(int);
int getPriorityInfo(int);
int getHitInfo(int);
int getCrashInfo(int);
float getGravityMove(float,float,float);
float getParabolaTopTime(float);
float getSqrtDistant(int,int,int,int);

int getColorInfo(int object){
	return (object/10)%10*8+object%10;
}
char getGradationInfo(int object){
	switch((object/100)%10){
		case 0:return ' ';
		case 1:return 'r';
		case 2:return 'x';
		case 3:return '3';
		case 4:return ')';
		default:return ' ';
	}
}
int getPriorityInfo(int object){
	return (object/1000)%10;
}
int getHitInfo(int object){
	return (object/10000)%10;
}
int getCrashInfo(int object){
	return (object/100000)%10;
}
float getGravityMove(float v0,float vA,float fallTime){
	return (ACCEL/2)*fallTime*fallTime+v0*fallTime+vA;
}
float getParabolaTopTime(float v0){
	return (-v0+sqrt(v0*v0))/((ACCEL/2)*2);
}
float getSqrtDistant(int y1,int x1,int y2,int x2){
	return sqrt((y2-y1)*(y2-y1)+(x2-x1)*(x2-x1));
}

