#pragma once
#include <stdio.h>
#include "SDL.h"
#include "SDL_ttf.h"
#include <vector>
#include <string>
#include <fstream>
#include "Turtle.h"
using namespace std;

class commandDisplayManager
{
private:
	std::vector<std::string> UserInputLines;
	void displayUserInputText(std::string lineToDisplay, int indexOfLine);
	SDL_Renderer* commandRenderer_;
	SDL_Rect* commandArea_;
	SDL_Rect* runButton_;
	SDL_Rect* saveButton_;
	SDL_Rect* loadButton_;
	SDL_Colour textcolour_;
	TTF_Font *commandFont_;
public:
	std::vector<std::string> getInputFromUser();

	commandDisplayManager(SDL_Renderer* commandRenderer, SDL_Rect* commandArea, SDL_Rect* runButton, SDL_Rect* saveButton, SDL_Rect* loadButton);
	~commandDisplayManager();
};

