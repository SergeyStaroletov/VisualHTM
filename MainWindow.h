#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QHBoxLayout>
#include <QLocale>
#include <QMainWindow>

#include "GLWidget.h"
#include "OptionsDialog.h"
#include "ScaleDialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

protected:
  void keyPressEvent(QKeyEvent *event);
  void writeToLog(const CRegion *pRegion) const;
protected slots:
  void optionsAccepted();
  void initFromFile(const QString &FileName);
private slots:
  void on_action_Init_triggered();

private:
  Ui::MainWindow *ui;
  GLWidget *VisualHTM;
  OptionsDialog *OptionsDialogObject;
  ScaleDialog *ScaleDialogObject;
  QFileDialog *OpenFileDialog;
  QFile *LogFile;
};

#endif
