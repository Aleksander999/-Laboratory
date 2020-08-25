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