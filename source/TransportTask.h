#ifndef __TRANSPORT_TASK_H__
#define __TRANSPORT_TASK_H__


#include "TransportTaskFederate.h"


class Position {
public:
  // 经度
  double lng;
  // 维度
  double lat;
  static  double getDistance(Position x,Position y);
};

class TransportTask {
private:
  int idx;
  Position pos;
  Position destination;
  double speed;
  double local_time;
  double timeStep;
  bool arriveDesitination;


  TransportTaskFederate * federate;
public:
  bool arrive();
  void findPath();
  void iterate();
  void goNextPosition();
};

void dispatchTransportTask(TransportTask task);


#endif // __TRANSPORT_TASK_H__
