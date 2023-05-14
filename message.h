//Преобразует входные данные в единую строку 
//с добавлением текущего системного времени.
//Версия 2.0: учтены рекомендации ментора Андрея Золотых после проверки 
//предыдущей версии.
#pragma once
#include <vector>
#include <string>

struct Message
{
	std::string const to_, text_, from_, timeStamp_;

	Message(std::string const &timeStamp, std::string const &from, std::string 
		const &to, std::string const &text);
	~Message() = default;

	auto TimeStamp() -> std::string const;
	auto MessageConstructor() -> std::string const;
};