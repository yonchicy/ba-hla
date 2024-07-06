#ifndef __TRANSPORT_TASK_H__
#define __TRANSPORT_TASK_H__

#include "TransportTaskFederate.h"
#include "Road.h"
class Position {
private:
  // 6378.137km
  static constexpr double EARTH_RADIUS = 6378.137;
  static constexpr double PI = 3.141592653589793238462643383279;
  static double rad(double d);
  

public:
  // 经度
  double lng;
  // 维度
  double lat;
  Position(double lng_, double lat_);
  static double getRealDistance(Position x, Position y);
  static double getEulerDistance(Position x, Position y);
};

class TransportTask {
private:
  unsigned long long idx;
  Node pos;
  Node destination;
  double speed;
  double localTime;
  double timeStep;
  bool arriveDesitination;

  TransportTaskFederate *federate;
  std::list<Position> path;

public:
  TransportTask(unsigned long long idx_, Node from_, Node to_,
                double speed_, double localTime_, double timeStep_,
                TransportTaskFederate *fed);
  bool arrive();
  unsigned long long getIdx();
  void findPath();
  void iterate();
  void goNextPosition();
};

void dispatchTransportTask(TransportTask task);

#endif // __TRANSPORT_TASK_H__
