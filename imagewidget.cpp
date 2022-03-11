#include "imagewidget.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>
#include <QGraphicsSceneDragDropEvent>
#include <QDrag>
#include <math.h>

ImageWidget::ImageWidget(QPixmap *pixmap)
{
    m_pix = *pixmap;
    //If enabled is true, this item will accept hover events; otherwise, it will ignore them. By default, items do not accept hover events.
    setAcceptDrops(true);
    m_scaleValue = 0;
    m_scaleDafault = 0;
    m_leftButtonPressed = false;
}

QRectF ImageWidget::boundingRect() const
{
    return QRectF(-m_pix.width() / 2, -m_pix.height() / 2, m_pix.width(), m_pix.height());
}

void ImageWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->drawPixmap(-m_pix.width() / 2, -m_pix.height() / 2, m_pix);
}

void ImageWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        m_startPos = event->pos();//鼠标左击时，获取当前鼠标在图片中的坐标，
        m_leftButtonPressed = true;//标记鼠标左键被按下
    }else if(event->button() == Qt::RightButton){
        ResetItemPos();//右击鼠标重置大小
    }
}

void ImageWidget::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_leftButtonPressed){
        QPointF point = (event->pos() - m_startPos) * m_scaleValue;
        moveBy(point.x(), point.y());
    }
}

void ImageWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    m_leftButtonPressed = false;//标记鼠标左键已经抬起
}


void ImageWidget::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    //Zoom up to 50 times the original image
    if((event->delta() > 0) && (m_scaleValue >= 50)){
        return;
    }else if((event->delta() < 0) && (m_scaleValue <= m_scaleDafault)){
        ResetItemPos();//重置图片大小和位置，使之自适应控件窗口大小
    }else{
        qreal qrealOriginScale = m_scaleValue;
        if(event->delta() > 0)//鼠标滚轮向前滚动
        {
            m_scaleValue *= 1.1;//每次放大10%
        }
        else
        {
            m_scaleValue *= 0.9;//每次缩小10%
        }
        setScale(m_scaleValue);
        if(event->delta() > 0)
        {
            moveBy(-event->pos().x() * qrealOriginScale * 0.1, -event->pos().y() * qrealOriginScale * 0.1);
        }
        else
        {
            moveBy(event->pos().x() * qrealOriginScale * 0.1, event->pos().y() * qrealOriginScale * 0.1);
        }
    }
}

void ImageWidget::setQGraphicsViewWH(int nwidth, int nheight)
{
    int nImgWidth = m_pix.width();
    int nImgHeight = m_pix.height();
    qreal temp1 = nwidth * 1.0 / nImgWidth;
    qreal temp2 = nheight * 1.0 / nImgHeight;
    if(temp1 > temp2)
    {
        m_scaleDafault = temp2;
    }
    else
    {
        m_scaleDafault = temp1;
    }
    setScale(m_scaleDafault);
    m_scaleValue = m_scaleDafault;
}

void ImageWidget::ResetItemPos()
{
    m_scaleValue = m_scaleDafault;//缩放比例回到一开始的自适应比例
    setScale(m_scaleDafault);//缩放到一开始的自适应大小
    setPos(0,0);
}

qreal ImageWidget::getScaleValue() const
{
    return m_scaleValue;
}
