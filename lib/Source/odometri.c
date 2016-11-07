#include "odometri.h"
double odometryVector(double x, double y){
	double move = 0;
	move = sqrt((x * x) + (y * y));
	return move;
}

double odometryDirection(double x, double y, double angle){
	double move = 0;
	move = sqrt(((x * x) + (y * y)) / 2 * x * y * sin(x * y));
	return move;
}

double odometryAngle(double angle){
	double direction = 0;
	direction += angle;
	return direction;
}

double odometryX(double x){
	double move = 0;
	move += x;
	return move;
}

double odometryY(double y){
	double move = 0;
	move += y;
	return move;
}
