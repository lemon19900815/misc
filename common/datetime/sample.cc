#include "datetime.h"

void datetime_test()
{
  DateTime dt1;
  std::cout << "dt1 datetime:" << dt1.asString() << std::endl;

  dt1.addYear(1);
  std::cout << "after add 1 year:" << dt1.asString() << std::endl;

  dt1.addYear(-2);
  std::cout << "after add -2 year:" << dt1.asString() << std::endl;

  DateTime dt2(2016, 9, 8, 15, 31, 0);
  std::cout << "dt2 datetime:" << dt2.asString() << std::endl;

  DateTime dt3(dt2.unixstamp());
  std::cout << "dt3 datetime:" << dt3.asString() << std::endl;
}

int main(int argc, char** argv)
{
  datetime_test();
  return 0;
}
