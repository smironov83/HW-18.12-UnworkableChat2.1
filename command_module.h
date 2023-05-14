//Реализует все функциональные команды, предусмотренные программой.
//Меню входа в чат: регистрация, аутентификация, выход из программы.
//Меню чата: отправка сообщений всем, выбранному пользователю,
//просмотр профиля выбранного пользователя, выход из аккаунта.
//Отправка сообщений. Сохранение истории сообщений. 
//ChatBot - принцип попугая, реагирует на все сообщения в чате, в зоне его
//видимости. Всегда онлайн. Словарный запас ограничен, но доступно расширение 
//через код. Задел на будущего конкурента AI ChatGPT (пока времени не хватило).
//Версия 2.0: учтены рекомендации ментора Андрея Золотых после проверки 
//предыдущей версии. Добавлена функция автодополнения слов по префиксу.
#pragma once
#include "message.h"
#include "user.h"
#include "autocomplete_dictionary.h"
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;
#define ATTENTION std::cout << "Внимание! Файл не открылся!!!" << std::endl

#if defined(_WIN32)
#define SCREEN_CLEAR system("cls")
#define LOCALE imbue(std::locale("rus_RUS.UTF-8"))
#define U_IFSTREAM std::wifstream
#define U_OFSTREAM std::wofstream
#define U_STRING std::wstring
#define CONVERT_IN Convert1251toUnicode
#define CONVERT_OUT ConvertUnicodeto1251
#elif define(_WIN64)
#define SCREEN_CLEAR system("cls")
#define LOCALE imbue(std::locale("rus_RUS.UTF-8"))
#define U_IFSTREAM std::wifstream
#define U_OFSTREAM std::wofstream
#define U_STRING std::wstring
#define CONVERT_IN Convert1251toUnicode
#define CONVERT_OUT ConvertUnicodeto1251
#else 
#define SCREEN_CLEAR system("clear")
#define LOCALE imbue(std::locale("ru_RU.UTF-8"))
#define U_IFSTREAM std::ifstream
#define U_OFSTREAM std::ofstream
#define U_STRING std::string
#define CONVERT_IN
#define CONVERT_OUT
#endif

class CommandModule
{
	std::vector<User<std::string, std::vector<std::string>>> users_;
	User<std::string, std::vector<std::string>>* user_ = nullptr;
	Message* message_ = nullptr;
	std::vector<std::string> chatBotAnswers_;
	size_t currentUser_ = 0;
#if OS_WIND_COMPATIBLE
	AutocompleteDictionary* autoDict_ = new AutocompleteDictionary;
#endif
	char wordConstructor_[100] = {};
	char symbol_ = {};
	size_t position_ = 0;
	std::string prefix_;

	void Registration();
	auto LogIn() -> bool;
	void MessageToAll();
	void MessageToUser();
	void PrintUsers();
	void UserInfo();
	void AnswerChatBot();

public:
	bool worksChat_ = true;
	bool worksUser_ = false;
	bool autocompleteEnable_ = false;

	CommandModule() = default;
	~CommandModule() = default;
	auto LoginMenu() -> bool;
	void InitChatBot();
	auto ChatMenu() -> bool;
	void PrintHistory();
	void InitAutoDict();
	auto characterInput(std::string const text) -> std::string;
	auto Convert1251toUnicode(std::string const& str1251) -> std::wstring;
	auto ConvertUnicodeto1251(std::wstring const& strUnic) -> std::string;
};