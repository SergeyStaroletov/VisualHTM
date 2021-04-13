#ifndef CREGION_H
#define CREGION_H

#include <QColor>
#include <QObject>

#include "Defines.h"

class CRegion : public QObject {
  Q_OBJECT

protected:
  enum _CellState {
    INACTIVE = 0x0000,
    ACTIVE_FEED_FORFARD = 0x0001,
    ACTIVE_LATERAL = 0x0002,
    PREDICTION = ACTIVE_FEED_FORFARD,
    ACTIVE_STATE = ACTIVE_FEED_FORFARD,
    LEARN_STATE = ACTIVE_LATERAL
  };

  // temporal
  struct SSegmentUpdate {
    // HERE!!!
  };

  struct SDistalSynapse {
    enum _ConnectMethod {
      FULL_RANDOM = 0x0000,
      SPHERE_RADIUS = 0x0001,
      CORTEX_RANDOM = 0x0002,
      CORTEX_RADIUS = 0x0003
    };

    SDistalSynapse()
        : ColumnIndex(-1), CellIndex(-1), CurrentPermanenceValue(-1),
          PreviousPermanenceValue(-1), InitialPermanenceValue(-1) {}
    SDistalSynapse(const int &columnIndex, const int &cellIndex,
                   const int &permanenceValue);
    int ColumnIndex, // координаты нейрона, с которым связан текущий нейрон,
        CellIndex; // который содержит дистальный сегмент, содержащий данный
                   // дистальный синапс
    int CurrentPermanenceValue, // текущее значение перманентности
        PreviousPermanenceValue, // значение перманентности на предыдущей
                                 // итерации
        InitialPermanenceValue; // инициализирующее значение перманентности
  };

  struct SDistalSegment {
    SDistalSegment();
    SDistalSynapse *Synapses;
    int SynapseListSize;
    QList<SSegmentUpdate> SegmentUpdateList;
    int ActivationThreshold, // порог активации дистального сегмента
        ActiveSynapseCount, // количество активных синапсов на текущей итеации
        PrevActiveSynapseCount; // количество активных синапсов на предыдущей
                                // итерации
    bool IsSequence;
  };

  struct SCell {
    SCell();
    SDistalSegment *DistalSegment;
    int CurrentState,  // Текущее состояние нейрона
        PreviousState, // Предыдущее состояние нейрона
        DistalSegmentSize; // Размер дистального сегмента
    bool IsPredicted, // Текущее значение состояния "предчувствия"(клетка стала
                      // активной благодаря латеральным связям)
        WasPredicted, // Предыдущее значение состояния "предчувствия"(клетка
                      // стала активной благодаря латеральным связям)
        IsActive, // Нейрон находится в состоянии активности
        WasActive, // Нейрон находился в состоянии активности на предыдущей
                   // итерации
        IsLearning, // Нейрон находится в состоянии обучения
        WasLearning; // Нейрон находился в состоянии обучения на предыдущей
                     // итерации
    SDistalSegment *getActiveSegment();
    int bestMatchingSegmentIndex(const int &minOverlap,
                                 const bool &IsCurrentStep);
  };
  // end of temporal

  struct SProximalSynapse {
    bool IsActive; // флаг активности проксимального синапса
    int BitIndex, // индекс входного вектора
        PermanenceValue; // значение перманентности синапса
  };

  struct SColumn {
    SColumn();
    SColumn(const int &height);
    ~SColumn();

