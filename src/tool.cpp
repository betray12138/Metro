#include "tool.h"
#include <QDebug>

static QColor Line_colour[20]={QColor{153,0,255},QColor{255,0,0},QColor{255,153,0},
                   QColor{255,204,255},QColor{204,0,204},QColor{255,255,204},
                   QColor{153,255,102},QColor{0,153,0},QColor{0,204,153},
                   QColor{204,204,0},QColor{0,255,204},QColor{255,255,0},
                   QColor{255,102,153},QColor{153,51,102},QColor{128,128,0},
                   QColor{102,102,153},QColor{204,0,0},QColor{204,236,255},
                   QColor{51,51,204},QColor{255,124,128}
                       };
static int Station_Count=1;      //全局变量，对地铁站点进行赋值


//Station 类函数实现
Station::Station()
{
    id=0;
    longitude=0;
    lantitude=0;
    name="";
    belonging.clear();
    is_query=false;
}

Station::Station(const int id_,const double long_,const double lant_,const QString name_)
{//带参构造函数
    id=id_;
    longitude=long_;
    lantitude=lant_;
    name=name_;
}

int Station::Find_id_longitude_lantitude(const double longi,const double lanti)      //使用经纬度寻找地铁站点的序号
{
    if(fabs(longi-longitude)<ERROR && fabs(lanti-lantitude)<ERROR)
        return id;
    return Wrong;
}

int Station::Find_id_name(const QString name_)                                       //使用站点名字寻找地铁线路的序号
{
    if(name_==name)
        return id;
    else return Wrong;
}

int Station::show_id()                                                               //输出当前点的id
{
    return id;
}

void Station::show_longitude_lantitude(double &longi,double &lanti)                  //输出经纬度信息
{
    longi=longitude;
    lanti=lantitude;
    return;
}

void Station::show_name(QString &nam_)                                               //输出地铁站点的名字
{
    nam_=name;
}

void Station::show_belonging(std::set<int> &belong_)                                 //输出所属站点信息
{
    belong_.clear();
    for(auto it=belonging.begin();it!=belonging.end();it++)
        belong_.insert(*it);
    return;
}

void Station::value_id(const int id_)                                                //为该站点的id赋值
{
    id=id_;
}

void Station::value_longitude_lantitude(const int longi,const int lanti)             //为该站点的经纬度赋值
{
    longitude=longi;
    lantitude=lanti;
}

void Station::value_name(const QString nam_)                                         //为该站点的名字赋值
{
    name=nam_;
}

void Station::add_belonging(const int id)                                            //为该站点增添对应的归属线路
{
    if(belonging.count(id)==0)
        belonging.insert(id);
    return;
}

void Station::Paint_Station(QPainter &paint,const int condition,const double factor) //完成站点的绘制
{
    QPen pen;
    pen.setWidth(Station_width*factor);
    if(condition==NORMAL)
        pen.setColor(Init_Station_Color);
    else
        pen.setColor(Qt::gray);
    paint.setPen(pen);
    paint.drawEllipse(QPoint((longitude-Standard_longitude)*Coefficient*factor,(Standard_lantitude-lantitude)*Coefficient*factor),Station_radius*factor,Station_radius*factor);
    if(belonging.size()>1){
        paint.drawEllipse(QPoint((longitude-Standard_longitude)*Coefficient*factor,(Standard_lantitude-lantitude)*Coefficient*factor),Switch_radius*factor,Switch_radius*factor);
    }
    paint.setFont(QFont("aerial",15*factor,300*factor));
    paint.drawText(QRect(((longitude-Standard_longitude)*Coefficient-100-Station_radius)*factor,(Standard_lantitude-lantitude)*Coefficient*factor,100*factor,50*factor),name,QTextOption(Qt::AlignRight));
}

int Station::Check_Line(const int Line)                                              //查看该站点是否属于所给线路
{
    if(belonging.count(Line)==1)
        return Right_;
    return Wrong;
}

QString Station::generate_show_info()                                                //将显示的站点信息输出为一个QString类型
{
    QString str("");
    str+=name;
    str+=QString("\n经度:");
    char buffer[20];
    gcvt(longitude,8,buffer);
    str+=QString(buffer);
    str+=QString("\n纬度:");
    memset(buffer,0,sizeof(buffer));
    gcvt(lantitude,7,buffer);
    str+=QString(buffer);
    str+=QString("\n经过:");
    for(auto it=belonging.begin();it!=belonging.end();it++){
        memset(buffer,0,sizeof(buffer));
        _itoa(*it,buffer,10);
        str+=QString(buffer);
        str+=QString("号线 ");
    }
    return str;
}

bool Station::Check_query()                                                          //返回当前是否查询的状态
{
    return is_query;
}

