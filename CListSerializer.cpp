#include "CListSerializer.hpp"
#include <fstream>
#include <iostream>

struct ListSerializer
{
  std::string data;
  int num;
};

void CListSerializer::clear() noexcept
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

size_t CListSerializer::utf8_length(const std::string &s)
{
  size_t len = 0;

  for (char c : s)
  {
    len += (c & 0xC0) != 0x80;
  }

  return len;
}

CListSerializer::CListSerializer() : head(nullptr)
{
}

CListSerializer::~CListSerializer()
{
  clear();
}

bool CListSerializer::deserializeFromText(const std::string &TextFileName)
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

  INPUT.open(TextFileName);

  if (!INPUT.is_open())
  {
    std::cerr << "Не удалось открыть файл: " + TextFileName;
    return false;
  }

  while (std::getline(INPUT, data_plus_rand))
  {
    if (data_plus_rand.empty())
    {
      continue;
    }

    pos = data_plus_rand.find(';');

    if (pos != std::string::npos)
    {
      data = data_plus_rand.substr(0, pos);

      if (utf8_length(data) > 1000)
      {
        std::cerr << "Длина data > 1000 элементов";
        return false;
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
          counter_ptr[i].first->rand = counter_ptr[rand].first;
        }
        else
        {
          counter_ptr[i].first->rand = nullptr;
        }
      }
    }
    else
    {
      std::cerr << "неверная строка";
      return false;
    }
  }

  if (INPUT.bad())
  {
    throw std::runtime_error("Ошибка чтения файла" + TextFileName);
  }

  if (counter_ptr.size() > 1e6)
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
      std::cerr << "Неверный индекс" << std::endl;
      return false;
    }
  }

  return true;
}

bool CListSerializer::serializeToBinary(const std::string &binaryFileName)
{
  if (binaryFileName.size() == 0)
  {
    return false;
  }

  // Файл для записи данных
  std::ofstream OUTPUT;

  OUTPUT.open(binaryFileName, std::ios::binary);

  if (!OUTPUT.is_open())
  {
    std::cerr << "Не удалось открыть файл: " + binaryFileName;
    return false;
  }

  return true;
}
