import path1
import path2
import json
import pymysql

ids = []
paths = []
points = []   # [[path1, path2, [x, y]], ...]

def load_data():
  # 横向道路
  idx = 0
  for _, _, path in path1.paths_1:
    idx += 1
    ids.append(idx)
    paths.append(path)
  # 纵向道路
  for _, _, path in path2.paths_2:
    idx += 1
    ids.append(idx)
    paths.append(path)

# 计算两条线段的交点
# line1, line2: [[x1, y1], [x2, y2]]
# idx1, idx2: 两条线段所在的道路编号
def intersection(idx1, line_a, idx2, line_b):
  a_x1 = line_a[0][0]
  a_y1 = line_a[0][1]
  a_x2 = line_a[1][0]
  a_y2 = line_a[1][1]

  b_x1 = line_b[0][0]
  b_y1 = line_b[0][1]
  b_x2 = line_b[1][0]
  b_y2 = line_b[1][1]

  # 先排除一定不相交的情况
  if (max(a_x1, a_x2) < min(b_x1, b_x2)) or (min(a_x1, a_x2) > max(b_x1, b_x2)) or \
      (max(a_y1, a_y2) < min(b_y1, b_y2)) or (min(a_y1, a_y2) > max(b_y1, b_y2)):
    return
  else:
    # 计算两个线段的斜率
    k1 = (a_y1 - a_y2) / (a_x1 - a_x2) if a_x1 != a_x2 else float('inf')
    k2 = (b_y1 - b_y2) / (b_x1 - b_x2) if b_x1 != b_x2 else float('inf')

    # 如果两个线段平行,则不相交
    if k1 == k2:
      return

    # 计算两个线段的截距
    b1 = a_y1 - k1 * a_x1
    b2 = b_y1 - k2 * b_x1

    # 计算交点坐标
    x = (b2 - b1) / (k1 - k2)
    y = k1 * x + b1

    if (x >= min(a_x1, a_x2) and x <= max(a_x1, a_x2)) and \
        (y >= min(a_y1, a_y2) and y <= max(a_y1, a_y2)) and \
        (x >= min(b_x1, b_x2) and x <= max(b_x1, b_x2)) and \
        (y >= min(b_y1, b_y2) and y <= max(b_y1, b_y2)):
      points.append([ids[idx1], ids[idx2], [round(x, 6), round(y, 6)]])
    return

def cacl():  
  for i1 in range(len(paths)):
    for j1 in range(len(paths[i1]) - 1):
      line1 = [paths[i1][j1], paths[i1][j1 + 1]]
      # 每一条路都要和它之后的所有道路进行匹配
      for i2 in range(i1 + 1, len(paths)):
        for j2 in range(len(paths[i2]) - 1):
          line2 = [paths[i2][j2], paths[i2][j2 + 1]]
          intersection(i1, line1, i2, line2)

def save_into_db():
  with open('config.json', 'r') as file: config = json.load(file)
  db = pymysql.connect(host = config['host'],
                      user=config['user'],
                      password = config['password'],
                      database='hla')
  cursor = db.cursor()

  create_table_stmt = "create table if not exists path_cross(id int not null auto_increment primary key, path1 int, path2 int, longitude double, latitude double)"
  cursor.execute(create_table_stmt)

  for point in points:
    # print(f"insert into path_cross(path1, path2, longitude, latitude) values({point[0]}, {point[1]}, {point[2][0]}, {point[2][1]})")
    cursor.execute(f"insert into path_cross(path1, path2, longitude, latitude) values({point[0]}, {point[1]}, {point[2][0]}, {point[2][1]})")

  db.commit()
  db.close()
  return

def main():
  load_data()
  cacl()
  save_into_db()

if __name__ == "__main__":
  main()
