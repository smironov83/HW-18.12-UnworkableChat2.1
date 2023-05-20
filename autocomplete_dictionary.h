//Реализация функции автодополнения слов по введенному префиксу в консоли.
//Работает только с кириллицей.
//Версия 2.1: Ё и ё автоматически приравниваются к Е и и, что сокращает 
//количество операций и переменных, а также исключает ошибки, связанные 
//с размещением символов кириллицы в разных кодировках.
#pragma once

#if defined(_WIN32)
#define OS_WIND_COMPATIBLE true
#elif defined(_WIN64)
#define OS_WIND_COMPATIBLE true
#else 
#define	OS_WIND_COMPATIBLE false
#endif

#include <string>
#include <vector>

#if OS_WIND_COMPATIBLE
const size_t ALPHABET_SIZE = 32;

// Структура узела дерева 
class AutocompleteDictionary
{
  AutocompleteDictionary* child_[ALPHABET_SIZE] = {};
  bool endOfWord_ = false;

public:
  std::vector<std::string> prefixes_;
  int prefCount = 0;
  //Контейнер, в который будет складывать весь поток из строки ввода.
  std::string stream_;

  AutocompleteDictionary();
  ~AutocompleteDictionary();

  void Remove(AutocompleteDictionary* &root);
  void Insert(AutocompleteDictionary* &root, std::string const& word);
  auto Search(AutocompleteDictionary* &root, std::string const& word) -> bool;
  void FindAllPrefixes(AutocompleteDictionary* &root, char* wordConstructor, 
    int index, std::string const& word);
  void PrintAutocomplete(AutocompleteDictionary* &root, size_t position);
};
#endif