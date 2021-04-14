#include "CRegion.h"
#include <QDebug>

CRegion::SDistalSynapse::SDistalSynapse(const int &columnIndex,
                                        const int &cellIndex,
                                        const int &permanenceValue) {
  ColumnIndex = columnIndex;
  CellIndex = cellIndex;
  CurrentPermanenceValue = permanenceValue;
  PreviousPermanenceValue = 0;
}

CRegion::SDistalSegment::SDistalSegment() {
  Synapses = NULL;
  SynapseListSize = 0;
  ActivationThreshold = 0;
  ActiveSynapseCount = PrevActiveSynapseCount = 0;
  IsSequence = false;
}

CRegion::SDistalSegment *CRegion::SCell::getActiveSegment() {
  CRegion::SDistalSegment *ActiveSegment = NULL;
  bool IsSequenceFound = false;
  int SegmentIndex, PrevActiveSynapses, MaxActiveSynapses = 0;

  for (SegmentIndex = 0; SegmentIndex < DistalSegmentSize; ++SegmentIndex) {
    PrevActiveSynapses = DistalSegment[SegmentIndex].PrevActiveSynapseCount;
    if (PrevActiveSynapses >= DistalSegment[SegmentIndex].ActivationThreshold) {
      if (DistalSegment[SegmentIndex].IsSequence) {
        IsSequenceFound = true;
        if (PrevActiveSynapses > MaxActiveSynapses) {
          MaxActiveSynapses = PrevActiveSynapses;
          ActiveSegment = &DistalSegment[DistalSegmentSize];
        }
      } else if (!IsSequenceFound && (PrevActiveSynapses > MaxActiveSynapses)) {
        MaxActiveSynapses = PrevActiveSynapses;
        ActiveSegment = &DistalSegment[DistalSegmentSize];
      }
    }
  }
  return ActiveSegment;
}

int CRegion::SCell::bestMatchingSegmentIndex(const int &minOverlap,
                                             const bool &IsCurrentStep) {
  int BestSegmentIndex = -1, ActiveSynapseCount, SegmentIndex,
      MaxOverlap = minOverlap;
  for (SegmentIndex = 0; SegmentIndex < DistalSegmentSize; ++SegmentIndex) {
    ActiveSynapseCount =
        IsCurrentStep ? DistalSegment[SegmentIndex].ActiveSynapseCount
                      : DistalSegment[SegmentIndex].PrevActiveSynapseCount;
    if (ActiveSynapseCount >= MaxOverlap) {
      MaxOverlap = ActiveSynapseCount;
      BestSegmentIndex = SegmentIndex;
    }
  }
  return BestSegmentIndex;
}

CRegion::SCell::SCell() {
  DistalSegment = NULL;
  PreviousState = CurrentState = INACTIVE;
  IsPredicted = WasPredicted = false;
  IsActive = WasActive = false;
  IsLearning = WasLearning = false;
}

CRegion::SColumn::SColumn() {
  ProximalDendriticSegmentSize = 0;
  Threshold = 0;
  MinOverlapValue = 0;
  OverlapValue = 0;
  PrevBoostTime = 0;
  Center = -1;
  IsActive = false;
  ColumnCells = NULL;
  ProximalDendriticSegment = NULL;
  MinBoost = Boost = DefaultValues.Boost;
  MinDutyCycle = MaxDutyCycle = 0.;
  ActiveDutyCycle = DefaultValues.ActiveDutyCycle;
  OverlapDutyCycle = DefaultValues.OverlapDutyCycle;
}

CRegion::SColumn::SColumn(const int &height) {
  ProximalDendriticSegmentSize = 0;
  Threshold = 0;
  MinOverlapValue = 0;
  OverlapValue = 0;
  PrevBoostTime = 0;
  Center = -1;
  IsActive = false;
  ColumnCells = new SCell[height];
  ProximalDendriticSegment = NULL;
  Boost = DefaultValues.Boost;
  MinDutyCycle = MaxDutyCycle = 0.;
  ActiveDutyCycle = DefaultValues.ActiveDutyCycle;
  OverlapDutyCycle = DefaultValues.OverlapDutyCycle;
  for (int CellIndex = 0; CellIndex < height; ++CellIndex) {
    ColumnCells[CellIndex].CurrentState = INACTIVE;
    ColumnCells[CellIndex].PreviousState = INACTIVE;
  }
}

CRegion::SColumn::~SColumn() {
  if (ColumnCells != NULL)
    delete[] ColumnCells;
  if (ProximalDendriticSegment != NULL)
    delete[] ProximalDendriticSegment;
}

