#ifndef SET_H
#define SET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QCalendarWidget>
#include <QRadioButton>
#include <QPushButton>
#include <QString>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSettings>
#include <QDate>
#include <QVariant>
#include <QFileInfo>
#include <QMessageBox>
#include <QSlider>

//#include "bridge.h"

//设置回调函数类型
typedef void(*onCallBack)(int tranValue, bool powerOn, QDate timeString,QString beginString,void *context);

class set : public QWidget
{
    Q_OBJECT
private:
    //ui部件
    //节假日
    QLabel *setBeginText;
    QLineEdit *setString;
    //开机自启
    QRadioButton *poweronAction;
    //透明度
    QSlider *tranControl;
    QLabel *tranValue;
    //日期
    QCalendarWidget *dateSelect;
    //确认按钮
    QPushButton *configBtn;

    //配置文件对象
    //QSettings countTimeSetting;

    //初始化ui
    void initUI();
    void initSlots();
    void getStateForUI();

    //回调函数对象以及上下文
    onCallBack m_cb;
    void *m_context;

private slots:
    void onConfigBtnClick();
    void onTranControlValueChange();


public:
    QString getDataCount();
    QString getBeginString();


    //注册回调函数
    void registerCallBack(onCallBack cb, void *context);

    set(QWidget *parent = 0);
    ~set();

protected:

};

#endif // SET_H
