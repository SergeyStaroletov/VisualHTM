#ifndef DEFINES_H
#define DEFINES_H

#include <QBitArray>
#include <QColor>
#include <QList>
#include <QVector>
#include <qmath.h>

const double MovingAverage =
    0.005l; // константа для определения среднего интервального
const double BoostVariation = 0.01l; // вариация для значения ускорения колонки
const double BoostIncThreshold =
    0.01l; // порог для увеличения значения ускорения колонки
const double BoostDecThreshold =
    0.05l; // порог для уменьшения значения ускорения колонки
const double BoostRate = 0.01l; // скорость увеличения ускорения

inline double distributionFunction(const double &Sigma, const double &Mu,
                                   const double &x) {
  return exp(-(x - Mu) * (x - Mu) / (2. * Sigma * Sigma));
}

inline int normalDistribution(const int &value, const int &persentage) {
  return (value + (rand() % 2 == 0 ? 1 : -1) *
                      (rand() % (value * persentage + 1)) / 100);
}

inline int mPersentage(const int &value, const int &persentage,
                       const int &m = 100000) {
  return (value + (value * persentage) / m);
}

#define CELL_SPACE_DEF_VALUE 20.
#define CELL_RADIUS_DEF_VALUE 30.

#define FILE_TYPE_UNDEF 0
#define FILE_TYPE_BINARY 1
#define FILE_TYPE_TEXT 2

#define PROXIMAL_SYNAPSE_VALUE_DEF_VALUE 100
#define PROXIMAL_SYNAPSE_VARIATION_PERSENTAGE_DEF_VALUE 15
#define MIN_OVERLAP_VALUE_PERSENTAGE_DEF_VALUE 25
#define PROXIMAL_SYNAPSE_PERMANENCE_VALUE_PERSENTAGE_DEF_VALUE 70
#define PROXIMAL_SYNAPSE_PERMANENCE_VARIATION_PERSENTAGE_DEF_VALUE 20
#define DESIRED_LOCAL_ACTIVITY_DEF_VALUE 10

#define DISTAL_SEGMENT_COUNT_DEF_VALUE 15
#define DISTAL_SEGMENT_VARIATION_PERSENTAGE_DEF_VALUE 10
#define DISTAL_SEGMENT_SYNAPSE_COUNT_DEF_VALUE 30
#define DISTAL_SEGMENT_SYNAPSE_VARIATION_PERSENTAGE_DEF_VALUE 10
#define DISTAL_SEGMENT_SYNAPSE_THRESHOLD_PERSENTAGE_DEF_VALUE 70

#define ITERATION_FOR_SPARTIAL_POOLER 1000
#define PERMANENCE_INC_M_PERSENTAGE_DEF_VALUE 1000
#define PERMANENCE_DEC_M_PERSENTAGE_DEF_VALUE 1000
#define BOOST_DEF_VALUE 1.
#define MAX_BOOST_DEF_VALUE 5.
#define SPREADING_RADIUS_DEF_VALUE 50.
#define ACTIVE_DUTY_CYCLE_DEF_VALUE 1.
#define OVERLAP_DUTY_CYCLE_DEF_VALUE 1.

