#include "ScaleDialog.h"
#include "ui_ScaleDialog.h"

ScaleDialog::ScaleDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::ScaleDialog) {
  ui->setupUi(this);
  ComboBoxLineEdit = new QLineEdit();
  ComboBoxLineEdit->setReadOnly(true);
  ComboBoxLineEdit->setAlignment(Qt::AlignCenter);
  SceneFactorDoubleSpinBox = ui->doubleSpinBox_SceneFactor;
  ui->comboBox_Scale->setLineEdit(ComboBoxLineEdit);
  setWindowTitle("Scale");
  QObject::connect(ui->comboBox_Scale, SIGNAL(currentIndexChanged(int)), this,
                   SLOT(_scaleChanged(int)));
}

ScaleDialog::~ScaleDialog() { delete ui; }

void ScaleDialog::on_pushButton_Cancel_clicked() { reject(); }

void ScaleDialog::_scaleChanged(int) {
  QString Value = ui->comboBox_Scale->currentText();
  Value = Value.remove(0, 1);
  emit scaleChanged_(Value.toDouble());
}
