#ifndef HEIGHT_METER_H
#define HEIGHT_METER_H

#include <QWidget>
#include <QDebug>
#include <QtCore/qmath.h>
#include <QPainter>
#include <QPaintEvent>
#include <QPalette>
#include <QPoint>
#include <QFont>
#include <QRect>
#include <QTextOption>
#include <QPushButton>
#include <QLabel>
#include <QEvent>
#include <QMouseEvent>
#include <QBrush>
#include <QPen>
#include <QKeyEvent>
#include <QVector>

#define DRAW_TEXTRECT_WIDTH	     33		//利用rect_text绘制数字时的矩阵宽度  //33
#define DRAW_TEXTRECT_HEIGHT     16		//利用rect_text绘制数字时的矩阵高度
#define DRAW_LINE_WIDTH          10		//刻度条长度	
#define PIX_INTERVAL             28	    //每个刻度条之间的像素数目
#define ONE_NUM_PIX              0.28   //每升高一米所代表的像素值
#define DRAW_TEXTRECT_X          12		//利用rect_text绘制数字时的矩阵的左上角 x坐标
#define DRAW_TEXTRECT_Y          -8	    //利用rect_text绘制数字时的矩阵的左上角 y坐标
#define BACKGROUND_RECT_X         3     //背景矩阵的x坐标
#define BACKGROUND_RECT_Y        39     //背景矩阵的y坐标
#define BACKGROUND_RECT_WIDTH    46	    //背景矩阵的宽度		//46
#define BACKGROUND_RECT_HEIGHT   224    //背景矩阵的高度
#define CENTER_POINT_Y           149	//中心点y坐标
#define CENTER_POINT_X           0		//中心点x坐标
#define LOOP_NUM                 5		//绘制文本时的循环次数

#define MAX_HEIGHT_NUM           90000  //飞机高度最大数值
#define MIN_HEIGHT_NUM           -8000  //飞机高度最小数值

#define HEIBOX_NUMRECT_WID       9      //在高度表的框中绘制每个数字的矩形的宽度
#define HEIBOX_NUMRECT_HEI       16     //在高度表中的框中绘制每个数字的矩形的高度
#define HEIBOX_NUMRECT_Y         2      //高度表框中的文本绘制的外接矩形的y坐标
#define ROLL_NUM_PIX             0.8    //每20之间的每个数值的像素偏移量   

#define test 5						//测试值
#define step 1

class Height_meter : public QWidget
{
	Q_OBJECT

public:
	Height_meter(QWidget *parent = 0);
	~Height_meter();

	void TEST();
	void initUI();
public slots:
	void test_add();
	void test_sub();
protected:
	void paintEvent(QPaintEvent* event);
	void initLayout();
	inline void drawHeightmark(QPainter* &);
	void draw_Line_NumText(QPainter* &, int&, QTextOption&);
	void judge_currentheight(int&);

	inline void init_height_mark();
	void init_height_box();

	inline void draw_height_box(QPainter* &);
	inline void draw_height_box_text(QPainter* &);
	inline void draw_wan_bit(int, double, QPainter*&);
	inline void draw_qian_bit(int, double, QPainter*&);
	inline void draw_bai_bit(int&, double, QPainter*&);
	inline void draw_shi_bit(int, double, QPainter*&);
	inline void judge_toZero(int&);

	void Translate_y(double, QPainter*&);
private:
	//Ui::Height_meterClass ui;
	double current_airHeight;

	//这个结构体用于保存绘制高度显示框的时候的画笔等选项。
	typedef struct
	{
		QTextOption option;		//文字编排
		QPen pen;               //画笔设置
		QFont font;             //文本格式
		QFont font1;            //百位和十位的字体格式
		QBrush brush;           //画刷设置
		QRectF rectf;           //设置画笔只能绘制的矩形区域
		QRectF num_rectf;       //绘制高度表框中的文字的时候
		QRectF townum_rect;	    //绘制十位和个位的矩形

		QString box_num_string; //保存在高度表框中的十位数值的字符串
		double offset_pix;       //对于每增加一个数值单位的像素偏移量
		bool negative;		    //是否小于0，为负数

	} box_draw_option;
	//测试用	
	QLabel* label;
	QPushButton* btn;
	QPushButton* btn1;
	//

	QPointF center_point;
	QFont mark_text_font;

	QRectF background_rectf;
	QColor white_color;
	QColor background_color;

	QPainterPath height_box;
	box_draw_option box_option;

	QPainterPath* height_mark;
	QPainterPath temp_height_mark;

};

#endif // HEIGHT_METER_H