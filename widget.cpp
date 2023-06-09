#include "widget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

#define AUTO_RUN "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->setAttribute(Qt::WA_TranslucentBackground);
    //this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    this->initUI();
    this->initSlots();
    this->initSysTray();
    this->setWindowIcon(QIcon(":/images/countTimeIcon.jpg"));

}

void Widget::initUI()
{
    //——————————————————————定时器——————————————————————————————————//
    setDayTime = new QTimer();
    QTime currentTime  = QTime::currentTime();
    int remainMsec = 24*60*60*1000 - (currentTime.hour()*60*60 + currentTime.minute()*60 + currentTime.second())*1000 - currentTime.msec();
    setDayTime->start(remainMsec);

    //——————————————————————中间三个label————————————————————————————//
    //三个label
    begin = new QLabel();
    time = new QLabel();
    day = new QLabel();
    //调整大小的策略
    begin->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    time->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    day->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    //设置文字，之后使用配置文件统一起来
    QSettings config(":/images/countTimeSetting.ini",QSettings::IniFormat);
    config.setIniCodec("utf-8");

    //设置开头文本
    QVariant distance = config.value("set/beginText");
    this->setbeginString(distance.toString());

    //设置天数文本
    QVariant dayCount = config.value("set/targetData");
    this->settimeString(dayCount.toDate());

    //设置透明度
    QVariant tranConfig = config.value("set/tran");
    this->setTranValue(tranConfig.toInt());

    //设置自动启动
    QVariant isPowerOn = config.value("set/poweron");
    if(this->isAutoRun(QApplication::applicationFilePath()) | isPowerOn.toBool())
    {
        this->setAutoRun(QApplication::applicationFilePath(), isPowerOn.toBool());
    }

    //设置最后一个天字
    day->setText("天");

    //设置文字大小
    QFont font;
    font.setPointSize(32);
    begin->setFont(font);
    time->setFont(font);
    day->setFont(font);

    //设置文字背景透明
    begin->setAttribute(Qt::WA_TranslucentBackground);
    time->setAttribute(Qt::WA_TranslucentBackground);
    day->setAttribute(Qt::WA_TranslucentBackground);

    //调试代码
//    begin->setStyleSheet("background-color:yellow");
//    time->setStyleSheet("background-color:yellow");
//    day->setStyleSheet("background-color:yellow");

    //——————————————————————————按钮设置————————————————————————//
    //菜单按钮
    menuBtn = new QPushButton();
    //设置样式
    menuBtn->setFlat(true);
    menuBtn->setAttribute(Qt::WA_TranslucentBackground);
    menuBtn->setStyleSheet("border-image:url(:/images/set.jpg);");

    //设置功能
    QMenu *myMemu = new QMenu();
    setting = new QAction(tr("基本设置"));
    setImg = new QAction(tr("自定义背景图片"));
    setDefaultImg = new QAction(tr("使用默认背景图"));
    setClose = new QAction(tr("退出"));
    myMemu->addAction(setting);
    myMemu->addAction(setImg);
    myMemu->addAction(setDefaultImg);
    myMemu->addAction(setClose);
    menuBtn->setMenu(myMemu);

    menuBtn->setMaximumSize(QSize(10,10));
    //——————————————————————widget设置——————————————//
    //外部widget
    innerWidget = new QWidget();
    //设置样式以及大小
    QString imgPath = config.value("set/backgroundImgPath").toString();
    if(QFileInfo::exists(imgPath) && !QString::compare(":/images/background.jpg",imgPath))
    {
        innerWidget->setStyleSheet(" \
                                   border-radius:10px; \
                                   border-image: url("+imgPath+");");
    }
    else
    {
        innerWidget->setStyleSheet(" \
                                   border-radius:10px; \
                                   border-image: url(:/images/background.jpg);");
    }

    innerWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    innerWidget->setWindowFlags(Qt::FramelessWindowHint);

    //——————————————————————布局设置——————————————————//
    QHBoxLayout *firstLayout = new QHBoxLayout();
    firstLayout->addWidget(begin);
    firstLayout->addWidget(time);
    firstLayout->addWidget(day);
    firstLayout->addWidget(menuBtn);

    firstLayout->setMargin(10);
    firstLayout->setSpacing(10);
    firstLayout->setContentsMargins(10,10,10,10);

    innerWidget->setLayout(firstLayout);

    QVBoxLayout *lastLayout = new QVBoxLayout();
    lastLayout->addWidget(innerWidget);

    setLayout(lastLayout);

}

void Widget::initSlots()
{
    connect(setting,SIGNAL(triggered(bool)),this,SLOT(on_settingClick()));
    connect(setImg,SIGNAL(triggered(bool)),this,SLOT(on_setImgClick()));
    connect(setDefaultImg,SIGNAL(triggered(bool)),this,SLOT(on_setDefaultImg()));
    connect(setClose,SIGNAL(triggered(bool)),this,SLOT(on_setCloseClick()));
    connect(setDayTime,SIGNAL(timeout()),this,SLOT(onTimerAction()));
}

