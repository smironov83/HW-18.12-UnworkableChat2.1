//Версия 2.0: учтены рекомендации ментора Андрея Золотых после проверки 
//предыдущей версии. Добавлена инкапсуляция.
//Версия 2.1: удален класс bad_range, т.к. исключена возможность ошибки, 
//которую он обрабатывал. Исключено совпадение кодов символов заглавных
//латинских символов с кодами нажатия стрелок при использовании функции 
//автодополнения. Установлено ограничение на количество символов в словах
//при сохранении в словаре автодополнения. Реализовано сохранение учетных 
//данных пользователей в едином файле и индивидуально - файлы словаря 
//автодополнения и истории переписки текущего пользователя в индивидуальной
//директории. Реализована конвертация символов кириллицы из sting 1251 в 
//wstring UNICODE и обратно с применением перекодировки для корректной 
//работы приложения с кириллицей на всех поддерживаемых платформах.
#include "command_module.h"
#include <iostream>
#include <conio.h>

//Меню входа. Обрабатывает ввод номера команды, ввод посторонних символов не
//принимается. Предлагает войти в чат, зарегистрироваться в чате, выйти из 
//программы. Возвращает значение true для main - программа запущена.
//При вводе команды выхода - возвращает false и цикл в main завершается
//с последующим завершением работы программы.
auto CommandModule::LoginMenu()->bool
{
	std::cout << std::endl;
	size_t command = 0;
	std::cout << "Для продолжения работы введите номер команды:" << std::endl;
	std::cout << "1 - войти в чат" << std::endl;
	std::cout << "2 - зарегистрироваться в чате:" << std::endl;
	std::cout << "3 - выйти из программы" << std::endl;
	while (!(std::cin >> command) || (std::cin.peek() != '\n') || !(command == 1
		|| command == 2 || command == 3))
	{
		std::cin.clear();
		while (std::cin.get() != '\n');
		std::cout << "Вы ввели неверный номер команды. Пожалуйста, повторите ввод: "
			<< std::endl;
	}
	switch (command)
	{
	case 1:
		users_[currentUser_].online_ = LogIn();
		worksUser_ = users_[currentUser_].online_;
		break;
	case 2:
		Registration();
		break;
	case 3:
		std::cout << std::endl <<
			"Очень жаль, что Вы покидаете Неработающий Чат! Ждём Вашего возвращения!"
			<< std::endl;
		worksChat_ = false;
		autoDict_->Remove(autoDict_);
		delete autoDict_;
		break;
	}
	return worksChat_;
}

//Регистрирует ChatBot'а. Авторизует его в чате со статусом online.
//Инициализирует словарный запас бота. Проверяет наличие файла данных
//пользователей и считывает его, в случае отсутствия - создает новый и 
//записывает данные бота. 
void CommandModule::InitChatBot()
{
	if (!fs::exists("./logs/"))
		fs::create_directories("./logs/");
	if (!fs::exists("./logs/users"))
	{
		U_OFSTREAM file("./logs/users", std::ios::app);
		file.LOCALE;
		if (!file.is_open()) ATTENTION;
		user_ = new User<std::string, std::vector<std::string>> { "ChatBot", 
			"qwerty", "Бот", true };
		file << CONVERT_IN (user_->login_ + " " + user_->get_password () + 
			" " + user_->name_);
		file.close();
		users_.push_back(*user_);
	}
	else
	{
		U_IFSTREAM file("./logs/users");
		file.LOCALE;
		if (!file.is_open()) ATTENTION;
		U_STRING login, password, name;
		user_ = new User<std::string, std::vector<std::string>> {};
		while (!file.eof())
		{
			file >> login >> password >> name;
			user_->login_ = CONVERT_OUT (login);
			user_->set_password (CONVERT_OUT (password));
			user_->name_ = CONVERT_OUT (name);
			user_->online_ = false;
			users_.push_back (*user_);
		}
		users_[0].online_ = true;
	}
	chatBotAnswers_ = { "Здесь кто-то есть?",
		"Совершенно верно! Мои мысли ушли в том же направлении!",
		"В этом и заключался мой коварный замысел!",
		"А в это время в Африке дети голодают!",
		"А у меня есть котик! Неправда ли чудесное животное?",
		"Ну и кто Вы после этого?",
		"А я - Великий и Ужасный ChatBot этого Неработающего чата!",
		"Я Вас не понимаю!",
		"Я не говорю по-русски!" };
	delete user_;
}