void CRegion::SColumn::resizeColumn(const int &height) {
  if (ColumnCells != NULL)
    delete[] ColumnCells;
  ColumnCells = new SCell[height];
  for (int CellIndex = 0; CellIndex < height; ++CellIndex) {
    ColumnCells[CellIndex].CurrentState = INACTIVE;
    ColumnCells[CellIndex].PreviousState = INACTIVE;
  }
}

void CRegion::setGeometry(const int &width, const int &height,
                          const int &columnHeight) {
  int TempWidth = width > 0 ? width : 1, TempHeight = height > 0 ? height : 1,
      TempColumnHeight = columnHeight > 0 ? columnHeight : 1, ColumnIndex;

  if (Width == TempWidth && Height == TempHeight &&
      ColumnHeight == TempColumnHeight)
    return;
  for (ColumnIndex = 0; ColumnIndex < Width * Height; ++ColumnIndex) {
    delete[] Columns[ColumnIndex].ProximalDendriticSegment;
    delete[] Columns[ColumnIndex].ColumnCells;
    Columns[ColumnIndex].ProximalDendriticSegment = NULL;
    Columns[ColumnIndex].ColumnCells = NULL;
  }
  if (Width != TempWidth || Height != TempHeight) {
    delete[] Columns;
    Width = TempWidth;
    Height = TempHeight;
    ColumnHeight = TempColumnHeight;
    Columns = new SColumn[Width * Height];
    for (ColumnIndex = 0; ColumnIndex < Width * Height; ++ColumnIndex)
      Columns[ColumnIndex].resizeColumn(ColumnHeight);
  } else {
    ColumnHeight = TempColumnHeight;
    for (ColumnIndex = 0; ColumnIndex < Width * Height; ++ColumnIndex)
      Columns[ColumnIndex].resizeColumn(ColumnHeight);
  }
  emit geometryChanged(Width, Height, ColumnHeight);
}

CRegion::CRegion(const int &width, const int &height, const int &columnHeight) {
  Iteration = 0;
  Width = width > 0 ? width : 1;
  Height = height > 0 ? height : 1;
  ColumnHeight = columnHeight > 0 ? columnHeight : 1;
  ProximalSynapseValue = DefaultValues.ProximalSynapseValue;
  PermanenceIncMPersentage = DefaultValues.PermanenceIncMPersentage;
  PermanenceDecMPersentage = DefaultValues.PermanenceDecMPersentage;
  ProximalSynapseVariationPersentage =
      DefaultValues.ProximalSynapseVariationPersentage;
  MinOverlapValuePersentage = DefaultValues.MinOverlapValuePersentage;
  ProximalSynapsePermanenceValuePersentage =
      DefaultValues.ProximalSynapsePermanenceValuePersentage;
  ProximalSynapsePermanenceVariationPersentage =
      DefaultValues.ProximalSynapsePermanenceVariationPersentage;

  DistalSegmentValue = DefaultValues.DistalSegmentCount;
  DistalSegmentVariationPersentage =
      DefaultValues.DistalSegmentVariationPersentage;
  DistalSegmentSynapseValue = DefaultValues.DistalSegmentSynapseCount;
  DistalSegmentSynapseVariationPersentage =
      DefaultValues.DistalSegmentSynapseVariationPersentage;
  DistalSegmentSynapseThresholdPersentage =
      DefaultValues.DistalSegmentSynapseThresholdPersentage;

  DesiredLocalActivity = 10;

  Columns = new SColumn[Width * Height];
  for (int ColumnIndex = 0; ColumnIndex < Width * Height; ++ColumnIndex)
    Columns[ColumnIndex].resizeColumn(ColumnHeight);
  pCurrentBitSet = NULL;
  SpreadingRadius = DefaultValues.SpreadingRadius;
}

CRegion::~CRegion() {
  if (Columns != NULL)
    delete[] Columns;
}

