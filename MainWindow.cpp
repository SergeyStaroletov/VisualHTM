#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  VisualHTM = new GLWidget();
  OptionsDialogObject = new OptionsDialog(this);
  ScaleDialogObject = new ScaleDialog(this);
  OpenFileDialog =
      new QFileDialog(this, tr("Open init file"), QDir::currentPath());
  LogFile = new QFile(QDir::currentPath() + QDir::separator() + "log_file.txt");
  if (!LogFile->open(QIODevice::WriteOnly | QIODevice::Text))
    QMessageBox::warning(this, "Warning", "Can't open log file",
                         QMessageBox::Ok);

  QObject::connect(ui->actionE_xit, SIGNAL(triggered()), this, SLOT(close()));
  QObject::connect(ui->actionOptions, SIGNAL(triggered()), OptionsDialogObject,
                   SLOT(exec()));
  QObject::connect(OptionsDialogObject, SIGNAL(accepted()), this,
                   SLOT(optionsAccepted()));
  QObject::connect(ui->action_Scale, SIGNAL(triggered()), ScaleDialogObject,
                   SLOT(show()));
  QObject::connect(ScaleDialogObject, SIGNAL(scaleChanged_(double)), VisualHTM,
                   SLOT(setScale(double)));
  QObject::connect(ScaleDialogObject->sceneFactorDoubleSpinBox(),
                   SIGNAL(valueChanged(double)), VisualHTM,
                   SLOT(setSceneFactor(double)));
  QObject::connect(ui->action_Init, SIGNAL(triggered()), OpenFileDialog,
                   SLOT(exec()));
  QObject::connect(OpenFileDialog, SIGNAL(fileSelected(QString)), this,
                   SLOT(initFromFile(QString)));

  QObject::connect(OptionsDialogObject->proximalSynapseValueSpinBox(),
                   SIGNAL(valueChanged(int)), VisualHTM->region(),
                   SLOT(setProximalSynapseValue(int)));
  QObject::connect(
      OptionsDialogObject->proximalSynapseVariationPersentageSpinBox(),
      SIGNAL(valueChanged(int)), VisualHTM->region(),
      SLOT(setProximalSynapseVariationPersentage(int)));
  QObject::connect(
      OptionsDialogObject->proximalSynapsePermanenceValuePersentageSpinBox(),
      SIGNAL(valueChanged(int)), VisualHTM->region(),
      SLOT(setProximalSynapsePermanenceValuePersentage(int)));
  QObject::connect(OptionsDialogObject
                       ->proximalSynapsePermanenceVariationPersentageSpinBox(),
                   SIGNAL(valueChanged(int)), VisualHTM->region(),
                   SLOT(setProximalSynapseVariationPersentage(int)));
  QObject::connect(OptionsDialogObject->desiredLocalActivitySpinBox(),
                   SIGNAL(valueChanged(int)), VisualHTM->region(),
                   SLOT(setDesiredLocalActivity(int)));
  QObject::connect(OptionsDialogObject->minOverlapValuePersentageSpinBox(),
                   SIGNAL(valueChanged(int)), VisualHTM->region(),
                   SLOT(setMinOverlapValuePersentage(int)));

  QObject::connect(OptionsDialogObject->distalSegmentCountSpinBox(),
                   SIGNAL(valueChanged(int)), VisualHTM->region(),
                   SLOT(setDistalSegmentValue(int)));
  QObject::connect(
      OptionsDialogObject->distalSegmentVariationPersentageSpinBox(),
      SIGNAL(valueChanged(int)), VisualHTM->region(),
      SLOT(setDistalSegmentVariationPersentage(int)));
  QObject::connect(OptionsDialogObject->distalSegmentSynapseCountSpinBox(),
                   SIGNAL(valueChanged(int)), VisualHTM->region(),
                   SLOT(setDistalSegmentSynapseValue(int)));
  QObject::connect(
      OptionsDialogObject->distalSegmentSynapseVariationPersentageSpinBox(),
      SIGNAL(valueChanged(int)), VisualHTM->region(),
      SLOT(setDistalSegmentSynapseVariationPersentage(int)));
  QObject::connect(
      OptionsDialogObject->distalSegmentSynapseThresholdPersentageSpinBox(),
      SIGNAL(valueChanged(int)), VisualHTM->region(),
      SLOT(setDistalSegmentSynapseThresholdPersentage(int)));

  QObject::connect(OptionsDialogObject->activeCellColorDialog(),
                   SIGNAL(colorSelected(QColor)), VisualHTM->regionDraw(),
                   SLOT(setActiveCellColor(QColor)));
  QObject::connect(OptionsDialogObject->inactiveCellColorDialog(),
                   SIGNAL(colorSelected(QColor)), VisualHTM->regionDraw(),
                   SLOT(setInactiveCellColor(QColor)));
  QObject::connect(OptionsDialogObject->activeCellColorDialog(),
                   SIGNAL(colorSelected(QColor)), VisualHTM, SLOT(repaint()));
  QObject::connect(OptionsDialogObject->inactiveCellColorDialog(),
                   SIGNAL(colorSelected(QColor)), VisualHTM, SLOT(repaint()));
  setCentralWidget(VisualHTM);
  setWindowTitle(tr("Visual HTM - OpenGL"));
}