//Регистрирует нового пользователя. Ввод данных: логин, пароль и имя.
//Проверяет логин на уникальность по всем зарегистрированным пользователям.
//Слово "exit" исключено из возможных значений логина, т.к. зарезервировано
//для выхода из меню входа в чат. Сохраняет данные пользователя в файл.
//Создает для пользователя отдельную директорию, куда размещает файлы словаря
//автодополнения и истории переписки.
void CommandModule::Registration()
{
	std::cout << std::endl;
	user_ = new User<std::string, std::vector<std::string>>{};
	auto unique = true;
	std::cout << "Введите логин: ";
	do
	{
		std::cin >> user_->login_;
		unique = true;
		for (auto& user : users_)
		{
			if ((user_->login_ == user.login_) || (user_->login_ == "exit"))
			{
				unique = false;
				std::cout << "Логин занят. Введите другой логин: ";
			}
		}
	} while (!(unique));
	std::cout << "Введите пароль: ";
	std::string passEnter;
	std::cin >> passEnter;
	user_->set_password(passEnter);
	std::cout << "Введите Ваше имя: ";
	std::cin >> user_->name_;
	user_->online_ = false;
	users_.push_back(*user_);
	std::cout << "Регистрация успешно завершена. Для продолжения работы войдите "
		"в Неработающий Чат под Вашими учетными данными!" << std::endl;
	fs::create_directory("./logs/" + std::to_string(users_.size() - 1));
	U_OFSTREAM file("./logs/" + std::to_string(users_.size() - 1) + "/history", 
		std::ios::trunc);
	if (!file.is_open()) ATTENTION;
	file.LOCALE;
	file.close();
	file.open("./logs/" + std::to_string(users_.size() - 1) +
		"/dictionary", std::ios::trunc);
	if (!file.is_open()) ATTENTION;
	file.LOCALE;
	file.close();
	file.open("./logs/users", std::ios::trunc);
	if (!file.is_open()) ATTENTION;
	file.LOCALE;
	file << CONVERT_IN("\n" + user_->login_ + " " + user_->get_password() + " " 
		+ user_->name_);
	file.close();
	delete user_;
}
!!!

//Авторизирует пользователя по связке логин/пароль, сверяет по массиву
//зарегистрированных пользователей. Предусмотрен выход в предыдущее меню
//по ключевому слову "exit" на случай, если пользователь забыл учетные данные.
//Возвращает значение "true" для сетевого статуса пользователя и статуса 
//входа авторизованного пользователя в чат для main. Фиксирует значение индекса
//элемента массива, в котором хранятся данные пользователя, выполнившего вход в
// чат. После авторизации предлагает включить режим автодополнения.
auto CommandModule::LogIn() -> bool
{
	std::cout << std::endl;
	user_ = new User<std::string, std::vector<std::string>>{};
	std::cout << "Для входа в Неработающий Чат введите Ваши учетные данные!"
		<< std::endl;
	auto unique = true;
	auto online = true;
	do
	{
		do
		{
			auto counter = 0;
			std::cout << "Введите логин: ";
			std::cin >> user_->login_;
			for (auto& user : users_)
			{
				if (user_->login_ == user.login_)
				{
					unique = false;
					currentUser_ = counter;
				}
				if (unique && (counter == users_.size() - 1) && (user_->login_ != 
					"exit"))
				{
					std::cout << "Логин не зарегистрирован. Повторите ввод или введите "
						"exit для возврата в предыдущее меню." << std::endl;
				}
				if (user_->login_ == "exit")
				{
					unique = false;
					online = false;
				}
				counter++;
			}
		} while (unique);
		if (user_->login_ != "exit")
		{
			std::cout << "Введите пароль: ";
			std::string passEnter;
			std::cin >> passEnter;
			if (user_->CheckPassword(passEnter) != users_[currentUser_].
				get_password())
			{
				std::cout << "Пароль неверный. Повторите ввод логина и пароля или "
					"введите exit для возврата в предыдущее меню." << std::endl;
				unique = true;
			}
		}
	} while (unique);
	delete user_;
	if (!online)
		return online;
	std::cout << std::endl;
	size_t command = 0;
	std::cout << "Для включения режима автодополнения введите 1, иначе - 0" << 
		std::endl;
	while (!(std::cin >> command) || (std::cin.peek() != '\n') || !(command == 0
		|| command == 1))
	{
		std::cin.clear();
		while (std::cin.get() != '\n');
		std::cout << 
			"Вы ввели неверный номер команды. Пожалуйста, повторите ввод: "	
			<< std::endl;
	}
	if (command == 1)
	{
		autocompleteEnable_ = true;
		system("cls");
		std::cout << "Здравствуйте!\n\nВ данной программе реализован поиск в слова"
			"ре вариантов автодополнения по введенному префиксу.\nПрограмма работает"
			" только с символами кириллицы любого регистра.\nБазовый словарь огранич"
			"ен. Все введенные слова на кириллице добавляются в словарь.\nПереключен"
			"ие между доступными вариантами автодополнения по префиксу осуществляетс"
			"я стрелками влево и вправо.\nНаличие такой возможности подсвечивается <"
			"- и ->, соответственно.\nДля выбора текущего варианта автодополнения на"
			"жмите стрелку вверх.\n\nP.S.Не рекомендуется перемещение указателя в ст"
			"орону начала строки от текущего положения.\nНапример, Backspace. Возмож"
			"но неопредленное поведение программы.\nНо это не точно!\n\n";
		system("pause");
	}
	else
		autocompleteEnable_ = false;
	return online;
}

