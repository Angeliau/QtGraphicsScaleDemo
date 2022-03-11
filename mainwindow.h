#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include "imagewidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void loadConfig();
    void showImage();

private:
    Ui::MainWindow *ui;
    ImageWidget  *m_Image;
    QMap<QString,QString> form;
    QMap<QString,QString> parseMap;
public:
    void parseReport(const QString& report, QMap<QString,QString>& parseMap);
    void  showPicture(QGraphicsView* view, QImage image);

};

#endif // MAINWINDOW_H
