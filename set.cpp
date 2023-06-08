#include "set.h"

set::set(QWidget *parent): QWidget(parent)
{
    this->initUI();
}

void set::initUI()
{
    this->setWindowFlags(Qt::Tool);
    //距离之后的节日
    setBeginText = new QLabel();
    setString = new QLineEdit();

    //开机自启选项
    poweronAction = new QRadioButton();

    //透明度控制器
    QLabel *tranTitle = new QLabel();
    tranControl = new QSlider();
    tranValue = new QLabel();

    //日期选择器
    dateSelect = new QCalendarWidget();

    //确认按钮
    configBtn = new QPushButton();

    setBeginText->setText(tr("节日"));
    poweronAction->setText(tr("开机自启"));
    configBtn->setText(tr("确认"));

    tranTitle->setText(tr("透明度："));
    tranControl->setMaximum(100);
    tranControl->setMinimum(0);
    tranControl->setSingleStep(1);
    tranControl->setTracking(true);
    tranControl->setTickPosition(QSlider::TicksRight);
    tranControl->setOrientation(Qt::Horizontal);

    this->getStateForUI();

    //水平布局
    QHBoxLayout *beginTextLayout = new QHBoxLayout();
    beginTextLayout->addWidget(setBeginText);
    beginTextLayout->addWidget(setString);

    QHBoxLayout *tranLayout = new QHBoxLayout();
    tranLayout->addWidget(tranTitle);
    tranLayout->addWidget(tranControl);
    tranLayout->addWidget(tranValue);

    //垂直布局

    QVBoxLayout *allLayout = new QVBoxLayout();
    allLayout->addLayout(beginTextLayout);
    allLayout->addWidget(poweronAction);
    allLayout->addLayout(tranLayout);
    allLayout->addWidget(dateSelect);
    allLayout->addWidget(configBtn);

    //连接槽函数
    connect(configBtn,SIGNAL(clicked(bool)),this,SLOT(onConfigBtnClick()));
    connect(tranControl,SIGNAL(valueChanged(int)),this,SLOT(onTranControlValueChange()));


    setLayout(allLayout);

}

void set::getStateForUI()
{
    QFileInfo fileInfo = QFileInfo(":/images/countTimeSetting.ini");
    if(!fileInfo.isFile())
    {
        QMessageBox::critical(NULL,tr("错误"),tr("缺少配置文件"));
        return;
    }

    QSettings countTimeSetting(":/images/countTimeSetting.ini",QSettings::IniFormat);
    countTimeSetting.setIniCodec("utf-8");

    //开机自启
    QVariant poweronValue = countTimeSetting.value("set/poweron");
    poweronAction->setChecked(poweronValue.toBool());

    //设置的节日名称
    QVariant begin = countTimeSetting.value("set/beginText");
    setString->setText(begin.toString());

    //设置透明度
    QVariant tranInConfig = countTimeSetting.value("set/tran");
    tranControl->setValue(tranInConfig.toInt());
    tranValue->setText(QString::number(tranInConfig.toInt(),10));

    //设置日期选择器
    QVariant date = countTimeSetting.value("set/targetData");
    dateSelect->setSelectedDate(date.toDate());

}

void set::onConfigBtnClick()
{
    QSettings countTimeSetting(":/images/countTimeSetting.ini",QSettings::IniFormat);
    countTimeSetting.setValue("set/poweron",poweronAction->isChecked());
    countTimeSetting.setValue("set/beginText",setString->text());
    countTimeSetting.setValue("set/tran",tranControl->value());
    countTimeSetting.setValue("set/targetData",dateSelect->selectedDate());
    //QDate date = dateSelect->selectedDate();
    if(m_cb)
    {
        m_cb(tranControl->value(),poweronAction->isChecked(),dateSelect->selectedDate(),setString->text(),m_context);
    }
    this->close();
}

void set::onTranControlValueChange()
{
    tranValue->setText(QString::number(tranControl->value(),10));
}

void set::registerCallBack(onCallBack cb, void *context)
{
    m_cb = cb;
    m_context = context;
}

set::~set()
{

}