//Меню чата. Обрабатывает ввод номера команды, ввод посторонних символов не
//принимается. Предлагает написать сообщение всем пользователям, выбранному 
//пользователю (личное сообщение), просмотреть профиль выбранного 
//пользователя, либо выйти из аккаунта. Возвращает значение true для main -
//пользователь авторизован, работает вложенный цикл.
//При вводе команды выхода - возвращает false и вложенный цикл в main 
//завершается с последующим возвратом в меню входа.
auto CommandModule::ChatMenu() -> bool
{
	size_t command = 0;
	std::cout << "Выберите действие:" << std::endl;
	std::cout << "1 - написать сообщение всем" << std::endl;
	std::cout << "2 - написать выбранному пользователю:" << std::endl;
	std::cout << "3 - просмотреть профиль выбранного пользователя" << std::endl;
	std::cout << "4 - выйти из чата" << std::endl;
	while (!(std::cin >> command) || (std::cin.peek() != '\n') || !(command == 1
		|| command == 2 || command == 3 || command == 4))
	{
		std::cin.clear();
		while (std::cin.get() != '\n');
		std::cout << 
			"Вы ввели неверный номер комманды. Пожалуйста, повторите ввод: "
			<< std::endl;
	}
	switch (command)
	{
	case 1:
		system("cls");
		MessageToAll();
		break;
	case 2:
		system("cls");
		MessageToUser();
		break;
	case 3:
		system("cls");
		UserInfo();
		break;
	case 4:
		system("cls");
		std::cout << "Вы вышли из чата!" << std::endl;
		worksUser_ = users_[currentUser_].online_ = false;
		break;
	}
	return worksUser_;
}

//Команда отправляет сообщение всем. Сообщение сохраняется у всех 
//зарегистрированных пользователей. Сообщение видит ChatBot и реагирует на 
//него.
void CommandModule::MessageToAll()
{
	PrintHistory();
	std::string text{ "[" + users_[currentUser_].login_ + "]->[All] : " };
	std::cout << text;
	std::cin.ignore();
	if (autocompleteEnable_)
	{
		text = characterInput(text);
		text.erase(0, users_[currentUser_].login_.size() + 12);
		autoDict_->stream_.clear();
	}
	else std::getline(std::cin, text);
	message_ = new Message{ message_->TimeStamp(), users_[currentUser_].login_,
		"All", text };
	for (auto& user : users_)
	{
		user.history_.push_back(message_->MessageConstructor());
	}
	delete message_;
	AnswerChatBot();
}

//Выводит всю историю переписки для текущего пользователя.
void CommandModule::PrintHistory()
{
	system("cls");
	for (auto& user : users_[currentUser_].history_)
		std::cout << user << std::endl;
	if (autocompleteEnable_)
		std::cout << autoDict_->stream_;
}