void Station::value_query(const bool query)                                          //为查询状态赋值
{
    is_query=query;
}



//Edge 类函数实现
void Edge::value_color(const QColor color_)                               //为边的颜色进行赋值
{
    color=color_;
}

void Edge::value_distance(const double dist_)                             //为边的距离进行赋值
{
    distance=dist_;
}

void Edge::add_belonging(const int id)                                    //为该边追加所属线路 便于呈现
{
    if(belonging.count(id)==0)
        belonging.insert(id);
}

void Edge::get_Color(QColor &color_)                                      //获取该边的颜色
{
    color_=color;
}

void Edge::get_distance(double &dist)                                     //获取该边的直线距离
{
    dist=distance;
}

int Edge::check_Line_id(const int id)                                     //检查该边是否属于id所对应地铁线路
{
    if(belonging.count(id)==1)
        return Right_;
    else
        return Wrong;
}

void Edge::Paint_Connect(QPainter &paint,const double long1,const double lant1,const double long2,const double lant2,const int condition,const double factor)//完成连接线的绘制
{
    QPen pen;
    pen.setWidth(Connect_width*factor);
    if(condition==NORMAL)
        pen.setColor(color);
    else
        pen.setColor(Qt::gray);
    paint.setPen(pen);
    paint.drawLine((long1-Standard_longitude)*Coefficient*factor,(Standard_lantitude-lant1)*Coefficient*factor,(long2-Standard_longitude)*Coefficient*factor,(Standard_lantitude-lant2)*Coefficient*factor);
}

int Edge::get_belonging_size()                                            //返回所属线路的个数
{
    return belonging.size();
}

bool Edge::Check_query()                                                  //返回当前是否查询的状态
{
    return is_query;
}

void Edge::value_query(const bool query)                                  //为查询状态赋值
{
    is_query=query;
}









//Metro_Line 类函数实现
void Metro_Line::value_name(const QString name_)         //为该地铁线路命名
{
    name=name_;
}

void Metro_Line::add_cover(const int id)                 //为该地铁线路添加站点序号
{
    if(cover.count(id)==0)
        cover.insert(id);
}

void Metro_Line::get_cover(std::set<int> &cover_)        //输出该地铁线路含有的全部节点序号
{
    cover_.clear();
    for(auto it=cover.begin();it!=cover.end();it++)
        cover_.insert(*it);
}

int Metro_Line::get_cover_size()                         //输出该条线路此刻对应站点个数
{
    return cover.size();
}

void Metro_Line::get_name(QString &nam_)                 //获得该条地铁线路的名字
{
    nam_=name;
}





//Graph 类函数实现
Graph::Graph()
{
}

double Graph::rad(double d)                   //将角度转换为弧度
{
    return d*3.1415926/180;
}

double Graph::compute_distance(const int id1,const int id2)                        //利用经纬度计算两点间距离
{//该函数利用经纬度计算距离
    double long1,long2,lant1,lant2;
    distribute[id1].show_longitude_lantitude(long1,lant1);
    distribute[id2].show_longitude_lantitude(long2,lant2);
    double Lant1=rad(lant1);
    double Lant2=rad(lant2);
    double a=Lant1-Lant2;
    double b=rad(long1)-rad(long2);
    double s=2*asin(sqrt(pow(sin(a/2),2)+cos(Lant1)*cos(Lant2)*pow(sin(b/2),2)));
    s*=6378137.0;     //乘赤道半径
    s=round(s*10000)/10000;
    return s;
}

void Graph::Fill_edge_distance(const int id1,const int id2)                        //利用经纬度填充edge的distance
{
    double s=compute_distance(id1,id2);
    edge[id1][id2].value_distance(s);
}

void Graph::Add_edge_assist(const int Line,const int id1,const int id2,QPainter &paint,const double factor) //完成两种线路添加方式中的相同功能
{
    /*参数设置：
     * Line：地铁站的序号
     * id1：起始的站点号码
     * id2：到达的站点号码
     * paint:绘制的画板
     * Func：完成两种线路添加方式的相同功能，节省空间
     */
    //为新边添加边权，颜色以及线路更新(无向图)
    Fill_edge_distance(id1,id2);
    edge[id1][id2].value_color(Line_colour[Line-1]);
    edge[id1][id2].add_belonging(Line);
    Fill_edge_distance(id2,id1);
    edge[id2][id1].value_color(Line_colour[Line-1]);
    edge[id2][id1].add_belonging(Line);

    //地铁线路数据更新
    QString metro;
    char buffer[20];
    if(Subway[Line].get_cover_size()==0){
        //表明此刻该地铁线路为新建地铁线路--为地铁线路加上名字
        int count=0;
        memset(buffer,0,sizeof(buffer));
        num_to_str(Line,buffer,count);
        metro+=QString(buffer);
        metro+="号线";
        Subway[Line].value_name(metro);
    }
    Subway[Line].add_cover(id1);
    Subway[Line].add_cover(id2);

    double long1,long2,lant1,lant2;
    //地铁站点数据更新
    distribute[id1].add_belonging(Line);
    distribute[id2].add_belonging(Line);
    distribute[id1].show_longitude_lantitude(long1,lant1);
    distribute[id2].show_longitude_lantitude(long2,lant2);

    //绘制连接线
    edge[id1][id2].Paint_Connect(paint,long1,lant1,long2,lant2,NORMAL,factor);

    //绘制站点
    distribute[id1].Paint_Station(paint,NORMAL,factor);
    distribute[id2].Paint_Station(paint,NORMAL,factor);
}

