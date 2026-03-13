
int main(int argc, char *argv[])
{
  char *InputName = nullptr;
  char *OutputName = nullptr;
  if (argc > 1)
  {
    InputName = argv[1];

    if (argc > 2)
    {
      OutputName = argv[2];
    }
    else
    {
      char ON[] = "outlet.out";
      OutputName = ON;
    }
  }
  else if (argc == 1)
  {
    char IN[] = "inlet.in";
    InputName = IN;
    char ON[] = "outlet.out";
    OutputName = ON;
  }
  else
  {
    throw "Wrong Parametrs";
  }

  return 0;
}