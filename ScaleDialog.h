#ifndef SCALEDIALOG_H
#define SCALEDIALOG_H

#include <QDialog>
#include <QDoubleSpinBox>
#include <QLineEdit>

namespace Ui {
class ScaleDialog;
}

class ScaleDialog : public QDialog {
  Q_OBJECT

public:
  explicit ScaleDialog(QWidget *parent = 0);
  ~ScaleDialog();

  QDoubleSpinBox *sceneFactorDoubleSpinBox() const {
    return SceneFactorDoubleSpinBox;
  }
signals:
  void scaleChanged_(double);
private slots:
  void on_pushButton_Cancel_clicked();
  void _scaleChanged(int);

private:
  Ui::ScaleDialog *ui;
  QLineEdit *ComboBoxLineEdit;
  QDoubleSpinBox *SceneFactorDoubleSpinBox;
};

#endif