void CRegion::init(const int &bitSetSize) {
  if (bitSetSize <= 0)
    throw;
  BitSetSize = bitSetSize;
  int ColumnIndex, SynapseIndex, MinBitIndex, MaxBitIndex, SynapseCount,
      Distance, Center, ColumnThreshold;

  timespec Time;
  clock_gettime(CLOCK_MONOTONIC, &Time);
  srand(Time.tv_nsec);

  for (ColumnIndex = 0; ColumnIndex < Width * Height; ++ColumnIndex) {
    SynapseCount = normalDistribution(ProximalSynapseValue,
                                      ProximalSynapseVariationPersentage);
    Columns[ColumnIndex].ProximalDendriticSegmentSize = SynapseCount;
    Columns[ColumnIndex].ProximalDendriticSegment =
        new SProximalSynapse[SynapseCount];
    Columns[ColumnIndex].MinOverlapValue =
        SynapseCount * MinOverlapValuePersentage / 100;
    Columns[ColumnIndex].MinBoost = Columns[ColumnIndex].Boost =
        DefaultValues.Boost +
        ((double)rand() / (double)RAND_MAX) * BoostVariation;
    Columns[ColumnIndex].MaxBoost =
        DefaultValues.MaxBoost -
        ((double)rand() / (double)RAND_MAX) * BoostVariation;

    MinBitIndex = rand() % bitSetSize;
    MinBitIndex =
        (MinBitIndex + SynapseCount < bitSetSize)
            ? MinBitIndex
            : (MinBitIndex - (MinBitIndex + SynapseCount - bitSetSize - 1));
    MaxBitIndex = MinBitIndex + SynapseCount - 1;

    Distance = MaxBitIndex - MinBitIndex + 1;
    Columns[ColumnIndex].Center = Center = (MaxBitIndex + MinBitIndex) / 2;
    Columns[ColumnIndex].Threshold = ColumnThreshold =
        SynapseCount * ProximalSynapsePermanenceValuePersentage / 100;

    for (SynapseIndex = 0; SynapseIndex < SynapseCount; ++SynapseIndex) {
      Columns[ColumnIndex].ProximalDendriticSegment[SynapseIndex].BitIndex =
          MinBitIndex + SynapseIndex;
      Columns[ColumnIndex]
          .ProximalDendriticSegment[SynapseIndex]
          .PermanenceValue =
          double(ColumnThreshold *
                 ProximalSynapsePermanenceVariationPersentage) /
              100. *
              (distributionFunction(Distance / 6., Center,
                                    (double)Columns[ColumnIndex]
                                        .ProximalDendriticSegment[SynapseIndex]
                                        .BitIndex) -
               0.5) +
          normalDistribution(ColumnThreshold,
                             ProximalSynapsePermanenceVariationPersentage);
      Columns[ColumnIndex].ProximalDendriticSegment[SynapseIndex].IsActive =
          Columns[ColumnIndex]
              .ProximalDendriticSegment[SynapseIndex]
              .PermanenceValue >= Columns[ColumnIndex].Threshold;
    }
  }
  initDistalSegments();
}

void CRegion::initDistalSegments() {
  int ColumnIndex, CellIndex, DistalSegmentSize;

  timespec Time;
  clock_gettime(CLOCK_MONOTONIC, &Time);
  srand(Time.tv_nsec);

  for (ColumnIndex = 0; ColumnIndex < Width * Height; ++ColumnIndex) {
    for (CellIndex = 0; CellIndex < ColumnHeight; ++CellIndex) {
      DistalSegmentSize = normalDistribution(DistalSegmentValue,
                                             DistalSegmentVariationPersentage);
      Columns[ColumnIndex].ColumnCells[CellIndex].DistalSegmentSize =
          DistalSegmentSize;
      Columns[ColumnIndex].ColumnCells[CellIndex].DistalSegment =
          new SDistalSegment[DistalSegmentSize];
      initDistalSynapses(ColumnIndex, CellIndex,
                         CRegion::SDistalSynapse::CORTEX_RADIUS);
    }
  }
}

