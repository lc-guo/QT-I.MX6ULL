#include "mainwindow.h"
#include <QFontDatabase>
#include <QApplication>

int scrollBarValue = 0;

int brightness = 6;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    int fontId = QFontDatabase::addApplicationFont(":/fonts/Monaco.ttf");
    // 检查字体是否成功加载
    if (fontId != -1) {
        // 获取新加载的字体的家族名称列表
        QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);

        // 假设我们只加载了一个字体，或者只关心第一个字体家族
        if (!fontFamilies.isEmpty()) {
            QString familyName = fontFamilies.first();

            // 创建字体对象并设置属性
           QFont font(familyName);
            font.setPointSize(10); // 设置字体大小

           // 设置应用程序的全局字体
           a.setFont(font);
        }
    }
    else{
        printf("load font error\r\n");
    }

    w.show();

    return a.exec();
}
