#pragma once
#include <QWidget>
#include <QImage>

class QImageViewer :
    public QWidget
{
    Q_OBJECT
public:
    QImageViewer(QWidget*parent = 0);
    virtual ~QImageViewer(void);

    void setImage(QImage &image);
    QImage &getImage(){return m_sourceImage;}
    QRect getSelection() {return m_selection;}
    QImage getSelectionImage();
    void zoom();
    QPointF screenToWorld(QPointF &pt, bool includeRotation = true);
    QPointF worldToScreen(QPointF &pt, bool includeRotation = true);
protected:
    virtual void paintEvent(QPaintEvent * event);
    //virtual void resizeEvent(QResizeEvent * event);
    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseReleaseEvent(QMouseEvent * event);
    virtual void keyPressEvent(QKeyEvent * event);
    virtual void wheelEvent(QWheelEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent * event);

    QTransform getTransform(bool includeRotation = true);
private:
    QImage m_sourceImage;
    
    qreal m_scale;
    qreal m_rotation;
    QPointF m_scaleBasePt;

    QPointF m_lastPressedMousePt;
    QRect m_selection;

    bool m_buttonLeftPressed;
    bool m_buttonMiddlePressed;
};