void CRegion::initDistalSynapses(
    const int &columnIndex, const int &cellIndex,
    const CRegion::SDistalSynapse::_ConnectMethod &method) {
  int DistalSegmentIndex, SynapseIndex, SynapseValue, SynapseColumnIndex,
      SynapseCellIndex, RowIndex, ColumnIndex, CurrentRowIndex,
      CurrentColumnIndex, iSpreadingRadius;

  timespec Time;
  clock_gettime(CLOCK_MONOTONIC, &Time);
  srand(Time.tv_nsec);
  switch (method) {
  case CRegion::SDistalSynapse::FULL_RANDOM:
    for (DistalSegmentIndex = 0;
         DistalSegmentIndex <
         Columns[columnIndex].ColumnCells[cellIndex].DistalSegmentSize;
         ++DistalSegmentIndex) {
      Columns[columnIndex]
          .ColumnCells[cellIndex]
          .DistalSegment[DistalSegmentIndex]
          .SynapseListSize = SynapseValue = normalDistribution(
          DistalSegmentSynapseValue, DistalSegmentSynapseVariationPersentage);
      Columns[columnIndex]
          .ColumnCells[cellIndex]
          .DistalSegment[DistalSegmentIndex]
          .Synapses = new SDistalSynapse[SynapseValue];
      Columns[columnIndex]
          .ColumnCells[cellIndex]
          .DistalSegment[DistalSegmentIndex]
          .ActivationThreshold =
          (SynapseValue * DistalSegmentSynapseThresholdPersentage) / 100;
      for (SynapseIndex = 0; SynapseIndex < SynapseValue; ++SynapseIndex) {
        SynapseColumnIndex = rand() % (Width * Height);
        Columns[columnIndex]
            .ColumnCells[cellIndex]
            .DistalSegment[DistalSegmentIndex]
            .Synapses[SynapseIndex]
            .ColumnIndex = SynapseColumnIndex;
        while ((SynapseCellIndex = rand() % ColumnHeight) == cellIndex &&
               columnIndex == SynapseColumnIndex)
          ;
        Columns[columnIndex]
            .ColumnCells[cellIndex]
            .DistalSegment[DistalSegmentIndex]
            .Synapses[SynapseIndex]
            .CellIndex = SynapseCellIndex;
        Columns[columnIndex]
            .ColumnCells[cellIndex]
            .DistalSegment[DistalSegmentIndex]
            .Synapses[SynapseIndex]
            .PreviousPermanenceValue = Columns[columnIndex]
                                           .ColumnCells[cellIndex]
                                           .DistalSegment[DistalSegmentIndex]
                                           .Synapses[SynapseIndex]
                                           .CurrentPermanenceValue =
            Columns[columnIndex]
                .ColumnCells[cellIndex]
                .DistalSegment[DistalSegmentIndex]
                .Synapses[SynapseIndex]
                .InitialPermanenceValue = 0;
      }
    }
    break;
  case CRegion::SDistalSynapse::SPHERE_RADIUS: {
    QList<int> PermitColumns;
    CurrentRowIndex = columnIndex / Width;
    CurrentColumnIndex = columnIndex % Width;
    for (RowIndex = 0; RowIndex < Height; ++RowIndex)
      for (ColumnIndex = 0; ColumnIndex < Width; ++ColumnIndex)
        if (sqrt((CurrentColumnIndex - ColumnIndex) *
                     (CurrentColumnIndex - ColumnIndex) +
                 (CurrentRowIndex - RowIndex) * (CurrentRowIndex - RowIndex)) <
            SpreadingRadius)
          PermitColumns.append(RowIndex * Height + ColumnIndex);
    iSpreadingRadius = (int)SpreadingRadius;
    for (DistalSegmentIndex = 0;
         DistalSegmentIndex <
         Columns[columnIndex].ColumnCells[cellIndex].DistalSegmentSize;
         ++DistalSegmentIndex) {
      Columns[columnIndex]
          .ColumnCells[cellIndex]
          .DistalSegment[DistalSegmentIndex]
          .SynapseListSize = SynapseValue = normalDistribution(
          DistalSegmentSynapseValue, DistalSegmentSynapseVariationPersentage);
      Columns[columnIndex]
          .ColumnCells[cellIndex]
          .DistalSegment[DistalSegmentIndex]
          .Synapses = new SDistalSynapse[SynapseValue];
      Columns[columnIndex]
          .ColumnCells[cellIndex]
          .DistalSegment[DistalSegmentIndex]
          .ActivationThreshold =
          (SynapseValue * DistalSegmentSynapseThresholdPersentage) / 100;
      for (SynapseIndex = 0; SynapseIndex < SynapseValue; ++SynapseIndex) {
        SynapseColumnIndex = PermitColumns[rand() % (PermitColumns.size())];
        Columns[columnIndex]
            .ColumnCells[cellIndex]
            .DistalSegment[DistalSegmentIndex]
            .Synapses[SynapseIndex]
            .ColumnIndex = SynapseColumnIndex;
        while ((SynapseCellIndex =
                    (rand() % iSpreadingRadius) % ColumnHeight) == cellIndex &&
               columnIndex == SynapseColumnIndex)
          ;
        Columns[columnIndex]
            .ColumnCells[cellIndex]
            .DistalSegment[DistalSegmentIndex]
            .Synapses[SynapseIndex]
            .CellIndex = SynapseCellIndex;
        Columns[columnIndex]
            .ColumnCells[cellIndex]
            .DistalSegment[DistalSegmentIndex]
            .Synapses[SynapseIndex]
            .PreviousPermanenceValue = Columns[columnIndex]
                                           .ColumnCells[cellIndex]
                                           .DistalSegment[DistalSegmentIndex]
                                           .Synapses[SynapseIndex]
                                           .CurrentPermanenceValue =
            Columns[columnIndex]
                .ColumnCells[cellIndex]
                .DistalSegment[DistalSegmentIndex]
                .Synapses[SynapseIndex]
                .InitialPermanenceValue = 0;
      }
    }
  } break;
  case CRegion::SDistalSynapse::CORTEX_RANDOM:
    for (DistalSegmentIndex = 0;
         DistalSegmentIndex <
         Columns[columnIndex].ColumnCells[cellIndex].DistalSegmentSize;
         ++DistalSegmentIndex) {
      Columns[columnIndex]
          .ColumnCells[cellIndex]
          .DistalSegment[DistalSegmentIndex]
          .SynapseListSize = SynapseValue = normalDistribution(
          DistalSegmentSynapseValue, DistalSegmentSynapseVariationPersentage);
      Columns[columnIndex]
          .ColumnCells[cellIndex]
          .DistalSegment[DistalSegmentIndex]
          .Synapses = new SDistalSynapse[SynapseValue];
      Columns[columnIndex]
          .ColumnCells[cellIndex]
          .DistalSegment[DistalSegmentIndex]
          .ActivationThreshold =
          (SynapseValue * DistalSegmentSynapseThresholdPersentage) / 100;
      for (SynapseIndex = 0; SynapseIndex < SynapseValue; ++SynapseIndex) {
        while ((SynapseColumnIndex = rand() % (Width * Height)) == columnIndex)
          ;
        Columns[columnIndex]
            .ColumnCells[cellIndex]
            .DistalSegment[DistalSegmentIndex]
            .Synapses[SynapseIndex]
            .ColumnIndex = SynapseColumnIndex;
        Columns[columnIndex]
            .ColumnCells[cellIndex]
            .DistalSegment[DistalSegmentIndex]
            .Synapses[SynapseIndex]
            .CellIndex = cellIndex;
        Columns[columnIndex]
            .ColumnCells[cellIndex]
            .DistalSegment[DistalSegmentIndex]
            .Synapses[SynapseIndex]
            .PreviousPermanenceValue = Columns[columnIndex]
                                           .ColumnCells[cellIndex]
                                           .DistalSegment[DistalSegmentIndex]
                                           .Synapses[SynapseIndex]
                                           .CurrentPermanenceValue =
            Columns[columnIndex]
                .ColumnCells[cellIndex]
                .DistalSegment[DistalSegmentIndex]
                .Synapses[SynapseIndex]
                .InitialPermanenceValue = 0;
      }
    }
    break;
  case CRegion::SDistalSynapse::CORTEX_RADIUS: {
    QList<int> PermitColumns;
    CurrentRowIndex = columnIndex / Width;
    CurrentColumnIndex = columnIndex % Width;
    for (RowIndex = 0; RowIndex < Height; ++RowIndex)
      for (ColumnIndex = 0; ColumnIndex < Width; ++ColumnIndex)
        if (sqrt((CurrentColumnIndex - ColumnIndex) *
                     (CurrentColumnIndex - ColumnIndex) +
                 (CurrentRowIndex - RowIndex) * (CurrentRowIndex - RowIndex)) <
            SpreadingRadius)
          PermitColumns.append(RowIndex * Height + ColumnIndex);
    iSpreadingRadius = (int)SpreadingRadius;
    for (DistalSegmentIndex = 0;
         DistalSegmentIndex <
         Columns[columnIndex].ColumnCells[cellIndex].DistalSegmentSize;
         ++DistalSegmentIndex) {
      Columns[columnIndex]
          .ColumnCells[cellIndex]
          .DistalSegment[DistalSegmentIndex]
          .SynapseListSize = SynapseValue = normalDistribution(
          DistalSegmentSynapseValue, DistalSegmentSynapseVariationPersentage);
      Columns[columnIndex]
          .ColumnCells[cellIndex]
          .DistalSegment[DistalSegmentIndex]
          .Synapses = new SDistalSynapse[SynapseValue];
      Columns[columnIndex]
          .ColumnCells[cellIndex]
          .DistalSegment[DistalSegmentIndex]
          .ActivationThreshold =
          (SynapseValue * DistalSegmentSynapseThresholdPersentage) / 100;
      for (SynapseIndex = 0; SynapseIndex < SynapseValue; ++SynapseIndex) {
        while ((SynapseColumnIndex =
                    PermitColumns[rand() % (PermitColumns.size())]) ==
               columnIndex)
          ;
        Columns[columnIndex]
            .ColumnCells[cellIndex]
            .DistalSegment[DistalSegmentIndex]
            .Synapses[SynapseIndex]
            .ColumnIndex = SynapseColumnIndex;
        Columns[columnIndex]
            .ColumnCells[cellIndex]
            .DistalSegment[DistalSegmentIndex]
            .Synapses[SynapseIndex]
            .CellIndex = cellIndex;
        Columns[columnIndex]
            .ColumnCells[cellIndex]
            .DistalSegment[DistalSegmentIndex]
            .Synapses[SynapseIndex]
            .PreviousPermanenceValue = Columns[columnIndex]
                                           .ColumnCells[cellIndex]
                                           .DistalSegment[DistalSegmentIndex]
                                           .Synapses[SynapseIndex]
                                           .CurrentPermanenceValue =
            Columns[columnIndex]
                .ColumnCells[cellIndex]
                .DistalSegment[DistalSegmentIndex]
                .Synapses[SynapseIndex]
                .InitialPermanenceValue = 0;
      }
    }
  } break;
  }
}

