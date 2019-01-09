#pragma once

#include "test.h"

Height_meter::Height_meter(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);
	initUI();
	height_mark = NULL;
}
void Height_meter::initUI()
{
	initLayout();

	TEST();

	update();
}
void Height_meter::TEST()
{
	btn = new QPushButton(this);
	btn->move(0, 0);
	btn->resize(40, 20);
	btn->setText("add");
	btn->setAutoRepeatDelay(10);
	btn->setAutoRepeat(true);
	connect(btn, SIGNAL(clicked()), this, SLOT(test_add()));

	btn1 = new QPushButton(this);
	btn1->move(42, 0);
	btn1->resize(40, 20);
	btn1->setText("sub");
	btn1->setAutoRepeatDelay(10);
	btn1->setAutoRepeat(true);
	connect(btn1, SIGNAL(clicked()), this, SLOT(test_sub()));

	label = new QLabel(this);
	label->move(0, 22);
	label->resize(50, 22);
}
/***************************
Function：
Description：
Calls:
Called By:
Table Accessed:
Table Updated:
Input:
Output：
Return:
Other:
****************************/
void Height_meter::initLayout()
{
	current_airHeight = test;//test
	background_rectf = QRectF(BACKGROUND_RECT_X, BACKGROUND_RECT_Y,
		BACKGROUND_RECT_WIDTH, BACKGROUND_RECT_HEIGHT);

	//设置字体
	mark_text_font.setFamily(QString::fromUtf8("\347\255\211\347\272\277 Light"));
	mark_text_font.setPixelSize(11);

	//中心点设置
	center_point = QPointF(CENTER_POINT_X, CENTER_POINT_Y);
	background_color = QColor(99, 90, 90, 255);
	white_color = QColor(255, 255, 255, 255);

	init_height_box();
}

void Height_meter::paintEvent(QPaintEvent* event)
{
	QPainter* p = new QPainter(this);

	//反走样 技术
	p->setRenderHint(QPainter::Antialiasing);
	//初始化高度背景图，刻度图
	init_height_mark();

	p->setWindow(0, 0, 64, 282);

	if (p != NULL)
	{
		drawHeightmark(p);
		draw_height_box(p);
	}
	else
	{
		delete p;
	}
}

/***************************
Function：
Description：
Calls:
Called By:
Table Accessed:
Table Updated:
Input:
Output：
Return:
Other:
****************************/
void Height_meter::drawHeightmark(QPainter* & p_painter)
{
	p_painter->save();

	//设置绘图的区域  不怕超出范围
	p_painter->setClipRect(background_rectf);
	p_painter->setPen(background_color);
	p_painter->setBrush(background_color);

	//绘制背景矩形
	p_painter->drawRect(background_rectf);

	//绘制刻度
	QPen tmp_pen(white_color);

	//刻度的线宽
	tmp_pen.setWidthF(0.9);

	p_painter->setPen(tmp_pen);
	p_painter->setFont(mark_text_font);
	//变换到当前空速数字显示窗应该指向的位置，即显示窗固定不变的位置点
	p_painter->translate(center_point);

	//恢复到窗口（0,0）
	p_painter->drawPath(*height_mark);
	p_painter->restore();
}

