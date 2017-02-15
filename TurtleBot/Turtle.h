#pragma once
#include "SDL.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include "SDL_image.h"
#include <vector>
class Turtle
{
private:

	float currentPosition_[2] = {};
	std::vector<SDL_Point> lineVertices_;
	std::vector<bool> penStates_;
	SDL_Rect turtleRect_;
	SDL_Rect turtleFieldRect_;
	double currentAngle_;
	SDL_Texture* turtleTexture_;
	SDL_Renderer* turtleRenderer_;
	bool penState_ = true;
	int delay_;
	SDL_Colour penColour_;

	SDL_Texture* loadTurtleTexture(std::string path);
	void drawCurrentLine(SDL_Point startPos);
	SDL_Point getTurtleCenter();
	void drawLines();
	void drawTurtle();
public:
	void rotate(int angle);
	void goForward(int distance);
	void setPenState(bool thepenState);
	void setDelay(int delay);
	void subtractDelay(int delay);
	void addDelay(int delay);
	void resetTurtle();

	Turtle(SDL_Renderer* renderer, SDL_Rect* fieldRect);
	~Turtle();

	
};