void CRegion::recognizeAndPredict(QBitArray &BitSet) {
  pCurrentBitSet = &BitSet;
  spartialPooler();
  temporalPooler();
}

void CRegion::spartialPooler() {
  int ColumnIndex, SynapseIndex, OverlapValue,
      InactiveConnectedProximalSynapses;
  /*
   *  Phase 1: Overlap
   */

  qDebug() << "Phase 1: Overlap\n";

  for (ColumnIndex = 0; ColumnIndex < Width * Height; ++ColumnIndex) {
    OverlapValue = 0;
    InactiveConnectedProximalSynapses = 0;

    qDebug() << "max = " << Columns[ColumnIndex].ProximalDendriticSegmentSize << "\n";

    for (SynapseIndex = 0;
         SynapseIndex < Columns[ColumnIndex].ProximalDendriticSegmentSize;
         ++SynapseIndex)
      if (Columns[ColumnIndex]
              .ProximalDendriticSegment[SynapseIndex]
              .IsActive) {

        qDebug() << "accessing " << Columns[ColumnIndex]
                    .ProximalDendriticSegment[SynapseIndex]
                    .BitIndex << "\n";

        OverlapValue +=
            (*pCurrentBitSet)[Columns[ColumnIndex]
                                  .ProximalDendriticSegment[SynapseIndex]
                                  .BitIndex]
                ? 1
                : 0;
        if (!(*pCurrentBitSet)[Columns[ColumnIndex]
                                   .ProximalDendriticSegment[SynapseIndex]
                                   .BitIndex])
          ++InactiveConnectedProximalSynapses;
      }
    Columns[ColumnIndex].OverlapValue =
        (OverlapValue >= Columns[ColumnIndex].MinOverlapValue)
            ? OverlapValue * ((float)(OverlapValue) /
                              ((float)(OverlapValue) +
                               (float)(InactiveConnectedProximalSynapses)) *
                              Columns[ColumnIndex].Boost)
            : 0;
  }
  AverageReceptiveFieldSize = averageReceptiveFieldSize();
  /*
   * Phase 2: Inhibition
   */
  qDebug() << "Phase 2: Inhibition\n";

  QList<int> NeighborsList;
  for (ColumnIndex = 0; ColumnIndex < Width * Height; ++ColumnIndex) {
    neighbors(ColumnIndex, NeighborsList);
    Columns[ColumnIndex].IsActive = (Columns[ColumnIndex].OverlapValue > 0 &&
                                     kthScore(NeighborsList, ColumnIndex));
    NeighborsList.clear();
    emit redrawColumn(ColumnIndex, Columns[ColumnIndex].IsActive);
  }

  /*
   * Phase 3: Learning
   */
  qDebug() << "Phase 3: Learning\n";


  for (ColumnIndex = 0; ColumnIndex < Width * Height; ++ColumnIndex) {
    if (Columns[ColumnIndex].IsActive) {
      for (SynapseIndex = 0;
           SynapseIndex < Columns[ColumnIndex].ProximalDendriticSegmentSize;
           ++SynapseIndex) {
        if (Columns[ColumnIndex]
                .ProximalDendriticSegment[SynapseIndex]
                .IsActive)
          Columns[ColumnIndex]
              .ProximalDendriticSegment[SynapseIndex]
              .PermanenceValue =
              qMin(Columns[ColumnIndex].ProximalDendriticSegmentSize,
                   mPersentage(Columns[ColumnIndex]
                                   .ProximalDendriticSegment[SynapseIndex]
                                   .PermanenceValue,
                               PermanenceIncMPersentage));
        else
          Columns[ColumnIndex]
              .ProximalDendriticSegment[SynapseIndex]
              .PermanenceValue =
              qMax(0, mPersentage(Columns[ColumnIndex]
                                      .ProximalDendriticSegment[SynapseIndex]
                                      .PermanenceValue,
                                  -PermanenceDecMPersentage));
      }
    }
    neighbors(ColumnIndex, NeighborsList);
    Columns[ColumnIndex].MinDutyCycle =
        0.01 *
        (Columns[ColumnIndex].MaxDutyCycle = maxDutyCycle(NeighborsList));
    updateActiveDutyCycle(ColumnIndex);
    updateOverlapDutyCycle(ColumnIndex);
    boostFunction(ColumnIndex);
  }
  // DesiredLocalActivity was here!
}

