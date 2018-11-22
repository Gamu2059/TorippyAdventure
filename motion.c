#include "define.h"

void walk(CD*);
void jump(CD*);

void walk(CD* cd){
	if(cd->fallFlag==FALSE){
		if(cd->direction==RIGHT){
			cd->x+=cd->aX;
			if((cd->vX+=(int)(cd->aX/4))>(int)(cd->aX/4)*3)
				cd->vX=(int)(cd->aX/4)*3;
		}
		else{
			cd->x-=cd->aX;
			if((cd->vX-=(int)(cd->aX/4))<-(int)(cd->aX/4)*3)
				cd->vX=-(int)(cd->aX/4)*3;
		}
	}
	else{
		if(cd->vX*cd->direction<0){
			cd->vX*=-0.5;
			if(cd->vX>-1&&cd->vX<1)
				cd->vX=0;
		}
	}
}
void jump(CD* cd){
	cd->jumpCount++;
	cd->fallTime=0;
	cd->fallFlag=TRUE;
	cd->v0=cd->a0;
	cd->vA=cd->aA;
}