MainWindow::~MainWindow() {
  delete ui;
  LogFile->close();
  delete LogFile;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
    switch (event->key()) {
    case Qt::Key_S:
      VisualHTM->show();
      break;
    case Qt::Key_H:
      VisualHTM->hide();
      break;
    case Qt::Key_Left:
      VisualHTM->setXTranslate(-0.05 * VisualHTM->width());
      break;
    case Qt::Key_Right:
      VisualHTM->setXTranslate(0.05 * VisualHTM->width());
      break;
    case Qt::Key_Up:
      VisualHTM->setYTranslate(0.05 * VisualHTM->height());
      break;
    case Qt::Key_Down:
      VisualHTM->setYTranslate(-0.05 * VisualHTM->height());
      break;
    }
  } else {
    switch (event->key()) {
    case Qt::Key_Left:
      VisualHTM->setXTranslate(-0.01 * VisualHTM->width());
      break;
    case Qt::Key_Right:
      VisualHTM->setXTranslate(0.01 * VisualHTM->width());
      break;
    case Qt::Key_Up:
      VisualHTM->setYTranslate(0.01 * VisualHTM->height());
      break;
    case Qt::Key_Down:
      VisualHTM->setYTranslate(-0.01 * VisualHTM->height());
      break;
    }
  }
}

void MainWindow::writeToLog(const CRegion *pRegion) const {
  int ColumnIndex, ProximalSynapseIndex, CellIndex, DistalSegmentIndex,
      DistalSynapseIndex;
  QTextStream Stream(LogFile);
  Stream << "Iteration = " << pRegion->iteration() << endl;
  Stream << "[Width,Height,ColumnHeight] = [" << pRegion->width() << ","
         << pRegion->height() << "," << pRegion->columnHeight() << "]" << endl;
  Stream << "PermanenceInc,PermanenceDec = "
         << pRegion->permanenceIncMPersentage() << ","
         << pRegion->permanenceDecMPersentage() << endl;
  Stream << "ProximalSynapseValue,% = " << pRegion->proximalSynapseValue()
         << "," << pRegion->proximalSynapseVariationPersentage() << endl;
  Stream << "ProximalSynapsePermanenceValuePersentage,% = "
         << pRegion->proximalSynapsePermanenceValuePersentage() << ","
         << pRegion->proximalSynapsePermanenceVariationPersentage() << endl;
  Stream << "DesiredLocalActivity = " << pRegion->desiredLocalActivity()
         << endl;
  Stream << "MinOverlapValuePersentage = "
         << pRegion->minOverlapValuePersentage() << endl;
  Stream << "DistalSegmentValue,% = " << pRegion->distalSegmentValue() << ","
         << pRegion->distalSegmentVariationPersentage() << endl;
  Stream << "DistalSegmentSynapseValue,% = "
         << pRegion->distalSegmentSynapseValue() << ","
         << pRegion->distalSegmentSynapseVariationPersentage() << endl;
  Stream << "DistalSegmentSynapseThresholdPersentage = "
         << pRegion->distalSegmentSynapseThresholdPersentage() << endl;
  Stream << "SpreadingRadius = " << pRegion->spreadingRadius() << endl;
  Stream << "[COLUMNS] : " << endl;
  for (ColumnIndex = 0; ColumnIndex < pRegion->width() * pRegion->height();
       ++ColumnIndex) {
    Stream << "\tColumns[" << ColumnIndex
           << "] - ProximalDendriticSegmentSize = "
           << pRegion->columns()[ColumnIndex].ProximalDendriticSegmentSize
           << endl;
    for (ProximalSynapseIndex = 0;
         ProximalSynapseIndex <
         pRegion->columns()[ColumnIndex].ProximalDendriticSegmentSize;
         ++ProximalSynapseIndex)
      Stream << "\t\tProximalDendriticSegment[" << ProximalSynapseIndex
             << "].[BitIndex,PermanenceValue] = ["
             << pRegion->columns()[ColumnIndex]
                    .ProximalDendriticSegment[ProximalSynapseIndex]
                    .BitIndex
             << ","
             << pRegion->columns()[ColumnIndex]
                    .ProximalDendriticSegment[ProximalSynapseIndex]
                    .PermanenceValue
             << "]" << endl;
    Stream << "\tThreshold = " << pRegion->columns()[ColumnIndex].Threshold
           << endl;
    Stream << "\t[MinOverlapValue,OverlapValue] = ["
           << pRegion->columns()[ColumnIndex].MinOverlapValue << ","
           << pRegion->columns()[ColumnIndex].OverlapValue << "]" << endl;
    Stream << "\tCenter = " << pRegion->columns()[ColumnIndex].Center << endl;
    Stream << "\tIsActive = " << pRegion->columns()[ColumnIndex].IsActive
           << endl;
    Stream << "\tBoost = " << pRegion->columns()[ColumnIndex].Boost << endl;
    Stream << "\t[MinDutyCycle,ActiveDutyCycle,OverlapDutyCycle] = "
           << pRegion->columns()[ColumnIndex].MinDutyCycle << ","
           << pRegion->columns()[ColumnIndex].ActiveDutyCycle << ","
           << pRegion->columns()[ColumnIndex].OverlapDutyCycle << endl;
    Stream << "\t[CELLS] : " << endl;
    for (CellIndex = 0; CellIndex < pRegion->columnHeight(); ++CellIndex) {
      Stream << "\t\tCell[" << CellIndex << "].[PreviousState,CurrentState] = ["
             << pRegion->columns()->ColumnCells[CellIndex].PreviousState << ","
             << pRegion->columns()->ColumnCells[CellIndex].CurrentState << "]"
             << endl;
      Stream << "\t\tDistalSegmentSize = "
             << pRegion->columns()->ColumnCells[CellIndex].DistalSegmentSize
             << endl;
      for (DistalSegmentIndex = 0;
           DistalSegmentIndex <
           pRegion->columns()->ColumnCells[CellIndex].DistalSegmentSize;
           ++DistalSegmentIndex) {
        Stream << "\t\t\tActivationThreshold = "
               << pRegion->columns()
                      ->ColumnCells[CellIndex]
                      .DistalSegment[DistalSegmentIndex]
                      .ActivationThreshold
               << endl;
        Stream << "\t\t\tSynapseCount = "
               << pRegion->columns()
                      ->ColumnCells[CellIndex]
                      .DistalSegment[DistalSegmentIndex]
                      .SynapseListSize
               << endl;
        for (DistalSynapseIndex = 0;
             DistalSynapseIndex < pRegion->columns()
                                      ->ColumnCells[CellIndex]
                                      .DistalSegment[DistalSegmentIndex]
                                      .SynapseListSize;
             ++DistalSynapseIndex) {
          Stream << "\t\t\tSYNAPSE[" << DistalSynapseIndex
                 << "] = [Column,Cell].[" << ColumnIndex << "," << CellIndex
                 << "]"
                 << " to [Column,Cell].["
                 << pRegion->columns()
                        ->ColumnCells[CellIndex]
                        .DistalSegment[DistalSegmentIndex]
                        .Synapses[DistalSynapseIndex]
                        .ColumnIndex
                 << ","
                 << pRegion->columns()
                        ->ColumnCells[CellIndex]
                        .DistalSegment[DistalSegmentIndex]
                        .Synapses[DistalSynapseIndex]
                        .CellIndex
                 << "]" << endl;
          Stream << "\t\t\t\t[PreviousPermanenceValue,CurrentPermanenceValue,"
                    "InitialPermanenceValue] = ["
                 << pRegion->columns()
                        ->ColumnCells[CellIndex]
                        .DistalSegment[DistalSegmentIndex]
                        .Synapses[DistalSynapseIndex]
                        .PreviousPermanenceValue
                 << ","
                 << pRegion->columns()
                        ->ColumnCells[CellIndex]
                        .DistalSegment[DistalSegmentIndex]
                        .Synapses[DistalSynapseIndex]
                        .CurrentPermanenceValue
                 << ","
                 << pRegion->columns()
                        ->ColumnCells[CellIndex]
                        .DistalSegment[DistalSegmentIndex]
                        .Synapses[DistalSynapseIndex]
                        .InitialPermanenceValue
                 << "]" << endl;
        }
        Stream << endl;
      }
      Stream << endl;
    }
  }
}

