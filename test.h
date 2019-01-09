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

#define DRAW_TEXTRECT_WIDTH	     33		//����rect_text��������ʱ�ľ�����  //33
#define DRAW_TEXTRECT_HEIGHT     16		//����rect_text��������ʱ�ľ���߶�
#define DRAW_LINE_WIDTH          10		//�̶�������	
#define PIX_INTERVAL             28	    //ÿ���̶���֮���������Ŀ
#define ONE_NUM_PIX              0.28   //ÿ����һ�������������ֵ
#define DRAW_TEXTRECT_X          12		//����rect_text��������ʱ�ľ�������Ͻ� x����
#define DRAW_TEXTRECT_Y          -8	    //����rect_text��������ʱ�ľ�������Ͻ� y����
#define BACKGROUND_RECT_X         3     //���������x����
#define BACKGROUND_RECT_Y        39     //���������y����
#define BACKGROUND_RECT_WIDTH    46	    //��������Ŀ��		//46
#define BACKGROUND_RECT_HEIGHT   224    //��������ĸ߶�
#define CENTER_POINT_Y           149	//���ĵ�y����
#define CENTER_POINT_X           0		//���ĵ�x����
#define LOOP_NUM                 5		//�����ı�ʱ��ѭ������

#define MAX_HEIGHT_NUM           90000  //�ɻ��߶������ֵ
#define MIN_HEIGHT_NUM           -8000  //�ɻ��߶���С��ֵ

#define HEIBOX_NUMRECT_WID       9      //�ڸ߶ȱ�Ŀ��л���ÿ�����ֵľ��εĿ��
#define HEIBOX_NUMRECT_HEI       16     //�ڸ߶ȱ��еĿ��л���ÿ�����ֵľ��εĸ߶�
#define HEIBOX_NUMRECT_Y         2      //�߶ȱ���е��ı����Ƶ���Ӿ��ε�y����
#define ROLL_NUM_PIX             0.8    //ÿ20֮���ÿ����ֵ������ƫ����   

#define test 5						//����ֵ
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

	//����ṹ�����ڱ�����Ƹ߶���ʾ���ʱ��Ļ��ʵ�ѡ�
	typedef struct
	{
		QTextOption option;		//���ֱ���
		QPen pen;               //��������
		QFont font;             //�ı���ʽ
		QFont font1;            //��λ��ʮλ�������ʽ
		QBrush brush;           //��ˢ����
		QRectF rectf;           //���û���ֻ�ܻ��Ƶľ�������
		QRectF num_rectf;       //���Ƹ߶ȱ���е����ֵ�ʱ��
		QRectF townum_rect;	    //����ʮλ�͸�λ�ľ���

		QString box_num_string; //�����ڸ߶ȱ���е�ʮλ��ֵ���ַ���
		double offset_pix;       //����ÿ����һ����ֵ��λ������ƫ����
		bool negative;		    //�Ƿ�С��0��Ϊ����

	} box_draw_option;
	//������	
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