/***************************
Function：draw_Line_NumText
Description：在相应的位置绘制出线条，在需要绘制文字的地方绘制文字
Calls:
Called By:
Table Accessed:
Table Updated:
Input:
Output：
Return:
Other:
****************************/
void Height_meter::draw_Line_NumText(QPainter* & p_painter, int& num, QTextOption& option)
{
	//如果 是奇数  说明不需要绘制 文字
	//高度最大不能超过50000英尺
	if ((num % 2) == 0)
	{
		QString tmp_string;
		tmp_string.setNum(num * 100);
		p_painter->drawText(QRectF(DRAW_TEXTRECT_X, DRAW_TEXTRECT_Y,
			DRAW_TEXTRECT_WIDTH, DRAW_TEXTRECT_HEIGHT),
			tmp_string, option);
	}
	p_painter->drawLine(0, 0, DRAW_LINE_WIDTH, 0);
}
/***************************
Function：
Description：
Calls:
Called By:
Table Accessed:
Table Updated:
Input:
Output：
Return:
Other:
****************************/
void Height_meter::judge_currentheight(int& height_num)
{
	if (height_num > MAX_HEIGHT_NUM)
	{
		height_num = MAX_HEIGHT_NUM;
	}
	if (height_num < MIN_HEIGHT_NUM)
	{
		height_num = MIN_HEIGHT_NUM;
	}
}
/***************************
Function：
Description：
Calls:
Called By:
Table Accessed:
Table Updated:
Input:
Output：
Return:
Other:
****************************/
void Height_meter::init_height_mark()
{
	QString tmp_string;
	if (height_mark != NULL)
	{
		(*height_mark) -= temp_height_mark;
	}
	else
	{
		height_mark = new QPainterPath();
	}

	int		tmp_int_airHeight = (int)current_airHeight;
	judge_currentheight(tmp_int_airHeight);
	//对飞行高度取余
	double		offset_num = tmp_int_airHeight % 100;
	offset_num += (current_airHeight - tmp_int_airHeight);
	//偏移量乘以像素值，得到应该偏移的像素值
	double		offset_pix = offset_num * ONE_NUM_PIX;
	//将path根据painter的坐标点向下画第一条线
	height_mark->moveTo(0, offset_pix);

	//因为每100高度为一次刻度线
	int begin_num = tmp_int_airHeight / 100;
	//绘制上方线条
	for (int i = 0; i < 6; i++)
	{
		tmp_string.setNum(begin_num * 100);
		if (i != 0)
		{
			height_mark->moveTo(0, -PIX_INTERVAL * i + offset_pix);
			//画刻度线
			height_mark->lineTo(10, -PIX_INTERVAL * i + offset_pix);
			if (begin_num % 2 == 0)
			{
				//加文字
				height_mark->addText(DRAW_TEXTRECT_X, -i * PIX_INTERVAL + offset_pix + 4, mark_text_font, tmp_string);
			}
		}
		else
		{
			//画刻度线
			height_mark->lineTo(10, offset_pix);
			if (begin_num % 2 == 0)
			{
				//写文字
				height_mark->addText(DRAW_TEXTRECT_X, offset_pix + 4, mark_text_font, tmp_string);
			}
		}
		//高度总数目加一
		begin_num++;
	}
	//下方同理  画负的线条
	begin_num = tmp_int_airHeight / 100 - 1;
	//绘制下方线条
	for (int i = 1; i < 5; i++)
	{
		tmp_string.setNum(begin_num * 100);
		height_mark->moveTo(0, offset_pix + PIX_INTERVAL * i);
		height_mark->lineTo(10, offset_pix + PIX_INTERVAL * i);
		if (begin_num % 2 == 0)
		{
			height_mark->addText(DRAW_TEXTRECT_X, i * PIX_INTERVAL + offset_pix + 4, mark_text_font, tmp_string);
		}
		begin_num--;
	}
	height_mark->closeSubpath();
	//将当前的path赋值给这个临时的变量path，以便开始时候再删除
	temp_height_mark = (*height_mark);
}

