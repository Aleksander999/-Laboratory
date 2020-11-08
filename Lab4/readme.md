МИНИСТЕРСТВО НАУКИ И ВЫСШЕГО ОБРАЗОВАНИЯ РОССИЙСКОЙ ФЕДЕРАЦИИ  
Федеральное государственное автономное образовательное учреждение высшего образования  
"КРЫМСКИЙ ФЕДЕРАЛЬНЫЙ УНИВЕРСИТЕТ им. В. И. ВЕРНАДСКОГО"  
ФИЗИКО-ТЕХНИЧЕСКИЙ ИНСТИТУТ  
Кафедра компьютерной инженерии и моделирования
<br/><br/>

### Отчёт по лабораторной работе №4<br/> по дисциплине "Программирование"
<br/>

студента 1 курса группы ИВТ-б-0-192(2)
Менжелеев Александр Федорович
направления подготовки 09.03.01 "Информатика и вычислительная техника"  
<br/>

<table>
<tr><td>Научный руководитель<br/> старший преподаватель кафедры<br/> компьютерной инженерии и моделирования</td>
<td>(оценка)</td>
<td>Чабанов В.В.</td>
</tr>
</table>
<br/><br/>

Симферополь, 2019

<br/><br/>

# Иксики-нолики

## Цель

- Закрепить навыки работы с перечислениями
- Закрепить навыки работы с структурами
- Освоить методы составления многофайловых программ

## Постановка задачи

Создать три файла:
- Файл реализующий игру «Иксики-нолики» используя функции описанные в вспомогательном файле;
- Вспомогательный файл. Содержит реализацию основных функций игры;
- Заголовочный файл. Нужен для связи главного и вспомогательного файлов.


## Исходный код программы


### code.cpp

```cpp
#include <iostream>
#include "game/game.h"

using namespace std;

int main()
{
	system("chcp 1251");
	system("cls");
	char userSign;
	cout << "Введите ваш знак для использования на доске[X,O]: ";
	do
	{
		cin >> userSign;
		if (userSign == 'X' || userSign == 'O')
			break;
		cout << "Неверный знак, повторите попытку." << endl;
	} while (true);

	auto gameUnit = initGame(userSign);


	while (!updateGame(&gameUnit)){
		if (gameUnit.isUserTurn)
			userTurn(&gameUnit);
		else 
			botTurn(&gameUnit);
	}

	if (gameUnit.status == USER_WIN)
		cout << "Вы победили!";

	if (gameUnit.status == BOT_WIN)
		cout << "Бот победил!";

	if (gameUnit.status == NOT_WIN)
		cout << "Ничья!";

	system("pause");

	return EXIT_SUCCESS;
}
```

### game.h

```cpp
#pragma once

/* Состояния игры */
enum Status {
	PLAY,            // Игра продолжается
	USER_WIN,        // Игрок победил
	BOT_WIN,         // Бот победил
	NOT_WIN          // Ничья. Победителя нет, но и на поле нет свободной ячейки
};
struct Game {
	char bord[3][3];  // Игровое поле
	bool isUserTurn;  // Чей ход. Если пользователя, то isUserTurn = true
	char userChar;    // Символ которым играет пользователь
	char botChar;     // Символ которым играет бот
	Status status;
};

/**
* Выполняет следующие действия:
*  1. Очищает игровое поле заполняя его пробелами
*  2. Случайным образом определяет чей ход
*  3. Устанавливает символ для Игрока (Задаётся параметром userChar)
*  4. Устанавливает символ для бота
*  5. Возвращает заполненную структуру
*/
Game initGame(char userChar);
/**
* Выполняет следующие действия:
*  1. Очищает экран
*  2. Отображает содержимое игрового поля. Например так (можно по своему):
*         a   b   c
*       -------------
*     1 | O | O | X |
*       -------------
*     2 |   |   |   |
*       -------------
*     3 |   |   | X |
*       -------------
*/
void updateDisplay(const Game game);
/**
* Выполняет ход бота. В выбранную ячейку устанавливается символ которым играет бот.
* Бот должен определять строку, столбец или диагональ в которой у игрока больше всего иксиков/ноликов и ставить туда свой символ. Если на поле ещё нет меток, бот должен ставить свой знак в центр. В остальных случаях бот ходит рандомно.
*/
void botTurn(Game* game);
/**
* Функция отвечает за ход игрока. Если игрок вводит не допустимые
* значения, ругнуться и попросить ввести заново
*/
void userTurn(Game* game);
/**
* Функция определяет как изменилось состояние игры после последнего хода.
* Функция сохраняет новое состояние игры в структуре game и передаёт ход другому
* игроку.
* Функция возвращает true, если есть победитель или ничья, иначе false.
*/
bool updateGame(Game* game);
```


### game.cpp


```cpp
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
			std::cout << "¬ведите N ¤чейки дл¤ выбора строки[1-3]: ";
			std::cin >> userRow;
			--userRow;
			std::cout << "¬ведите N ¤чейки дл¤ выбора столбца[1-3]: ";
			std::cin >> userColumn;
			--userColumn;

			if (userRow < 3 && userRow >= 0 && userColumn < 3 && userColumn >= 0)
				break;

			cout << "¬ведены недопустимые значени¤ дл¤ ¤чейки, повторите попытку";
		} while (true);

		if (game->bord[userRow][userColumn] == BORDSPACE)
			break;
		
		std::cout << "ƒанна¤ ¤чейка уже зан¤та, повторите попытку.";
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
```

## Вывод

В ходе проделанной работы были получены навыки создания многофайловых программ.<br>
Были изучены STL-контейнеры, такой как vector и применено его использование на практике.<br>
Также изучены навыки работы с алгоритмами обработки STL-контейнеров(sort).<br> 