void MainWindow::optionsAccepted() {
  VisualHTM->setRegionGeometry(OptionsDialogObject->getRegionWidth(),
                               OptionsDialogObject->getRegionHeight(),
                               OptionsDialogObject->getColumnHeight());
  VisualHTM->setRegionCellRadius(OptionsDialogObject->getCellRadius());
  VisualHTM->setRegionCellSpace(OptionsDialogObject->getCellSpace());
}

void MainWindow::initFromFile(const QString &FileName) {
  QFile InputFile(FileName);
  if (!InputFile.open(QIODevice::ReadOnly))
    ui->statusBar->showMessage("Can't open input file");
  QBitArray BitArray;
  int Size;
  switch (OptionsDialogObject->getFileType()) {
  case FILE_TYPE_TEXT: {
    QTextStream InputStream(&InputFile);
    QString BitString = InputStream.readAll();

    Size = BitString.size() - 1;
    BitArray.resize(Size);
    for (int Index = 0; Index < Size; ++Index)
      if (BitString[Index] == '0')
        BitArray.setBit(Index, false);
      else if (BitString[Index] == '1')
        BitArray.setBit(Index, true);
  } break;
  case FILE_TYPE_BINARY: {
    QDataStream InputStream(&InputFile);
    InputStream.setVersion(QDataStream::Qt_4_8);
    InputStream >> BitArray;
    break;
  }
  case FILE_TYPE_UNDEF:
    break;
  default:;
  }
  InputFile.close();
  VisualHTM->initRegion(BitArray);
  writeToLog(VisualHTM->region());
}

void MainWindow::on_action_Init_triggered() {}