//Отправляет сообщение выбранному пользователю. После выбора команды предлагает
//выбрать получателя, путем вывода списка всех зарегистрированных 
//пользователей, путем ввода номера пользователя. Реализована обработка ввода,
//исключающая обращение к несуществующему элементу массива. Сохраняет историю
//для текущего пользователя и для получателя. Остальные пользователи не видят
//этого сообщения. ChatBot видит сообщение и реагирует на него, только если
//является получателем. Можно отправить личное сообщение самому себе, при этом
//история сохраняется только для текущего пользователя.
void CommandModule::MessageToUser()
{
	PrintHistory();
	PrintUsers();
	std::cout <<
		"Введите номер пользователя, которому Вы хотите отправить сообщение: ";
	size_t command = 0;
	while (!(std::cin >> command) || (std::cin.peek() != '\n') || (command == 0)
		|| !(command < users_.size() + 1))
	{
		std::cin.clear();
		while (std::cin.get() != '\n');
		std::cout 
			<< "Вы ввели неверный номер комманды. Пожалуйста, повторите ввод: "
			<< std::endl;
	}
	--command;
	PrintHistory();
	std::string text{ "[" + users_[currentUser_].login_ + "]->" 
		+ users_[command].login_ + " : " };
	std::cout << text;
	std::cin.ignore();
	if (autocompleteEnable_)
	{
		text = characterInput(text);
		text.erase(0, users_[currentUser_].login_.size() + 7 
			+ users_[command].login_.size());
		autoDict_->stream_.clear();
	}
	else std::getline(std::cin, text);
	message_ = new Message{ message_->TimeStamp(), users_[currentUser_].login_,
		users_[command].login_, text };
	users_[currentUser_].history_.push_back(message_->MessageConstructor());
	if (command != currentUser_)
	{
		users_[command].history_.push_back(message_->MessageConstructor());
	}
	delete message_;
	if (users_[command].login_ == "ChatBot") { AnswerChatBot(); }
}

//Выводит на экран всех зарегистрированных пользователей с присвоением 
//порядкового номера.
void CommandModule::PrintUsers()
{
	size_t counter = 0;
	for (auto& user : users_)
	{
		++counter;
		std::cout << counter << " - " << user.login_;
		std::cout << (user.online_ ? " (online) " : " (offline) ");
		std::cout << std::endl;
	}
}

//Предлагает просмотреть профиль выбранного пользователя путем ввода 
//порядкового пользователя из выведенного списка. Реализована обработка ввода,
//исключающая обращение к несуществующему элементу массива. Выводит логин, имя 
//и текущий сетевой статус пользователя.
void CommandModule::UserInfo()
{
	PrintUsers();
	std::cout <<
		"Введите номер пользователя, профиль которого Вы хотите посмотреть: ";
	size_t command = 0;
	while (!(std::cin >> command) || (std::cin.peek() != '\n') || (command == 0)
		|| !(command < users_.size() + 1))
	{
		std::cin.clear();
		while (std::cin.get() != '\n');
		std::cout << 
			"Вы ввели неверный номер комманды. Пожалуйста, повторите ввод: "
			<< std::endl;
	}
	std::cout << "Логин: " << users_[--command].login_ << std::endl;
	std::cout << "Имя: " << users_[command].name_ << std::endl;
	std::cout << (users_[command].online_ == true ? "В сети: да" : "В сети: нет")
		<< std::endl;
	std::cout << std::endl;
	system("pause");
}

//Реализует реакцию ChatBot'а на появление новых сообщений. ChatBot отвечает
//на любое новое сообщение в зоне его видимости личным сообщением отправителю.
//Текст сообщения - случайный, из списка фраз массива ответов ChatBot'а.
//Ответ ChatBot'а сохраняется у получателя и отправителя.
void CommandModule::AnswerChatBot()
{
	size_t answer = rand() % chatBotAnswers_.size();
	std::string text;
	message_ = new Message{ message_->TimeStamp(), users_[0].login_,
		users_[currentUser_].login_, text };
	users_[currentUser_].history_.push_back(message_->MessageConstructor());
	users_[0].history_.push_back(message_->MessageConstructor());
	delete message_;
}

//Инициализирует словарь префиксного дерева из файла пользователя
void CommandModule::InitAutoDict()
{
	
//	for (auto& word : words)
	//	autoDict_->Insert(autoDict_, word);
}

