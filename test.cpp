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
Function��
Description��
Calls:
Called By:
Table Accessed:
Table Updated:
Input:
Output��
Return:
Other:
****************************/
void Height_meter::initLayout()
{
	current_airHeight = test;//test
	background_rectf = QRectF(BACKGROUND_RECT_X, BACKGROUND_RECT_Y,
		BACKGROUND_RECT_WIDTH, BACKGROUND_RECT_HEIGHT);

	//��������
	mark_text_font.setFamily(QString::fromUtf8("\347\255\211\347\272\277 Light"));
	mark_text_font.setPixelSize(11);

	//���ĵ�����
	center_point = QPointF(CENTER_POINT_X, CENTER_POINT_Y);
	background_color = QColor(99, 90, 90, 255);
	white_color = QColor(255, 255, 255, 255);

	init_height_box();
}

void Height_meter::paintEvent(QPaintEvent* event)
{
	QPainter* p = new QPainter(this);

	//������ ����
	p->setRenderHint(QPainter::Antialiasing);
	//��ʼ���߶ȱ���ͼ���̶�ͼ
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
Function��
Description��
Calls:
Called By:
Table Accessed:
Table Updated:
Input:
Output��
Return:
Other:
****************************/
void Height_meter::drawHeightmark(QPainter* & p_painter)
{
	p_painter->save();

	//���û�ͼ������  ���³�����Χ
	p_painter->setClipRect(background_rectf);
	p_painter->setPen(background_color);
	p_painter->setBrush(background_color);

	//���Ʊ�������
	p_painter->drawRect(background_rectf);

	//���ƿ̶�
	QPen tmp_pen(white_color);

	//�̶ȵ��߿�
	tmp_pen.setWidthF(0.9);

	p_painter->setPen(tmp_pen);
	p_painter->setFont(mark_text_font);
	//�任����ǰ����������ʾ��Ӧ��ָ���λ�ã�����ʾ���̶������λ�õ�
	p_painter->translate(center_point);

	//�ָ������ڣ�0,0��
	p_painter->drawPath(*height_mark);
	p_painter->restore();
}

/***************************
Function��draw_Line_NumText
Description������Ӧ��λ�û��Ƴ�����������Ҫ�������ֵĵط���������
Calls:
Called By:
Table Accessed:
Table Updated:
Input:
Output��
Return:
Other:
****************************/
void Height_meter::draw_Line_NumText(QPainter* & p_painter, int& num, QTextOption& option)
{
	//��� ������  ˵������Ҫ���� ����
	//�߶�����ܳ���50000Ӣ��
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
Function��
Description��
Calls:
Called By:
Table Accessed:
Table Updated:
Input:
Output��
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
Function��
Description��
Calls:
Called By:
Table Accessed:
Table Updated:
Input:
Output��
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
	//�Է��и߶�ȡ��
	double		offset_num = tmp_int_airHeight % 100;
	offset_num += (current_airHeight - tmp_int_airHeight);
	//ƫ������������ֵ���õ�Ӧ��ƫ�Ƶ�����ֵ
	double		offset_pix = offset_num * ONE_NUM_PIX;
	//��path����painter����������»���һ����
	height_mark->moveTo(0, offset_pix);

	//��Ϊÿ100�߶�Ϊһ�ο̶���
	int begin_num = tmp_int_airHeight / 100;
	//�����Ϸ�����
	for (int i = 0; i < 6; i++)
	{
		tmp_string.setNum(begin_num * 100);
		if (i != 0)
		{
			height_mark->moveTo(0, -PIX_INTERVAL * i + offset_pix);
			//���̶���
			height_mark->lineTo(10, -PIX_INTERVAL * i + offset_pix);
			if (begin_num % 2 == 0)
			{
				//������
				height_mark->addText(DRAW_TEXTRECT_X, -i * PIX_INTERVAL + offset_pix + 4, mark_text_font, tmp_string);
			}
		}
		else
		{
			//���̶���
			height_mark->lineTo(10, offset_pix);
			if (begin_num % 2 == 0)
			{
				//д����
				height_mark->addText(DRAW_TEXTRECT_X, offset_pix + 4, mark_text_font, tmp_string);
			}
		}
		//�߶�����Ŀ��һ
		begin_num++;
	}
	//�·�ͬ��  ����������
	begin_num = tmp_int_airHeight / 100 - 1;
	//�����·�����
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
	//����ǰ��path��ֵ�������ʱ�ı���path���Ա㿪ʼʱ����ɾ��
	temp_height_mark = (*height_mark);
}

//����ʾ���ڻ���ǰ���г�ʼ�����ֻ���ѡ��
void Height_meter::init_height_box()
{
	box_option.font = mark_text_font;
	box_option.font.setPointSizeF(11.5);

	box_option.font1 = mark_text_font;
	box_option.font1.setPointSizeF(10);

	//��ָ���ľ��ο��� ���ж���
	box_option.option = QTextOption(Qt::AlignRight | Qt::AlignVCenter);

	//���� ָʾ�ı߿�ͱ���ɫ
	box_option.pen = QPen(white_color);
	box_option.pen.setWidthF(1.5);

	box_option.rectf = QRectF(0, -17, 61, 34);
	box_option.num_rectf = QRectF(0, HEIBOX_NUMRECT_Y, 9, HEIBOX_NUMRECT_HEI);
	box_option.townum_rect = QRectF(0, HEIBOX_NUMRECT_Y, 18, HEIBOX_NUMRECT_HEI);
	box_option.brush = QBrush(QColor(0, 0, 0, 255));
	//���� ·���ĳ�ʼ��		
	//ָʾ���� �м���Ǹ�����ģ��  ���� ����� center_point
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
	//�ƶ������ĵ�
	p_painter->translate(center_point);

	p_painter->setClipRect(box_option.rectf);
	p_painter->setFont(box_option.font);

	p_painter->setPen(box_option.pen);								//���� �ߵ���ɫ
	p_painter->setBrush(box_option.brush);		                    //���� ���ɫ
	p_painter->drawPath(height_box);

	//����ͼ�λ��ƺ���
	draw_height_box_text(p_painter);
}
void Height_meter::draw_height_box_text(QPainter* &p_painter)
{
	p_painter->setClipPath(height_box);
	p_painter->save();
	QString tmp_string;

	//�Ƿ�Ϊ����
	box_option.negative = (current_airHeight > 0) ? false : true;
	//�����ַ���
	int one_bit = qAbs(((int)current_airHeight) % 10);
	int shi_bit = qAbs(((int)current_airHeight / 10) % 10);
	int bai_bit = qAbs(((int)current_airHeight / 100) % 10);
	int qian_bit = qAbs(((int)current_airHeight / 1000) % 10);
	int wan_bit = qAbs(((int)current_airHeight / 10000) % 10);
	//С������
	double decimal = (current_airHeight - ((int)current_airHeight));
	//��λ��С��λ�ĺͣ�
	double ge_decimal_bit = one_bit + decimal;
	//�������λ���ٻ�����λ����

	draw_wan_bit(wan_bit, ((int)current_airHeight % 10000 + decimal), p_painter);
	draw_qian_bit(qian_bit, ((int)current_airHeight % 1000 + decimal), p_painter);
	draw_bai_bit(bai_bit, (shi_bit * 10 + ge_decimal_bit), p_painter);
	draw_shi_bit(shi_bit, ge_decimal_bit, p_painter);
}
void Height_meter::draw_wan_bit(int wan_bit, double num, QPainter*& p_painter)
{
	//��p_painter�ƶ���14��-12λ�û��ƾ��Σ�Ȼ������������ڲ������ı�
	p_painter->translate(14, -12);
	//�ƶ�������㣨14��-12��
	p_painter->save();
	//��ʾ��λ
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
	//����ǧλ
	//��translate������һ�εĻ�����ƫ��
	p_painter->translate(HEIBOX_NUMRECT_WID, 0);

	//�ƶ��������(14 + HEIBOX_NUMRECT_WID, -12 )
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
	//�ָ�������㣨14��-12��
	p_painter->restore();
}
void Height_meter::draw_bai_bit(int& bai_bit, double num, QPainter*& p_painter)
{
	//��Ϊǧλ���ֻ��ƺ����Ѿ�����һ���жϣ��������Ĵ��벻�����ж���λ�Ƿ�Ϊ0�������ͺ��ƶ�ָ��
	p_painter->translate(HEIBOX_NUMRECT_WID, 0);
	//��������㣨32,0��
	p_painter->setFont(box_option.font1);
	p_painter->save();
	p_painter->setClipRect(0, 0, HEIBOX_NUMRECT_WID, HEIBOX_NUMRECT_HEI);
	//�����м���·�������
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

	//�ָ�������㣨32,0����
	p_painter->restore();
}
void Height_meter::draw_shi_bit(int shi_bit, double ge_decimal_bit, QPainter*& p_painter)
{
	//����ʮλ����ֵ
	p_painter->translate(HEIBOX_NUMRECT_WID - 2, 0);
	p_painter->setClipRect(0, -4, 2*HEIBOX_NUMRECT_WID, 2*HEIBOX_NUMRECT_HEI);

	//���Ƶ�ǰ����ֵ
	if (shi_bit % 2 == 0)      //�����ż��
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

	//���Ƹ���ֵ�Ϸ�������  �߶�Ϊ��ֵʱ����Ϊ�·���
	//p_painter->translate(0.0 ,-HEIBOX_NUMRECT_HEI );
	Translate_y(-HEIBOX_NUMRECT_HEI, p_painter);
	shi_bit += 2;
	judge_toZero(shi_bit);
	p_painter->drawText(box_option.townum_rect, box_option.box_num_string, box_option.option);
	

	//�����·�������
	//p_painter->translate(0.0,HEIBOX_NUMRECT_HEI*2 );
	Translate_y(HEIBOX_NUMRECT_HEI * 2, p_painter);
	shi_bit -= 4;
	shi_bit = shi_bit < 0 ? 8 : shi_bit;
	judge_toZero(shi_bit);
	p_painter->drawText(box_option.townum_rect, box_option.box_num_string, box_option.option);


}
//�����ַ���Ҫ��ʾ������
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
