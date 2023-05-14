#HW-16.9-UnworkableChat2.0
UnworkableChat - Это итоговый проект по второму модулю курса «Разработчик на C++» от oнлайн-школы SkillFactory.

#  Список команды:
|  Логин        |  Фамилия Имя 
| ------        | ------                                                   
| smironov83    | Миронов Сергей        

`smironov83` - all-in-one

В UnworkableChat реализовано:
- Регистрация новых пользователей
- Авторизация пользователей с использованием связки логин/пароль
- Отображение информации о статусе пользователей
- Создание сообщений и выбор пользователей для отправки
- Отображение истории сообщений

Изменения в версии 2.0:
- Учтены все рекомендации ментора Андрея Золотых по предыдущей версии программы
- Добавлено защищенное хранение паролей (хеширование)
- Возможность включения функции автодополнения по префиксу вводимого слова

> Чтобы пользователь не заскучал, пока его друзья оффлайн, реализован пользователь чат-бот. 
> Его ответы попадают всегда в точку. 
> Он лучше всяких исскуственных интелектов ChatGPT

#  Классы, реализованные в UnworkableChat

| Class                  | Including                                                                                                                                 |
| ------                 | ------                                                                                                                                    |
| User                   | `string login`, `string password`, `string name`, `bool online`, `vector<string> history`                                                 |
| Message                | `string timeStamp`, `string from`, `string to`, `string text`                                                                             |
| CommandModule          | `vector<Users> users`, `Users* user`, `Message* message`, `vector<string> chatBotAnswers`, `size_t currentUser`, `bool worksChat`, `bool worksUser`|
| bad_range              |                                                                                                                                           |
| AutocompleteDictionary | `bool endOfWord`, `vector<string> prefixes`, `int prefCount`, `string stream`                                                             |

#  Не одно отправленное сообщение не потеряется!
