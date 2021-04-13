#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QCheckBox>
#include <QColorDialog>
#include <QDialog>
#include <QLineEdit>
#include <QScrollArea>
#include <QSpinBox>

#include "Defines.h"

namespace Ui {
class OptionsDialog;
}

class OptionsDialog : public QDialog {
  Q_OBJECT

public:
  explicit OptionsDialog(QWidget *parent = 0);
  ~OptionsDialog();
  int getRegionWidth() const;
  int getRegionHeight() const;
  int getColumnHeight() const;
  QColor getCellColor() const { return ActiveCellColor; }
  double getCellRadius() const;
  double getCellSpace() const;
  int getFileType() const;

  QSpinBox *proximalSynapseValueSpinBox() const {
    return ProximalSynapseValueSpinBox;
  }
  QSpinBox *proximalSynapseVariationPersentageSpinBox() const {
    return ProximalSynapseVariationPersentageSpinBox;
  }
  QSpinBox *proximalSynapsePermanenceValuePersentageSpinBox() const {
    return ProximalSynapsePermanenceValuePersentageSpinBox;
  }
  QSpinBox *proximalSynapsePermanenceVariationPersentageSpinBox() const {
    return ProximalSynapsePermanenceVariationPersentageSpinBox;
  }
  QSpinBox *desiredLocalActivitySpinBox() const {
    return DesiredLocalActivitySpinBox;
  }
  QSpinBox *minOverlapValuePersentageSpinBox() const {
    return MinOverlapValuePersentageSpinBox;
  }

  QSpinBox *distalSegmentCountSpinBox() const {
    return DistalSegmentCountSpinBox;
  }
  QSpinBox *distalSegmentVariationPersentageSpinBox() const {
    return DistalSegmentVariationPersentageSpinBox;
  }
  QSpinBox *distalSegmentSynapseCountSpinBox() const {
    return DistalSegmentSynapseCountSpinBox;
  }
  QSpinBox *distalSegmentSynapseVariationPersentageSpinBox() const {
    return DistalSegmentSynapseVariationPersentageSpinBox;
  }
  QSpinBox *distalSegmentSynapseThresholdPersentageSpinBox() const {
    return DistalSegmentSynapseThresholdPersentageSpinBox;
  }

  QColorDialog *activeCellColorDialog() const { return ActiveCellColorDialog; }
  QColorDialog *inactiveCellColorDialog() const {
    return InactiveCellColorDialog;
  }
private slots:
  void on_pushButton_Ok_clicked();
  void on_pushButton_Cancel_clicked();
  void _activeCellColorChanged();
  void _inactiveCellColorChanged();
  void on_pushButton_ActiveCellColor_clicked();
signals:
  void activeCellColorChanged_(const QColor &color);
  void inactiveCellColorChanged_(const QColor &color);

private:
  Ui::OptionsDialog *ui;
  QColorDialog *ActiveCellColorDialog, *InactiveCellColorDialog;
  QColor ActiveCellColor, InactiveCellColor;
  QSpinBox *ProximalSynapseValueSpinBox,
      *ProximalSynapseVariationPersentageSpinBox,
      *ProximalSynapsePermanenceValuePersentageSpinBox,
      *ProximalSynapsePermanenceVariationPersentageSpinBox,
      *DesiredLocalActivitySpinBox, *MinOverlapValuePersentageSpinBox,
      *DistalSegmentCountSpinBox, *DistalSegmentVariationPersentageSpinBox,
      *DistalSegmentSynapseCountSpinBox,
      *DistalSegmentSynapseVariationPersentageSpinBox,
      *DistalSegmentSynapseThresholdPersentageSpinBox;
};

#endif
