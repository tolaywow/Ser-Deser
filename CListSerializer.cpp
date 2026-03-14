#include "CListSerializer.hpp"
#include <fstream>
#include <iostream>

struct ListSerializer
{
  std::string data;
  size_t num;
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
  size_t rand;
  // Указатель на новый элемент списка
  ListNode *var;

  INPUT.open(TextFileName);

  if (!INPUT.is_open())
  {
    std::cerr << "Не удалось открыть файл: " + TextFileName;
    return false;
  }

  clear();

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

      rand = std::stoi(data_plus_rand.substr(pos + 1));

      var = new ListNode();

      size_t i = counter_ptr.size();

      counter_ptr.push_back({var, 0});
      counter_ptr[i].first->data = data;

      if (i > 1)
      {
        counter_ptr[i].first->prev = counter_ptr[i - 1].first;
        counter_ptr[i - 1].first->next = counter_ptr[i].first;
      }

      if (rand > counter_ptr.size())
      {
        vec_pair.emplace_back(counter_ptr[i].first, rand);
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
  // Временная переменная для записи индекса случайного элемента
  size_t num;

  OUTPUT.open(binaryFileName, std::ios::binary);

  if (!OUTPUT.is_open())
  {
    std::cerr << "Не удалось открыть файл: " + binaryFileName;
    return false;
  }

  OUTPUT.clear();

  for (size_t i = 0; i < counter_ptr.size(); i++)
  {
    num = -1;

    for (size_t j = 0; j < counter_ptr.size(); j++)
    {
      if (counter_ptr[j].first == counter_ptr[i].first->rand)
      {
        num = j;
      }
    }

    // Записываем длину строки
    OUTPUT.write(reinterpret_cast<const char *>(counter_ptr[i].first->data.size()), sizeof(counter_ptr[i].first->data.size()));

    // Записываем индекс rand
    OUTPUT.write(reinterpret_cast<const char *>(num), sizeof(num));

    // Записываем данные строки
    OUTPUT.write(counter_ptr[i].first->data.c_str(), counter_ptr[i].first->data.size());
  }

  OUTPUT.close();

  clear();

  return true;
}

bool CListSerializer::deserializeFromBinary(const std::string &binaryFileName)
{
  if (binaryFileName.size() == 0)
  {
    return false;
  }

  // Файл для записи данных
  std::ifstream INPUT;
  // Размер пакета данных
  size_t sizeofdata;
  // Номер случайного элемента(индекс)
  size_t rand;
  // Буфер данных
  std::vector<char> buffer;
  // указатель на элемент списка
  ListNode *LN;

  INPUT.open(binaryFileName, std::ios::binary);

  if (!INPUT.is_open())
  {
    std::cerr << "Не удалось открыть файл: " + binaryFileName;
    return false;
  }

  clear();

  while (!INPUT.eof())
  {
    INPUT >> sizeofdata;
    INPUT >> rand;
    buffer.resize(sizeofdata);

    INPUT.read(buffer.data(), sizeofdata);

    LN = new ListNode();
    size_t i = counter_ptr.size();

    counter_ptr.push_back({LN, 0});
    counter_ptr[i].first->data = std::string(buffer.data(), sizeofdata);

    if (i > 1)
    {
      counter_ptr[i].first->prev = counter_ptr[i - 1].first;
      counter_ptr[i - 1].first->next = counter_ptr[i].first;
    }

    if (rand > counter_ptr.size())
    {
      vec_pair.emplace_back(counter_ptr[i].first, rand);
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
