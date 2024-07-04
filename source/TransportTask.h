#ifndef __TRANSPORT_TASK_H__
#define __TRANSPORT_TASK_H__

#include "TransportTaskFederate.h"

class Position {
public:
  // 经度
  double lng;
  // 维度
  double lat;
  Position(double lng_,double lat_);
  static double getDistance(Position x, Position y);
};

class TransportTask {
private:
  unsigned long long idx;
  Position pos;
  Position destination;
  double speed;
  double localTime;
  double timeStep;
  bool arriveDesitination;

  TransportTaskFederate *federate;

public:
  TransportTask(unsigned long long idx_, Position from_, Position to_,
                double speed_, double localTime_, double timeStep_,TransportTaskFederate *fed);
  bool arrive();
  void findPath();
  void iterate();
  void goNextPosition();
};

void dispatchTransportTask(TransportTask task);

#endif // __TRANSPORT_TASK_H__
