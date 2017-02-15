#include "Turtle.h"

Turtle::Turtle(SDL_Renderer* renderer, SDL_Rect* fieldRect)
{
	//constructor
	turtleFieldRect_ = *fieldRect;
	turtleRenderer_ = renderer;
	//puts the turtle in the middle of the fieldRect
	currentPosition_[0] = turtleFieldRect_.w / 2;
	currentPosition_[1] = turtleFieldRect_.h / 2;
	//makes penColour black
	penColour_ = { 0, 0, 0, 0 };
	currentAngle_ = 0;
	turtleTexture_ = loadTurtleTexture("turtle.png");
	drawTurtle();
	lineVertices_.push_back(getTurtleCenter());
}
Turtle::~Turtle()
{
	//destructor
}
void Turtle::drawTurtle(){
	SDL_SetRenderDrawColor(turtleRenderer_, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderSetViewport(turtleRenderer_, &turtleFieldRect_);
	SDL_RenderFillRect(turtleRenderer_, &turtleFieldRect_);
	//converts to int and adds 0.5 so it rounds correctly
	turtleRect_ = { int(currentPosition_[0] + 0.5), int(currentPosition_[1] + 0.5), 50, 50 };
	//renders the turtle at the correct position and angle
	SDL_RenderCopyEx(turtleRenderer_, turtleTexture_, NULL, &turtleRect_, currentAngle_, NULL, SDL_FLIP_NONE);
}
SDL_Texture* Turtle::loadTurtleTexture(std::string path){
	SDL_Texture* TurtleTexture;
	SDL_Surface* TurtleSurface = IMG_Load(path.c_str());
	//needs to be a texture so it can be rotated
	TurtleTexture = SDL_CreateTextureFromSurface(turtleRenderer_, TurtleSurface);
	SDL_FreeSurface(TurtleSurface);
	return TurtleTexture;
}
void Turtle::drawLines(){
	//ensures lines are draw from the turtles center
	SDL_Point TurtleCenter = getTurtleCenter();
	SDL_SetRenderDrawColor(turtleRenderer_, penColour_.r, penColour_.g, penColour_.b, penColour_.a);
	SDL_RenderSetViewport(turtleRenderer_, &turtleFieldRect_);

	//last point the turtle turned
	SDL_Point lastVertex = lineVertices_.back();
	
	//draw line from turtle's current position to the last point the turtle turned
	if (penState_){
		SDL_RenderDrawLine(turtleRenderer_, lastVertex.x, lastVertex.y, TurtleCenter.x, TurtleCenter.y);
	}

	//draw all other lines
	for (int i = 0; i < lineVertices_.size() - 1; i++){
		if ((penStates_[i] == true) && (i != (lineVertices_.size() - 1))){
			//draws a line from a point in lineVertices to the next point in lastVertice
			SDL_RenderDrawLine(turtleRenderer_, lineVertices_[i].x, lineVertices_[i].y, lineVertices_[i + 1].x, lineVertices_[i + 1].y);
		}
	}
}

SDL_Point Turtle::getTurtleCenter(){
	//used so lines are drawn from turtle's center
	SDL_Point center;
	center.x = turtleRect_.x;
	center.y = turtleRect_.y;
	center.x += turtleRect_.w / 2;
	center.y += turtleRect_.h / 2;
	return center;
}

void Turtle::goForward(int distance){
	
		float dx;
		float dy;
		if (currentAngle_ == 0){
			//to prevent divide 0 errors
			dx = 0;
			dy = 1;
		}
		else{
			//gets the vertical and horizontal components of current angle
			dx = sin(((currentAngle_)*M_PI) / 180);
			dy = cos(((currentAngle_)*M_PI) / 180);
		}
	for (int i = 0; i < distance; i++){
		//add the vertical and horizontal components to currentPosition
		currentPosition_[0] += dx;
		currentPosition_[1] -= dy;
		//delay to make turtle appear to move slower
		SDL_Delay(delay_);

		drawTurtle();
		drawLines();
		SDL_RenderPresent(turtleRenderer_);
	}
	penStates_.push_back(penState_);
	lineVertices_.push_back(getTurtleCenter());
	
}
void Turtle::rotate(int angle){
	currentAngle_ += angle;
	drawTurtle();
	drawLines();

	SDL_RenderPresent(turtleRenderer_);
}
void Turtle::setPenState(bool thepenState){
	penState_ = thepenState;
}
void Turtle::setDelay(int delay){
	if (delay < 0){
		delay = 0;
	}
	delay_ = delay;
}
void Turtle::subtractDelay(int delay){
	setDelay(delay_ - delay);
}
void Turtle::addDelay(int delay){
	setDelay(delay_ + delay);
}
void Turtle::resetTurtle(){
	//resets the turtle's position, angle, penstate and lines
	currentPosition_[0] = turtleFieldRect_.w / 2;
	currentPosition_[1] = turtleFieldRect_.h / 2;
	currentAngle_ = 0;
	penState_ = true;
	lineVertices_.clear();
	penStates_.clear();
	drawTurtle();
	SDL_RenderPresent(turtleRenderer_);
	lineVertices_.push_back(getTurtleCenter());
	penStates_.push_back(penState_);
}