#include "Road.h"
#include "sql/SQLConnect.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
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

void RoadInfo::init() {
  load_basic_data();
}
void RoadInfo::load_basic_data() {
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
    nodes.emplace_back(Node{id, lat, lng, isIntersection});
  }
  for (MYSQL_ROW row = con.beginRow("select * from edges;"); !con.endRow();
       row = con.nextRow()) {
    int begin, end;
    double dis;
    begin = atoi(con.beginField(row));
    end = atoi(con.nextField(row));
    dis = std::stod(con.nextField(row));
    edges.emplace_back(Edge{begin, end, dis});
  }

   //std::cout << nodes.size() << " " << edges.size() << std::endl;
   //for (auto &node : nodes) {
   //  std::cout << node.id << " " << node.lat << " " << node.lng << std::endl;
   //}
   //for (auto &e : edges) {
   //  std::cout << e.begin << " " << e.end << " " << e.distance << std::endl;
   //}
}
