#pragma once
#include <iostream>
#include <stdio.h>
#include "Turtle.h"
#include <string>
#include <map>
#include <vector>
#include <functional>
using namespace std;

class controller
{
public:
	controller(std::vector<string> userInputs, Turtle &theTurtle);
	~controller();
	
	

private:
	Turtle *theTurtle_;
	int iterator_=1;
	map<std::string, void(controller::*)(int)> inputMap;

	void goForward(int distance);
	void turnLeft(int angle);
	void turnRight(int angle);
	void penUp(int nn);
	void penDown(int nn);
	void setIterator(int nn);
	void setSpeed(int theSpeed);
	void addSpeed(int theSpeed);
	void decspeed(int theSpeed);
	void reset(int nn);
	void controller::populateMap();
};
