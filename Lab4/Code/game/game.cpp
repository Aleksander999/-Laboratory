#include "game.h"
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>

using namespace std;

const char BORDSPACE	= ' ';
const char BORDX		= 'X';
const char BORDO		= 'O';

typedef char* cell;
typedef cell line[3];

std::random_device rd;
std::mt19937 rng(rd());


Game initGame(char userChar) {

	std::uniform_int_distribution<int> uni(0, 1); 

	Game gameUnit;
	gameUnit.userChar = userChar;
	gameUnit.botChar = userChar == BORDX ? BORDO : BORDX;
	gameUnit.isUserTurn = uni(rng);
	gameUnit.status = PLAY;
	for (size_t i = 0; i < 9; i++)
	{
		*((char*)gameUnit.bord + i) = BORDSPACE;
	}

	updateDisplay(gameUnit);
	return gameUnit;
}

void updateDisplay(const Game game) {
	system("cls");
	std::cout << "    A   B   C   " << endl;
	std::cout << "  --------------" << endl;
	std::cout << "1 | " << game.bord[0][0] << " | " << game.bord[0][1] << " | " << game.bord[0][2] << " |" << endl;
	std::cout << "  --------------" << endl;
	std::cout << "2 | " << game.bord[1][0] << " | " << game.bord[1][1] << " | " << game.bord[1][2] << " |" << endl;
	std::cout << "  --------------" << endl;
	std::cout << "3 | " << game.bord[2][0] << " | " << game.bord[2][1] << " | " << game.bord[2][2] << " |" << endl;
	std::cout << "  --------------" << endl;
}

void userTurn(Game* game) {
	
	short userRow;
	short userColumn;
	do {
		do {
			std::cout << "¬ведите N €чейки дл€ выбора строки[1-3]: ";
			std::cin >> userRow;
			--userRow;
			std::cout << "¬ведите N €чейки дл€ выбора столбца[1-3]: ";
			std::cin >> userColumn;
			--userColumn;

			if (userRow < 3 && userRow >= 0 && userColumn < 3 && userColumn >= 0)
				break;

			cout << "¬ведены недопустимые значени€ дл€ €чейки, повторите попытку";
		} while (true);

		if (game->bord[userRow][userColumn] == BORDSPACE)
			break;
		
		std::cout << "ƒанна€ €чейка уже зан€та, повторите попытку.";
	} while (true);

	game->bord[userRow][userColumn] = game->userChar;
}

void copyLine(line& one, line& two) {
	for (size_t i = 0; i < 3; i++)
	{
		one[i] = two[i];
	}
}

void botTurn(Game* game) {
	line rows[3];
	line columns[3];
	line dias[2];
	
	// Get lines
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			rows[i][j] = &game->bord[i][j];
			columns[i][j] = &game->bord[j][i];
		}
		dias[0][i] = &game->bord[i][i];
		dias[1][i] = &game->bord[i][2 - i];
	}

	struct countedLine {
		short counter;
		line line;
	};

	vector<countedLine> linesArray;


	// Compute shared array

	for (size_t i = 0; i < 3; i++) {
		countedLine newLine{
			0,
			nullptr
		};
		copyLine(newLine.line, rows[i]);
		linesArray.push_back(newLine);
	}

	for (size_t i = 0; i < 3; i++) {
		countedLine newLine{
			0,
			nullptr
		};
		copyLine(newLine.line, columns[i]);
		linesArray.push_back(newLine);
	}

	for (size_t i = 0; i < 2; i++) {
		countedLine newLine{
			0,
			nullptr
		};
		copyLine(newLine.line, dias[i]);
		linesArray.push_back(newLine);
	}

	for (auto &it : linesArray) {
		for (size_t i = 0; i < 3; i++)
			if (*it.line[i] == game->userChar)
				it.counter++;
	}

	// Sort descending
	std::sort(linesArray.begin(), linesArray.end(), [](countedLine one, countedLine two) {
		return one.counter > two.counter;
	});

	// Filter arry for excessive lines
	vector<countedLine> filtredArray;
	for (auto &line: linesArray) {
		short freeCounter = 0;
		for (size_t i = 0; i < 3; i++)
			if (*line.line[i] == BORDSPACE) freeCounter++;

		if (line.counter > 0 && line.counter < 3 && freeCounter)
			filtredArray.push_back(line);
	}


	if (filtredArray.size()) {
		vector<countedLine> bestMatchLines;
		
		// Sort strings with identical counter
		for (auto& line : filtredArray)
			if (line.counter == filtredArray.at(0).counter) bestMatchLines.push_back(line);
		
		
		// Choose random line from priority lines
		std::uniform_int_distribution<int> genLine(0, bestMatchLines.size()-1);

		int lineN = genLine(rng);

		countedLine chosedLine = bestMatchLines.at(lineN);


		// Choose random cell from line
		vector<cell> freeCells;

		for (size_t i = 0; i < 3; i++)
			if (*chosedLine.line[i] == BORDSPACE) freeCells.push_back(chosedLine.line[i]);

		std::uniform_int_distribution<int> genCell(0, freeCells.size() - 1);

		int cellN = genCell(rng);

		cell botCell = freeCells.at(cellN);

		*botCell = game->botChar;
	}
	else {
		game->bord[1][1] = game->botChar;
	}
}

bool updateGame(Game *game){

	updateDisplay(*game);
 
	game->isUserTurn = !game->isUserTurn;


	struct counter_t {
		short user;
		short bot;
	};

	counter_t rows[3] = { 0,0,0 };
	counter_t cols[3] = { 0,0,0 };
	counter_t dias[3] = { 0,0,0 };

	short commonCounter = 9;

	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			if (game->bord[i][j] == game->userChar)
				++rows[i].user;
			if (game->bord[i][j] == game->botChar)
				++rows[i].bot;
			if (game->bord[j][i] == game->userChar)
				++cols[i].user;
			if (game->bord[j][i] == game->botChar)
				++cols[i].bot;

			if (game->bord[i][j] != BORDSPACE)
				--commonCounter;
		}
		if (game->bord[i][i] == game->userChar)
			dias[0].user++;
		if (game->bord[i][i] == game->botChar)
			dias[0].bot++;

		if (game->bord[i][2 - i] == game->userChar)
			dias[1].user++;
		if (game->bord[i][2 - i] == game->botChar)
			dias[1].bot++;
	}

	Status gameStatus = PLAY;

	if (commonCounter == 0)
		gameStatus = NOT_WIN;

	for (size_t i = 0; i < 3; i++) {
		if (rows[i].user == 3) gameStatus = USER_WIN;
		if (rows[i].bot == 3) gameStatus = BOT_WIN;
		if (cols[i].user == 3) gameStatus = USER_WIN;
		if (cols[i].bot == 3) gameStatus = BOT_WIN;
	}

	for (size_t i = 0; i < 2; i++) {
		if (dias[i].user == 3) gameStatus = USER_WIN;
		if (dias[i].bot == 3) gameStatus = BOT_WIN;
	}

	game->status = gameStatus;

	return gameStatus != PLAY;
}