int CRegion::averageReceptiveFieldSize() {
  int ColumnIndex, SinapseIndex, ColumnActiveSinapseCount(0);
  for (ColumnIndex = 0; ColumnIndex < Width * Height; ++ColumnIndex)
    for (SinapseIndex = 0;
         SinapseIndex < Columns[ColumnIndex].ProximalDendriticSegmentSize;
         ++SinapseIndex)
      if (Columns[ColumnIndex]
              .ProximalDendriticSegment[SinapseIndex]
              .PermanenceValue >= Columns[ColumnIndex].Threshold)
        ++ColumnActiveSinapseCount;
  return ColumnActiveSinapseCount / (Width * Height);
}

void CRegion::neighbors(const int &columnIndex, QList<int> &neighborsList) {
  int ColumnIndex;
  for (ColumnIndex = 0; ColumnIndex < Width * Height; ++ColumnIndex) {
    if (ColumnIndex == columnIndex)
      continue;
    if ((Columns[ColumnIndex].Center <= Columns[columnIndex].Center &&
         Columns[ColumnIndex].Center >=
             (Columns[columnIndex].Center - AverageReceptiveFieldSize)) ||
        (Columns[ColumnIndex].Center >= Columns[columnIndex].Center &&
         Columns[ColumnIndex].Center <=
             Columns[columnIndex].Center + AverageReceptiveFieldSize))
      neighborsList.push_back(ColumnIndex);
  }
}

