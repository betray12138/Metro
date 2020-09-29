#ifndef TOOL_H
#define TOOL_H

#endif // TOOL_H

#include <QString>
#include <set>
#include <vector>
#include <QColor>
#include <string.h>
#include <QPainter>
#include <QPen>
#include <QPoint>
#include <QRect>
#include "mainwindow.h"
#include <queue>
#include <algorithm>
#include <stack>

#define Right_ 1
#define Wrong -1
#define Init_Station_Color QColor(0,0,0)         //定义每一个站点的初始颜色
#define Station_radius 10                         //定义站点半径
#define Switch_radius 6                          //若站点存在中转的情况，则在圆中在套一个小圆
#define Station_width 3                          //定义站点小圈的画笔宽度
#define Connect_width 6                          //定义连线的宽度
#define Scroll_x 200                             //设定滚动窗口的左上角坐标位置
#define Scroll_y 100
#define Scroll_width 1200                        //设定滚动窗口单页面的长宽
#define Scroll_height 600
#define Standard_longitude 121                   //设定相对位置的经纬度
#define Standard_lantitude 31.45
#define NORMAL 1                                 //定义绘制线段和点的状态
#define GRAY 0
#define ERROR 1e-4                               //定义同一地铁站的经纬度误差
#define Coefficient 10000                        //确定比例系数
#define text_width 200                           //确定悬浮显示的文件框大小
#define text_height 130

//为dij算法做准备
struct info {
    int vertex;
    double weight;
};


struct heap {
    int index;
    double distance;
    bool operator <(const heap& B) const{
        return B.distance < distance;
    }
};




//定义地铁站点类
class Station{
    int id;                     //定义站点序号——使用序号建立邻接表
    double longitude;           //定义经纬度
    double lantitude;
    QString name;               //定义站点名字
    std::set<int> belonging;    //定义归属的线路
    bool is_query;              //表明是否处于查询状态

public:
    Station();
    Station(const int id_,const double long_,const double lant_,const QString name_);
    int Find_id_longitude_lantitude(const double longi,const double lanti);      //使用经纬度寻找地铁站点的序号
    int Find_id_name(const QString name_);                                       //使用站点名字寻找地铁线路的序号
    int show_id();                                                               //输出当前点的id
    void show_longitude_lantitude(double &longi,double &lanti);                  //输出经纬度信息
    void show_name(QString &nam_);                                               //输出地铁站点的名字
    void show_belonging(std::set<int> &belong_);                                 //输出所属站点信息
    void value_id(const int id_);                                                //为该站点的id赋值
    void value_longitude_lantitude(const int longi,const int lanti);             //为该站点的经纬度赋值
    void value_name(const QString nam_);                                         //为该站点的名字赋值
    void add_belonging(const int id);                                            //为该站点增添对应的归属线路
    void Paint_Station(QPainter &paint,const int condition,const double factor=1);//完成站点的绘制
    int Check_Line(const int Line);                                              //查看该站点是否属于所给线路
    QString generate_show_info();                                                //将显示的站点信息输出为一个QString类型
    bool Check_query();                                                          //返回当前是否查询的状态
    void value_query(const bool query);                                          //为查询状态赋值
};


//定义连接边的类
class Edge{
    QColor color;
    std::set<int>belonging;     //定义该边所属的线路
    double distance;            //连接该边的直线距离
    bool is_query;              //定义是否处于查询状态

public:
    void value_color(const QColor color_);                               //为边的颜色进行赋值
    void value_distance(const double dist_);                             //为边的距离进行赋值
    void add_belonging(const int id);                                    //为该边追加所属线路 便于呈现
    void get_Color(QColor &color_);                                      //获取该边的颜色
    void get_distance(double &dist);                                     //获取该边的直线距离
    int check_Line_id(const int id);                                     //检查该边是否属于id所对应地铁线路
    void Paint_Connect(QPainter &paint,const double long1,const double lant1,const double long2,const double lant2,const int condition,const double factor=1); //完成连接线的绘制
    int get_belonging_size();                                            //返回所属线路的个数
    bool Check_query();                                                  //返回当前是否查询的状态
    void value_query(const bool query);                                  //为查询状态赋值
};


//定义地铁线路类(每条地铁的颜色由QColor数组提供）
class Metro_Line{
    QString name;
    std::set<int> cover;        //定义该条地铁线路所包含的站点信息

public:
    void value_name(const QString name_);         //为该地铁线路命名
    void add_cover(const int id);                 //为该地铁线路添加站点序号
    void get_cover(std::set<int> &cover_);        //输出该地铁线路含有的全部节点序号
    int get_cover_size();                         //输出该条线路此刻对应站点个数
    void get_name(QString &nam_);                 //获得该条地铁线路的名字
};


//定义整个线路图类
class Graph{
    Station distribute[500];               //定义整个地铁网络的站点分布信息(由于要索引，因此使用顺序表)
    Metro_Line Subway[21];                 //定义地铁线路的集合
    Edge edge[500][500];
    std::vector<info> save[1000];          //用于dij算法
    int pre[500];                          //用于dij算法中寻路/最短换乘算法中寻路
    double rad(double d);                  //将角度转换为弧度
    double compute_distance(const int id1,const int id2);                        //利用经纬度计算两点间距离
    void Fill_edge_distance(const int id1,const int id2);                        //利用经纬度填充edge的distance
    void Add_edge_assist(const int Line,const int id1,const int id2,QPainter &paint,const double factor=1); //完成两种线路添加方式中的相同功能


public:
    Graph();
    int Find_id_longitude_lantitude(const double longi,const double lanti);      //使用经纬度寻找地铁站点的序号
    int Find_id_name(const QString name_);                                       //使用站点名字寻找地铁线路的序号
    int Add_edge_name(const int Line,const QString name1,const QString name2,QPainter &paint,const double factor=1); //线路添加方式一，直接输入站点名字
    int Add_edge_longi_lanti(const int Line,const QString name1,const double long1,const double lant1,const QString name2,const double long2,const double lant2,QPainter &paint,const double factor=1); //线路添加方式二，输入经纬度
    int dijkstra(const int src,const int n,const int dst);                       //使用dij寻找最短路径
    int dij_preprocess(const int src,const int dst);                             //完成dij的前置处理以及调用dij函数
    void dij_postprocess(QString &str,const int id1,const int id2);              //完成dij的后置处理以及生成显示换乘的字符串
    friend class MainWindow;
};

void num_to_str(const int num,char *buffer,int &count);                      //将数据转换为字符串
