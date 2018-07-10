#include "QImageViewer.h"
#include <QSize>
#include <QResizeEvent>
#include <QPainter>

QImageViewer::QImageViewer( QWidget*parent /*= 0*/ )
    :QWidget(parent)
{
    m_scale = 1.0;
    m_rotation = 0.0;
    m_scaleBasePt = QPointF(0, 0);
    m_buttonLeftPressed = false;
    m_buttonMiddlePressed = false;
}

QImageViewer::~QImageViewer(void)
{
}

void QImageViewer::setImage( QImage &image )
{
    m_sourceImage = image;
    zoom();
}

QImage QImageViewer::getSelectionImage()
{
    if (m_sourceImage.isNull())
    {
        return QImage();
    }

    QImage img;
    if (m_rotation != 0)
    {
        QTransform t;
        t.rotate(m_rotation);
        img = m_sourceImage.transformed(t);

        QPointF c1(m_sourceImage.width() / qreal(2), m_sourceImage.height() / qreal(2));
        QPointF c2(img.width() / qreal(2), img.height() / qreal(2));
        
        QRect r = m_selection.translated((c2 - c1).toPoint());
        img = img.copy(r);
    }
    else
    {
        img = m_sourceImage.copy(m_selection);
    }

    return img;
}

void QImageViewer::zoom()
{
    if (!m_sourceImage.isNull())
    {
        double sx = m_sourceImage.width() * 1.2 / this->width();
        double sy = m_sourceImage.height() * 1.2 / this->height();
        m_scale = 1. / qMax(sx, sy);

        qreal x = this->width() - m_sourceImage.width() * m_scale;
        qreal y = this->height() - m_sourceImage.height() * m_scale;
        m_scaleBasePt = QPointF(x/2, y/2);
        repaint();
    }	
}

QPointF QImageViewer::screenToWorld( QPointF &pt, bool includeRotation/* = true*/ )
{
    QTransform t = getTransform(includeRotation);
    QTransform t1 = t.inverted();
    return t1.map(pt);
}

QPointF QImageViewer::worldToScreen( QPointF &pt, bool includeRotation/* = true*/ )
{
    QTransform t = getTransform(includeRotation);
    return t.map(pt);
}

void QImageViewer::paintEvent(QPaintEvent * event)
{
    QPainter p(this);
    p.fillRect(this->rect(), Qt::gray);

    p.save();
    QTransform transform = getTransform();
    p.setWorldTransform(transform);
    p.drawImage(QPoint(0, 0), m_sourceImage);
    p.restore();
    
    if (!m_selection.isNull())
    {
        p.save();
        QTransform transform = getTransform(false);
        p.setWorldTransform(transform);
        QVector<QLineF> lines;
        qreal x1 = m_selection.left();
        qreal y1 = m_selection.top();
        qreal x2 = m_selection.right();
        qreal y2 = m_selection.bottom();

        lines.append(QLineF(x1, y1, x2, y1));
        lines.append(QLineF(x2, y1, x2, y2));
        lines.append(QLineF(x2, y2, x1, y2));
        lines.append(QLineF(x1, y2, x1, y1));

        QPen pen(Qt::blue);
        QTransform t1 = transform.inverted();
        QPointF pt = t1.map(QPointF(0, 0));
        QPointF pt1 = t1.map(QPointF(1, 0));
        pen.setWidthF(abs(pt1.x() - pt.x()));
        p.setPen(pen);
        p.drawLines(lines);
        p.restore();
    }
}

void QImageViewer::mouseMoveEvent(QMouseEvent * event)
{   
    if (m_buttonLeftPressed)
    {
        QPointF curPt = event->pos();
        QPointF pt1 = screenToWorld(m_lastPressedMousePt, false);
        QPointF pt2 = screenToWorld(curPt, false);

        m_selection = QRect(QPoint(qMin(pt1.x(), pt2.x()), qMin(pt1.y(), pt2.y())), QPoint(qMax(pt1.x(), pt2.x()), qMax(pt1.y(), pt2.y())));
    }
    else if (m_buttonMiddlePressed)
    {
        QPointF mv = event->pos() - m_lastPressedMousePt;
        m_scaleBasePt += mv;
        m_lastPressedMousePt = event->pos();
    }   
    repaint();
}

void QImageViewer::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton)
        m_buttonLeftPressed = true;
    else if (event->button() == Qt::MidButton)
        m_buttonMiddlePressed = true;
    else if (event->button() == Qt::RightButton)
    {
    }
    m_lastPressedMousePt = event->pos();
}

void QImageViewer::mouseReleaseEvent(QMouseEvent * event)
{
    m_buttonLeftPressed = false;
    m_buttonMiddlePressed = false;
    if (event->button() == Qt::RightButton)
    {
        m_selection = QRect();
    }
}

void QImageViewer::keyPressEvent(QKeyEvent * event)
{

}

