#include "controller.h"
#include "Turtle.h"

void controller::populateMap(){
	//puts the strings and function pointers into the map
	inputMap["FORWARD"] = &controller::goForward;
	inputMap["TURNL"] = &controller::turnLeft;
	inputMap["TURNR"] = &controller::turnRight;
	inputMap["PENUP"] = &controller::penUp;
	inputMap["PENDOWN"] = &controller::penDown;
	inputMap["REPEAT"] = &controller::setIterator;
	inputMap["RESET"] = &controller::reset;
	inputMap["SETSPEED"] = &controller::setSpeed;
	inputMap["ADDSPEED"] = &controller::addSpeed;
	inputMap["DECSPEED"] = &controller::decspeed;
}

controller::controller(std::vector<string> userInputs, Turtle &theTurtle)
{
	theTurtle_ = &theTurtle;

	populateMap();
	std::vector<int> operands;
	std::vector<string> oper;
	
	//iterates through the commands, putting the operator part in oper
	// and the operand part in operands
	for (int j = 0; j < userInputs.size(); j++){
		int spaceIndex;
		spaceIndex = userInputs[j].find_first_of(" ");
		string theCommand = (userInputs[j].substr(0, spaceIndex));
		for (int i=0; i < theCommand.length(); i++){
			theCommand[i] = toupper(theCommand[i]);
		}
		oper.push_back(theCommand);
		try
		{
			operands.push_back(stoi(userInputs[j].substr(spaceIndex + 1)));	
		}
		catch (std::invalid_argument e)
		{
			// dont do anything with e if not an integer
			operands.push_back(0);
		}
		catch (std::out_of_range e)
		{
			// ignore the instruction
			operands.push_back(0);
		}
	}
	for (int j = 0; j < iterator_; j++){
		//loops through the commands an amount of times based on the iterator
		for (int i = 0;i< userInputs.size(); i++){
			if (inputMap.find(oper[i]) != inputMap.end())
			{
				//calls the corresponding function based on the operator
				(this->*inputMap[oper[i]])(operands[i]);
			}
		}
	}
}

controller::~controller()
{
}

void controller::goForward(int distance){
	//calls theTurtle's goforward function
	theTurtle_->goForward(distance);
}
void controller::turnLeft(int angle){
	//rotate turns to the right, so to turn to the left, sub from 360
	int a = 360 - angle;
	theTurtle_->rotate(a);
}
void controller::turnRight(int angle){
	//calls theTurtle's rotate function
	theTurtle_->rotate(angle);
}
void controller::penUp(int nn){
	theTurtle_->setPenState(false);
}
void controller::penDown(int nn){
	theTurtle_->setPenState(true);
}
void controller::setSpeed(int theSpeed){
	//max speed is 50 (no delay) so if above 50, make 50
	if (theSpeed > 50){
		theSpeed = 50;
	}
	else{
		//converts theSpeed into delay
		theSpeed = 50 - theSpeed;
	}
	theTurtle_->setDelay(theSpeed);
}
void controller::addSpeed(int theSpeed){
	theTurtle_->subtractDelay(theSpeed);
}
void controller::decspeed(int theSpeed){
	theTurtle_->addDelay(theSpeed);
}
void controller::setIterator(int amountToRepeatBy){
	iterator_ = amountToRepeatBy;
}
void controller::reset(int nn){
	theTurtle_->resetTurtle();
}