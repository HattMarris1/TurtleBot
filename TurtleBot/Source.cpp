#include <iostream>
#include <stdio.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <vector>
#include <string>
#include "Turtle.h"
#include <map>
#include <functional>
#include "controller.h"
#include "commandDisplayManager.h"
using namespace std;

int SCREEN_WIDTH;
int SCREEN_HEIGHT;
TTF_Font *buttonFont;
SDL_Colour textColour = { 0, 0, 0 };

void init(SDL_Rect commandArea, SDL_Rect runButton, SDL_Rect loadButton, SDL_Rect saveButton);
void drawButton(SDL_Rect button, string buttonText);

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* theImage = NULL;

void drawButton(SDL_Rect button, string buttonText){
	SDL_Surface* textSurface = TTF_RenderText_Solid(buttonFont, buttonText.c_str(), textColour);
	int textWidth = textSurface->w;
	int textHeight = textSurface->h;
	SDL_Texture* texttexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_Rect textrect = { button.x, button.y, textWidth, textHeight };
	SDL_RenderCopy(renderer, texttexture, NULL, &textrect);
}

void init(SDL_Rect commandArea, SDL_Rect runButton, SDL_Rect loadButton, SDL_Rect saveButton){
	
	//initialises SDL_ttf
	int e = TTF_Init();
	//loads the button font
	buttonFont = TTF_OpenFont("AGENCYB.TTF", 48);
	//creates an window 
	window = SDL_CreateWindow("TurtleBot", 0, 30, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	//intialises SDL_IMAGE
	int imageFlags = IMG_INIT_PNG;
	IMG_Init(imageFlags)&imageFlags;

	//Clears the Screen
	SDL_RenderClear(renderer);
	//puts a green rectangle on the right side of the screen, creates the rects for the buttons
    SDL_SetRenderDrawColor(renderer, 128, 255, 128, 255);
	SDL_RenderFillRect(renderer, &commandArea);
	SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
	SDL_RenderFillRect(renderer, &runButton);
	SDL_SetRenderDrawColor(renderer, 255,255 , 0, 255);
	SDL_RenderFillRect(renderer, &loadButton);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &saveButton);

	drawButton(runButton, "run");
	drawButton(loadButton, "load");
	drawButton(saveButton, "save");
	SDL_RenderPresent(renderer);
}


int main(int, char**) {	
	//initialises SDL
	SDL_Init(SDL_INIT_VIDEO);

	//gets the size of the user's screen
	SDL_DisplayMode currentDisplayMode;
	SDL_GetCurrentDisplayMode(0, &currentDisplayMode);
	SCREEN_WIDTH = currentDisplayMode.w;
	SCREEN_HEIGHT = currentDisplayMode.h-75;

	//creates the commandRect, fieldRect and buttons based on the size of the user's screen
	SDL_Rect commandRect = { SCREEN_WIDTH - 400, 0, 400, SCREEN_HEIGHT -150 };
	SDL_Rect fieldRect = { 0, 0, SCREEN_WIDTH - 400, SCREEN_HEIGHT};
	//puts runButtonRect under the commandArea
	SDL_Rect runButtonRect = { commandRect.x, commandRect.h, commandRect.w, 50 };
	//puts saveButtonRect under runButtonRect
	SDL_Rect saveButtonRect = { runButtonRect.x, runButtonRect.y +runButtonRect.h, runButtonRect.w, 50 };
	//puts loadButtonRect under saveButtonRect
	SDL_Rect loadButtonRect = { saveButtonRect.x, saveButtonRect.y+ saveButtonRect.h, saveButtonRect.w, 50 };
	init(commandRect, runButtonRect, loadButtonRect, saveButtonRect);

	std::vector<std::string> commandList;
	Turtle theTurtle(renderer, &fieldRect);
	
	bool quit = false;
	SDL_Event theEvent;
	while (!quit){
		while (SDL_PollEvent(&theEvent)){
			if (theEvent.type == SDL_MOUSEBUTTONUP){
				SDL_Point mousePosition = { 0, 0 };
				SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
				if (SDL_PointInRect(&mousePosition, &commandRect)){
					//changes the box colour when clicked, allows user to enter text
					SDL_RenderSetViewport(renderer, &commandRect);
					SDL_SetRenderDrawColor(renderer, 0, 153, 0, 255);
					SDL_RenderFillRect(renderer, &fieldRect);
					SDL_RenderPresent(renderer);
	
					commandDisplayManager commandDisplay = commandDisplayManager(renderer, &commandRect, &runButtonRect, &saveButtonRect,&loadButtonRect);
					commandList = commandDisplay.getInputFromUser();
					
					controller thecontoller = controller(commandList, theTurtle);
					SDL_RenderSetViewport(renderer, &commandRect);
					SDL_SetRenderDrawColor(renderer, 128, 255, 128, 255);
					SDL_RenderFillRect(renderer, &fieldRect);
					SDL_RenderPresent(renderer);
				}
			}
			if(theEvent.type == SDL_QUIT){
				quit = true;
			}
		}
		SDL_RenderPresent(renderer);
	}
	return 0;
}