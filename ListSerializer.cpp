#include "ListSerializer.hpp"
#include <fstream>
#include <iostream>

void ListSerializer::clear() noexcept
{
  {
    ListNode *current = head;
    while (current)
    {
      ListNode *next = current->next;
      delete current;
      current = next;
    }
    head = nullptr;
  }
}

size_t ListSerializer::utf8_length(const std::string &s)
{
  size_t len = 0;

  for (char c : s)
  {
    len += (c & 0xC0) != 0x80;
  }

  return len;
}

ListSerializer::ListSerializer() : head(nullptr)
{
}

ListSerializer::~ListSerializer()
{
  clear();
}

bool ListSerializer::deserializeFromText(const std::string &TextFileName)
{
  if (TextFileName.size() == 0)
  {
    return false;
  }

  // Файл для чтения данных
  std::ifstream INPUT;
  // номер элемента ; в строке
  size_t pos;
  // Строка считанная из файла
  std::string data_plus_rand;
  // Строка с данными
  std::string data;
  // Номер случайного элемента
  int rand;
  // Указатель на новый элемент списка
  ListNode *var;
  // Счетчик узлов
  int counter_list = 0;

  INPUT.open(TextFileName);

  if (!INPUT.is_open())
  {
    throw std::runtime_error("Не удалось открыть файл: " + TextFileName);
  }

  while (std::getline(INPUT, data_plus_rand))
  {
    if (data_plus_rand.empty())
    {
      continue;
    }

    counter_list++;

    pos = data_plus_rand.find(';');

    if (pos != std::string::npos)
    {
      data = data_plus_rand.substr(0, pos);

      if (utf8_length(data)>1000)
      {
        throw std::runtime_error("Длина data >1000 элементов");
      }

      int rand = std::stoi(data_plus_rand.substr(pos + 1));

      var = new ListNode();

      int i = counter_ptr.size();

      counter_ptr.push_back({var, 0});
      counter_ptr[i].first->data = data;

      if (i > 1)
      {
        counter_ptr[i].first->prev = counter_ptr[i - 1].first;
        counter_ptr[i - 1].first->next = counter_ptr[i].first;
      }

      if (rand > counter_ptr.size())
      {
        vec_pair.emplace_back(counter_ptr[i], rand);
      }
      else
      {
        if (rand >= 0)
        {
          counter_ptr[i]->rand = counter_ptr[rand];
        }
        else
        {
          counter_ptr[i]->rand = nullptr;
        }
      }
    }
    else
    {
      throw std::runtime_error("неверная строка");
    }
  }

  if (INPUT.bad())
  {
    throw std::runtime_error("Ошибка чтения файла" + TextFileName);
  }

  if (counter_list>1e6)
  {
    throw std::runtime_error("Cлишком большое количество узлов");
  }
  
  INPUT.close();

  for (size_t i = 0; i < vec_pair.size(); i++)
  {
    if (vec_pair[i].second <= counter_ptr.size())
    {
      vec_pair[i].first->rand = counter_ptr[vec_pair[i].second].first;
    }
    else
    {
      std::cerr << "неверный индекс" << std::endl;
      return false;
    }
  }

  return true;
}