    int ProximalDendriticSegmentSize, // количество проксимальных синапсов
        Threshold, // порог синапсов, необходимых для активации колонки
        MinOverlapValue, // минимальное значение перекрытия для колонки, а также
                         // минимальный порог, необходимый для задействования
                         // сегмента в процессе обучения
        OverlapValue, // значение перекрытия колонки -> число действующих
                      // синапсов, подключенных к активным входным битам
                      // подмножества образа, умноженное на фактор
                      // ускорения("агрессивности") колонки
        PrevBoostTime, // предыдущая итерация на которой колонка была ускорена
        Center; // центр колонки -> индекс центра масс подмножества
                // исходного множества(битовый массив),
                // инициализирующее проксимальный дендритный сегмент
    bool IsActive; // состояние активности колонки
    //        CCircularBitQueue<ITERATION_FOR_SPARTIAL_POOLER>    ActiveDuty, //
    //        состояния активности колонки на 1000 итераций
    //                                                            OverlapDuty;
    //                                                            // состояния
    //                                                            значения
    //                                                            перекрытия
    //                                                            колонки
    //                                                                                    // больше Threshold на 1000 итераций
    SCell *ColumnCells;
    SProximalSynapse *ProximalDendriticSegment;
    double Boost, // фактор ускорения колонки
        MinBoost, // минимальное значение ускорения колонки
        MaxBoost, // максисальное значение ускорения колонки
        MinDutyCycle, // минимальная желательная частота активации для клетки ->
                      // заменено на колонку
        MaxDutyCycle, // максимальная частота активации клетки -> заменено на
                      // колонку
        ActiveDutyCycle, // интервальное среднее частоты активности колонки
                         // после подавления
        OverlapDutyCycle; // интервальное среднее частоты превышения колонки
                          // значения минимального перекрытия
    void resizeColumn(const int &height);
  };

public slots:
  void setGeometry(const int &width, const int &height,
                   const int &columnHeight);

  // spartial pooler functions
  inline void setProximalSynapseValue(const int &sinapseValue) {
    ProximalSynapseValue = sinapseValue;
  }
  inline void setProximalSynapseVariationPersentage(const int &persent) {
    ProximalSynapseVariationPersentage = persent;
  }
  inline void setProximalSynapsePermanenceValuePersentage(const int &persent) {
    ProximalSynapsePermanenceValuePersentage = persent;
  }
  inline void
  setProximalSynapsePermanenceVariationPersentage(const int &persent) {
    ProximalSynapsePermanenceVariationPersentage = persent;
  }
  inline void setDesiredLocalActivity(const int &desiredLocalActivity) {
    DesiredLocalActivity = desiredLocalActivity;
  }
  inline void setMinOverlapValuePersentage(const int &persent) {
    MinOverlapValuePersentage = persent;
  }
  //

  // temporal pooler functions
  inline void setDistalSegmentValue(const int &segmentValue) {
    DistalSegmentValue = segmentValue;
  }
  inline void setDistalSegmentVariationPersentage(const int &persent) {
    DistalSegmentVariationPersentage = persent;
  }
  inline void setDistalSegmentSynapseValue(const int &synapseValue) {
    DistalSegmentSynapseValue = synapseValue;
  }
  inline void setDistalSegmentSynapseVariationPersentage(const int &persent) {
    DistalSegmentSynapseVariationPersentage = persent;
  }
  inline void setDistalSegmentSynapseThresholdPersentage(const int &persent) {
    DistalSegmentSynapseThresholdPersentage = persent;
  }
  //

signals:
  void geometryChanged(const int &width, const int &height,
                       const int &columnHeight);
  void redrawColumn(const int &columnIndex, const bool &isActive);

public:
  CRegion(const int &width, const int &height, const int &columnHeight);
  ~CRegion();

  // temporal pooler
  bool segmentWasActiveFromLearning(SDistalSegment *&segmentIndex);
  void getBestMatchingCell(const int &columnIndex, const bool &IsCurrentStep,
                           int &bestCellIndex, int &bestSegmentIndex);
  //

  void init(const int &BitSetSize);
  void recognizeAndPredict(const QBitArray *BitSet);
  void spartialPooler();
  void temporalPooler();

