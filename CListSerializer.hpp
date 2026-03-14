#pragma once
#include "ListNode.hpp"
#include <string>
#include <vector>
#include <utility>

class CListSerializer
{
private:
  bool readData();
  bool writeData();

  ListNode *head;
  
  /**
   * @brief Очистка двусвязного списка
   *
   */
  void clear() noexcept;

  /**
   * @brief Метод подсчета количества символов UTF-8 в строке
   *
   * @param s строка
   * @return size_t количество символов
   */
  size_t utf8_length(const std::string &s);

  /// @brief Вектор пар из указателя на элемент списка и его случайного указателя
  std::vector<std::pair<ListNode *, int>> vec_pair;
  /// @brief Вектор для упрощения доступа к элементам списка во время их заполнения и сериализации
  std::vector<std::pair<ListNode *, int>> counter_ptr;

public:
  CListSerializer();
  ~CListSerializer();

  // Запрещено копирование
  CListSerializer(const CListSerializer &) = delete;
  CListSerializer &operator=(const CListSerializer &) = delete;

  /**
   * @brief Десериализация из текстового файла
   *
   * @return  true Успешная десериализация
   * @return false ошибки при десериализации
   */
  bool deserializeFromText(const std::string &TextFileName);

  /**
   * @brief Сериализация в бинарный файл
   *
   * @return true Успешная сериализация
   * @return false ошибки при сериализации
   */
  bool serializeToBinary(const std::string &binaryFileName);

  /**
   * @brief Десериализация из бинарного файла
   *
   * @return  true Успешная десериализация
   * @return false ошибки при десериализации
   */
  bool deserializeFromBinary(const std::string &binaryFileName);

};
