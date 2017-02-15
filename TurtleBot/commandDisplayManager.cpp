#include "commandDisplayManager.h"


std::vector<std::string> commandDisplayManager::getInputFromUser(){
	//sets the viewport to be commandRenderer so text is only drawn there
	SDL_RenderSetViewport(commandRenderer_, commandArea_);
	bool userIsNotDone = true;
	SDL_Event theEvent;
	std::vector<std::string> allUserInputLines;
	std::string currentInputLine;
	int currentlineIndex = 0;
	//causes SDL to start emitting SDL_TextinputEvents
	SDL_StartTextInput();
	//clears the command area
	SDL_RenderFillRect(commandRenderer_, commandArea_);

	while (userIsNotDone){
		while (SDL_PollEvent(&theEvent) != 0){
			if (theEvent.type == SDL_KEYDOWN){
				if (theEvent.key.keysym.sym == SDLK_BACKSPACE){
					if ((currentInputLine.length() == 0) && (currentlineIndex != 0)){
						//goes up a line if there's nothing in currentline
						currentlineIndex -= 1;
						currentInputLine = allUserInputLines.back();
						allUserInputLines.pop_back();
						displayUserInputText(currentInputLine, currentlineIndex);
					}
					else if (currentInputLine.length() >= 1){
						//removes the last character of currenline
						currentInputLine.pop_back();
						//displays the updated currentline
						displayUserInputText(currentInputLine, currentlineIndex);
					}
				}
				else if (theEvent.key.keysym.sym == SDLK_RETURN){
					//adds the line the user was on to the vector
					allUserInputLines.push_back(currentInputLine);
					//resets currentline for the next line
					currentInputLine = "";
					//allows the user to type on the next line
					currentlineIndex += 1;
				}
			}
			else if (theEvent.type == SDL_TEXTINPUT){
				//updates the current line the user's typing on by appending the character
				//associated with the key they just pressed
				currentInputLine += theEvent.text.text;
				//displays the new current line on screen
				displayUserInputText(currentInputLine, currentlineIndex);
			}
			else if (theEvent.type == SDL_MOUSEBUTTONUP){
				SDL_Point mousePosition = { 0, 0 };
				SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
				if (SDL_PointInRect(&mousePosition, runButton_)){
					//puts the line the "cursor" is on in the vector
					allUserInputLines.push_back(currentInputLine);
					//runs the program when the user clicks run
					userIsNotDone = false;
				}
				else if (SDL_PointInRect(&mousePosition, saveButton_)){
					//puts the line the "cursor" is on in the vector
					allUserInputLines.push_back(currentInputLine);
					//saves the file into savedProgram.txt
					ofstream theFile("savedProgram.txt");
					for (int i = 0; i < allUserInputLines.size(); i++){
						string theCommand = allUserInputLines[i];
						theFile << theCommand<< "\n";
					}
					theFile.close();
				}
				else if (SDL_PointInRect(&mousePosition, loadButton_)){
					//loads the program stored in savedProgram.txt
					UserInputLines.clear();
					currentlineIndex = 0;
					ifstream theFile("savedProgram.txt");
					string inputstring;
					//reads through until eof, programs could be of any size
					for (int j = 0; !(theFile.eof()); j++){
						string inputcommand;
						getline(theFile, inputcommand);
						allUserInputLines.push_back(inputcommand);
						//updating currentlineIndex allows the user to add to the program after it has
						//been loaded in
						currentlineIndex += 1;
						displayUserInputText(inputcommand, j);
					}
					theFile.close();
					currentInputLine = "";
				}

			}
		}
	}
	SDL_StopTextInput();
	return allUserInputLines;
}

void commandDisplayManager::displayUserInputText(std::string lineToDisplay, int indexOfLine){
	//puts a space in if lineToDisplay is empty to prevent ttf from throwing an error
	if (lineToDisplay == ""){
		lineToDisplay = " ";
	}
	//creates a surface containing lineToDisplay
	SDL_Surface* textSurface = TTF_RenderText_Solid(commandFont_, lineToDisplay.c_str(), textcolour_);
	//gets the widths and heights of the surface to prevent stretching later
	int textWidth = textSurface->w;
	int textHeight = textSurface->h;
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(commandRenderer_, textSurface);
	//a rectangle that's the size of the text texture and drawn in the correct line position
	SDL_Rect displayLine = { 0, 60 * (indexOfLine), textWidth, textHeight };
	//rectangle used to clear the previous text in this position
	SDL_Rect clearRect = { 0, 60 * (indexOfLine), 400, textHeight };
	SDL_FreeSurface(textSurface);

	SDL_RenderFillRect(commandRenderer_, &clearRect);
	SDL_RenderCopy(commandRenderer_, textTexture, NULL, &displayLine);
	SDL_RenderPresent(commandRenderer_);
}

commandDisplayManager::commandDisplayManager(SDL_Renderer* commandRenderer, SDL_Rect* commandArea, SDL_Rect* runButton, SDL_Rect* saveButton, SDL_Rect* loadButton)
{
	//makes the text colour black
	textcolour_ = { 0, 0, 0 };
	//sets the command font
	commandFont_ = TTF_OpenFont("AGENCYB.TTF", 48);
	commandArea_ = commandArea;
	runButton_ = runButton;
	saveButton_ = saveButton;
	loadButton_ = loadButton;
	commandRenderer_ = commandRenderer;
}

commandDisplayManager::~commandDisplayManager()
{
}
