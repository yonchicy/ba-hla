#ifndef __ROAD_H__
#define __ROAD_H__

#include <list>
#include <map>
#include <queue>
#include <unordered_map>
#include <vector>

class Position;
class RoadInfo;

extern RoadInfo roads;
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

  friend class AStartAlgorithm;

private:
  std::map<int, Node> nodes;
  std::vector<std::vector<Edge>> edges;

public:
  void init();
  Node getNode(int idx);
  static void pathFind(Position start, Position destination);

private:
  void loadBasicData();
};

class AStartAlgorithmNodeHelper {
  friend class AStartAlgorithm;

public:
  int id;
  double distance;

  bool operator<(const AStartAlgorithmNodeHelper &o) const {
    return distance < o.distance;
  }
};

class AStartAlgorithm {
public:
  static std::list<Position> pathFind(Node start, Node destination);

private:
  static double hDistance(Position x, Position y);
  static std::list<Position>
  reconstructRoad(std::unordered_map<int, int> &history_path, int begin,
                  int end);
};

#endif // !__ROAD_H__
