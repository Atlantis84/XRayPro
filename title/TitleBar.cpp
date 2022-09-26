#include <QHBoxLayout>
#include <QEvent>
#include <QMouseEvent>
#include <QApplication>
#include "TitleBar.h"
#include <QPainter>
#include <QPen>
#include <QColor>
#include "titlewidget.h"

#ifdef Q_OS_WIN
#include <qt_windows.h>
#endif
#pragma comment(lib,"User32.lib")

TitleBar::TitleBar(QWidget *parent) : QWidget(parent)
{
    setFixedHeight(40);
    TitleText = u8"未设置";
    m_pIconLabel = new QLabel(this);
    m_pTitleLabel = new QLabel(this);
    m_pTitleLabel->setStyleSheet("QLabel{color:rgba(0,0,0,255);background-color:rgba(0,0,0,0);font-size:13pt;font-family:'KaiTi';}");
    m_pMinimizeButton = new QPushButton(this);
    m_pMaximizeButton = new QPushButton(this);
    m_pMaximizeButton->setEnabled(false);
    m_pCloseButton = new QPushButton(this);

    m_pIconLabel->setFixedSize(30, 30);
    m_pIconLabel->setScaledContents(true);
    m_pIconLabel->setPixmap(QPixmap(":/icon/24x24.png"));

    m_pMinimizeButton->setFixedSize(30, 30);
    m_pMaximizeButton->setFixedSize(30, 30);
    m_pCloseButton->setFixedSize(30, 30);

    m_pTitleLabel->setObjectName("whiteLabel");
    m_pMinimizeButton->setObjectName("minimizeButton");
    m_pMaximizeButton->setObjectName("maximizeButton");
    m_pCloseButton->setObjectName("closeButton");

    m_pMinimizeButton->setToolTip("Minimize");
    m_pMaximizeButton->setToolTip("Maximize");
    m_pCloseButton->setToolTip("Close");

//    m_pTitleLabel->setContentsMargins(8,0,0,0);
    m_pTitleLabel->setAlignment(Qt::AlignHCenter| Qt::AlignVCenter);
    m_pTitleLabel->setText(TitleText);

    m_pMinimizeButton->setIcon(QIcon(":/icon/minus.png"));
    m_pMaximizeButton->setIcon(QIcon(":/icon/maximize.png"));
    m_pCloseButton->setIcon(QIcon(":/icon/close.png"));
    m_pCloseButton->setStyleSheet("QPushButton:hover{background-color:rgba(255,0,0,255);}");
    m_pMinimizeButton->setStyleSheet("QPushButton:hover{background-color:rgba(255,255,255,255);}");

    QHBoxLayout *mainWidgetLayout = new QHBoxLayout(this);
    TitleWidget *mainWidget = new TitleWidget;
    QHBoxLayout *pLayout = new QHBoxLayout;

    mainWidgetLayout->addWidget(mainWidget);
    mainWidget->setLayout(pLayout);
    mainWidgetLayout->setMargin(0);
    pLayout->setContentsMargins(10,0,0,0);
    pLayout->setSpacing(0);
    mainWidget->setStyleSheet("QWidget{border-color:rgba(255,255,255,100);background-color:rgba(0,0,0,0);}");
    pLayout->addWidget(m_pIconLabel);
    pLayout->addWidget(m_pTitleLabel);
    pLayout->addStretch(1);
    pLayout->addWidget(m_pMinimizeButton);
    pLayout->addWidget(m_pMaximizeButton);
    pLayout->addWidget(m_pCloseButton);

    connect(m_pMinimizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(m_pMaximizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(m_pCloseButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));

}

void TitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

//    emit m_pMaximizeButton->clicked();
}

void TitleBar::mousePressEvent(QMouseEvent *event)
{
#ifdef Q_OS_WIN
    if (ReleaseCapture())
    {
        QWidget *pWindow = this->window();
        if (pWindow->isTopLevel())
        {
           SendMessage(HWND(pWindow->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
        }
    }
       event->ignore();
#else
#endif
}

bool TitleBar::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::WindowTitleChange:
    {
        QWidget *pWidget = qobject_cast<QWidget *>(obj);
        if (pWidget)
        {
            m_pTitleLabel->setText(pWidget->windowTitle());
            return true;
        }
    }
    case QEvent::WindowIconChange:
    {
        QWidget *pWidget = qobject_cast<QWidget *>(obj);
        if (pWidget)
        {
            QIcon icon = pWidget->windowIcon();
            m_pIconLabel->setPixmap(icon.pixmap(m_pIconLabel->size()));
            return true;
        }
    }
    case QEvent::WindowStateChange:
     case QEvent::Resize:
         updateMaximize();
         return true;
     default:
         return QWidget::eventFilter(obj, event);

     }
     return QWidget::eventFilter(obj, event);
}

void TitleBar::onClicked()
{
    QPushButton *pButton = qobject_cast<QPushButton *>(sender());
    QWidget *pWindow = this->window();
    if (pWindow->isTopLevel())
    {
        if (pButton == m_pMinimizeButton)
        {
            pWindow->showMinimized();
        }
        else if (pButton == m_pMaximizeButton)
        {
            pWindow->isMaximized() ? pWindow->showNormal() : pWindow->showMaximized();
        }
        else if (pButton == m_pCloseButton)
        {
            pWindow->close();
        }
    }
}

void TitleBar::updateMaximize()
{
    QWidget *pWindow = this->window();
    if (pWindow->isTopLevel())
    {
        bool bMaximize = pWindow->isMaximized();
        if (bMaximize)
        {
            m_pMaximizeButton->setToolTip(tr("Restore"));
            m_pMaximizeButton->setProperty("maximizeProperty", "restore");
        }
        else
        {
            m_pMaximizeButton->setProperty("maximizeProperty", "maximize");
            m_pMaximizeButton->setToolTip(tr("Maximize"));
        }
        m_pMaximizeButton->setStyle(QApplication::style());
    }
}

void TitleBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    QLinearGradient Linear(0,0,0,this->height());//垂直渐变,x1=x2为垂直渐变
    Linear.setColorAt(0,QColor(235,236,240));
    Linear.setColorAt(0.5,QColor(207,207,214));
    Linear.setColorAt(1,QColor(235,236,240));
    painter.fillRect(this->rect(),QBrush(Linear));
//    background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 #D8D9DE,stop:1 #C8C8D0);
//    painter.fillRect(this->rect(),QBrush(QColor(22,194,194)));
    painter.setPen(QPen(QColor(0,0,0,100),3));
    painter.drawRect(this->rect());
}
