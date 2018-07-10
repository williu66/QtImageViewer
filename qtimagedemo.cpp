#include "qtimagedemo.h"
#include "QImageViewer.h"
#include <QSplitter>
#include <QHBoxLayout>
#include <QFileDialog>

QtImageDemo::QtImageDemo(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    connect(ui.acOpen, SIGNAL(triggered()), this, SLOT(onOpen()));
    connect(ui.acRun, SIGNAL(triggered()), this, SLOT(onRun()));
    connect(ui.acZoom1, SIGNAL(triggered()), this, SLOT(onZoom1()));
    connect(ui.acZoom2, SIGNAL(triggered()), this, SLOT(onZoom2()));
    connect(ui.acZoom3, SIGNAL(triggered()), this, SLOT(onZoom3()));

    m_view1 = new QImageViewer(this);
    m_view2 = new QImageViewer(this);
    m_view3 = new QImageViewer(this);
    QSplitter *sUp = new QSplitter(this);
    sUp->addWidget(m_view1);
    sUp->addWidget(m_view2);
    QList<int> sizes;
    sizes << 1000 << 1000;
    sUp->setSizes(sizes);

    QSplitter *sMain = new QSplitter(Qt::Vertical, this);
    sMain->addWidget(sUp);
    sMain->addWidget(m_view3);
    sMain->setSizes(sizes);
    QHBoxLayout *lay = new QHBoxLayout(this);
    lay->addWidget(sMain);
    //lay->addWidget(m_view1);
    QWidget *w = new QWidget(this);

    w->setLayout(lay);
    setCentralWidget(w);
}

QtImageDemo::~QtImageDemo()
{

}

void QtImageDemo::onOpen()
{
    QDialog d(this);
    dia.setupUi(&d);
    connect(dia.btOpen, SIGNAL(clicked()), this, SLOT(onOpenFile()));
    if (d.exec() == QDialog::Accepted)
    {
        if (dia.comboBox->currentText().compare("view1") == 0)
        {
            m_view1->setImage(QImage(fileName));
        }
        else if (dia.comboBox->currentText().compare("view2") == 0)
        {
            m_view2->setImage(QImage(fileName));
        }
        else if (dia.comboBox->currentText().compare("view3") == 0)
        {
            m_view3->setImage(QImage(fileName));
        }
    }
}
QString s_lastDir = QDir::currentPath() + "\\Sample\\";
void QtImageDemo::onOpenFile()
{
    fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"), s_lastDir, QString("All Files (*.png *.bmp *.jpg *.jpeg)"));
    s_lastDir = QFileInfo(fileName).absolutePath();
    dia.lineEdit->setText(fileName);
}

void QtImageDemo::onRun()
{
    QImage img1 = m_view1->getSelectionImage();
    QImage img2 = m_view2->getSelectionImage();
    QImage img3 = img1;//todo
    m_view3->setImage(img3);
}

void QtImageDemo::onZoom1()
{
    m_view1->zoom();
}

void QtImageDemo::onZoom2()
{
    m_view2->zoom();
}

void QtImageDemo::onZoom3()
{
    m_view3->zoom();
}