  // get parameters of region
  inline SColumn *columns() const { return Columns; }
  inline int bitSetSize() const { return BitSetSize; }
  inline int iteration() const { return Iteration; }
  inline int width() const { return Width; }
  inline int height() const { return Height; }
  inline int columnHeight() const { return ColumnHeight; }
  inline int proximalSynapseValue() const { return ProximalSynapseValue; }
  inline int permanenceIncMPersentage() const {
    return PermanenceIncMPersentage;
  }
  inline int permanenceDecMPersentage() const {
    return PermanenceDecMPersentage;
  }
  inline int proximalSynapseVariationPersentage() const {
    return ProximalSynapseVariationPersentage;
  }
  inline int proximalSynapsePermanenceValuePersentage() const {
    return ProximalSynapsePermanenceValuePersentage;
  }
  inline int proximalSynapsePermanenceVariationPersentage() const {
    return ProximalSynapsePermanenceVariationPersentage;
  }
  inline int desiredLocalActivity() const { return DesiredLocalActivity; }
  inline int minOverlapValuePersentage() const {
    return MinOverlapValuePersentage;
  }
  inline int distalSegmentValue() const { return DistalSegmentValue; }
  inline int distalSegmentVariationPersentage() const {
    return DistalSegmentVariationPersentage;
  }
  inline int distalSegmentSynapseValue() const {
    return DistalSegmentSynapseValue;
  }
  inline int distalSegmentSynapseVariationPersentage() const {
    return DistalSegmentSynapseVariationPersentage;
  }
  inline int distalSegmentSynapseThresholdPersentage() const {
    return DistalSegmentSynapseThresholdPersentage;
  }
  inline double spreadingRadius() const { return SpreadingRadius; }
  //
protected:
  void initDistalSegments();
  void
  initDistalSynapses(const int &columnIndex, const int &cellIndex,
                     const CRegion::SDistalSynapse::_ConnectMethod &method);
  int averageReceptiveFieldSize();
  void neighbors(const int &columnIndex, QList<int> &neighborsList);
  bool kthScore(QList<int> &neighborsList, const int &columnIndex);
  double maxDutyCycle(QList<int> &neighborsList);
  void updateActiveDutyCycle(const int &columnIndex);
  void updateOverlapDutyCycle(const int &columnIndex);
  void boostFunction(const int &columnIndex);

protected:
  int BitSetSize, // размер исходного битового образа
      Iteration,    // значение текущей итерации
      Width,        // ширина региона
      Height,       // высота региона
      ColumnHeight, // высота колонки - количество нейронов в колонке
      ProximalSynapseValue, // количество синапсов для проксимального
                            // дендритного сегмента каждой колонки
      PermanenceIncMPersentage, // процент(с точностью до тысячных) от
                                // количества синапсов проксимального
                                // дендритного сегмента колонки на который
                                // увеличивается значение пермаментности во
                                // время обучения пространственного
                                // группировщика
      PermanenceDecMPersentage, // процент(с точностью до тысячных) от
                                // количества синапсов проксимального
                                // дендритного сегмента колонки на который
                                // уменьшается значение пермаментности во время
                                // обучения пространственного группировщика
      ProximalSynapseVariationPersentage, // процент разброса значения
                                          // количества синапсов для каждой
                                          // колонки (битовый размер)
      ProximalSynapsePermanenceValuePersentage, // процент синапсов от их общего
                                                // количества в колонке
                                                // необходимый для активации
                                                // колонки(порог активации)
      ProximalSynapsePermanenceVariationPersentage, // процент разброса значения
                                                    // количества синапсов при
                                                    // вероятностном
                                                    // разбиении(разброс от
                                                    // значения порога
                                                    // активации)
      DesiredLocalActivity, // количество колонок необходимое для победы
                            // текущей колонки над соседями
      MinOverlapValuePersentage, // процент от общего количества проксималных
                                 // синапсов колонки, задающий минимальное
                                 // значение перекрытия для колонки
      AverageReceptiveFieldSize; // средний радиус подключенных рецептивных
                                 // полей всех колонок. Размер поля определяется
                                 // только по подключенным синапсам
                                 // (Synapse.PermanenceValue > Column.Threshold)
  // Temporal pooler
  int DistalSegmentValue, // количество дистальных сегментов каждого нейрона
      DistalSegmentVariationPersentage, // процент разброса значения количества
                                        // дистальных сегментов каждого нейрона
      DistalSegmentSynapseValue, // количество синапсов для дистальных
                                 // дендритных сегментов нейронов
      DistalSegmentSynapseVariationPersentage, // процент разброса значения
                                               // количества дистальных синапсов
      DistalSegmentSynapseThresholdPersentage; // процент синапсов необходимых
                                               // для активации сегмента от
                                               // общего количества синапсов
                                               // дистального сегмента
  double SpreadingRadius; // Значение радиуса распределения дистальных синапсов.
                          // Выражено в количестве нейронов по каждой из
                          // координатных осей.
  //
  SColumn *Columns;
  QBitArray *pCurrentBitSet;
};

#endif
