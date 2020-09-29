#include "tool.h"
#include "ui_mainwindow.h"
#include <QDebug>

static int count=1;

void MainWindow::query_show_line()                           //显示查询的线路
{
    paint.drawPixmap(0,0,Metro_Width,Metro_Height,QPixmap(":/white_bg.jpg"));
    //遍历图中的每个点以及每一条边，如果点或边在所给线路上，则NORMAL画出，否则GRAY画出
    //首先遍历边
    for(int i=1;graph->distribute[i].show_id()!=0;i++){
        for(int j=1;graph->distribute[j].show_id()!=0;j++){
            if(graph->edge[i][j].get_belonging_size()>0){
                //需要保证该边被某条线路经过
                double long1,lant1,long2,lant2;
                graph->distribute[i].show_longitude_lantitude(long1,lant1);
                graph->distribute[j].show_longitude_lantitude(long2,lant2);
                if(graph->edge[i][j].Check_query()==true)
                    graph->edge[i][j].Paint_Connect(paint,long1,lant1,long2,lant2,NORMAL,get_factor());
                else
                    graph->edge[i][j].Paint_Connect(paint,long1,lant1,long2,lant2,GRAY,get_factor());
            }
        }
    }
    //其次遍历点
    for(int i=1;graph->distribute[i].show_id()!=0;i++){
        if(graph->distribute[i].Check_query()==true)
            graph->distribute[i].Paint_Station(paint,NORMAL,get_factor());
        else
            graph->distribute[i].Paint_Station(paint,GRAY,get_factor());
    }
    ui->scrollArea->setWidget(label);
    label->setPixmap(pixmap);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)          //捕捉展示窗口移动事件
{
    int mouse_x=event->x(),mouse_y=event->y();
    if(mouse_x>=Scroll_x && mouse_x<=Scroll_x+Scroll_width && mouse_y>=140 && mouse_y<=140+Scroll_height){
        //在矩形区域内才执行移动功能
        ui->scrollArea->verticalScrollBar()->setValue(ui->scrollArea->verticalScrollBar()->value()+click_y-mouse_y);
        ui->scrollArea->horizontalScrollBar()->setValue(ui->scrollArea->horizontalScrollBar()->value()-mouse_x+click_x);
        click_x=mouse_x;
        click_y=mouse_y;
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)         //捕捉地铁站点按下事件
{
    int mouse_x=event->x(),mouse_y=event->y();
    if(mouse_x<Scroll_x || mouse_x>Scroll_x+Scroll_width || mouse_y<140 || mouse_y>140+Scroll_height)
        return;             //在矩形区域外不执行位置捕捉功能
    click_x=mouse_x;
    click_y=mouse_y;
    int real_x,real_y;     //记录当前鼠标实际的位置点
    real_x=mouse_x+ui->scrollArea->horizontalScrollBar()->value();
    real_y=mouse_y+ui->scrollArea->verticalScrollBar()->value();
    for(int i=1;graph->distribute[i].show_id()!=0;i++){
        double long_,lant_;
        graph->distribute[i].show_longitude_lantitude(long_,lant_);
        if(fabs((long_-Standard_longitude)*Coefficient*factor-real_x+200)<Station_radius*factor &&
                fabs((Standard_lantitude-lant_)*Coefficient*factor-real_y+140)<Station_radius*factor){
            text->hide();       //必须先关闭前一个才能开启下一个
            QString str=graph->distribute[i].generate_show_info();
            text->setFont(QFont("aerial",10*factor,300*factor));
            text->setText(str);
            text->setGeometry(QRect(mouse_x,mouse_y,text_width*factor,text_height*factor));
            text->show();
            return;
        }
    }
    text->hide();        //如果没找到则关闭
}

void MainWindow::wheelEvent(QWheelEvent *event)              //设置滚轮事件——滚轮能进行缩放
{
    text->hide();
    const int mouse_x=event->x(),mouse_y=event->y();
    if(mouse_x>=Scroll_x && mouse_x<=Scroll_x+Scroll_width && mouse_y>=140 && mouse_y<=140+Scroll_height)
        return;             //在矩形区域内不执行缩放功能
    if(event->delta()>0)
        Line_Scaling(Bigger);

    else
        Line_Scaling(Smaller);
}

void MainWindow::Line_Scaling(const int condition)           //完成线路图缩放事件
{
    if(condition==Bigger){
        if(get_factor()<1.1){
            value_factor(get_factor()+Scale_factor);
            if(in_query==true)
                query_show_line();
            else{
                if(Check_button()==Right_)
                    recover();
                else
                    show_Line(get_button_check());
            }
        }
    }
    else if(condition==Smaller){
        if(get_factor()>0.5){
            value_factor(get_factor()-Scale_factor);
            if(in_query==true)
                query_show_line();
            else{
                if(Check_button()==Right_)
                    recover();
                else
                    show_Line(get_button_check());
            }
        }
    }
}

double MainWindow::get_factor()                              //返回此时的缩放值
{
    return factor;
}

void MainWindow::value_factor(const double num)              //为factor赋值
{
    factor=num;
}

void MainWindow::value_button_check(const int num)           //为button赋值
{
    button_check=num;
}

int MainWindow::get_button_check()                           //返回已被按下的按键值
{
    return button_check;
}

int MainWindow::Infor_solve(QString src_string,int &Line,QString &name1,double &long1,double &lant1,QString &name2,double &long2,double &lant2)  //将输入的信息处理为数值
{
    char src[100];
    strcpy(src,src_string.toUtf8().data());
    char buffer[30];
    int pre_loc=0,cur_loc=0;      //标记string中的点
    for(int i=0;i<7;i++){
        for(pre_loc=cur_loc;src[cur_loc]!='\n' && src[cur_loc]!='\0';cur_loc++)
            ;     //使用循环找到一行的末尾
        if(i==0){
            Line=0;
            if(src[pre_loc]>='0' && src[pre_loc]<='9')
                Line=src[pre_loc]-'0';
            else
                return Wrong;
            if(src[pre_loc+1]>='0' && src[pre_loc+1]<='9')
                Line=Line*10+src[pre_loc+1]-'0';
            if(Line>20)
                return Wrong;
            if(src[pre_loc+1]<'0'||src[pre_loc+1]>'9'){
                if(strncmp(&src[pre_loc+1],"号线",4)!=0)
                    return Wrong;
            }
            else{
                if(strncmp(&src[pre_loc+2],"号线",4)!=0)
                    return Wrong;
            }
        }
        else if(i==1){
            memset(buffer,0,sizeof(buffer));
            strncpy(buffer,&src[pre_loc],cur_loc-pre_loc);
            name1=QString(buffer);
        }
        else if(i==2){
            if(strncmp(&src[pre_loc],"经度:",5)!=0)
                return Wrong;
            long1=0;
            int j=pre_loc+7;
            for(;src[j]!='.';j++){
                if(src[j]>='0' && src[j]<='9')
                    long1=long1*10+src[j]-'0';
                else
                    return Wrong;
            }
            j++;
            for(int k=j;j!=cur_loc;j++){
                if(src[j]>='0' && src[j]<='9')
                    long1+=(src[j]-'0')*pow(10,-(j-k+1));
                else
                    return Wrong;
            }
            if((long1-Standard_longitude)*Coefficient*factor<0 ||(long1-Standard_longitude)*Coefficient*factor>Metro_Width)
                return Wrong;
        }
        else if(i==3){
            if(strncmp(&src[pre_loc],"纬度:",5)!=0)
                return Wrong;
            lant1=0;
            int j=pre_loc+7;
            for(;src[j]!='.';j++){
                if(src[j]>='0' && src[j]<='9')
                    lant1=lant1*10+src[j]-'0';
                else
                    return Wrong;
            }
            j++;
            for(int k=j;j!=cur_loc;j++){
                if(src[j]>='0' && src[j]<='9')
                    lant1+=(src[j]-'0')*pow(10,-(j-k+1));
                else
                    return Wrong;
            }
            if((Standard_lantitude-lant1)*Coefficient*factor<0 || (Standard_lantitude-lant1)*Coefficient*factor>Metro_Height)
                return Wrong;
        }
        else if(i==4){
            memset(buffer,0,sizeof(buffer));
            strncpy(buffer,&src[pre_loc],cur_loc-pre_loc);
            name2=QString(buffer);
        }
        else if(i==5){
            if(strncmp(&src[pre_loc],"经度:",5)!=0)
                return Wrong;
            long2=0;
            int j=pre_loc+7;
            for(;src[j]!='.';j++){
                if(src[j]>='0' && src[j]<='9')
                    long2=long2*10+src[j]-'0';
                else
                    return Wrong;
            }
            j++;
            for(int k=j;j!=cur_loc;j++){
                if(src[j]>='0' && src[j]<='9')
                    long2+=(src[j]-'0')*pow(10,-(j-k+1));
                else
                    return Wrong;
            }
            if((long2-Standard_longitude)*Coefficient*factor<0 ||(long2-Standard_longitude)*Coefficient*factor>Metro_Width)
                return Wrong;
        }
        else if(i==6){
            if(strncmp(&src[pre_loc],"纬度:",5)!=0)
                return Wrong;
            lant2=0;
            int j=pre_loc+7;
            for(;src[j]!='.';j++){
                if(src[j]>='0' && src[j]<='9')
                    lant2=lant2*10+src[j]-'0';
                else
                    return Wrong;
            }
            j++;
            for(int k=j;j!=cur_loc;j++){
                if(src[j]>='0' && src[j]<='9')
                    lant2+=(src[j]-'0')*pow(10,-(j-k+1));
                else
                    return Wrong;
            }
            if((Standard_lantitude-lant2)*Coefficient*factor<0 || (Standard_lantitude-lant2)*Coefficient*factor>Metro_Height)
                return Wrong;
        }

        cur_loc++;
    }
    return Right_;
}

int MainWindow::Check_button()                               //是否有按键仍处于check状态
{
    if(ui->pushButton_1->isChecked()==false && ui->pushButton_2->isChecked()==false &&
            ui->pushButton_3->isChecked()==false && ui->pushButton_4->isChecked()==false &&
            ui->pushButton_5->isChecked()==false && ui->pushButton_6->isChecked()==false &&
            ui->pushButton_7->isChecked()==false && ui->pushButton_8->isChecked()==false &&
            ui->pushButton_9->isChecked()==false && ui->pushButton_10->isChecked()==false &&
            ui->pushButton_11->isChecked()==false && ui->pushButton_12->isChecked()==false &&
            ui->pushButton_13->isChecked()==false && ui->pushButton_14->isChecked()==false &&
            ui->pushButton_15->isChecked()==false && ui->pushButton_16->isChecked()==false &&
            ui->pushButton_17->isChecked()==false && ui->pushButton_18->isChecked()==false &&
            ui->pushButton_19->isChecked()==false && ui->pushButton_20->isChecked()==false)
        return Right_;
    return Wrong;

}

void MainWindow::Button_Block(const int num)                 //当按下某按键时，自动屏蔽其他按键
{
    if(num!=1)
        ui->pushButton_1->setChecked(false);
    if(num!=2)
        ui->pushButton_2->setChecked(false);
    if(num!=3)
        ui->pushButton_3->setChecked(false);
    if(num!=4)
        ui->pushButton_4->setChecked(false);
    if(num!=5)
        ui->pushButton_5->setChecked(false);
    if(num!=6)
        ui->pushButton_6->setChecked(false);
    if(num!=7)
        ui->pushButton_7->setChecked(false);
    if(num!=8)
        ui->pushButton_8->setChecked(false);
    if(num!=9)
        ui->pushButton_9->setChecked(false);
    if(num!=10)
        ui->pushButton_10->setChecked(false);
    if(num!=11)
        ui->pushButton_11->setChecked(false);
    if(num!=12)
        ui->pushButton_12->setChecked(false);
    if(num!=13)
        ui->pushButton_13->setChecked(false);
    if(num!=14)
        ui->pushButton_14->setChecked(false);
    if(num!=15)
        ui->pushButton_15->setChecked(false);
    if(num!=16)
        ui->pushButton_16->setChecked(false);
    if(num!=17)
        ui->pushButton_17->setChecked(false);
    if(num!=18)
        ui->pushButton_18->setChecked(false);
    if(num!=19)
        ui->pushButton_19->setChecked(false);
    if(num!=20)
        ui->pushButton_20->setChecked(false);

}

void MainWindow::show_Line(const int Line)                   //显示某条特定的地铁线路
{
    in_query=false;
    query_remind->hide();        //在点击的时刻使得原始的查询状态自动退出
    paint.drawPixmap(0,0,Metro_Width,Metro_Height,QPixmap(":/white_bg.jpg"));
    //遍历图中的每个点以及每一条边，如果点或边在所给线路上，则NORMAL画出，否则GRAY画出
    //首先遍历边
    for(int i=1;graph->distribute[i].show_id()!=0;i++){
        for(int j=1;graph->distribute[j].show_id()!=0;j++){
            if(graph->edge[i][j].get_belonging_size()>0){
                //需要保证该边被某条线路经过
                double long1,lant1,long2,lant2;
                graph->distribute[i].show_longitude_lantitude(long1,lant1);
                graph->distribute[j].show_longitude_lantitude(long2,lant2);
                if(graph->edge[i][j].check_Line_id(Line)==Right_)
                    graph->edge[i][j].Paint_Connect(paint,long1,lant1,long2,lant2,NORMAL,get_factor());
                else
                    graph->edge[i][j].Paint_Connect(paint,long1,lant1,long2,lant2,GRAY,get_factor());
            }
        }
    }
    //其次遍历点
    for(int i=1;graph->distribute[i].show_id()!=0;i++){
        if(graph->distribute[i].Check_Line(Line)==Right_)
            graph->distribute[i].Paint_Station(paint,NORMAL,get_factor());
        else
            graph->distribute[i].Paint_Station(paint,GRAY,get_factor());
    }
    ui->scrollArea->setWidget(label);
    label->setPixmap(pixmap);
    qDebug()<<"执行力"<<count++;
}

void MainWindow::recover()                                   //复原地铁线路图
{
    paint.drawPixmap(0,0,Metro_Width,Metro_Height,QPixmap(":/white_bg.jpg"));
    //遍历图中的每个点以及每一条边，如果点或边在所给线路上，则NORMAL画出，否则GRAY画出
    //首先遍历边
    for(int i=1;graph->distribute[i].show_id()!=0;i++){
        for(int j=1;graph->distribute[j].show_id()!=0;j++){
            if(graph->edge[i][j].get_belonging_size()>0){
                //需要保证该边被某条线路经过
                double long1,lant1,long2,lant2;
                graph->distribute[i].show_longitude_lantitude(long1,lant1);
                graph->distribute[j].show_longitude_lantitude(long2,lant2);
                graph->edge[i][j].Paint_Connect(paint,long1,lant1,long2,lant2,NORMAL,get_factor());
            }
        }
    }
    //其次遍历点
    for(int i=1;graph->distribute[i].show_id()!=0;i++)
            graph->distribute[i].Paint_Station(paint,NORMAL,get_factor());
    ui->scrollArea->setWidget(label);
    label->setPixmap(pixmap);
}

void MainWindow::Initial()                                   //完成地铁线路的初始化工作
{
    paint.begin(&pixmap);
    paint.drawPixmap(0,0,Metro_Width,Metro_Height,QPixmap(":/white_bg.jpg"));
    //地铁1号线
    graph->Add_edge_longi_lanti(1,"莘庄",121.3921,31.1168,"外环路",121.3996,31.1266,paint);
    graph->Add_edge_longi_lanti(1,"外环路",121.3996,31.1266,"莲花路",121.4093,31.1367,paint);
    graph->Add_edge_longi_lanti(1,"莲花路",121.4093,31.1367,"锦江乐园",121.4206,31.1480,paint);
    graph->Add_edge_longi_lanti(1,"锦江乐园",121.4206,31.1480,"上海南站",121.4367,31.1604,paint);
    graph->Add_edge_longi_lanti(1,"上海南站",121.4367,31.1604,"漕宝路",121.4404,31.1748,paint);
    graph->Add_edge_longi_lanti(1,"漕宝路",121.4404,31.1748,"上海体育馆",121.4433,31.1888,paint);
    graph->Add_edge_longi_lanti(1,"上海体育馆",121.4433,31.1888,"徐家汇",121.4432,31.2006,paint);
    graph->Add_edge_longi_lanti(1,"徐家汇",121.4432,31.2006,"衡山路",121.4531,31.2113,paint);
    graph->Add_edge_longi_lanti(1,"衡山路",121.4531,31.2113,"常熟路",121.4566,31.2198,paint);
    graph->Add_edge_longi_lanti(1,"常熟路",121.4566,31.2198,"陕西南路",121.4656,31.2220,paint);
    graph->Add_edge_longi_lanti(1,"陕西南路",121.4656,31.2220,"黄陂南路",121.4797,31.2287,paint);
    graph->Add_edge_longi_lanti(1,"黄陂南路",121.4797,31.2287,"人民广场",121.4810,31.2386,paint);
    graph->Add_edge_longi_lanti(1,"人民广场",121.4810,31.2386,"新闸路",121.4748,31.2446,paint);
    graph->Add_edge_longi_lanti(1,"新闸路",121.4748,31.2446,"汉中路",121.4652,31.2472,paint);
    graph->Add_edge_longi_lanti(1,"汉中路",121.4652,31.2472,"上海火车站",121.4623,31.2536,paint);
    graph->Add_edge_longi_lanti(1,"上海火车站",121.4623,31.2536,"中山北路",121.4656,31.2649,paint);
    graph->Add_edge_longi_lanti(1,"中山北路",121.4656,31.2649,"延长路",121.4618,31.2780,paint);
    graph->Add_edge_longi_lanti(1,"延长路",121.4618,31.2780,"上海马戏城",121.4585,31.2858,paint);
    graph->Add_edge_longi_lanti(1,"上海马戏城",121.4585,31.2858,"汶水路",121.4565,31.2989,paint);
    graph->Add_edge_longi_lanti(1,"汶水路",121.4565,31.2989,"彭浦新村",121.4550,31.3129,paint);
    graph->Add_edge_longi_lanti(1,"彭浦新村",121.4550,31.3129,"共康路",121.4534,31.3253,paint);
    graph->Add_edge_longi_lanti(1,"共康路",121.4534,31.3253,"通河新村",121.4479,31.3374,paint);
    graph->Add_edge_longi_lanti(1,"通河新村",121.4479,31.3374,"呼兰路",121.4441,31.3461,paint);
    graph->Add_edge_longi_lanti(1,"呼兰路",121.4441,31.3461,"共富新村",121.4405,31.3614,paint);
    graph->Add_edge_longi_lanti(1,"共富新村",121.4405,31.3614,"宝安公路",121.4374,31.3758,paint);
    graph->Add_edge_longi_lanti(1,"宝安公路",121.4374,31.3758,"友谊西路",121.4344,31.3874,paint);
    graph->Add_edge_longi_lanti(1,"友谊西路",121.4344,31.3874,"富锦路",121.4310,31.3985,paint);

    //地铁2号线
    graph->Add_edge_longi_lanti(2,"徐泾东",121.3058,31.1944,"虹桥火车站",121.3279,31.2003,paint);
    graph->Add_edge_longi_lanti(2,"虹桥火车站",121.3279,31.2003,"虹桥2号航站楼",121.3327,31.2006,paint);
    graph->Add_edge_longi_lanti(2,"虹桥2号航站楼",121.3327,31.2006,"淞虹路",121.3660,31.2239,paint);
    graph->Add_edge_longi_lanti(2,"淞虹路",121.3660,31.2239,"北新泾",121.3806,31.2221,paint);
    graph->Add_edge_longi_lanti(2,"北新泾",121.3806,31.2221,"威宁路",121.3936,31.2204,paint);
    graph->Add_edge_longi_lanti(2,"威宁路",121.3936,31.2204,"娄山关路",121.4103,31.2170,paint);
    graph->Add_edge_longi_lanti(2,"娄山关路",121.4103,31.2170,"中山公园",121.4228,31.2243,paint);
    graph->Add_edge_longi_lanti(2,"中山公园",121.4228,31.2243,"江苏路",121.4377,31.2265,paint);
    graph->Add_edge_longi_lanti(2,"江苏路",121.4377,31.2265,"静安寺",121.4540,31.2295,paint);
    graph->Add_edge_longi_lanti(2,"静安寺",121.4540,31.2295,"南京西路",121.4659,31.2341,paint);
    graph->Add_edge_longi_lanti(2,"南京西路",121.4659,31.2341,"人民广场",121.4810,31.2386,paint);
    graph->Add_edge_longi_lanti(2,"人民广场",121.4810,31.2386,"南京东路",121.4908,31.2437,paint);
    graph->Add_edge_longi_lanti(2,"南京东路",121.4908,31.2437,"陆家嘴",121.5089,31.2438,paint);
    graph->Add_edge_longi_lanti(2,"陆家嘴",121.5089,31.2438,"东昌路",121.5219,31.2390,paint);
    graph->Add_edge_longi_lanti(2,"东昌路",121.5219,31.2390,"世纪大道",121.5335,31.2348,paint);
    graph->Add_edge_longi_lanti(2,"世纪大道",121.5335,31.2348,"上海科技馆",121.5513,31.2248,paint);
    graph->Add_edge_longi_lanti(2,"上海科技馆",121.5513,31.2248,"世纪公园",121.5573,31.2156,paint);
    graph->Add_edge_longi_lanti(2,"世纪公园",121.5573,31.2156,"龙阳路",121.5639,31.2091,paint);
    graph->Add_edge_longi_lanti(2,"龙阳路",121.5639,31.2091,"张江高科",121.5941,31.2077,paint);
    graph->Add_edge_longi_lanti(2,"张江高科",121.5941,31.2077,"金科路",121.6085,31.2099,paint);
    graph->Add_edge_longi_lanti(2,"金科路",121.6085,31.2099,"广兰路",121.6277,31.2165,paint);
    graph->Add_edge_longi_lanti(2,"广兰路",121.6277,31.2165,"唐镇",121.6627,31.2204,paint);
    graph->Add_edge_longi_lanti(2,"唐镇",121.6627,31.2204,"创新中路",121.6801,31.2198,paint);
    graph->Add_edge_longi_lanti(2,"创新中路",121.6801,31.2198,"华夏东路",121.6873,31.2029,paint);
    graph->Add_edge_longi_lanti(2,"华夏东路",121.6873,31.2029,"川沙",121.7044,31.1928,paint);
    graph->Add_edge_longi_lanti(2,"川沙",121.7044,31.1928,"凌空路",121.7306,31.1985,paint);
    graph->Add_edge_longi_lanti(2,"凌空路",121.7306,31.1985,"远东大道",121.7620,31.2051,paint);
    graph->Add_edge_longi_lanti(2,"远东大道",121.7620,31.2051,"海天三路",121.8033,31.1748,paint);
    graph->Add_edge_longi_lanti(2,"海天三路",121.8033,31.1748,"浦东国际机场",121.8129,31.1560,paint);

    //3号线
    graph->Add_edge_longi_lanti(3,"上海南站",121.4367,31.1604,"石龙路",121.4495,31.1642,paint);
    graph->Add_edge_longi_lanti(3,"石龙路",121.4495,31.1642,"龙漕路",121.4504,31.1761,paint);
    graph->Add_edge_longi_lanti(3,"龙漕路",121.4504,31.1761,"漕溪路",121.4447,31.1829,paint);
    graph->Add_edge_longi_lanti(3,"漕溪路",121.4447,31.1829,"宜山路",121.4336,31.1925,paint);
    graph->Add_edge_longi_lanti(3,"宜山路",121.4336,31.1925,"虹桥路",121.4280,31.2031,paint);
    graph->Add_edge_longi_lanti(3,"虹桥路",121.4280,31.2031,"延安西路",121.4235,31.2157,paint);
    graph->Add_edge_longi_lanti(3,"延安西路",121.4235,31.2157,"中山公园",121.4228,31.2243,paint);
    graph->Add_edge_longi_lanti(3,"中山公园",121.4228,31.2243,"金沙江路",121.4193,31.2379,paint);
    graph->Add_edge_longi_lanti(3,"金沙江路",121.4193,31.2379,"曹杨路",121.4244,31.2446,paint);
    graph->Add_edge_longi_lanti(3,"曹杨路",121.4244,31.2446,"镇坪路",121.4368,31.2528,paint);
    graph->Add_edge_longi_lanti(3,"镇坪路",121.4368,31.2528,"中潭路",121.4474,31.2609,paint);
    graph->Add_edge_longi_lanti(3,"中潭路",121.4474,31.2609,"上海火车站",121.4623,31.2536,paint);
    graph->Add_edge_longi_lanti(3,"上海火车站",121.4623,31.2536,"宝山路",121.4829,31.2573,paint);
    graph->Add_edge_longi_lanti(3,"宝山路",121.4829,31.2573,"东宝兴路",121.4866,31.2657,paint);
    graph->Add_edge_longi_lanti(3,"东宝兴路",121.4866,31.2657,"虹口足球场",121.4857,31.2771,paint);
    graph->Add_edge_longi_lanti(3,"虹口足球场",121.4857,31.2771,"赤峰路",121.4889,31.2869,paint);
    graph->Add_edge_longi_lanti(3,"赤峰路",121.4889,31.2869,"大柏树",121.4897,31.2952,paint);
    graph->Add_edge_longi_lanti(3,"大柏树",121.4897,31.2952,"江湾镇",121.4917,31.3113,paint);
    graph->Add_edge_longi_lanti(3,"江湾镇",121.4917,31.3113,"殷高西路",121.4914,31.3255,paint);
    graph->Add_edge_longi_lanti(3,"殷高西路",121.4914,31.3255,"长江南路",121.4981,31.3378,paint);
    graph->Add_edge_longi_lanti(3,"长江南路",121.4981,31.3378,"淞发路",121.5070,31.3508,paint);
    graph->Add_edge_longi_lanti(3,"淞发路",121.5070,31.3508,"张华浜",121.5053,31.3634,paint);
    graph->Add_edge_longi_lanti(3,"张华浜",121.5053,31.3634,"淞滨路",121.4993,31.3769,paint);
    graph->Add_edge_longi_lanti(3,"淞滨路",121.4993,31.3769,"水产路",121.4947,31.3870,paint);
    graph->Add_edge_longi_lanti(3,"水产路",121.4947,31.3870,"宝杨路",121.4861,31.4011,paint);
    graph->Add_edge_longi_lanti(3,"宝杨路",121.4861,31.4011,"友谊路",121.4823,31.4100,paint);
    graph->Add_edge_longi_lanti(3,"友谊路",121.4823,31.4100,"铁力路",121.4676,31.4142,paint);
    graph->Add_edge_longi_lanti(3,"铁力路",121.4676,31.4142,"江杨北路",121.4462,31.4142,paint);

    //4号线
    graph->Add_edge_longi_lanti(4,"宜山路",121.4336,31.1925,"虹桥路",121.4280,31.2031,paint);
    graph->Add_edge_longi_lanti(4,"虹桥路",121.4280,31.2031,"延安西路",121.4235,31.2157,paint);
    graph->Add_edge_longi_lanti(4,"延安西路",121.4235,31.2157,"中山公园",121.4228,31.2243,paint);
    graph->Add_edge_longi_lanti(4,"中山公园",121.4228,31.2243,"金沙江路",121.4193,31.2379,paint);
    graph->Add_edge_longi_lanti(4,"金沙江路",121.4193,31.2379,"曹杨路",121.4244,31.2446,paint);
    graph->Add_edge_longi_lanti(4,"曹杨路",121.4244,31.2446,"镇坪路",121.4368,31.2528,paint);
    graph->Add_edge_longi_lanti(4,"镇坪路",121.4368,31.2528,"中潭路",121.4474,31.2609,paint);
    graph->Add_edge_longi_lanti(4,"中潭路",121.4474,31.2609,"上海火车站",121.4623,31.2536,paint);
    graph->Add_edge_longi_lanti(4,"上海火车站",121.4623,31.2536,"宝山路",121.4829,31.2573,paint);
    graph->Add_edge_longi_lanti(4,"宝山路",121.4829,31.2573,"海伦路",121.4956,31.2648,paint);
    graph->Add_edge_longi_lanti(4,"海伦路",121.4956,31.2648,"临平路",121.5076,31.2665,paint);
    graph->Add_edge_longi_lanti(4,"临平路",121.5076,31.2665,"大连路",121.5194,31.2639,paint);
    graph->Add_edge_longi_lanti(4,"大连路",121.5194,31.2639,"杨树浦路",121.5239,31.2576,paint);
    graph->Add_edge_longi_lanti(4,"杨树浦路",121.5239,31.2576,"浦东大道",121.5260,31.2457,paint);
    graph->Add_edge_longi_lanti(4,"浦东大道",121.5260,31.2457,"世纪大道",121.5335,31.2348,paint);
    graph->Add_edge_longi_lanti(4,"世纪大道",121.5335,31.2348,"浦电路",121.5386,31.2285,paint);
    graph->Add_edge_longi_lanti(4,"浦电路",121.5386,31.2285,"蓝村路",121.5343,31.2176,paint);
    graph->Add_edge_longi_lanti(4,"蓝村路",121.5343,31.2176,"塘桥",121.5253,31.2154,paint);
    graph->Add_edge_longi_lanti(4,"塘桥",121.5253,31.2154,"南浦大桥",121.5060,31.2140,paint);
    graph->Add_edge_longi_lanti(4,"南浦大桥",121.5060,31.2140,"西藏南路",121.4960,31.2076,paint);
    graph->Add_edge_longi_lanti(4,"西藏南路",121.4960,31.2076,"鲁班路",121.4813,31.2050,paint);
    graph->Add_edge_longi_lanti(4,"鲁班路",121.4813,31.2050,"大木桥路",121.4701,31.1998,paint);
    graph->Add_edge_longi_lanti(4,"大木桥路",121.4701,31.1998,"东安路",121.4611,31.1973,paint);
    graph->Add_edge_longi_lanti(4,"东安路",121.4611,31.1973,"上海体育场",121.4498,31.1918,paint);
    graph->Add_edge_longi_lanti(4,"上海体育场",121.4498,31.1918,"上海体育馆",121.4433,31.1888,paint);
    graph->Add_edge_longi_lanti(4,"上海体育馆",121.4433,31.1888,"宜山路",121.4336,31.1925,paint);

    //5号线
    graph->Add_edge_longi_lanti(5,"莘庄",121.3921,31.1168,"春申路",121.3923,31.1039,paint);
    graph->Add_edge_longi_lanti(5,"春申路",121.3923,31.1039,"银都路",121.3968,31.0950,paint);
    graph->Add_edge_longi_lanti(5,"银都路",121.3968,31.0950,"颛桥",121.4084,31.0728,paint);
    graph->Add_edge_longi_lanti(5,"颛桥",121.4084,31.0728,"北桥",121.4164,31.0511,paint);
    graph->Add_edge_longi_lanti(5,"北桥",121.4164,31.0511,"剑川路",121.4229,31.0325,paint);
    graph->Add_edge_longi_lanti(5,"剑川路",121.4229,31.0325,"东川路",121.4263,31.0244,paint);
    graph->Add_edge_longi_lanti(5,"东川路",121.4263,31.0244,"金平路",121.4165,31.0171,paint);
    graph->Add_edge_longi_lanti(5,"金平路",121.4165,31.0171,"华宁路",121.4017,31.0131,paint);
    graph->Add_edge_longi_lanti(5,"华宁路",121.4017,31.0131,"文井路",121.3874,31.0091,paint);
    graph->Add_edge_longi_lanti(5,"文井路",121.3874,31.0091,"闵行开发区",121.3765,31.0062,paint);
    graph->Add_edge_longi_lanti(5,"东川路",121.4263,31.0244,"江川路",121.4298,31.0117,paint);
    graph->Add_edge_longi_lanti(5,"江川路",121.4298,31.0117,"西渡",121.4389,30.9956,paint);
    graph->Add_edge_longi_lanti(5,"西渡",121.4389,30.9956,"萧塘",121.4483,30.9722,paint);
    graph->Add_edge_longi_lanti(5,"萧塘",121.4483,30.9722,"奉浦大道",121.4557,30.9481,paint);
    graph->Add_edge_longi_lanti(5,"奉浦大道",121.4557,30.9481,"环城东路",121.4700,30.9371,paint);
    graph->Add_edge_longi_lanti(5,"环城东路",121.4700,30.9371,"望园路",121.4902,30.9377,paint);
    graph->Add_edge_longi_lanti(5,"望园路",121.4902,30.9377,"金海湖",121.4990,30.9346,paint);
    graph->Add_edge_longi_lanti(5,"金海湖",121.4990,30.9346,"奉贤新城",121.5027,30.9198,paint);

    //6号线
    graph->Add_edge_longi_lanti(6,"东方体育中心",121.4871,31.1592,"灵岩南路",121.5018,31.1543,paint);
    graph->Add_edge_longi_lanti(6,"灵岩南路",121.5018,31.1543,"上南路",121.5129,31.1547,paint);
    graph->Add_edge_longi_lanti(6,"上南路",121.5129,31.1547,"华夏西路",121.5212,31.1555,paint);
    graph->Add_edge_longi_lanti(6,"华夏西路",121.5212,31.1555,"高青路",121.5220,31.1655,paint);
    graph->Add_edge_longi_lanti(6,"高青路",121.5220,31.1655,"东明路",121.5179,31.1784,paint);
    graph->Add_edge_longi_lanti(6,"东明路",121.5179,31.1784,"高科西路",121.5165,31.1914,paint);
    graph->Add_edge_longi_lanti(6,"高科西路",121.5165,31.1914,"临沂新村",121.5235,31.1987,paint);
    graph->Add_edge_longi_lanti(6,"临沂新村",121.5235,31.1987,"上海儿童医学中心",121.5300,31.2089,paint);
    graph->Add_edge_longi_lanti(6,"上海儿童医学中心",121.5300,31.2089,"蓝村路",121.5343,31.2176,paint);
    graph->Add_edge_longi_lanti(6,"蓝村路",121.5343,31.2176,"浦电路",121.5386,31.2285,paint);
    graph->Add_edge_longi_lanti(6,"浦电路",121.5386,31.2285,"世纪大道",121.5335,31.2348,paint);
    graph->Add_edge_longi_lanti(6,"世纪大道",121.5335,31.2348,"源深体育中心",121.5411,31.2390,paint);
    graph->Add_edge_longi_lanti(6,"源深体育中心",121.5411,31.2390,"民生路",121.5499,31.2421,paint);
    graph->Add_edge_longi_lanti(6,"民生路",121.5499,31.2421,"北洋泾路",121.5588,31.2455,paint);
    graph->Add_edge_longi_lanti(6,"北洋泾路",121.5588,31.2455,"德平路",121.5706,31.2517,paint);
    graph->Add_edge_longi_lanti(6,"德平路",121.5706,31.2517,"云山路",121.5792,31.2566,paint);
    graph->Add_edge_longi_lanti(6,"云山路",121.5792,31.2566,"金桥路",121.5883,31.2632,paint);
    graph->Add_edge_longi_lanti(6,"金桥路",121.5883,31.2632,"博兴路",121.5932,31.2695,paint);
    graph->Add_edge_longi_lanti(6,"博兴路",121.5932,31.2695,"五莲路",121.5945,31.2780,paint);
    graph->Add_edge_longi_lanti(6,"五莲路",121.5945,31.2780,"巨峰路",121.5955,31.2862,paint);
    graph->Add_edge_longi_lanti(6,"巨峰路",121.5955,31.2862,"东靖路",121.5953,31.2966,paint);
    graph->Add_edge_longi_lanti(6,"东靖路",121.5953,31.2966,"五洲大道",121.5958,31.3086,paint);
    graph->Add_edge_longi_lanti(6,"五洲大道",121.5958,31.3086,"洲海路",121.5959,31.3183,paint);
    graph->Add_edge_longi_lanti(6,"洲海路",121.5959,31.3183,"外高桥保税区南",121.6085,31.3274,paint);
    graph->Add_edge_longi_lanti(6,"外高桥保税区南",121.6085,31.3274,"航津路",121.6005,31.3413,paint);
    graph->Add_edge_longi_lanti(6,"航津路",121.6005,31.3413,"外高桥保税区北",121.5934,31.3538,paint);
    graph->Add_edge_longi_lanti(6,"外高桥保税区北",121.5934,31.3538,"港城路",121.5813,31.3592,paint);

    //7号线
    graph->Add_edge_longi_lanti(7,"花木路",121.5692,31.2175,"龙阳路",121.5639,31.2091,paint);
    graph->Add_edge_longi_lanti(7,"龙阳路",121.5639,31.2091,"芳华路",121.5565,31.1994,paint);
    graph->Add_edge_longi_lanti(7,"芳华路",121.5565,31.1994,"锦绣路",121.5465,31.1936,paint);
    graph->Add_edge_longi_lanti(7,"锦绣路",121.5465,31.1936,"杨高南路",121.5318,31.1933,paint);
    graph->Add_edge_longi_lanti(7,"杨高南路",121.5318,31.1933,"高科西路",121.5165,31.1914,paint);
    graph->Add_edge_longi_lanti(7,"高科西路",121.5165,31.1914,"云台路",121.5069,31.1877,paint);
    graph->Add_edge_longi_lanti(7,"云台路",121.5069,31.1877,"耀华路",121.5012,31.1841,paint);
    graph->Add_edge_longi_lanti(7,"耀华路",121.5012,31.1841,"长清路",121.4926,31.1803,paint);
    graph->Add_edge_longi_lanti(7,"长清路",121.4926,31.1803,"后滩",121.4805,31.1778,paint);
    graph->Add_edge_longi_lanti(7,"后滩",121.4805,31.1778,"龙华中路",121.4632,31.1906,paint);
    graph->Add_edge_longi_lanti(7,"龙华中路",121.4632,31.1906,"东安路",121.4611,31.1973,paint);
    graph->Add_edge_longi_lanti(7,"东安路",121.4611,31.1973,"肇嘉浜路",121.4569,31.2053,paint);
    graph->Add_edge_longi_lanti(7,"肇嘉浜路",121.4569,31.2053,"常熟路",121.4566,31.2198,paint);
    graph->Add_edge_longi_lanti(7,"常熟路",121.4566,31.2198,"静安寺",121.4540,31.2295,paint);
    graph->Add_edge_longi_lanti(7,"静安寺",121.4540,31.2295,"昌平路",121.4490,31.2401,paint);
    graph->Add_edge_longi_lanti(7,"昌平路",121.4490,31.2401,"长寿路",121.4447,31.2467,paint);
    graph->Add_edge_longi_lanti(7,"长寿路",121.4447,31.2467,"镇坪路",121.4368,31.2528,paint);
    graph->Add_edge_longi_lanti(7,"镇坪路",121.4368,31.2528,"岚皋路",121.4283,31.2625,paint);
    graph->Add_edge_longi_lanti(7,"岚皋路",121.4283,31.2625,"新村路",121.4290,31.2701,paint);
    graph->Add_edge_longi_lanti(7,"新村路",121.4290,31.2701,"大华三路",121.4296,31.2799,paint);
    graph->Add_edge_longi_lanti(7,"大华三路",121.4296,31.2799,"行知路",121.4278,31.2910,paint);
    graph->Add_edge_longi_lanti(7,"行知路",121.4278,31.2910,"大场镇",121.4229,31.2997,paint);
    graph->Add_edge_longi_lanti(7,"大场镇",121.4229,31.2997,"场中路",121.4201,31.3098,paint);
    graph->Add_edge_longi_lanti(7,"场中路",121.4201,31.3098,"上大路",121.4150,31.3207,paint);
    graph->Add_edge_longi_lanti(7,"上大路",121.4150,31.3207,"南陈路",121.4054,31.3275,paint);
    graph->Add_edge_longi_lanti(7,"南陈路",121.4054,31.3275,"上海大学",121.3955,31.3261,paint);
    graph->Add_edge_longi_lanti(7,"上海大学",121.3955,31.3261,"祁华路",121.3801,31.3281,paint);
    graph->Add_edge_longi_lanti(7,"祁华路",121.3801,31.3281,"顾村公园",121.3796,31.3502,paint);
    graph->Add_edge_longi_lanti(7,"顾村公园",121.3796,31.3502,"刘行",121.3689,31.3633,paint);
    graph->Add_edge_longi_lanti(7,"刘行",121.3689,31.3633,"潘广路",121.3623,31.3701,paint);
    graph->Add_edge_longi_lanti(7,"潘广路",121.3623,31.3701,"罗南新村",121.3640,31.3945,paint);
    graph->Add_edge_longi_lanti(7,"罗南新村",121.3640,31.3945,"美兰湖",121.3564,31.4078,paint);

    //8号线
    graph->Add_edge_longi_lanti(8,"沈杜公路",121.5190,31.0670,"联航路",121.5172,31.0792,paint);
    graph->Add_edge_longi_lanti(8,"联航路",121.5172,31.0792,"江月路",121.5152,31.0899,paint);
    graph->Add_edge_longi_lanti(8,"江月路",121.5152,31.0899,"浦江镇",121.5129,31.1022,paint);
    graph->Add_edge_longi_lanti(8,"浦江镇",121.5129,31.1022,"芦恒路",121.5047,31.1247,paint);
    graph->Add_edge_longi_lanti(8,"芦恒路",121.5047,31.1247,"凌兆新村",121.4961,31.1469,paint);
    graph->Add_edge_longi_lanti(8,"凌兆新村",121.4961,31.1469,"东方体育中心",121.4871,31.1592,paint);
    graph->Add_edge_longi_lanti(8,"东方体育中心",121.4871,31.1592,"杨思",121.5002,31.1666,paint);
    graph->Add_edge_longi_lanti(8,"杨思",121.5002,31.1666,"成山路",121.5033,31.1766,paint);
    graph->Add_edge_longi_lanti(8,"成山路",121.5033,31.1766,"耀华路",121.5012,31.1841,paint);
    graph->Add_edge_longi_lanti(8,"耀华路",121.5012,31.1841,"中华艺术宫",121.5002,31.1909,paint);
    graph->Add_edge_longi_lanti(8,"中华艺术宫",121.5002,31.1909,"西藏南路",121.4960,31.2076,paint);
    graph->Add_edge_longi_lanti(8,"西藏南路",121.4960,31.2076,"陆家浜路",121.4926,31.2173,paint);
    graph->Add_edge_longi_lanti(8,"陆家浜路",121.4926,31.2173,"老西门",121.4900,31.2247,paint);
    graph->Add_edge_longi_lanti(8,"老西门",121.4900,31.2247,"大世界",121.4858,31.2331,paint);
    graph->Add_edge_longi_lanti(8,"大世界",121.4858,31.2331,"人民广场",121.4810,31.2386,paint);
    graph->Add_edge_longi_lanti(8,"人民广场",121.4810,31.2386,"曲阜路",121.4781,31.2483,paint);
    graph->Add_edge_longi_lanti(8,"曲阜路",121.4781,31.2483,"中兴路",121.4754,31.2590,paint);
    graph->Add_edge_longi_lanti(8,"中兴路",121.4754,31.2590,"西藏北路",121.4752,31.2693,paint);
    graph->Add_edge_longi_lanti(8,"西藏北路",121.4752,31.2693,"虹口足球场",121.4857,31.2771,paint);
    graph->Add_edge_longi_lanti(8,"虹口足球场",121.4857,31.2771,"曲阳路",121.4973,31.2821,paint);
    graph->Add_edge_longi_lanti(8,"曲阳路",121.4973,31.2821,"四平路",121.5080,31.2805,paint);
    graph->Add_edge_longi_lanti(8,"四平路",121.5080,31.2805,"鞍山新村",121.5161,31.2789,paint);
    graph->Add_edge_longi_lanti(8,"鞍山新村",121.5161,31.2789,"江浦路",121.5247,31.2808,paint);
    graph->Add_edge_longi_lanti(8,"江浦路",121.5247,31.2808,"黄兴路",121.5349,31.2847,paint);
    graph->Add_edge_longi_lanti(8,"黄兴路",121.5349,31.2847,"延吉中路",121.5413,31.2944,paint);
    graph->Add_edge_longi_lanti(8,"延吉中路",121.5413,31.2944,"黄兴公园",121.5396,31.3017,paint);
    graph->Add_edge_longi_lanti(8,"黄兴公园",121.5396,31.3017,"翔殷路",121.5384,31.3110,paint);
    graph->Add_edge_longi_lanti(8,"翔殷路",121.5384,31.3110,"嫩江路",121.5385,31.3209,paint);
    graph->Add_edge_longi_lanti(8,"嫩江路",121.5385,31.3209,"市光路",121.5385,31.3287,paint);

    //9号线
    graph->Add_edge_longi_lanti(9,"松江南站",121.2373,30.9909,"醉白池",121.2356,31.0070,paint);
    graph->Add_edge_longi_lanti(9,"醉白池",121.2356,31.0070,"松江体育中心",121.2370,31.0220,paint);
    graph->Add_edge_longi_lanti(9,"松江体育中心",121.2370,31.0220,"松江新城",121.2372,31.0362,paint);
    graph->Add_edge_longi_lanti(9,"松江新城",121.2372,31.0362,"松江大学城",121.2392,31.0600,paint);
    graph->Add_edge_longi_lanti(9,"松江大学城",121.2392,31.0600,"洞泾",121.2370,31.0903,paint);
    graph->Add_edge_longi_lanti(9,"洞泾",121.2370,31.0903,"佘山",121.2362,31.1103,paint);
    graph->Add_edge_longi_lanti(9,"佘山",121.2362,31.1103,"泗泾",121.2669,31.1239,paint);
    graph->Add_edge_longi_lanti(9,"泗泾",121.2669,31.1239,"九亭",121.3259,31.1436,paint);
    graph->Add_edge_longi_lanti(9,"九亭",121.3259,31.1436,"中春路",121.3440,31.1556,paint);
    graph->Add_edge_longi_lanti(9,"中春路",121.3440,31.1556,"七宝",121.3560,31.1613,paint);
    graph->Add_edge_longi_lanti(9,"七宝",121.3560,31.1613,"星中路",121.3755,31.1638,paint);
    graph->Add_edge_longi_lanti(9,"星中路",121.3755,31.1638,"合川路",121.3912,31.1721,paint);
    graph->Add_edge_longi_lanti(9,"合川路",121.3912,31.1721,"漕河泾开发区",121.4043,31.1762,paint);
    graph->Add_edge_longi_lanti(9,"漕河泾开发区",121.4043,31.1762,"桂林路",121.4249,31.1810,paint);
    graph->Add_edge_longi_lanti(9,"桂林路",121.4249,31.1810,"宜山路",121.4336,31.1925,paint);
    graph->Add_edge_longi_lanti(9,"宜山路",121.4336,31.1925,"徐家汇",121.4432,31.2006,paint);
    graph->Add_edge_longi_lanti(9,"徐家汇",121.4432,31.2006,"肇嘉浜路",121.4569,31.2053,paint);
    graph->Add_edge_longi_lanti(9,"肇嘉浜路",121.4569,31.2053,"嘉善路",121.4675,31.2085,paint);
    graph->Add_edge_longi_lanti(9,"嘉善路",121.4675,31.2085,"打浦桥",121.4754,31.2120,paint);
    graph->Add_edge_longi_lanti(9,"打浦桥",121.4754,31.2120,"马当路",121.4833,31.2156,paint);
    graph->Add_edge_longi_lanti(9,"马当路",121.4833,31.2156,"陆家浜路",121.4926,31.2173,paint);
    graph->Add_edge_longi_lanti(9,"陆家浜路",121.4926,31.2173,"小南门",121.5048,31.2226,paint);
    graph->Add_edge_longi_lanti(9,"小南门",121.5048,31.2226,"商城路",121.5229,31.2361,paint);
    graph->Add_edge_longi_lanti(9,"商城路",121.5229,31.2361,"世纪大道",121.5335,31.2348,paint);
    graph->Add_edge_longi_lanti(9,"世纪大道",121.5335,31.2348,"杨高中路",121.5551,31.2338,paint);
    graph->Add_edge_longi_lanti(9,"杨高中路",121.5551,31.2338,"芳甸路",121.5648,31.2383,paint);
    graph->Add_edge_longi_lanti(9,"芳甸路",121.5648,31.2383,"蓝天路",121.5841,31.2471,paint);
    graph->Add_edge_longi_lanti(9,"蓝天路",121.5841,31.2471,"台儿庄路",121.6043,31.2584,paint);
    graph->Add_edge_longi_lanti(9,"台儿庄路",121.6043,31.2584,"金桥",121.6178,31.2663,paint);
    graph->Add_edge_longi_lanti(9,"金桥",121.6178,31.2663,"金吉路",121.6351,31.2700,paint);
    graph->Add_edge_longi_lanti(9,"金吉路",121.6351,31.2700,"金海路",121.6453,31.2692,paint);
    graph->Add_edge_longi_lanti(9,"金海路",121.6453,31.2692,"顾唐路",121.6630,31.2721,paint);
    graph->Add_edge_longi_lanti(9,"顾唐路",121.6630,31.2721,"民雷路",121.6746,31.2746,paint);
    graph->Add_edge_longi_lanti(9,"民雷路",121.6746,31.2746,"曹路",121.6894,31.2775,paint);

    //10号线
    graph->Add_edge_longi_lanti(10,"新江湾城",121.5131,31.3344,"殷高东路",121.5135,31.3275,paint);
    graph->Add_edge_longi_lanti(10,"殷高东路",121.5135,31.3275,"三门路",121.5145,31.3187,paint);
    graph->Add_edge_longi_lanti(10,"三门路",121.5145,31.3187,"江湾体育场",121.5200,31.3097,paint);
    graph->Add_edge_longi_lanti(10,"江湾体育场",121.5200,31.3097,"五角场",121.5211,31.3038,paint);
    graph->Add_edge_longi_lanti(10,"五角场",121.5211,31.3038,"国权路",121.5167,31.2950,paint);
    graph->Add_edge_longi_lanti(10,"国权路",121.5167,31.2950,"同济大学",121.5130,31.2882,paint);
    graph->Add_edge_longi_lanti(10,"同济大学",121.5130,31.2882,"四平路",121.5080,31.2805,paint);
    graph->Add_edge_longi_lanti(10,"四平路",121.5080,31.2805,"邮电新村",121.5008,31.2741,paint);
    graph->Add_edge_longi_lanti(10,"邮电新村",121.5008,31.2741,"海伦路",121.4956,31.2648,paint);
    graph->Add_edge_longi_lanti(10,"海伦路",121.4956,31.2648,"四川北路",121.4907,31.2577,paint);
    graph->Add_edge_longi_lanti(10,"四川北路",121.4907,31.2577,"天潼路",121.4887,31.2500,paint);
    graph->Add_edge_longi_lanti(10,"天潼路",121.4887,31.2500,"南京东路",121.4908,31.2437,paint);
    graph->Add_edge_longi_lanti(10,"南京东路",121.4908,31.2437,"豫园",121.4939,31.2340,paint);
    graph->Add_edge_longi_lanti(10,"豫园",121.4939,31.2340,"老西门",121.4900,31.2247,paint);
    graph->Add_edge_longi_lanti(10,"老西门",121.4900,31.2247,"新天地",121.4817,31.2220,paint);
    graph->Add_edge_longi_lanti(10,"新天地",121.4817,31.2220,"陕西南路",121.4656,31.2220,paint);
    graph->Add_edge_longi_lanti(10,"陕西南路",121.4656,31.2220,"上海图书馆",121.4507,31.2144,paint);
    graph->Add_edge_longi_lanti(10,"上海图书馆",121.4507,31.2144,"交通大学",121.4410,31.2084,paint);
    graph->Add_edge_longi_lanti(10,"交通大学",121.4410,31.2084,"虹桥路",121.4280,31.2031,paint);
    graph->Add_edge_longi_lanti(10,"虹桥路",121.4280,31.2031,"宋园路",121.4184,31.2025,paint);
    graph->Add_edge_longi_lanti(10,"宋园路",121.4184,31.2025,"伊犁路",121.4106,31.2046,paint);
    graph->Add_edge_longi_lanti(10,"伊犁路",121.4106,31.2046,"水城路",121.3984,31.2050,paint);
    graph->Add_edge_longi_lanti(10,"水城路",121.3984,31.2050,"龙溪路",121.3867,31.2000,paint);
    graph->Add_edge_longi_lanti(10,"龙溪路",121.3867,31.2000,"龙柏新村",121.3769,31.1832,paint);
    graph->Add_edge_longi_lanti(10,"龙柏新村",121.3769,31.1832,"紫藤路",121.3709,31.1755,paint);
    graph->Add_edge_longi_lanti(10,"紫藤路",121.3709,31.1755,"航中路",121.3614,31.1713,paint);
    graph->Add_edge_longi_lanti(10,"龙溪路",121.3867,31.2000,"上海动物园",121.3748,31.1959,paint);
    graph->Add_edge_longi_lanti(10,"上海动物园",121.3748,31.1959,"虹桥1号航站楼",121.3537,31.1976,paint);
    graph->Add_edge_longi_lanti(10,"虹桥1号航站楼",121.3537,31.1976,"虹桥2号航站楼",121.3327,31.2006,paint);
    graph->Add_edge_longi_lanti(10,"虹桥2号航站楼",121.3327,31.2006,"虹桥火车站",121.3279,31.2003,paint);

    //11号线
    graph->Add_edge_longi_lanti(11,"迪士尼",121.6744,31.1475,"康新公路",121.6239,31.1360,paint);
    graph->Add_edge_longi_lanti(11,"康新公路",121.6239,31.1360,"秀沿路",121.6050,31.1439,paint);
    graph->Add_edge_longi_lanti(11,"秀沿路",121.6050,31.1439,"罗山路",121.5996,31.1593,paint);
    graph->Add_edge_longi_lanti(11,"罗山路",121.5996,31.1593,"御桥",121.5771,31.1644,paint);
    graph->Add_edge_longi_lanti(11,"御桥",121.5771,31.1644,"浦三路",121.5455,31.1571,paint);
    graph->Add_edge_longi_lanti(11,"浦三路",121.5455,31.1571,"三林东",121.5296,31.1523,paint);
    graph->Add_edge_longi_lanti(11,"三林东",121.5296,31.1523,"三林",121.5181,31.1487,paint);
    graph->Add_edge_longi_lanti(11,"三林",121.5181,31.1487,"东方体育中心",121.4871,31.1592,paint);
    graph->Add_edge_longi_lanti(11,"东方体育中心",121.4871,31.1592,"龙耀路",121.4661,31.1659,paint);
    graph->Add_edge_longi_lanti(11,"龙耀路",121.4661,31.1659,"云锦路",121.4648,31.1730,paint);
    graph->Add_edge_longi_lanti(11,"云锦路",121.4648,31.1730,"龙华",121.4593,31.1790,paint);
    graph->Add_edge_longi_lanti(11,"龙华",121.4593,31.1790,"上海游泳馆",121.4479,31.1855,paint);
    graph->Add_edge_longi_lanti(11,"上海游泳馆",121.4479,31.1855,"徐家汇",121.4432,31.2006,paint);
    graph->Add_edge_longi_lanti(11,"徐家汇",121.4432,31.2006,"交通大学",121.4410,31.2084,paint);
    graph->Add_edge_longi_lanti(11,"交通大学",121.4410,31.2084,"江苏路",121.4377,31.2265,paint);
    graph->Add_edge_longi_lanti(11,"江苏路",121.4377,31.2265,"隆德路",121.4300,31.2367,paint);
    graph->Add_edge_longi_lanti(11,"隆德路",121.4300,31.2367,"曹杨路",121.4244,31.2446,paint);
    graph->Add_edge_longi_lanti(11,"曹杨路",121.4244,31.2446,"枫桥路",121.4176,31.2479,paint);
    graph->Add_edge_longi_lanti(11,"枫桥路",121.4176,31.2479,"真如",121.4137,31.2566,paint);
    graph->Add_edge_longi_lanti(11,"真如",121.4137,31.2566,"上海西站",121.4074,31.2686,paint);
    graph->Add_edge_longi_lanti(11,"上海西站",121.4074,31.2686,"李子园",121.3966,31.2747,paint);
    graph->Add_edge_longi_lanti(11,"李子园",121.3966,31.2747,"祁连山路",121.3824,31.2772,paint);
    graph->Add_edge_longi_lanti(11,"祁连山路",121.3824,31.2772,"武威路",121.3714,31.2825,paint);
    graph->Add_edge_longi_lanti(11,"武威路",121.3714,31.2825,"桃浦新村",121.3560,31.2875,paint);
    graph->Add_edge_longi_lanti(11,"桃浦新村",121.3560,31.2875,"南翔",121.3297,31.3033,paint);
    graph->Add_edge_longi_lanti(11,"南翔",121.3297,31.3033,"马陆",121.2836,31.3254,paint);
    graph->Add_edge_longi_lanti(11,"马陆",121.2836,31.3254,"嘉定新城",121.2610,31.3357,paint);
    graph->Add_edge_longi_lanti(11,"嘉定新城",121.2610,31.3357,"白银路",121.2519,31.3511,paint);
    graph->Add_edge_longi_lanti(11,"白银路",121.2519,31.3511,"嘉定西",121.2344,31.3832,paint);
    graph->Add_edge_longi_lanti(11,"嘉定西",121.2344,31.3832,"嘉定北",121.2440,31.3974,paint);
    graph->Add_edge_longi_lanti(11,"嘉定新城",121.2610,31.3357,"上海赛车场",121.2326,31.3379,paint);
    graph->Add_edge_longi_lanti(11,"上海赛车场",121.2326,31.3379,"昌吉东路",121.2068,31.2999,paint);
    graph->Add_edge_longi_lanti(11,"昌吉东路",121.2068,31.2999,"上海汽车城",121.1872,31.2915,paint);
    graph->Add_edge_longi_lanti(11,"上海汽车城",121.1872,31.2915,"安亭",121.1686,31.2943,paint);
    graph->Add_edge_longi_lanti(11,"安亭",121.1686,31.2943,"兆丰路",121.1568,31.2946,paint);
    graph->Add_edge_longi_lanti(11,"兆丰路",121.1568,31.2946,"光明路",121.1239,31.3021,paint);
    graph->Add_edge_longi_lanti(11,"光明路",121.1239,31.3021,"花桥",121.1107,31.3048,paint);

    //12号线
    graph->Add_edge_longi_lanti(12,"七莘路",121.3697,31.1376,"虹莘路",121.3867,31.1430,paint);
    graph->Add_edge_longi_lanti(12,"虹莘路",121.3867,31.1430,"顾戴路",121.3989,31.1466,paint);
    graph->Add_edge_longi_lanti(12,"顾戴路",121.3989,31.1466,"东兰路",121.3985,31.1614,paint);
    graph->Add_edge_longi_lanti(12,"东兰路",121.3985,31.1614,"虹梅路",121.4039,31.1660,paint);
    graph->Add_edge_longi_lanti(12,"虹梅路",121.4039,31.1660,"虹漕路",121.4170,31.1701,paint);
    graph->Add_edge_longi_lanti(12,"虹漕路",121.4170,31.1701,"桂林公园",121.4257,31.1731,paint);
    graph->Add_edge_longi_lanti(12,"桂林公园",121.4257,31.1731,"漕宝路",121.4404,31.1748,paint);
    graph->Add_edge_longi_lanti(12,"漕宝路",121.4404,31.1748,"龙漕路",121.4504,31.1761,paint);
    graph->Add_edge_longi_lanti(12,"龙漕路",121.4504,31.1761,"龙华",121.4593,31.1790,paint);
    graph->Add_edge_longi_lanti(12,"龙华",121.4593,31.1790,"龙华中路",121.4632,31.1906,paint);
    graph->Add_edge_longi_lanti(12,"龙华中路",121.4632,31.1906,"大木桥路",121.4701,31.1998,paint);
    graph->Add_edge_longi_lanti(12,"大木桥路",121.4701,31.1998,"嘉善路",121.4675,31.2085,paint);
    graph->Add_edge_longi_lanti(12,"嘉善路",121.4675,31.2085,"陕西南路",121.4656,31.2220,paint);
    graph->Add_edge_longi_lanti(12,"陕西南路",121.4656,31.2220,"南京西路",121.4659,31.2341,paint);
    graph->Add_edge_longi_lanti(12,"南京西路",121.4659,31.2341,"汉中路",121.4652,31.2472,paint);
    graph->Add_edge_longi_lanti(12,"汉中路",121.4652,31.2472,"曲阜路",121.4781,31.2483,paint);
    graph->Add_edge_longi_lanti(12,"曲阜路",121.4781,31.2483,"天潼路",121.4887,31.2500,paint);
    graph->Add_edge_longi_lanti(12,"天潼路",121.4887,31.2500,"国际客运中心",121.5048,31.2558,paint);
    graph->Add_edge_longi_lanti(12,"国际客运中心",121.5048,31.2558,"提篮桥",121.5132,31.2591,paint);
    graph->Add_edge_longi_lanti(12,"提篮桥",121.5132,31.2591,"大连路",121.5194,31.2639,paint);
    graph->Add_edge_longi_lanti(12,"大连路",121.5194,31.2639,"江浦公园",121.5302,31.2703,paint);
    graph->Add_edge_longi_lanti(12,"江浦公园",121.5302,31.2703,"宁国路",121.5389,31.2745,paint);
    graph->Add_edge_longi_lanti(12,"宁国路",121.5389,31.2745,"隆昌路",121.5508,31.2813,paint);
    graph->Add_edge_longi_lanti(12,"隆昌路",121.5508,31.2813,"爱国路",121.5594,31.2863,paint);
    graph->Add_edge_longi_lanti(12,"爱国路",121.5594,31.2863,"复兴岛",121.5675,31.2871,paint);
    graph->Add_edge_longi_lanti(12,"复兴岛",121.5675,31.2871,"东陆路",121.5857,31.2886,paint);
    graph->Add_edge_longi_lanti(12,"东陆路",121.5857,31.2886,"巨峰路",121.5955,31.2862,paint);
    graph->Add_edge_longi_lanti(12,"巨峰路",121.5955,31.2862,"杨高北路",121.6094,31.2858,paint);
    graph->Add_edge_longi_lanti(12,"杨高北路",121.6094,31.2858,"金京路",121.6222,31.2854,paint);
    graph->Add_edge_longi_lanti(12,"金京路",121.6222,31.2854,"申江路",121.6337,31.2858,paint);
    graph->Add_edge_longi_lanti(12,"申江路",121.6337,31.2858,"金海路",121.6453,31.2692,paint);

    //13号线
    graph->Add_edge_longi_lanti(13,"金运路",121.3260,31.2473,"金沙江西路",121.3410,31.2473,paint);
    graph->Add_edge_longi_lanti(13,"金沙江西路",121.3410,31.2473,"丰庄",121.3618,31.2483,paint);
    graph->Add_edge_longi_lanti(13,"丰庄",121.3618,31.2483,"祁连山南路",121.3739,31.2432,paint);
    graph->Add_edge_longi_lanti(13,"祁连山南路",121.3739,31.2432,"真北路",121.3885,31.2379,paint);
    graph->Add_edge_longi_lanti(13,"真北路",121.3885,31.2379,"大渡河路",121.4012,31.2375,paint);
    graph->Add_edge_longi_lanti(13,"大渡河路",121.4012,31.2375,"金沙江路",121.4193,31.2379,paint);
    graph->Add_edge_longi_lanti(13,"金沙江路",121.4193,31.2379,"隆德路",121.4300,31.2367,paint);
    graph->Add_edge_longi_lanti(13,"隆德路",121.4300,31.2367,"武宁路",121.4370,31.2406,paint);
    graph->Add_edge_longi_lanti(13,"武宁路",121.4370,31.2406,"长寿路",121.4447,31.2467,paint);
    graph->Add_edge_longi_lanti(13,"长寿路",121.4447,31.2467,"江宁路",121.4510,31.2507,paint);
    graph->Add_edge_longi_lanti(13,"江宁路",121.4510,31.2507,"汉中路",121.4652,31.2472,paint);
    graph->Add_edge_longi_lanti(13,"汉中路",121.4652,31.2472,"自然博物馆",121.4689,31.2424,paint);
    graph->Add_edge_longi_lanti(13,"自然博物馆",121.4689,31.2424,"南京西路",121.4659,31.2341,paint);
    graph->Add_edge_longi_lanti(13,"南京西路",121.4659,31.2341,"淮海中路",121.4706,31.2261,paint);
    graph->Add_edge_longi_lanti(13,"淮海中路",121.4706,31.2261,"新天地",121.4817,31.2220,paint);
    graph->Add_edge_longi_lanti(13,"新天地",121.4817,31.2220,"马当路",121.4833,31.2156,paint);
    graph->Add_edge_longi_lanti(13,"马当路",121.4833,31.2156,"世博会博物馆",121.4882,31.2031,paint);
    graph->Add_edge_longi_lanti(13,"世博会博物馆",121.4882,31.2031,"世博大道",121.4908,31.1885,paint);
    graph->Add_edge_longi_lanti(13,"世博大道",121.4908,31.1885,"长清路",121.4926,31.1803,paint);
    graph->Add_edge_longi_lanti(13,"长清路",121.4926,31.1803,"成山路",121.5033,31.1766,paint);
    graph->Add_edge_longi_lanti(13,"成山路",121.5033,31.1766,"东明路",121.5179,31.1784,paint);
    graph->Add_edge_longi_lanti(13,"东明路",121.5179,31.1784,"华鹏路",121.5331,31.1823,paint);
    graph->Add_edge_longi_lanti(13,"华鹏路",121.5331,31.1823,"下南路",121.5467,31.1854,paint);
    graph->Add_edge_longi_lanti(13,"下南路",121.5467,31.1854,"北蔡",121.5587,31.1864,paint);
    graph->Add_edge_longi_lanti(13,"北蔡",121.5587,31.1864,"陈春路",121.5640,31.1813,paint);
    graph->Add_edge_longi_lanti(13,"陈春路",121.5640,31.1813,"莲溪路",121.5728,31.1753,paint);
    graph->Add_edge_longi_lanti(13,"莲溪路",121.5728,31.1753,"华夏中路",121.5887,31.1812,paint);
    graph->Add_edge_longi_lanti(13,"华夏中路",121.5887,31.1812,"中科路",121.6086,31.1846,paint);
    graph->Add_edge_longi_lanti(13,"中科路",121.6086,31.1846,"学林路",121.6210,31.1891,paint);
    graph->Add_edge_longi_lanti(13,"学林路",121.6210,31.1891,"张江路",121.6360,31.1947,paint);

    //16号线
    graph->Add_edge_longi_lanti(16,"龙阳路",121.5639,31.2091,"华夏中路",121.5887,31.1812,paint);
    graph->Add_edge_longi_lanti(16,"华夏中路",121.5887,31.1812,"罗山路",121.5996,31.1593,paint);
    graph->Add_edge_longi_lanti(16,"罗山路",121.5996,31.1593,"周浦东",121.6136,31.1156,paint);
    graph->Add_edge_longi_lanti(16,"周浦东",121.6136,31.1156,"鹤沙航城",121.6178,31.0836,paint);
    graph->Add_edge_longi_lanti(16,"鹤沙航城",121.6178,31.0836,"航头东",121.6241,31.0605,paint);
    graph->Add_edge_longi_lanti(16,"航头东",121.6241,31.0605,"新场",121.6555,31.0515,paint);
    graph->Add_edge_longi_lanti(16,"新场",121.6555,31.0515,"野生动物园",121.7057,31.0564,paint);
    graph->Add_edge_longi_lanti(16,"野生动物园",121.7057,31.0564,"惠南",121.7682,31.0596,paint);
    graph->Add_edge_longi_lanti(16,"惠南",121.7682,31.0596,"惠南东",121.8002,31.0328,paint);
    graph->Add_edge_longi_lanti(16,"惠南东",121.8002,31.0328,"书院",121.8572,30.9648,paint);
    graph->Add_edge_longi_lanti(16,"书院",121.8572,30.9648,"临港大道",121.9172,30.9298,paint);
    graph->Add_edge_longi_lanti(16,"临港大道",121.9172,30.9298,"滴水湖",121.9363,30.9132,paint);

    //17号线
    graph->Add_edge_longi_lanti(17,"虹桥火车站",121.3279,31.2003,"诸光路",121.2996,31.1978,paint);
    graph->Add_edge_longi_lanti(17,"诸光路",121.2996,31.1978,"蟠龙路",121.2851,31.1924,paint);
    graph->Add_edge_longi_lanti(17,"蟠龙路",121.2851,31.1924,"徐盈路",121.2606,31.1839,paint);
    graph->Add_edge_longi_lanti(17,"徐盈路",121.2606,31.1839,"徐泾北城",121.2482,31.1812,paint);
    graph->Add_edge_longi_lanti(17,"徐泾北城",121.2482,31.1812,"嘉松中路",121.2305,31.1701,paint);
    graph->Add_edge_longi_lanti(17,"嘉松中路",121.2305,31.1701,"赵巷",121.1987,31.1675,paint);
    graph->Add_edge_longi_lanti(17,"赵巷",121.1987,31.1675,"汇金路",121.1583,31.1669,paint);
    graph->Add_edge_longi_lanti(17,"汇金路",121.1583,31.1669,"青浦新城",121.1319,31.1645,paint);
    graph->Add_edge_longi_lanti(17,"青浦新城",121.1319,31.1645,"漕盈路",121.1031,31.1666,paint);
    graph->Add_edge_longi_lanti(17,"漕盈路",121.1031,31.1666,"淀山湖大道",121.0884,31.1407,paint);
    graph->Add_edge_longi_lanti(17,"淀山湖大道",121.0884,31.1407,"朱家角",121.0555,31.1065,paint);
    graph->Add_edge_longi_lanti(17,"朱家角",121.0555,31.1065,"东方绿舟",121.0262,31.1041,paint);

}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(UI_Width,UI_Height);
    setFixedSize(UI_Width,UI_Height);
    setWindowIcon(QIcon(":/Metro.ico"));     //为主窗口设立图标
    value_factor(1.0);

    ui->scrollArea->setMouseTracking(true);  //设置滚动区域 鼠标位置追踪

    label=new QLabel(this);

    text=new QTextEdit(this);                //初始化文本框
    text->setReadOnly(true);
    text->setText("");
    text->hide();

    query_remind=new QTextEdit(this);        //初始化查询提示框
    query_remind->setReadOnly(true);
    query_remind->setText("");
    query_remind->hide();

    in_query=false;                          //初始化查询状态

    pixmap=QPixmap(Metro_Width,Metro_Height);
    graph=new Graph();
    Initial();
    ui->scrollArea->setWidget(label);
    label->setPixmap(pixmap);


    //为地铁按键设置地铁线路的颜色
    ui->pushButton_1->setStyleSheet("color:rgb(153,0,255)");
    ui->pushButton_2->setStyleSheet("color:rgb(255,0,0)");
    ui->pushButton_3->setStyleSheet("color:rgb(255,153,0)");
    ui->pushButton_4->setStyleSheet("color:rgb(255,204,255)");
    ui->pushButton_5->setStyleSheet("color:rgb(204,0,204)");
    ui->pushButton_6->setStyleSheet("color:rgb(255,255,204)");
    ui->pushButton_7->setStyleSheet("color:rgb(153,255,102)");
    ui->pushButton_8->setStyleSheet("color:rgb(0,153,0)");
    ui->pushButton_9->setStyleSheet("color:rgb(0,204,153)");
    ui->pushButton_10->setStyleSheet("color:rgb(204,204,0)");
    ui->pushButton_11->setStyleSheet("color:rgb(0,255,204)");
    ui->pushButton_12->setStyleSheet("color:rgb(255,255,0)");
    ui->pushButton_13->setStyleSheet("color:rgb(255,102,153)");
    ui->pushButton_14->setStyleSheet("color:rgb(153,51,102)");
    ui->pushButton_15->setStyleSheet("color:rgb(128,128,0)");
    ui->pushButton_16->setStyleSheet("color:rgb(102,102,153)");
    ui->pushButton_17->setStyleSheet("color:rgb(204,0,0)");
    ui->pushButton_18->setStyleSheet("color:rgb(204,236,255)");
    ui->pushButton_19->setStyleSheet("color:rgb(51,51,204)");
    ui->pushButton_20->setStyleSheet("color:rgb(255,124,128)");



    //地铁线路单独显示的触发
    connect(ui->pushButton_1,&QPushButton::clicked,[=](){
        if(ui->pushButton_1->isChecked()==true){
            Button_Block(1);
            value_button_check(1);
            emit this->Emit_show_Line(1);
        }
        else
            emit this->Recover();
    });
    connect(ui->pushButton_2,&QPushButton::clicked,[=](){
        if(ui->pushButton_2->isChecked()==true){
            Button_Block(2);
            value_button_check(2);
            emit this->Emit_show_Line(2);
        }
        else
            emit this->Recover();
    });
    connect(ui->pushButton_3,&QPushButton::clicked,[=](){
        if(ui->pushButton_3->isChecked()==true){
            Button_Block(3);
            value_button_check(3);
            emit this->Emit_show_Line(3);
        }
        else
            emit this->Recover();
    });
    connect(ui->pushButton_4,&QPushButton::clicked,[=](){
        if(ui->pushButton_4->isChecked()==true){
            Button_Block(4);
            value_button_check(4);
            emit this->Emit_show_Line(4);
        }
        else
            emit this->Recover();
    });
    connect(ui->pushButton_5,&QPushButton::clicked,[=](){
        if(ui->pushButton_5->isChecked()==true){
            Button_Block(5);
            value_button_check(5);
            emit this->Emit_show_Line(5);
        }
        else
            emit this->Recover();
    });
    connect(ui->pushButton_6,&QPushButton::clicked,[=](){
        if(ui->pushButton_6->isChecked()==true){
            Button_Block(6);
            value_button_check(6);
            emit this->Emit_show_Line(6);
        }
        else
            emit this->Recover();
    });
    connect(ui->pushButton_7,&QPushButton::clicked,[=](){
        if(ui->pushButton_7->isChecked()==true){
            Button_Block(7);
            value_button_check(7);
            emit this->Emit_show_Line(7);
        }
        else
            emit this->Recover();
    });
    connect(ui->pushButton_8,&QPushButton::clicked,[=](){
        if(ui->pushButton_8->isChecked()==true){
            Button_Block(8);
            value_button_check(8);
            emit this->Emit_show_Line(8);
        }
        else
            emit this->Recover();
    });
    connect(ui->pushButton_9,&QPushButton::clicked,[=](){
        if(ui->pushButton_9->isChecked()==true){
            Button_Block(9);
            value_button_check(9);
            emit this->Emit_show_Line(9);
        }
        else
            emit this->Recover();
    });
    connect(ui->pushButton_10,&QPushButton::clicked,[=](){
        if(ui->pushButton_10->isChecked()==true){
            Button_Block(10);
            value_button_check(10);
            emit this->Emit_show_Line(10);
        }
        else
            emit this->Recover();
    });
    connect(ui->pushButton_11,&QPushButton::clicked,[=](){
        if(ui->pushButton_11->isChecked()==true){
            Button_Block(11);
            value_button_check(11);
            emit this->Emit_show_Line(11);
        }
        else
            emit this->Recover();
    });
    connect(ui->pushButton_12,&QPushButton::clicked,[=](){
        if(ui->pushButton_12->isChecked()==true){
            Button_Block(12);
            value_button_check(12);
            emit this->Emit_show_Line(12);
        }
        else
            emit this->Recover();
    });
    connect(ui->pushButton_13,&QPushButton::clicked,[=](){
        if(ui->pushButton_13->isChecked()==true){
            Button_Block(13);
            value_button_check(13);
            emit this->Emit_show_Line(13);
        }
        else
            emit this->Recover();
    });
    connect(ui->pushButton_14,&QPushButton::clicked,[=](){
        if(ui->pushButton_14->isChecked()==true){
            if(graph->Subway[14].get_cover_size()==0){
                QMessageBox::information(this,"Remind","This metro line has not yet been established");
                ui->pushButton_14->setChecked(false);
                return;
            }
            Button_Block(14);
            value_button_check(14);
            emit this->Emit_show_Line(14);
        }
        else
            emit this->Recover();
    });
    connect(ui->pushButton_15,&QPushButton::clicked,[=](){
        if(ui->pushButton_15->isChecked()==true){
            if(graph->Subway[15].get_cover_size()==0){
                QMessageBox::information(this,"Remind","This metro line has not yet been established");
                ui->pushButton_15->setChecked(false);
                return;
            }
            Button_Block(15);
            value_button_check(15);
            emit this->Emit_show_Line(15);
        }
        else
            emit this->Recover();
    });
    connect(ui->pushButton_16,&QPushButton::clicked,[=](){
        if(ui->pushButton_16->isChecked()==true){
            Button_Block(16);
            value_button_check(16);
            emit this->Emit_show_Line(16);
        }
        else
            emit this->Recover();
    });
    connect(ui->pushButton_17,&QPushButton::clicked,[=](){
        if(ui->pushButton_17->isChecked()==true){
            Button_Block(17);
            value_button_check(17);
            emit this->Emit_show_Line(17);
        }
        else
            emit this->Recover();
    });
    connect(ui->pushButton_18,&QPushButton::clicked,[=](){
        if(ui->pushButton_18->isChecked()==true){
            if(graph->Subway[18].get_cover_size()==0){
                QMessageBox::information(this,"Remind","This metro line has not yet been established");
                ui->pushButton_18->setChecked(false);
                return;
            }
            Button_Block(18);
            value_button_check(18);
            emit this->Emit_show_Line(18);
        }
        else
            emit this->Recover();
    });
    connect(ui->pushButton_19,&QPushButton::clicked,[=](){
        if(ui->pushButton_19->isChecked()==true){
            if(graph->Subway[19].get_cover_size()==0){
                QMessageBox::information(this,"Remind","This metro line has not yet been established");
                ui->pushButton_19->setChecked(false);
                return;
            }
            Button_Block(19);
            value_button_check(19);
            emit this->Emit_show_Line(19);
        }
        else
            emit this->Recover();
    });
    connect(ui->pushButton_20,&QPushButton::clicked,[=](){
        if(ui->pushButton_20->isChecked()==true){
            if(graph->Subway[20].get_cover_size()==0){
                QMessageBox::information(this,"Remind","This metro line has not yet been established");
                ui->pushButton_20->setChecked(false);
                return;
            }
            Button_Block(20);
            value_button_check(20);
            emit this->Emit_show_Line(20);
        }
        else
            emit this->Recover();
    });

    connect(this,&MainWindow::Emit_show_Line,this,&MainWindow::show_Line);
    connect(this,&MainWindow::Recover,this,&MainWindow::recover);

    //菜单栏添加按钮的处理
    QMenuBar *bar=menuBar();
    setMenuBar(bar);
    QMenu *Add=bar->addMenu("Add");
    QMenu *Scale=bar->addMenu("Scaling");
    QMenu *Query=bar->addMenu("Query");
    QMenu *Help=bar->addMenu("Help");

    QAction *add1=Add->addAction("name");
    QAction *add2=Add->addAction("longitude-lantitude");
    add1->setShortcut(QKeySequence("Ctrl+N"));
    add2->setShortcut(QKeySequence("Ctrl+L"));

    connect(add1,&QAction::triggered,[=](){  //处理用名字添加连线的方式
        if(in_query==true || Check_button()==Wrong){
            QMessageBox::critical(this,"Wrong","Please go back to the initial surface");
            return;
        }
        else{
            bool ok1,ok2,ok3;
            int line;
            QString name1,name2;
            QInputDialog *input1=new QInputDialog(this);
            line=input1->getInt(this,"Line_Input","Please input the index of the metro",1,1,20,1,&ok1);
            if(ok1==true){
                QInputDialog *input2=new QInputDialog(this);
                name1=input2->getText(this,"Name_Input","Please input the Source Station's Name",QLineEdit::Normal,"",&ok2);
            }
            if(ok2==true){
                QInputDialog *input3=new QInputDialog(this);
                name2=input3->getText(this,"Name_Input","Please input the Destination Station's Name",QLineEdit::Normal,"",&ok3);
            }
            if(ok1 && ok2 && ok3){
                if(graph->Add_edge_name(line,name1,name2,paint,factor)==Wrong)
                    QMessageBox::critical(this,"Wrong","You have inputed the wrong information, please retry");
                else{
                    ui->scrollArea->setWidget(label);
                    label->setPixmap(pixmap);
                }
            }
        }
    });

    connect(add2,&QAction::triggered,[=](){  //处理用经纬度添加连线的方式
        if(in_query==true || Check_button()==Wrong){
            QMessageBox::critical(this,"Wrong","Please go back to the initial surface");
            return;
        }
        else{
            QInputDialog *input=new QInputDialog(this);
            bool ok;
            QString string_=input->getMultiLineText(this,"Input","please enter the information line by line as following:\n7号线\n莘庄\n经度:121.3921\n纬度:31.1168\n锦江乐园\n经度:121.4206\n纬度:31.1480","7号线\n莘庄\n经度:121.3921\n纬度:31.1168\n锦江乐园\n经度:121.4206\n纬度:31.1480",&ok);
            if(ok==true){
                int line;
                QString name1,name2;
                double long1,lant1,long2,lant2;
                if(Infor_solve(string_,line,name1,long1,lant1,name2,long2,lant2)==Right_){
                    if(graph->Add_edge_longi_lanti(line,name1,long1,lant1,name2,long2,lant2,paint,factor)==Right_){
                        ui->scrollArea->setWidget(label);
                        label->setPixmap(pixmap);
                    }
                    else
                        QMessageBox::critical(this,"Wrong","You have inputed the wrong information, please retry");
                }
                else
                    QMessageBox::critical(this,"Wrong","You have inputed the wrong information, please retry");
            }
        }
    });

    //处理按键控制放大缩小
    QAction *scale1=Scale->addAction("Scale up");
    QAction *scale2=Scale->addAction("Scale down");
    scale1->setShortcut(QKeySequence("ctrl+U"));
    scale2->setShortcut(QKeySequence("ctrl+D"));

    connect(scale1,&QAction::triggered,[=](){
        emit Scaling(Bigger);
    });
    connect(scale2,&QAction::triggered,[=](){
        emit Scaling(Smaller);
    });

    connect(this,&MainWindow::Scaling,this,&MainWindow::Line_Scaling);


    //处理查询状态的信息
    QAction *query1=Query->addAction("enter query condition");
    QAction *query2=Query->addAction("exit query condition");
    query1->setShortcut(QKeySequence("ctrl+Q"));
    query2->setShortcut(QKeySequence("ctrl+E"));

    connect(query1,&QAction::triggered,[=](){
        if(Check_button()==Wrong){
            QMessageBox::critical(this,"Wrong","Please go back to the initial surface");
            return;
        }
        QString name1,name2;
        QInputDialog *input1=new QInputDialog(this);
        bool ok1,ok2;
        name1=input1->getText(this,"Query_src","Please input the name of origin station",QLineEdit::Normal,"安亭",&ok1);
        if(ok1==true){
            QInputDialog *input2=new QInputDialog(this);
            name2=input2->getText(this,"Query_dst","Please input the name of terminal station",QLineEdit::Normal,"同济大学",&ok2);
        }
        if(ok1 && ok2){
            int id1,id2;
            id1=graph->Find_id_name(name1);
            id2=graph->Find_id_name(name2);
            if(id1==Wrong || id2==Wrong || id1==id2)
                QMessageBox::critical(this,"Wrong","You have input the Wrong information,please retry");
            else{
                in_query=true;               //将主窗口设置为查询状态
                if(graph->dij_preprocess(id1,id2)==Wrong){
                    QMessageBox::information(this,"Wrong","Can't Find the path");
                    return;
                }
                QString show_info;
                graph->dij_postprocess(show_info,id1,id2);
                query_remind->setText(show_info);
                query_remind->setGeometry(0,50,UI_Width,query_height);
                query_remind->show();
                query_show_line();
            }
        }
    });

    connect(query2,&QAction::triggered,[=](){
        if(in_query==false){
            QMessageBox::critical(this,"Wrong","You have not entered query condition");
            return;
        }
        in_query=false;
        query_remind->hide();
        recover();
    });

    //处理帮助
    QAction *help=Help->addAction("obtain attention");
    help->setShortcut(QKeySequence("ctrl+H"));
    connect(help,&QAction::triggered,[=](){
        QMessageBox::information(this,"Attention","This procedure is used to show how to switch from the source to the destination.It is designed by QT 5.9.1 whose author's student ID is 1853557.In addition,you can use the mouse wheel to do scaling out of the scroll area.");
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