bool CRegion::kthScore(QList<int> &neighborsList, const int &columnIndex) {
  int kth = 0;
  QList<int>::iterator I, End;
  for (I = neighborsList.begin(), End = neighborsList.end(); I != End; ++I)
    kth += (Columns[(*I)].OverlapValue > Columns[columnIndex].OverlapValue) ? 1
                                                                            : 0;
  return kth < DesiredLocalActivity;
}

double CRegion::maxDutyCycle(QList<int> &neighborsList) {
  QList<int>::iterator I, End;
  double MaxValue = 0.;
  for (I = neighborsList.begin(), End = neighborsList.end(); I != End; ++I)
    if (MaxValue < Columns[(*I)].ActiveDutyCycle)
      MaxValue =
          Columns[(*I)]
              .ActiveDutyCycle; // if(MaxValue < Region[(*I)].MinDutyCycle)
                                // MaxValue = Region[(*I)].MinDutyCycle; HERE!!!
  return MaxValue;
}

void CRegion::updateActiveDutyCycle(const int &columnIndex) {
  Columns[columnIndex].ActiveDutyCycle *= (1.l - MovingAverage);
  if (Columns[columnIndex].IsActive)
    Columns[columnIndex].ActiveDutyCycle += MovingAverage;
}

void CRegion::updateOverlapDutyCycle(const int &columnIndex) {
  Columns[columnIndex].OverlapDutyCycle *= (1.l - MovingAverage);
  if (Columns[columnIndex].OverlapValue / Columns[columnIndex].Boost >=
      Columns[columnIndex].MinOverlapValue)
    Columns[columnIndex].OverlapDutyCycle += MovingAverage;
}

void CRegion::boostFunction(const int &columnIndex) {
  SColumn &Column = Columns[columnIndex];
  int SynapseIndex;
  if (Column.ActiveDutyCycle < (Column.MaxDutyCycle * BoostIncThreshold)) {
    if (Column.Boost < Column.MaxBoost) {
      if (Column.PrevBoostTime < Iteration)
        for (SynapseIndex = 0;
             SynapseIndex < Column.ProximalDendriticSegmentSize; ++SynapseIndex)
          if (Column.ProximalDendriticSegment[SynapseIndex].IsActive)
            Column.ProximalDendriticSegment[SynapseIndex].PermanenceValue =
                Column.Threshold;
      Column.Boost = qMin(Column.Boost + BoostRate, Column.MaxBoost);
      Column.PrevBoostTime = Iteration;
    } else {
      for (SynapseIndex = 0; SynapseIndex < Column.ProximalDendriticSegmentSize;
           ++SynapseIndex)
        if (!Column.ProximalDendriticSegment[SynapseIndex].IsActive)
          Column.ProximalDendriticSegment[SynapseIndex].PermanenceValue = qMin(
              Column.Threshold,
              (int)(Column.ProximalDendriticSegment[SynapseIndex]
                        .PermanenceValue +
                    BoostRate *
                        (double)(Column.ProximalDendriticSegmentSize - 1)));
        else
          Column.ProximalDendriticSegment[SynapseIndex].PermanenceValue = qMax(
              Column.Threshold,
              (int)(Column.ProximalDendriticSegment[SynapseIndex]
                        .PermanenceValue -
                    BoostRate *
                        (double)(Column.ProximalDendriticSegmentSize - 1)));
    }
  } else if (Column.Boost > Column.MinBoost &&
             Column.ActiveDutyCycle > (Column.MaxDutyCycle * BoostIncThreshold))
    Column.Boost = qMax(Column.Boost - BoostRate, Column.MinBoost);
}

