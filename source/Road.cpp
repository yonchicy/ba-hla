#include "Road.h"
#include "TransportTask.h"
#include "sql/SQLConnect.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <list>
#include <queue>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

// nodes table struct:
// desc nodes;
//+--------------+------------+------+-----+---------+-------+
//| Field        | Type       | Null | Key | Default | Extra |
//+--------------+------------+------+-----+---------+-------+
//| id           | int        | NO   | PRI | NULL    |       |
//| longitude    | double     | YES  |     | NULL    |       |
//| latitude     | double     | YES  |     | NULL    |       |
//| intersection | tinyint(1) | YES  |     | NULL    |       |
//+--------------+------------+------+-----+---------+-------+
// desc edges;
//+----------+--------+------+-----+---------+-------+
//| Field    | Type   | Null | Key | Default | Extra |
//+----------+--------+------+-----+---------+-------+
//| begin    | int    | NO   | PRI | NULL    |       |
//| end      | int    | NO   | PRI | NULL    |       |
//| distance | double | YES  |     | NULL    |       |
//+----------+--------+------+-----+---------+-------+

Node RoadInfo::getNode(int idx){return nodes.find(idx)->second;}
void RoadInfo::init() { loadBasicData(); }
void RoadInfo::loadBasicData() {
  SQLConnector con;
  for (MYSQL_ROW row = con.beginRow("select * from nodes;"); !con.endRow();
       row = con.nextRow()) {

    int id;
    double lat, lng;
    bool isIntersection;
    id = atoi(con.beginField(row));
    lng = std::stod(con.nextField(row));
    lat = std::stod(con.nextField(row));
    isIntersection = std::stoi(con.nextField(row));
    nodes.emplace(std::make_pair(id, Node{id, lat, lng, isIntersection}));
  }
  edges.resize(nodes.size() * 2+2);
  for (MYSQL_ROW row = con.beginRow("select * from edges;"); !con.endRow();
       row = con.nextRow()) {
    int begin, end;
    double dis;
    begin = atoi(con.beginField(row));
    end = atoi(con.nextField(row));
    dis = std::stod(con.nextField(row));
    edges[begin].emplace_back(Edge{begin, end, dis});
    edges[end].emplace_back(Edge{end, begin, dis});
  }

  // std::cout << nodes.size() << " " << edges.size() << std::endl;
  // for (auto &node : nodes) {
  //   std::cout << node.id << " " << node.lat << " " << node.lng << std::endl;
  // }
  // for (auto &e : edges) {
  //   std::cout << e.begin << " " << e.end << " " << e.distance << std::endl;
  // }
}

std::list<Position>
AStartAlgorithm::reconstructRoad(std::unordered_map<int, int> &history_path,
                                 int begin, int end) {
  std::list<Position> q;
 while(true) {
    auto node = roads.nodes.find(end)->second;
    q.emplace_front(Position(node.lng, node.lat));
    if(end == begin){
      break;
    }
    end = history_path.find(end)->second;
  }
  
  return std::move(q);
}
std::list<Position> AStartAlgorithm::pathFind(Node start, Node destination) {
  std::unordered_map<int, int> history_path;
  std::priority_queue<AStartAlgorithmNodeHelper> open_list;
  std::queue<AStartAlgorithmNodeHelper> close_list;
  std::vector<double> gDistance;
  std::vector<double> fDistance;
  std::set<int> vis;
  gDistance.resize(roads.nodes.size()+2,999999999);
  gDistance[start.id] = 0;
  fDistance.resize(roads.nodes.size()+2,999999999);
  fDistance[start.id] = gDistance[start.id] +
                        hDistance(Position(start.lng, start.lat),
                                  Position(destination.lng, destination.lat));
  open_list.push(AStartAlgorithmNodeHelper{start.id, fDistance[start.id]});
  vis.insert(start.id);

  while (open_list.size()) {
    auto tmp = open_list.top();
    open_list.pop();
    auto tmp_node = roads.nodes.find(tmp.id)->second;
    if (tmp.id == destination.id) {

      return reconstructRoad(history_path, start.id, destination.id);
    }
    for (auto &e : roads.edges[tmp.id]) {
      auto neighbor_node = roads.nodes.find(e.end)->second;
      double g = gDistance[tmp.id] + e.distance;

      if (g < gDistance[e.end]) {
        history_path[e.end] = tmp.id;
        gDistance[e.end] = g;
        fDistance[e.end] =
            g + hDistance(Position(tmp_node.lng, tmp_node.lat),
                          Position(neighbor_node.lng, neighbor_node.lat));
        if (vis.find(e.end) == vis.end()) {
          vis.insert(e.end);
          open_list.push(AStartAlgorithmNodeHelper{e.end, fDistance[e.end]});
        }
      }
    }
  }
  return std::list<Position>();
}

double AStartAlgorithm ::hDistance(Position x, Position y) {
  // return Position::getDistance(x, y);
  return std::sqrt((x.lat - y.lat) * (x.lat - y.lat) +
                   (x.lng - y.lng) * (x.lng - y.lng));
}
