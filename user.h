//Шаблонный класс пользователей. Описаны значения: логин, пароль, имя, статус
//нахождения в сети, история переписки.
//Реализация в заголовочном файле.
//Версия 2.0: учтены рекомендации ментора Андрея Золотых после проверки 
//предыдущей версии. Добавлена инкапсуляция и хеширование паролей.
#pragma once

template <typename T1, typename T2>	class User
{
	T1 password_;

	//Хеширует пароль. Если верно усвоил тему, то необратимо. Реализация на 
	//перспективу, когда будет добавлена функиця сохранение данных в файл.
	auto HashPassword(T1 const& login, T1 const& password)
	{
		T1 hash{};
		for (auto& p : password)
			hash += std::to_string(size_t(1.49 * p * login.size()));
		hash.pop_back();
		for (auto& l : login)
			hash += std::to_string(size_t(3.14 * l));
		return hash;
	}
public:
	T1 login_;
	T1 name_;
	bool online_ = false;
	T2 history_;

	User(T1 const& login, T1 const& password, T1 const& name, bool online) :
		login_(login), password_(HashPassword(login, password)), name_(name), 
		online_(online) {};
	User() = default;
	~User() = default;

	auto get_password() const { return password_; };
	void set_password(T1 const& password) { password_ = HashPassword(login_, 
		password); };
	auto CheckPassword(T1 const& password) { return HashPassword(login_, 
		password); };
};