struct SDefaultValues {
  SDefaultValues(
      const QColor &activeCellColor = QColor(Qt::blue),
      const QColor &inactiveCellColor = QColor(Qt::gray),
      const int &proximalSynapseValue = PROXIMAL_SYNAPSE_VALUE_DEF_VALUE,
      const int &proximalSynapseVariationPersentage =
          PROXIMAL_SYNAPSE_VARIATION_PERSENTAGE_DEF_VALUE,
      const int &minOverlapValuePersentage =
          MIN_OVERLAP_VALUE_PERSENTAGE_DEF_VALUE,
      const int &proximalSynapsePermanenceValuePersentage =
          PROXIMAL_SYNAPSE_PERMANENCE_VALUE_PERSENTAGE_DEF_VALUE,
      const int &proximalSynapsePermanenceVariationPersentage =
          PROXIMAL_SYNAPSE_PERMANENCE_VARIATION_PERSENTAGE_DEF_VALUE,
      const int &desiredLocalActivity = DESIRED_LOCAL_ACTIVITY_DEF_VALUE,
      const int &distalSegmentCount = DISTAL_SEGMENT_COUNT_DEF_VALUE,
      const int &distalSegmentVariationPersentage =
          DISTAL_SEGMENT_VARIATION_PERSENTAGE_DEF_VALUE,
      const int &distalSegmentSynapseCount =
          DISTAL_SEGMENT_SYNAPSE_COUNT_DEF_VALUE,
      const int &distalSegmentSynapseVariationPersentage =
          DISTAL_SEGMENT_SYNAPSE_VARIATION_PERSENTAGE_DEF_VALUE,
      const int &distalSegmentSynapseThresholdPersentage =
          DISTAL_SEGMENT_SYNAPSE_THRESHOLD_PERSENTAGE_DEF_VALUE,
      const int &iterationForSpartialPooler = ITERATION_FOR_SPARTIAL_POOLER,
      const int &permanenceIncMPersentage =
          PERMANENCE_INC_M_PERSENTAGE_DEF_VALUE,
      const int &permanenceDecMPersentage =
          PERMANENCE_DEC_M_PERSENTAGE_DEF_VALUE,
      const double &boost = BOOST_DEF_VALUE,
      const double &maxBoost = MAX_BOOST_DEF_VALUE,
      const double &spreadingRadius = SPREADING_RADIUS_DEF_VALUE,
      const double &activeDutyCycle = ACTIVE_DUTY_CYCLE_DEF_VALUE,
      const double &overlapDutyCycle = OVERLAP_DUTY_CYCLE_DEF_VALUE)
      : ActiveCellColor(activeCellColor), InactiveCellColor(inactiveCellColor),
        ProximalSynapseValue(proximalSynapseValue),
        ProximalSynapseVariationPersentage(proximalSynapseVariationPersentage),
        MinOverlapValuePersentage(minOverlapValuePersentage),
        ProximalSynapsePermanenceValuePersentage(
            proximalSynapsePermanenceValuePersentage),
        ProximalSynapsePermanenceVariationPersentage(
            proximalSynapsePermanenceVariationPersentage),
        DesiredLocalActivity(desiredLocalActivity),
        DistalSegmentCount(distalSegmentCount),
        DistalSegmentVariationPersentage(distalSegmentVariationPersentage),
        DistalSegmentSynapseCount(distalSegmentSynapseCount),
        DistalSegmentSynapseVariationPersentage(
            distalSegmentSynapseVariationPersentage),
        DistalSegmentSynapseThresholdPersentage(
            distalSegmentSynapseThresholdPersentage),
        IterationForSpartialPooler(iterationForSpartialPooler),
        PermanenceIncMPersentage(permanenceIncMPersentage),
        PermanenceDecMPersentage(permanenceDecMPersentage), Boost(boost),
        MaxBoost(maxBoost), SpreadingRadius(spreadingRadius),
        ActiveDutyCycle(activeDutyCycle), OverlapDutyCycle(overlapDutyCycle) {}
  void optimizeProximalSynapseValue(const int &regionWidth,
                                    const int &regionHeight,
                                    const int &bitSetSize,
                                    const double &safeFactor);
  QColor ActiveCellColor, InactiveCellColor;
  int ProximalSynapseValue, ProximalSynapseVariationPersentage,
      MinOverlapValuePersentage, ProximalSynapsePermanenceValuePersentage,
      ProximalSynapsePermanenceVariationPersentage, DesiredLocalActivity,
      DistalSegmentCount, DistalSegmentVariationPersentage,
      DistalSegmentSynapseCount, DistalSegmentSynapseVariationPersentage,
      DistalSegmentSynapseThresholdPersentage, IterationForSpartialPooler,
      PermanenceIncMPersentage, PermanenceDecMPersentage;
  double Boost, MaxBoost, SpreadingRadius, ActiveDutyCycle, OverlapDutyCycle;
};

inline void SDefaultValues::optimizeProximalSynapseValue(
    const int &regionWidth, const int &regionHeight, const int &bitSetSize,
    const double &safeFactor) {
  if (regionWidth * regionHeight >= safeFactor * bitSetSize)
    ProximalSynapseValue = 1;
  else
    ProximalSynapseValue =
        (safeFactor * bitSetSize) / (regionWidth * regionHeight);
}

const SDefaultValues DefaultValues;

// using namespace std;

// template<int BitsetSize>
// class CCircularBitQueue{
// public:
//    enum _Error{
//        NO_ERROR            = 0x0000,
//        ERROR_EMPTY_QUEUE   = 0x0001
//    };

//    CCircularBitQueue() : Head(-1),Tail(-1),Size(BitsetSize),Count(0) {
//    fill(false); }

//    bool operator [](const int &index) const { return Values[index]; }
//    typename bitset<BitsetSize>::reference operator [](const int &index){
//    return Values[index]; }

//    int size() const { return Size; }

//    void fill(const bool &value);
//    void push_front(const bool &value);
//    bool pop_back();

// protected:
//    int Head,Tail,Size,Count;
//    bitset<BitsetSize> Values;
//};

// template<int BitsetSize>
// void CCircularBitQueue<BitsetSize>::push_front(const bool &value){
// if(Count == 0){
//    Head = Tail = 0;
//    Values[Tail] = value;
//    ++Count;
//    }
// else{
//    Values[(++Tail) % Size] = value;
//    if(Count < Size) ++Count;
//    }
//}

// template<int BitsetSize>
// bool CCircularBitQueue<BitsetSize>::pop_back(){
// if(Count == 0) throw ERROR_EMPTY_QUEUE;
//--Count;
// bool RetVal = Values[Head++];
// Head %= Size;
// return RetVal;
//}

// template<int BitsetSize>
// void CCircularBitQueue<BitsetSize>::fill(const bool &value){
// for(int Index = 0;Index < Size;++Index)
//    Values[Index] = value;
//}

#endif
