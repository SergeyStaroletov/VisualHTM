#include "OptionsDialog.h"
#include "ui_OptionsDialog.h"

OptionsDialog::OptionsDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::OptionsDialog) {
  ui->setupUi(this);
  setWindowTitle(tr("Options"));
  ActiveCellColor = DefaultValues.ActiveCellColor;
  InactiveCellColor = DefaultValues.InactiveCellColor;

  ActiveCellColorDialog = new QColorDialog(ActiveCellColor, this);
  InactiveCellColorDialog = new QColorDialog(InactiveCellColor, this);
  QObject::connect(ui->pushButton_ActiveCellColor, SIGNAL(clicked()),
                   ActiveCellColorDialog, SLOT(exec()));
  QObject::connect(ui->pushButton_InactiveCellColor, SIGNAL(clicked()),
                   InactiveCellColorDialog, SLOT(exec()));
  QObject::connect(ActiveCellColorDialog, SIGNAL(accepted()), this,
                   SLOT(_activeCellColorChanged()));
  QObject::connect(InactiveCellColorDialog, SIGNAL(accepted()), this,
                   SLOT(_inactiveCellColorChanged()));
  ui->pushButton_ActiveCellColor->setStyleSheet(
      "background-color : rgb(" + QString::number(ActiveCellColor.red()) + "," +
      QString::number(ActiveCellColor.green()) + "," +
      QString::number(ActiveCellColor.blue()) + ");");
  ui->pushButton_InactiveCellColor->setStyleSheet(
      "background-color : rgb(" + QString::number(InactiveCellColor.red()) +
      "," + QString::number(InactiveCellColor.green()) + "," +
      QString::number(InactiveCellColor.blue()) + ");");
  QLineEdit *FileTypeLineEdit = new QLineEdit();
  FileTypeLineEdit->setReadOnly(true);
  FileTypeLineEdit->setAlignment(Qt::AlignCenter);
  ui->comboBox_FileType->setLineEdit(FileTypeLineEdit);
  ui->pushButton_ActiveCellColor->setFocusPolicy(Qt::NoFocus);
  ui->doubleSpinBox_CellRadius->setValue(CELL_RADIUS_DEF_VALUE);
  ui->doubleSpinBox_CellSpace->setValue(CELL_SPACE_DEF_VALUE);

  ui->spinBox_ProximalSynapseValue->setValue(PROXIMAL_SYNAPSE_VALUE_DEF_VALUE);
  ui->spinBox_ProximalSynapseVariationPersentage->setValue(
      PROXIMAL_SYNAPSE_VARIATION_PERSENTAGE_DEF_VALUE);
  ui->spinBox_ProximalSynapsePermanenceValuePersentage->setValue(
      PROXIMAL_SYNAPSE_PERMANENCE_VALUE_PERSENTAGE_DEF_VALUE);
  ui->spinBox_ProximalSynapsePermanenceVariationPersentage->setValue(
      PROXIMAL_SYNAPSE_PERMANENCE_VARIATION_PERSENTAGE_DEF_VALUE);
  ui->spinBox_DesiredLocalActivity->setValue(DESIRED_LOCAL_ACTIVITY_DEF_VALUE);
  ui->spinBox_MinimumOverlapValuePersentage->setValue(
      MIN_OVERLAP_VALUE_PERSENTAGE_DEF_VALUE);

  ui->spinBox_DistalSegmentCount->setValue(DISTAL_SEGMENT_COUNT_DEF_VALUE);
  ui->spinBox_DistalSegmentVariationPersentage->setValue(
      DISTAL_SEGMENT_VARIATION_PERSENTAGE_DEF_VALUE);
  ui->spinBox_DistalSegmentSynapseCount->setValue(
      DISTAL_SEGMENT_SYNAPSE_COUNT_DEF_VALUE);
  ui->spinBox_DistalSegmentSynapseVariationPersentage->setValue(
      DISTAL_SEGMENT_SYNAPSE_VARIATION_PERSENTAGE_DEF_VALUE);
  ui->spinBox_DistalSegmentSynapseThresholdPersentage->setValue(
      DISTAL_SEGMENT_SYNAPSE_THRESHOLD_PERSENTAGE_DEF_VALUE);

  ProximalSynapseValueSpinBox = ui->spinBox_ProximalSynapseValue;
  ProximalSynapseVariationPersentageSpinBox =
      ui->spinBox_ProximalSynapseVariationPersentage;
  ProximalSynapsePermanenceValuePersentageSpinBox =
      ui->spinBox_ProximalSynapsePermanenceValuePersentage;
  ProximalSynapsePermanenceVariationPersentageSpinBox =
      ui->spinBox_ProximalSynapsePermanenceVariationPersentage;
  DesiredLocalActivitySpinBox = ui->spinBox_DesiredLocalActivity;
  MinOverlapValuePersentageSpinBox = ui->spinBox_MinimumOverlapValuePersentage;

  DistalSegmentCountSpinBox = ui->spinBox_DistalSegmentCount;
  DistalSegmentVariationPersentageSpinBox =
      ui->spinBox_DistalSegmentVariationPersentage;
  DistalSegmentSynapseCountSpinBox = ui->spinBox_DistalSegmentSynapseCount;
  DistalSegmentSynapseVariationPersentageSpinBox =
      ui->spinBox_DistalSegmentSynapseVariationPersentage;
  DistalSegmentSynapseThresholdPersentageSpinBox =
      ui->spinBox_DistalSegmentSynapseThresholdPersentage;

  ui->scrollArea->setFocusPolicy(Qt::NoFocus);
}

OptionsDialog::~OptionsDialog() { delete ui; }

void OptionsDialog::on_pushButton_Ok_clicked() { accept(); }

void OptionsDialog::on_pushButton_Cancel_clicked() { reject(); }

void OptionsDialog::_activeCellColorChanged() {
  ActiveCellColor = ActiveCellColorDialog->currentColor();
  ui->pushButton_ActiveCellColor->setStyleSheet(
      "background-color : rgb(" + QString::number(ActiveCellColor.red()) + "," +
      QString::number(ActiveCellColor.green()) + "," +
      QString::number(ActiveCellColor.blue()) + ");");
}

void OptionsDialog::_inactiveCellColorChanged() {
  InactiveCellColor = InactiveCellColorDialog->currentColor();
  ui->pushButton_InactiveCellColor->setStyleSheet(
      "background-color : rgb(" + QString::number(InactiveCellColor.red()) +
      "," + QString::number(InactiveCellColor.green()) + "," +
      QString::number(InactiveCellColor.blue()) + ");");
}

int OptionsDialog::getRegionWidth() const {
  return ui->spinBox_RegionWidth->value();
}

int OptionsDialog::getRegionHeight() const {
  return ui->spinBox_RegionHeight->value();
}

int OptionsDialog::getColumnHeight() const {
  return ui->spinBox_ColumnHeight->value();
}

double OptionsDialog::getCellRadius() const {
  return ui->doubleSpinBox_CellRadius->value();
}

double OptionsDialog::getCellSpace() const {
  return ui->doubleSpinBox_CellSpace->value();
}

int OptionsDialog::getFileType() const {
  if (ui->comboBox_FileType->currentText() == "Text")
    return FILE_TYPE_TEXT;
  else if (ui->comboBox_FileType->currentText() == "Binary")
    return FILE_TYPE_BINARY;
  return FILE_TYPE_UNDEF;
}

void OptionsDialog::on_pushButton_ActiveCellColor_clicked() {}
