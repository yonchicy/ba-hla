#include "TransportTask.h"
#include "TransportTaskFedAmb.h"
#include "TransportTaskFederate.h"

#include "Road.h"

#include <cmath>

#include <condition_variable>
#include <iostream>

Position::Position(double lng_, double lat_) : lng(lng_), lat(lat_) {}

double Position::rad(double d) { return d * PI / 180.0; }

double Position::getRealDistance(Position x, Position y) {

  double a, b;
  double radLat1 = rad(x.lat);
  double radLat2 = rad(y.lat);
  a = std::abs(radLat1 - radLat2);
  b = std::abs(rad(x.lng) - rad(y.lng));
  double s = 2 * asin(sqrt(pow(sin(a / 2), 2) +
                           cos(radLat1) * cos(radLat2) * pow(sin(b / 2), 2)));
  s = s * EARTH_RADIUS;
  s = s * 1000;
  return s;
}

double Position::getEulerDistance(Position x, Position y) {

  return std::sqrt((x.lat - y.lat) * (x.lat - y.lat) +
                   (x.lng - y.lng) * (x.lng - y.lng));
}

TransportTask::TransportTask(unsigned long long idx_, Node from_, Node to_,
                             double speed_, double localTime_, double timeStep_,
                             TransportTaskFederate *fed)
    : idx(idx_), pos(from_), destination(to_), speed(speed_),
      localTime(localTime_), timeStep(timeStep_), federate(fed) {}
void dispatchTransportTask(TransportTask task) {
  task.findPath();
  while (!task.arrive()) {

    task.iterate();
  }
  std::cout << "task " << task.getIdx() << " finish" << std::endl;
}
void TransportTask::findPath() {
  path = AStartAlgorithm::pathFind(pos, destination);
  std::cout << "path is :";
  for (auto node : path) {
    std::cout << "(" << node.lng << "," << node.lat << ")";
  }
  std::cout << std::endl;
}

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
  double dis_it = speed * timeStep;
  while (path.size() >= 2) {
    auto node1 = path.front();
    path.pop_front();
    auto node2 = path.front();
    auto d = Position::getEulerDistance(Position(node1.lng, node1.lat),
                                        Position(node2.lng, node2.lat));
    if (d > dis_it) {
      double dlat = node2.lat - node1.lat;
      double dlng = node2.lng - node1.lng;
      double delta = dis_it / d;
      Position nextPos(node1.lng, node1.lat);
      nextPos.lat += dlat * delta;
      nextPos.lng += dlng * delta;
      path.emplace_front(nextPos);
      break;
    } else {
      dis_it -= d;
    }
  }
  std::cout<< "-------"<<std::endl;
  std::cout << "transport task " << idx << " running to next position ";
  std::cout << "local_time is " << localTime << std::endl;
  std::cout << "now path is :";
  for (auto node : path) {
    std::cout << "(" << node.lng << "," << node.lat << ")";
  }
  std::cout<< std::endl<<"-------"<<std::endl;
  if(path.size()<2){
    arriveDesitination=true;
  }
}
bool TransportTask::arrive() { return arriveDesitination; }
