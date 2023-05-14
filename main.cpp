//Основной блок. Инициализирует переменную классом реализации
//основных команд и функций. Запускает ChatBot'а. Состоит из
//двух циклов. Внутренний работает до получения значения false
//при выходы из меню чата. Внешний до получения false при выходе 
//из меню входа.
//Версия 2.1: добавлена поддержка ОС Linux
#include <stdlib.h>
#include "command_module.h"
#include <iostream>

auto main() -> int
{
#if OS_WIND_COMPATIBLE
	system("chcp 1251");
	std::wcout.LOCALE;
	std::wcin.LOCALE;
#else
	std::locale("ru_RU.UTF-8");
#endif
	SCREEN_CLEAR;
	srand((unsigned int) time(NULL));
	CommandModule unworkableChat;
	unworkableChat.InitChatBot();
	std::cout << "Добро пожаловать в Неработающий Чат!" << std::endl;
	while (unworkableChat.worksChat_)
	{
		unworkableChat.LoginMenu();
		while (unworkableChat.worksUser_)
		{
			unworkableChat.PrintHistory();
			unworkableChat.ChatMenu();
		}
	}
	return 0;
}
