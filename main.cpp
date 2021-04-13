/**
  @author Denis Shitov, supervised by Sergey Staroletov, Polzunov AltSTU
  questions: serg_soft@mail.ru
*/

// todo
// 2. mouse click. => index of column => regionDraw getObject by point
// CRegion.cpp -> permanence value for distal synapse -> initValue
// DesiredLocalActivity -> create default value
// DesiredLocalActivity -> Phase 3 -> refresh

#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  MainWindow w;
  w.show();
  return app.exec();
}