int Graph::Find_id_longitude_lantitude(const double longi,const double lanti)      //使用经纬度寻找地铁站点的序号
{
    for(int i=1;i<Station_Count;i++){
        int id;
        if((id=distribute[i].Find_id_longitude_lantitude(longi,lanti))!=-1)
            return id;
    }
    return Wrong;
}

int Graph::Find_id_name(const QString name_)                                       //使用站点名字寻找地铁线路的序号
{
    for(int i=1;i<Station_Count;i++){
        int id;
        if((id=distribute[i].Find_id_name(name_))!=-1)
            return id;
    }
    return Wrong;
}

int Graph::Add_edge_name(const int Line,const QString name1,const QString name2,QPainter &paint,const double factor) //线路添加方式一，直接输入站点名字
{
    /*参数设置：
     * Line：地铁线路的序号
     * name1：起始位置的站点名字
     * name2：末位置的站点名字
     * paint:绘制的画板
     * Func：通过输入以上三个参数，达成新建连接的过程
     */
    int id1=Find_id_name(name1),id2=Find_id_name(name2);
    //错误原因：该两点为同一个位置
    if(id1==id2)
        return Wrong;
    //错误原因：存在某个站点的名字未知
    if(id1==Wrong || id2==Wrong)
        return Wrong;                             //使用消息框进行报错提示
    //错误原因：该两点之间已经存在相同站点的边权连接
    if(edge[id1][id2].check_Line_id(Line)==Right_)
        return Wrong;

    Add_edge_assist(Line,id1,id2,paint,factor);

    return Right_;
}

int Graph::Add_edge_longi_lanti(const int Line,const QString name1,const double long1,const double lant1,const QString name2,const double long2,const double lant2,QPainter &paint,const double factor)
{
    /*参数设置：
     * Line：地铁线路的序号
     * name1：新增经纬度的起始站点名字
     * long1：起始站点的经度
     * lant1：起始站点的纬度
     * name2：新增经纬度的到达站点名字
     * long2：到达站点的经度
     * lant2：到达站点的纬度
     * paint:绘制的画板
     */
    int id1=Find_id_longitude_lantitude(long1,lant1),id2=Find_id_longitude_lantitude(long2,lant2);
    //错误原因：前后经纬度相同
    if(fabs(long1-long2)<ERROR && fabs(lant1-lant2)<ERROR)
        return Wrong;
    //错误原因：站点名字无法与原位置匹配
    if(id2!=Wrong){
        QString name2_;
        distribute[id2].show_name(name2_);
        if(name2_!=name2)
            return Wrong;
    }
    if(id1!=Wrong)
    {
        QString name1_;
        distribute[id1].show_name(name1_);
        if(name1_!=name1)
            return Wrong;
    }
    //错误原因：两点之间已存在相同线路的边权连接
    if(id1!=Wrong && id2!=Wrong && edge[id1][id2].check_Line_id(Line)==Right_)
        return Wrong;
    //对新站点进行更新赋值
    if(id1==Wrong){
        id1=Station_Count++;
        distribute[id1]=Station(id1,long1,lant1,name1);
    }
    if(id2==Wrong){
        id2=Station_Count++;
        distribute[id2]=Station(id2,long2,lant2,name2);
    }
    Add_edge_assist(Line,id1,id2,paint,factor);
    return Right_;
}