auto CommandModule::characterInput(std::string const text)->std::string
{
	autoDict_->stream_ = text;
	do
	{
		//Считывает код нажатой клавиши.
		symbol_ = _getch();
		//Проверяет на наличие расширенного кода нажатой клавиши, в случе true - 
		//считывает повторно.
		if (_kbhit())
			symbol_ = _getch();
		//Switch по коду нажатой клавиши.
		switch (symbol_)
		{
			//Пробел. Сохраняет слово до пробела в дерево. Очищает текущий префикс.
			//Добавляет пробел в stream. Обновляет экран.
		case 32:
			autoDict_->Insert(autoDict_, prefix_);
			prefix_ = {};
			autoDict_->stream_.push_back(' ');
			PrintHistory();
			break;
			//Enter. Сохраняет слово до Enter в дерево. 
			//Очищает текущий префикс. Обновляет экран.
		case 13:
			std::cout << std::endl;
			autoDict_->Insert(autoDict_, prefix_);
			prefix_ = {};
			PrintHistory();
			break;
			//Стрелка вверх. Дополняет префикс до выбранного слова, если префикс не 
			//пустой. Дописывает в stream. Очищает текущий префикс. Обновляет экран.
		case 72:
			if (prefix_ != "")
				autoDict_->stream_ += autoDict_->prefixes_[position_] + " ";
			prefix_ = {};
			PrintHistory();
			break;
			//Стрелка влево. Если есть предыдущий вариант дополнения префикса - 
			//выводит его.
		case 75:
			if (position_ > 0 && autoDict_->prefCount != 0)
			{
				PrintHistory();
				--position_;
				autoDict_->PrintAutocomplete(autoDict_, position_);
			}
			break;
			//Стрелка вправо. Если есть последующий вариант дополнения префикса - 
			//выводит его.
		case 77:
			if (position_ + 1 < autoDict_->prefCount)
			{
				PrintHistory();
				++position_;
				autoDict_->PrintAutocomplete(autoDict_, position_);
			}
			break;
			//Все остальные варианты кода нажатой клавиши. Добавляет в stream.
			//Если символ на кириллице - добавляет к текущему префиксу.
			//Иначе - сохраняет текущий префикс в дереве и очищает текущий префикс.
			//Если текущий префикс не пустой и содержится в дереве - запускает поиск
			//всех слов про префиксу в дереве. По умолчанию выводит первый из 
			//найденных и помечает наличие/отстуствие последующих вариантов.
		default:
			autoDict_->stream_.push_back(symbol_);
			position_ = 0;
			PrintHistory();
			if (symbol_ - 'а' >= -32 && symbol_ - 'а' <= 31)
				prefix_.push_back(symbol_);
			else
			{
				autoDict_->Insert(autoDict_, prefix_);
				prefix_ = "";
			}
			if (autoDict_->Search(autoDict_, prefix_) && prefix_ != "")
			{
				autoDict_->FindAllPrefixes(autoDict_, wordConstructor_, 0, prefix_);
				autoDict_->PrintAutocomplete(autoDict_, position_);
			}
			break;
		}
		//Enter. Выходит из циклического ввода.
	} while (symbol_ != 13);
	return autoDict_->stream_;
}

//Конвертирует строку из string в кодировке 1251 в wstring в кодировке UNICODE
auto CommandModule::Convert1251toUnicode(std::string const& str1251)->std::wstring
{
	std::wstring strUnic;
	for (size_t i = 0; i < str1251.size(); ++i)
	{
		switch (str1251[i])
		{
		case -88:
			strUnic[i] = 1025;
			break;
		case -72:
			strUnic[i] = 1105;
			break;
		default:
			if (str1251[i] >= -64 && str1251[i] <= -1)
				strUnic.push_back(str1251[i] + 1104);
			else
				strUnic.push_back(str1251[i]);
			break;
		}
	}
	return strUnic;
}

//Конвертирует строку из wstring в кодировке UNICODE в string в кодировке 1251
auto CommandModule::ConvertUnicodeto1251(std::wstring const& strUnic)->std::string
{
	std::string str1251;
	for (size_t i = 0; i < strUnic.size(); ++i)
	{
		switch (strUnic[i])
		{
		case 1025:
			str1251[i] = -88;
			break;
		case 1105:
			str1251[i] = -72;
			break;
		default:
			if (strUnic[i] >= 1040 && strUnic[i] <= 1103)
				str1251.push_back(strUnic[i] - 1104);
			else
				str1251.push_back(strUnic[i] + 0);
			break;
		}
	}
	return str1251;
}
