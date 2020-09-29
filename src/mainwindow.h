#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QPixmap>
#include <QMainWindow>
#include <QLabel>
#include <QIcon>
#include <QMessageBox>
#include <QAbstractButton>
#include <QKeySequence>
#include <QInputDialog>
#include <QTextBrowser>
#include <math.h>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QScrollBar>
#include <QPainter>

#define Metro_Width 11000          //定义地铁布局的窗口
#define Metro_Height 6000
#define UI_Width 1600              //定义程序界面的大小
#define UI_Height 800
#define query_height 50           //定义查询栏的高度
#define Scale_factor 0.1
#define Bigger 1
#define Smaller 0

class Graph;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QPixmap pixmap;
    QLabel *label;
    QPainter paint;
    Graph *graph;
    double factor;        //用于决定图形区域的缩放程度
    int button_check;     //确定是否存在某个按键被按下
    QTextEdit *text;      //用于为地铁站点显示浮窗
    QTextEdit *query_remind;   //查询时的换乘提示框
    bool in_query;             //在查询状态中
    int click_x;               //记录鼠标点击的位置
    int click_y;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void Initial();
    void Button_Block(const int num);                 //当按下某按键时，自动屏蔽其他按键
    int Check_button();                               //是否有按键仍处于check状态
    int Infor_solve(QString src_string,int &Line,QString &name1,double &long1,double &lant1,QString &name2,double &long2,double &lant2);  //将输入的信息处理为数值
    int get_button_check();                           //返回已被按下的按键值
    void value_button_check(const int num);           //为button赋值
    void value_factor(const double num);              //为factor赋值
    double get_factor();                              //返回此时的缩放值

    //定义槽函数
    void show_Line(const int Line);                   //显示某条特定地铁线路的槽函数
    void query_show_line();                           //显示查询的线路
    void recover();                                   //复原地铁线路图
    void wheelEvent(QWheelEvent *event);              //设置滚轮事件——滚轮能进行缩放
    void Line_Scaling(const int condition);           //完成线路图缩放事件
    void mousePressEvent(QMouseEvent *event);         //捕捉地铁站点按下事件
    void mouseMoveEvent(QMouseEvent *event);

signals:
    void Emit_show_Line(const int Line);                  //显示特定线路的触发信号
    void Recover();                                       //复原地铁线路图的触发信号
    void Scaling(const int condition);                    //地铁线路图缩放的信号

private:
    Ui::MainWindow *ui;
};



#endif // MAINWINDOW_H