int Graph::dijkstra(const int src,const int n,const int dst)                       //使用dij寻找最短路径
{
    double dis[100010];       //记录距离的数组
    bool visit[100010];
    std::priority_queue<heap> Queue;    //使用堆优化，能达到nlogn量级
    memset(visit,0,sizeof(visit));
    memset(pre,0,sizeof(pre));
    for(int i=1;i<=n;i++)
        dis[i]=111111111;

    dis[src] = 0;
    Queue.push({ src,0 });
    while (!Queue.empty())
    {
        heap tmp = Queue.top();
        Queue.pop();
        if (visit[tmp.index] == true)
            continue;
        visit[tmp.index] = true;
        for (auto it = save[tmp.index].begin(); it != save[tmp.index].end(); it++) {
            if (dis[it->vertex] > dis[tmp.index] + it->weight){     //出现更小的情况  应该更新dis数组
                dis[it->vertex] = dis[tmp.index] + it->weight;
                pre[it->vertex]=tmp.index;                          //记录前驱值
            }
            Queue.push({ it->vertex,dis[it->vertex] });
        }
    }
    if(fabs(dis[dst]-111111111)<ERROR)
        return Wrong;
    else
        return Right_;
}

int Graph::dij_preprocess(const int src,const int dst)                             //完成dij的前置处理以及调用dij函数
{
    //每次查询前重置信息
    for(int i=1;distribute[i].show_id()!=0;i++)   //重置站点
        distribute[i].value_query(false);
    for(int i=1;distribute[i].show_id()!=0;i++)    //重置边
        for(int j=1;distribute[j].show_id()!=0;j++)
            edge[i][j].value_query(false);

    for(int i=1;distribute[i].show_id()!=0;i++){
        for(int j=1;distribute[j].show_id()!=0;j++){
            if(edge[i][j].get_belonging_size()>0){
                info tmp;
                tmp.vertex=j;
                edge[i][j].get_distance(tmp.weight);
                save[i].push_back(tmp);
            }
        }
    }
    if(dijkstra(src, Station_Count-1,dst)==Right_)
        return Right_;
    else
        return Wrong;

}

void Graph::dij_postprocess(QString &str,const int id1,const int id2)              //完成dij的后置处理以及生成显示换乘的字符串
{
    //执行到该函数时，所有的路线结果保存在pre数组中
    std::stack<Station> switch_;       //用于换乘地铁站点的保存
    std::stack<int> metro_line;
    int similar_Line[50];              //用于确定两个站点的相同线路
    std::set<int>belong;
    int ptr=id2;
    //任意相邻的两个地铁站点有且仅有一个共同的地铁线路（排除掉两条地铁线经过相邻相同站的情况）——始终取最前面的一个共同线路作为相同地铁线路
    int same_line=0;                   //用于记录相同地铁线路
    for(;ptr!=id1;ptr=pre[ptr]){       //结束条件一定会是pre[ptr]=id1执行后即ptr=id1
        distribute[ptr].value_query(true);
        edge[ptr][pre[ptr]].value_query(true);
        edge[pre[ptr]][ptr].value_query(true);
        memset(similar_Line,0,sizeof(similar_Line));
        belong.clear();
        distribute[ptr].show_belonging(belong);
        for(auto it=belong.begin();it!=belong.end();it++)
            similar_Line[*it]++;
        belong.clear();
        distribute[pre[ptr]].show_belonging(belong);
        for(auto it=belong.begin();it!=belong.end();it++)
            similar_Line[*it]++;
        if(similar_Line[same_line]==2 && (edge[ptr][pre[ptr]].check_Line_id(same_line)==Right_ || edge[pre[ptr]][ptr].check_Line_id(same_line)==Right_))
            continue;          //如果相邻两站点在same_line上都有地铁线，则可避免多次换乘
        for(int i=1;i<=20;i++){
            if(similar_Line[i]==2 && (edge[ptr][pre[ptr]].check_Line_id(i)==Right_ || edge[pre[ptr]][ptr].check_Line_id(i)==Right_)){
                if(i!=same_line){
                    switch_.push(distribute[ptr]);     //该站即为换乘站点
                    same_line=i;
                    metro_line.push(i);
                }
            break;                             //防止两条地铁均经过同一站导致出错
            }
        }
    }
    distribute[id1].value_query(true);
    switch_.push(distribute[id1]);

    //执行完大循环以后 经过的站点和连接边已经标记完毕，并且换乘站和换乘线路存在于相应的stack中

    //接下来形成提示信息QString
    str.clear();
    QString station_name,line_name;
    Station tmp_station;
    while(!metro_line.empty()){
        int tmp_line=metro_line.top();
        metro_line.pop();
        tmp_station=switch_.top();
        switch_.pop();
        tmp_station.show_name(station_name);
        str+=station_name;
        str+="--";
        Subway[tmp_line].get_name(line_name);
        str+=line_name;
        str+="-->";
    }
    tmp_station=switch_.top();
    switch_.pop();
    tmp_station.show_name(station_name);
    str+=station_name;
}






void num_to_str(const int num,char *buffer,int &count)       //将数据转换为字符串
{
    if(num/10)
        num_to_str(num/10,buffer,count);
    buffer[count++]=num%10+'0';
}
