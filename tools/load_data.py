from typing import final
import pymysql
import json
import path1
import path2





def connect_db():
    with open('config.json','r')  as file:
        config = json.load(file)


    db = pymysql.connect(host = config['host'],
                         user=config['user'],
                         password = config['password'],
                         )
    cursor = db.cursor()
    try : 
        cursor.execute("create database if not exists hla")
    except pymysql.MySQLError as e :
        print(f"Error：{e}")
        db.rollback()
    finally:
        cursor.close()
        db.close()

    db = pymysql.connect(host = config['host'],
                         user=config['user'],
                         password = config['password'],
                         database='hla'
                         )



    return db 


# insert into path_info (name,`desc` ,path) values('test','test',ST_GeometryFromText('linestring(1 2,2 2 )',4326));
# 
def insert_path(db):

    def point_to_str(point):
        return ' '.join(map(str,point))
    def my_map(func,iterable):
        result = []
        for item in iterable:
            result.append(func(item))

        return result
    path_str = ""
    sql_tmp = "insert into path_info (path_name,path_desc ,path) values(%s,%s,ST_GeometryFromText(%s));"
    values = []
    for name,desc,path in path1.paths_1:
        path_str = ','.join(my_map(point_to_str,path))
        path_str = 'linestring({})'.format(path_str)
        values.append((name,desc,path_str))
    for name,desc,path in path2.paths_2:
        path_str = ','.join(my_map(point_to_str,path))
        path_str = 'linestring({})'.format(path_str)
        values.append((name,desc,path_str))

    cursor  = db.cursor()



    try:
        cursor.executemany(sql_tmp,values)
        db.commit()
        cursor.execute("select count(*) from path_info");
        results = cursor.fetchall()

        print("insert {} roads suceeded!".format(str(results[0][0])))

    except pymysql.MySQLError as e :
        print(f"Error：{e}")
        db.rollback()
    finally:
        cursor.close()


def create_table_if_not_exist(db):
    cursor = db.cursor();

    try:
        cursor.execute("drop table if exists path_info")
        cursor.execute('create table if not exists path_info (id int not null auto_increment , path_name varchar(255),path_desc varchar(255),path linestring, primary key (id))')
        db.commit()
    except pymysql.MySQLError as e :
        print(f"Error：{e}")
        db.rollback()
    finally:
        cursor.close()

def main():
    db = connect_db()
    create_table_if_not_exist(db)
    insert_path(db)


    db.close()





if __name__ == "__main__":
    main()