// temporal
void CRegion::temporalPooler() {
  /*
   * Phase 1: Cells active state
   */
  int ColumnIndex, CellIndex, BestCellIndex, BestSegmentIndex;
  bool IsPredicted, IsLearnChosen;
  SDistalSegment *ActiveSegment;

  for (ColumnIndex = 0; ColumnIndex < Width * Height; ++ColumnIndex) {
    if (!Columns[ColumnIndex].IsActive)
      continue;
    IsPredicted = IsLearnChosen = false;
    for (CellIndex = 0; CellIndex < ColumnHeight; ++CellIndex) {
      if (Columns[ColumnIndex].ColumnCells[CellIndex].WasPredicted) {
        ActiveSegment =
            Columns[ColumnIndex].ColumnCells[CellIndex].getActiveSegment();
        if (ActiveSegment != NULL && ActiveSegment->IsSequence) {
          IsPredicted = true;
          Columns[ColumnIndex].ColumnCells[CellIndex].IsActive = true;
          if (segmentWasActiveFromLearning(ActiveSegment)) {
            IsLearnChosen = true;
            Columns[ColumnIndex].ColumnCells[CellIndex].IsLearning = true;
          }
        }
      }
    }
    if (!IsPredicted)
      for (CellIndex = 0; CellIndex < ColumnHeight; ++CellIndex)
        Columns[ColumnIndex].ColumnCells[CellIndex].IsActive = true;
    if (!IsLearnChosen) {
      getBestMatchingCell(ColumnIndex, false, BestCellIndex, BestSegmentIndex);
      Columns[ColumnIndex].ColumnCells[BestCellIndex].IsLearning = true;
    }
  }
}

bool CRegion::segmentWasActiveFromLearning(SDistalSegment *&activeSegment) {
  int SynapseIndex, activeFromLearningCount = 0, ColumnIndex, CellIndex;
  for (SynapseIndex = 0; SynapseIndex < activeSegment->SynapseListSize;
       ++SynapseIndex) {
    ColumnIndex = activeSegment->Synapses[SynapseIndex].ColumnIndex;
    CellIndex = activeSegment->Synapses[SynapseIndex].CellIndex;
    activeFromLearningCount +=
        (Columns[ColumnIndex].ColumnCells[CellIndex].WasActive &&
         Columns[ColumnIndex].ColumnCells[CellIndex].WasLearning)
            ? 1
            : 0;
  }
  return activeFromLearningCount >= activeSegment->ActivationThreshold;
}

void CRegion::getBestMatchingCell(const int &columnIndex,
                                  const bool &IsCurrentStep, int &bestCellIndex,
                                  int &bestSegmentIndex) {
  bestCellIndex = bestSegmentIndex = -1;
  int CellIndex, ActiveSynapseCount, SegmentIndex, MaxActiveSynapses = 0,
                                                   SegmentsSizes = INT_MAX,
                                                   MostLikelySegmentsSizes = 0;
  for (CellIndex = 0; CellIndex < ColumnHeight; ++CellIndex) {
    SegmentIndex =
        Columns[columnIndex].ColumnCells[CellIndex].bestMatchingSegmentIndex(
            Columns[columnIndex].MinOverlapValue, IsCurrentStep);
    if (SegmentIndex != -1) {
      ActiveSynapseCount = IsCurrentStep ? Columns[columnIndex]
                                               .ColumnCells[CellIndex]
                                               .DistalSegment[SegmentIndex]
                                               .ActiveSynapseCount
                                         : Columns[columnIndex]
                                               .ColumnCells[CellIndex]
                                               .DistalSegment[SegmentIndex]
                                               .PrevActiveSynapseCount;
      if (ActiveSynapseCount > MaxActiveSynapses) {
        MaxActiveSynapses = ActiveSynapseCount;
        bestCellIndex = CellIndex;
        bestSegmentIndex = SegmentIndex;
      }
    }
  }
  if (bestCellIndex == -1) {
    for (CellIndex = 0; CellIndex < ColumnHeight; ++CellIndex) {
      if (Columns[columnIndex].ColumnCells[CellIndex].DistalSegmentSize <
          SegmentsSizes)
        MostLikelySegmentsSizes = 1;
      else if (Columns[columnIndex].ColumnCells[CellIndex].DistalSegmentSize ==
               SegmentsSizes)
        ++MostLikelySegmentsSizes;
      if ((Columns[columnIndex].ColumnCells[CellIndex].DistalSegmentSize <
           SegmentsSizes) ||
          (Columns[columnIndex].ColumnCells[CellIndex].DistalSegmentSize ==
               SegmentsSizes &&
           (rand() % MostLikelySegmentsSizes) == 0)) {
        SegmentsSizes =
            Columns[columnIndex].ColumnCells[CellIndex].DistalSegmentSize;
        bestCellIndex = CellIndex;
      }
    }
  }
}

// end of temporal
