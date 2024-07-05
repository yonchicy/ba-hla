#ifndef __ROAD_H__
#define __ROAD_H__

#include <vector>
class Edge {
public:
  int begin, end;
  double distance;
};
class Node {
public:
  int id;
  double lat, lng;
  bool isIntersection;
};

class RoadInfo {
private:
  std::vector<Node> nodes;
  std::vector<Edge> edges;

public:
  void init();

public:
  void load_basic_data();


};

#endif // !__ROAD_H__
