#include "TransportTask.h"
#include "TransportTaskFedAmb.h"
#include "TransportTaskFederate.h"

#include <cmath>
#include <condition_variable>
#include <iostream>

Position::Position(double lng_, double lat_) : lng(lng_), lat(lat_) {}
double Position::getDistance(Position x, Position y) {
  return std::sqrt((x.lat - y.lat) * (x.lat - y.lat) +
                   (x.lng - y.lng) * (x.lng - y.lng));
}

TransportTask::TransportTask(unsigned long long idx_, Position from_,
                             Position to_, double speed_, double localTime_,
                             double timeStep_, TransportTaskFederate *fed)
    : idx(idx_), pos(from_), destination(to_), speed(speed_),
      localTime(localTime_), timeStep(timeStep_), federate(fed) {}
void dispatchTransportTask(TransportTask task) {
  while (!task.arrive()) {
    task.findPath();

    task.iterate();
  }
  std::cout << "task " << task.getIdx()<<" finish"<<std::endl;
}
void TransportTask::findPath() { std::cout << "finding path" << std::endl; }

void TransportTask::iterate() {
  std::unique_lock<std::mutex> lock(this->federate->mtx);
  // FIXME : 这里应当是 local_time + step <  federateTime + lookAhead ?
  this->federate->cv.wait(lock, [&] {
    return this->localTime < this->federate->fedamb->federateTime;
  });
  goNextPosition();

  this->localTime += this->federate->timeStep;

  lock.unlock();
  this->federate->cv.notify_all();
}
unsigned long long TransportTask::getIdx() { return this->idx; }

void TransportTask::goNextPosition() {
  double dlat = destination.lat - pos.lat;
  double dlng = destination.lng - pos.lng;
  double delta = speed * timeStep / Position::getDistance(pos, destination);
  if (delta > 1) {
    arriveDesitination = 1;
    pos = destination;
  } else {
    pos.lat += dlat * delta;
    pos.lng += dlng * delta;
  }
  std::cout << "transport task " << idx << " running to next position ";
  std::cout << "local_time is " << localTime << std::endl;
}
bool TransportTask::arrive() { return arriveDesitination; }
