#include "TransportTask.h"
#include "TransportTaskFedAmb.h"
#include "TransportTaskFederate.h"

#include <cmath>
#include <condition_variable>
#include <iostream>

double Position::getDistance(Position x, Position y) {
  return std::sqrt((x.lat - y.lat) * (x.lat - y.lat) +
                   (x.lng - y.lng) * (x.lng - y.lng));
}

void dispatchTransportTask(TransportTask task) {
  while (!task.arrive()) {
    task.findPath();

    task.iterate();
  }
}

void TransportTask::iterate() {
  while (true) {
    std::unique_lock<std::mutex> lock(this->federate->mtx);
    // FIXME : 这里应当是 local_time + step <  federateTime + lookAhead ?
    this->federate->cv.wait(lock, [&] {
      return this->local_time < this->federate->fedamb->federateTime;
    });
    goNextPosition();

    this->local_time += this->federate->timeStep;

    lock.unlock();
    this->federate->cv.notify_all();
  }
}

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
  std::cout << "local_time is " << local_time << std::endl;
}
bool TransportTask::arrive() { return arriveDesitination; }
