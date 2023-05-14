//Версия 2.0: учтены рекомендации ментора Андрея Золотых после проверки 
//предыдущей версии. Добавлена инкапсуляция.
#include "message.h"
#include <time.h>

#if defined(_WIN32)
#define OS_WIND_COMPATIBLE true
#elif define(_WIN64)
#define OS_WIND_COMPATIBLE true
#else 
#define	OS_WIND_COMPATIBLE false
#endif

//Конструктор принимает параметры: текущее системное время, логин отправителя,
//идентификатор получателя, текст сообщения
Message::Message(std::string const &timeStamp, std::string const &from,
  std::string const &to, std::string const &text) : timeStamp_(timeStamp),
  from_(from), to_(to), text_(text) {}

//Получает текущее системное время, переформатирует в понятный обычному 
// пользователю вид и возвращает полученное значение в формате string
auto Message::TimeStamp() -> std::string const
{
  const size_t range = 26;
  time_t timeLong = 0;
  char timebuf[range]{};
  time(&timeLong);
#if OS_WIND_COMPATIBLE
  ctime_s(timebuf, range, &timeLong);
#else
  ctime_s(timebuf, range, &timeLong);
#endif
  std::string timeNow = "[";
  for (auto& user : timebuf) { timeNow.push_back(user); }
  timeNow.resize(25);
  timeNow.push_back(']');
  return timeNow;
}

//Объединяет с форматированием входные переменные и возвращает единой строкой.
auto Message::MessageConstructor() -> std::string const
{
  std::string const message = timeStamp_ + " " + from_ + " to " + to_ +
    " : " + text_;
  return message;
}