void QImageViewer::wheelEvent(QWheelEvent *event)
{
    if (!m_sourceImage.isNull())
    {
        QPoint angle = event->angleDelta();

        if (event->modifiers() == Qt::ShiftModifier)
        {
            m_rotation += angle.y()/8;
        }
        else
        {
            QPointF pt1 = event->posF();
            QPointF p = worldToScreen(QPointF(0, 0));

            //{
            //
            //    QTransform translationTransform;
            //    translationTransform.translate(m_scaleBasePt.x(), m_scaleBasePt.y());

            //    QTransform rotationTransform;
            //    QPointF c(m_sourceImage.width() / qreal(2), m_sourceImage.height() / qreal(2));
            //    rotationTransform.translate(c.x(), c.y());
            //    rotationTransform.rotate(m_rotation);
            //    rotationTransform.translate(-c.x(), -c.y());

            //    QTransform scalingTransform;
            //    scalingTransform.scale(m_scale, m_scale);

            //    QTransform t =  rotationTransform* scalingTransform * translationTransform;
            //    QPointF a = t.map(QPointF(0, 0));

            //    QPointF a1 = scalingTransform.map(rotationTransform.map(translationTransform.map(QPointF(0, 0))));
            //    QPointF a2 = translationTransform.map(rotationTransform.map(scalingTransform.map(QPointF(0, 0))));
            //    QPointF a3 = translationTransform.map(scalingTransform.map(rotationTransform.map(QPointF(0, 0)))); //ok

            //    QPointF b = t.inverted().map(a);//ok
            //    QTransform t1 = translationTransform.inverted() * rotationTransform.inverted() * scalingTransform.inverted();
            //    QPointF b1 = t1.map(a);
            //    QTransform t12 = rotationTransform.inverted() * scalingTransform.inverted() * translationTransform.inverted();
            //    QPointF b2 = t12.map(a);
            //    QTransform t13 = scalingTransform.inverted() * rotationTransform.inverted() * translationTransform.inverted();
            //    QPointF b3 = t13.map(a);
            //    QPointF b4 = translationTransform.inverted().map(rotationTransform.inverted().map(scalingTransform.inverted().map(a)));
            //    QPointF b5 = translationTransform.inverted().map(scalingTransform.inverted().map(rotationTransform.inverted().map(a)));
            //    QPointF b6 = scalingTransform.inverted().map(rotationTransform.inverted().map(translationTransform.inverted().map(a)));
            //    QPointF b7 = scalingTransform.inverted().map(translationTransform.inverted().map(rotationTransform.inverted().map(a)));
            //    QPointF b8 = rotationTransform.inverted().map(scalingTransform.inverted().map(translationTransform.inverted().map(a)));//ok
            //    QPointF b9 = rotationTransform.inverted().map(translationTransform.inverted().map(scalingTransform.inverted().map(a)));

            //    QPointF b10 = translationTransform.inverted().map(a);

            //    QTransform t2 = rotationTransform.inverted() * scalingTransform.inverted();
            //    QPointF c1 = t2.map(a);
            //    QTransform t21 = scalingTransform.inverted() * rotationTransform.inverted();
            //    QPointF c2 = t21.map(a);
            //    QPointF c4 = (rotationTransform.inverted().map(scalingTransform.inverted().map(a)));
            //    QPointF c5 = (scalingTransform.inverted().map(rotationTransform.inverted().map(a)));

            //    QPointF d1 = rotationTransform.inverted().map(scalingTransform.inverted().map(a));

            //    QTransform t4 = t.inverted();
            //    QPointF e = t4.map(a);
            //}

            QPointF pt2;
            if (angle.y() > 0)
            {    
                pt2 = pt1 + (p-pt1)*0.618;
                m_scale *= 0.618;
            }
            else
            {
                pt2 = pt1 + (p-pt1)/0.618;
                m_scale /= 0.618;          
            }

            QTransform rotationTransform;
            QPointF c(m_sourceImage.width() / qreal(2), m_sourceImage.height() / qreal(2));
            rotationTransform.translate(c.x(), c.y());
            rotationTransform.rotate(m_rotation);
            rotationTransform.translate(-c.x(), -c.y());

            QTransform scalingTransform;
            scalingTransform.scale(m_scale, m_scale);

            QPointF a3 = (scalingTransform.map(rotationTransform.map(QPointF(0, 0)))); //ok

            QPointF pt3 = pt2 - a3;
            m_scaleBasePt = pt3;//pt1 + pt3;//(p-pt1)/0.618;

        }
        repaint();
    }
}

void QImageViewer::mouseDoubleClickEvent( QMouseEvent * event )
{
    //if (event->button() == Qt::LeftButton)
    {
        zoom();
    }
}

QTransform QImageViewer::getTransform(bool includeRotation/* = true*/)
{
    QTransform transform;
    if (!m_sourceImage.isNull())
    {
        transform.translate(m_scaleBasePt.x(), m_scaleBasePt.y());

        transform.scale(m_scale, m_scale);
        if (includeRotation)
        {
            QPointF c(m_sourceImage.width() / qreal(2), m_sourceImage.height() / qreal(2));
            transform.translate(c.x(), c.y());
            transform.rotate(m_rotation);
            transform.translate(-c.x(), -c.y());
        }

        ////{
        //    QTransform translationTransform;
        //    translationTransform.translate(m_scaleBasePt.x(), m_scaleBasePt.y());

        //    QTransform rotationTransform;
        //    QPointF c(m_sourceImage.width() / qreal(2), m_sourceImage.height() / qreal(2));
        //    rotationTransform.translate(c.x(), c.y());
        //    rotationTransform.rotate(m_rotation);
        //    rotationTransform.translate(-c.x(), -c.y());

        //    QTransform scalingTransform;
        //    scalingTransform.scale(m_scale, m_scale);

        //    QTransform t;
        //    t = scalingTransform * rotationTransform* translationTransform;
        //    bool e = transform == t;
        //    int a=0;
        //    t = transform;
        ////}

    }
    return transform;
}

//void QImageViewer::resizeEvent( QResizeEvent * event )
//{
//}