//设置点击窗口任意位置移动——重写以下两个方法
void Widget::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        m_poin = e->globalPos() - pos();
        e->accept();
    }
}

void Widget::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)
    {
        move(e->globalPos() - m_poin);
        e->accept();
    }
}

//设置时间文本的函数
void Widget::settimeString(QDate timeString)
{
    QDate currentDate = QDate::currentDate();
    int dayCount = currentDate.daysTo(timeString);
    time->setText(QString::number(dayCount,10));
}

//设置节日文本的函数
void Widget::setbeginString(QString beginString)
{
    QString beginText = "距离" + beginString + "还有：";
    begin->setText(beginText);
}

//设置透明度
void Widget::setTranValue(int value)
{
    float tranValue = (float)value / 100;
    this->setWindowOpacity(tranValue);
}

//点击设置之后的槽函数
void Widget::on_settingClick()
{
    setWindow = new set();
    if(setWindow)
    {
        setWindow->registerCallBack(onCallBack,this);
    }
    setWindow->show();
}

//点击背景之后的槽函数
void Widget::on_setImgClick()
{
    QString currentUrl = QDir::currentPath();
    QUrl fileUrl = QFileDialog::getOpenFileUrl(this,tr("图片选择器"),currentUrl,QStringLiteral("图片文件(*png; *jpg)"));
    if(fileUrl.isEmpty())
    {
        return;
    }
    else
    {
        QString imgUrl = fileUrl.path();
        imgUrl.remove(0,1);
        QString backgroundImg = "border-radius:10px; border-image: url(" + imgUrl + ");";
        QSettings setImgPath(":/images/countTimeSetting.ini",QSettings::IniFormat);
        setImgPath.setValue("set/backgroundImgPath",imgUrl);
        innerWidget->setStyleSheet(backgroundImg);
    }
    return;
}

//点击设置默认背景图片
void Widget::on_setDefaultImg()
{
    QSettings setImgPath(":/images/countTimeSetting.ini",QSettings::IniFormat);
    setImgPath.setValue("set/backgroundImgPath",":/images/background.jpg");

    innerWidget->setStyleSheet("border-radius:10px; \
                               border-image: url(:/images/background.jpg);");
}

//点击关闭的槽函数
void Widget::on_setCloseClick()
{
    this->close();
}

//定时器动作
void Widget::onTimerAction()
{
    setDayTime->start(24*60*60*1000);
    QSettings getTargetDay(":/images/countTimeSetting.ini",QSettings::IniFormat);
    QVariant targetDay = getTargetDay.value("set/targetData");
    this->settimeString(targetDay.toDate());
}

//回调函数
void Widget::onCallBack(int tranValue, bool powerOn, QDate timeString, QString beginString, void *context)
{
    Widget *a = (Widget*)context;
    a->settimeString(timeString);
    a->setbeginString(beginString);
    a->setTranValue(tranValue);
    a->setAutoRun(QApplication::applicationFilePath(),powerOn);
    return;
}

//开机启动
void Widget::setAutoRun(const QString &appPath, bool flag)
{
    //打开注册表
    QSettings autoRunTable(AUTO_RUN, QSettings::NativeFormat);

    QFileInfo fInfo(appPath);
    QString name = fInfo.baseName();

    QString oldPath = autoRunTable.value(name).toString();
    QString newPath = QDir::toNativeSeparators(appPath);

    if(flag)
    {
        if(oldPath!=newPath)
        {
            autoRunTable.setValue(name,newPath);
        }
    }
    else
    {
        autoRunTable.remove(name);
    }
}

//判断是否开机自启
bool Widget::isAutoRun(const QString &appPath)
{
    QSettings autoRunTable(AUTO_RUN,QSettings::NativeFormat);
    QFileInfo fInfo(appPath);
    QString name = fInfo.baseName();
    QString oldPath = autoRunTable.value(name).toString();
    QString newPath = QDir::toNativeSeparators(appPath);
    return (autoRunTable.contains(name) && newPath == oldPath);
}

//重写closeevent
void Widget::closeEvent(QCloseEvent *event)
{
    QApplication::setQuitOnLastWindowClosed(true);
    QString title = "关闭提示";
    QString msg = "是否退出程序？！";
    int select = QMessageBox::question(this,title,msg,
                                          QMessageBox::Yes,QMessageBox::No);
    if(select == QMessageBox::Yes)
    {
        qApp->quit();
    }
    else
    {
        event->ignore();
    }
}

void Widget::initSysTray()
{
    sysTray = new QSystemTrayIcon();

    QMenu *sysMenu = new QMenu();
    sysMenu->addAction(setting);
    sysMenu->addAction(setImg);
    sysMenu->addAction(setClose);

    sysTray->setContextMenu(sysMenu);
    sysTray->setIcon(QIcon(":/images/countTimeIcon.jpg"));

    sysTray->show();
}

Widget::~Widget()
{
    delete setDayTime;
    delete begin;
    delete time;
    delete day;

    delete menuBtn;

    delete innerWidget;

    delete setting;
    delete setImg;
    delete setDefaultImg;
    delete setClose;

    delete sysTray;
}
