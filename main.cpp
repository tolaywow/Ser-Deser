// #include "CSerialization.hpp"
// #include "CDeserialization.hpp"
#include "CListSerializer.hpp"
#include "iostream"

int main(int argc, char *argv[])
{
  std::string IN;
  std::string ON;

  if (argc > 1)
  {
    IN = argv[1];

    if (argc > 2)
    {
      ON = argv[2];
    }
    else
    {
      ON = "outlet.out";
    }
  }
  else if (argc == 1)
  {
    IN = "inlet.in";
    ON = "outlet.out";
  }
  else
  {
    return -1; //"Wrong Parametrs";
  }

  ListNode LN;

  CListSerializer LS;
  try
  {
    LS.deserializeFromText(IN);
    LS.serializeToBinary(ON);
    LS.deserializeFromBinary(ON);
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << '\n';
  }

  return 0;
}