//对显示框在绘制前进行初始化各种画笔选项
void Height_meter::init_height_box()
{
	box_option.font = mark_text_font;
	box_option.font.setPointSizeF(11.5);

	box_option.font1 = mark_text_font;
	box_option.font1.setPointSizeF(10);

	//在指定的矩形框中 居中对齐
	box_option.option = QTextOption(Qt::AlignRight | Qt::AlignVCenter);

	//绘制 指示的边框和背景色
	box_option.pen = QPen(white_color);
	box_option.pen.setWidthF(1.5);

	box_option.rectf = QRectF(0, -17, 61, 34);
	box_option.num_rectf = QRectF(0, HEIBOX_NUMRECT_Y, 9, HEIBOX_NUMRECT_HEI);
	box_option.townum_rect = QRectF(0, HEIBOX_NUMRECT_Y, 18, HEIBOX_NUMRECT_HEI);
	box_option.brush = QBrush(QColor(0, 0, 0, 255));
	//绘制 路径的初始化		
	//指示空速 中间的那个数字模块  坐标 相对于 center_point
	height_box.moveTo(4, 0);
	height_box.lineTo(13, -4);
	height_box.lineTo(13, -16);
	height_box.lineTo(60, -16);
	height_box.lineTo(60, 16);
	height_box.lineTo(13, 16);
	height_box.lineTo(13, 4);
	height_box.closeSubpath();
}
void Height_meter::draw_height_box(QPainter* & p_painter)
{
	p_painter->save();
	//移动到中心点
	p_painter->translate(center_point);

	p_painter->setClipRect(box_option.rectf);
	p_painter->setFont(box_option.font);

	p_painter->setPen(box_option.pen);								//设置 线的颜色
	p_painter->setBrush(box_option.brush);		                    //设置 填充色
	p_painter->drawPath(height_box);

	//调用图形绘制函数
	draw_height_box_text(p_painter);
}
void Height_meter::draw_height_box_text(QPainter* &p_painter)
{
	p_painter->setClipPath(height_box);
	p_painter->save();
	QString tmp_string;

	//是否为负数
	box_option.negative = (current_airHeight > 0) ? false : true;
	//将数字分离
	int one_bit = qAbs(((int)current_airHeight) % 10);
	int shi_bit = qAbs(((int)current_airHeight / 10) % 10);
	int bai_bit = qAbs(((int)current_airHeight / 100) % 10);
	int qian_bit = qAbs(((int)current_airHeight / 1000) % 10);
	int wan_bit = qAbs(((int)current_airHeight / 10000) % 10);
	//小数部分
	double decimal = (current_airHeight - ((int)current_airHeight));
	//个位和小数位的和；
	double ge_decimal_bit = one_bit + decimal;
	//如果有万位，再绘制万位数字

	draw_wan_bit(wan_bit, ((int)current_airHeight % 10000 + decimal), p_painter);
	draw_qian_bit(qian_bit, ((int)current_airHeight % 1000 + decimal), p_painter);
	draw_bai_bit(bai_bit, (shi_bit * 10 + ge_decimal_bit), p_painter);
	draw_shi_bit(shi_bit, ge_decimal_bit, p_painter);
}
void Height_meter::draw_wan_bit(int wan_bit, double num, QPainter*& p_painter)
{
	//将p_painter移动到14，-12位置绘制矩形，然后在这个机型内部绘制文本
	p_painter->translate(14, -12);
	//移动到坐标点（14，-12）
	p_painter->save();
	//显示万位
	p_painter->setClipRect(0, 0, HEIBOX_NUMRECT_WID, HEIBOX_NUMRECT_HEI);

	if (num > 9990)
	{
		Translate_y((num-9990)*1.6,p_painter);
	}

	if (current_airHeight >= 0)
	{
		box_option.box_num_string.setNum(wan_bit);
		p_painter->drawText(box_option.num_rectf, box_option.box_num_string, box_option.option);
	}

	if (num > 9990)
	{
		//p_painter->translate(0, -HEIBOX_NUMRECT_HEI);
		Translate_y(-HEIBOX_NUMRECT_HEI, p_painter);
		wan_bit++;
		box_option.box_num_string.setNum(wan_bit);
		p_painter->drawText(box_option.num_rectf, box_option.box_num_string, box_option.option);
	}
	else if (num <= 9990 && wan_bit == 0)
	{
		if (current_airHeight < 0)
		{
			box_option.box_num_string = "-";
		}
		else
		{
			box_option.box_num_string.setNum(0);
		}
		
		p_painter->drawText(box_option.num_rectf, box_option.box_num_string, box_option.option);
	}

	p_painter->restore();
}
void Height_meter::draw_qian_bit( int  qian_bit, double num, QPainter*& p_painter)
{
	//绘制千位
	//该translate是在上一次的基础上偏移
	p_painter->translate(HEIBOX_NUMRECT_WID, 0);

	//移动到坐标点(14 + HEIBOX_NUMRECT_WID, -12 )
	p_painter->save();
	p_painter->setClipRect(0, 0, HEIBOX_NUMRECT_WID, HEIBOX_NUMRECT_HEI);
	if (num > 990)
	{
		//p_painter->translate(0, (num - 990) * 1.6);
		Translate_y((num - 990)*1.6, p_painter);
	}

	box_option.box_num_string.setNum(qian_bit);
	p_painter->drawText(box_option.num_rectf, box_option.box_num_string, box_option.option);

	if (num > 990)
	{
		//p_painter->translate(0, -HEIBOX_NUMRECT_HEI);
		Translate_y(-HEIBOX_NUMRECT_HEI, p_painter);
		qian_bit++;
		qian_bit = (qian_bit > 9) ? 0 : qian_bit;
		box_option.box_num_string.setNum(qian_bit);
		p_painter->drawText(box_option.num_rectf, box_option.box_num_string, box_option.option);
	}
	//恢复到坐标点（14，-12）
	p_painter->restore();
}
void Height_meter::draw_bai_bit(int& bai_bit, double num, QPainter*& p_painter)
{
	//因为千位数字绘制函数已经做了一次判断，因此下面的代码不用再判断万位是否为0，这样就好移动指针
	p_painter->translate(HEIBOX_NUMRECT_WID, 0);
	//保存坐标点（32,0）
	p_painter->setFont(box_option.font1);
	p_painter->save();
	p_painter->setClipRect(0, 0, HEIBOX_NUMRECT_WID, HEIBOX_NUMRECT_HEI);
	//绘制中间点下方的数字
	if (num > 90)
	{
		Translate_y((num-90)*1.6,p_painter);
	}
	box_option.box_num_string.setNum(bai_bit);
	p_painter->drawText(box_option.num_rectf, box_option.box_num_string, box_option.option);

	if (num > 90)
	{
		Translate_y(-HEIBOX_NUMRECT_HEI, p_painter);
		bai_bit++;
		bai_bit = (bai_bit > 9) ? 0 : bai_bit;
		box_option.box_num_string.setNum(bai_bit);
		p_painter->drawText(box_option.num_rectf, box_option.box_num_string, box_option.option);
	}

	//恢复到坐标点（32,0）；
	p_painter->restore();
}
void Height_meter::draw_shi_bit(int shi_bit, double ge_decimal_bit, QPainter*& p_painter)
{
	//绘制十位的数值
	p_painter->translate(HEIBOX_NUMRECT_WID - 2, 0);
	p_painter->setClipRect(0, -4, 2*HEIBOX_NUMRECT_WID, 2*HEIBOX_NUMRECT_HEI);

	//绘制当前的数值
	if (shi_bit % 2 == 0)      //如果是偶数
	{
		box_option.offset_pix = ge_decimal_bit * ROLL_NUM_PIX;
		Translate_y(box_option.offset_pix, p_painter);
		judge_toZero(shi_bit);
	}
	else
	{
		shi_bit--;
		judge_toZero(shi_bit);
		box_option.offset_pix = (ge_decimal_bit + 10) * ROLL_NUM_PIX;
		Translate_y(box_option.offset_pix, p_painter);
	}
	p_painter->drawText(box_option.townum_rect, box_option.box_num_string, box_option.option);

	//绘制该数值上方的数字  高度为负值时，则为下方；
	//p_painter->translate(0.0 ,-HEIBOX_NUMRECT_HEI );
	Translate_y(-HEIBOX_NUMRECT_HEI, p_painter);
	shi_bit += 2;
	judge_toZero(shi_bit);
	p_painter->drawText(box_option.townum_rect, box_option.box_num_string, box_option.option);
	

	//绘制下方的数字
	//p_painter->translate(0.0,HEIBOX_NUMRECT_HEI*2 );
	Translate_y(HEIBOX_NUMRECT_HEI * 2, p_painter);
	shi_bit -= 4;
	shi_bit = shi_bit < 0 ? 8 : shi_bit;
	judge_toZero(shi_bit);
	p_painter->drawText(box_option.townum_rect, box_option.box_num_string, box_option.option);


}
//设置字符串要显示的内容
void Height_meter::judge_toZero(int& shi_bit)
{
	if (shi_bit == 0 || shi_bit == 10)
	{
		box_option.box_num_string = "00";
	}
	else
	{
		box_option.box_num_string.setNum(shi_bit * 10);
	}
}
void Height_meter::Translate_y(double offset_pix, QPainter*& p_painter)
{
	if (box_option.negative)
	{
		p_painter->translate(0.0, -offset_pix);
	}
	else
	{
		p_painter->translate(0.0, offset_pix);
	}
}
void Height_meter::test_add()
{
	current_airHeight += step;
	label->setNum(current_airHeight);
	update();
}

void Height_meter::test_sub()
{
	current_airHeight -= step;
	label->setNum(current_airHeight);
	update();
}

Height_meter::~Height_meter()
{

}
