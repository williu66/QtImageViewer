#ifndef QTIMAGEDEMO_H
#define QTIMAGEDEMO_H

#include <QtWidgets/QMainWindow>
#include "ui_qtimagedemo.h"
#include "ui_GOpenFileDialog.h"

class QImageViewer;
class QtImageDemo : public QMainWindow
{
    Q_OBJECT

public:
    QtImageDemo(QWidget *parent = 0);
    ~QtImageDemo();

private slots:
    void onOpen();
    void onOpenFile();
    void onRun();
    void onZoom1();
    void onZoom2();
    void onZoom3();
private:
    Ui::QtImageDemoClass ui;
    Ui::Dialog dia;
    QImageViewer* m_view1;
    QImageViewer* m_view2;
    QImageViewer* m_view3;
    QString fileName;
};

#endif // QTIMAGEDEMO_H
