#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //set default value
    parseMap["field1"] = "field1_value";
    parseMap["field2"] = "field2_value";
    parseMap["field3"] = "field3_value";
    parseMap["image"] = "/home/test/image_value";
    connect(ui->actionLoadCfg,&QAction::triggered,this,&MainWindow::loadConfig);
    connect(ui->showImageButton,&QPushButton::clicked, this, &MainWindow::showImage);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showImage(){
    QImage image("/home/images/1.jpg");
    showPicture(ui->ImageGraphic, image);
}

void MainWindow::showPicture(QGraphicsView *view, QImage image)
{
    //The QPixmap class is an off-screen image representation that can be used as a paint device
    QPixmap ConvertPixmap=QPixmap::fromImage(image);
    //To use QGraphicsview, it must be matched with QGraphicsscene
    QGraphicsScene* qgraphicsScene = new QGraphicsScene;
    m_Image = new ImageWidget(&ConvertPixmap);
    int width = view->width();
    int height = view->height();
    m_Image->setQGraphicsViewWH(width,height);
    qgraphicsScene->addItem(m_Image);
    //The size of the window is fixed at the original size and will not be enlarged with the enlargement of the picture
    // (in the default state, when the picture is enlarged, the scroll bars will appear on both sides of the window automatically,
    // and the field of view in the window will become larger),
    // so as to prevent the window from being too large to observe the picture when the picture is enlarged and reduced again
    view->setSceneRect(QRectF(-(width/2),-(height/2),width,height));
    //Sets the current scene to scene. If scene is already being viewed, this function does nothing.
    view->setScene(qgraphicsScene);
    //Sets the focus of the interface to the current graphics view control
    view->setFocus();
}

void MainWindow::parseReport(const QString& report, QMap<QString,QString>& parseMap){
    // TODO parse report
}

void MainWindow::loadConfig(){
    QString filename = QFileDialog::getOpenFileName(this,tr("Choose a config file"),".",
                                 tr("cfg file(*.cfg);;txt file(*.txt)"));

    QFile file(filename);
    int lineNum = 0;
    if (file.open(QIODevice::ReadOnly|QIODevice::Text)){
        while (!file.atEnd()) {
            QByteArray line = file.readLine();
            QString lineContent = QString(line).trimmed();
            if (lineContent == ""){
                qDebug() << "blank line,skip...";
                continue;
            }
            lineNum++;
            if (lineContent.count("=") != 1){
                qWarning() << "Parse config error in Line (" << lineNum << ")" << lineContent;
                continue;
            }
            if (lineContent.split("=")[0] == ""){
                qDebug() << "key is empty in Line (" << lineNum << ")" << lineContent;
                continue;
            }
            form.insert(lineContent.split("=")[0],lineContent.split("=")[1]);
        }
        file.close();
    }
    qDebug() << "total line is " << lineNum;

    //count is used to record the empty items
    int count = 0;
    auto iter = form.constBegin();
    while(iter != form.constEnd()){
        if (iter.value() == ""){
            count++;
        }
        ++iter;
    }
    // num of configuration item
    if (form.size() != 4){
        count++;
    }
    qDebug() << "count=" << count;

    if (count > 0){
#if 0
        // TODO: need parse report
        QMap<QString,QString> parseMap;
        // parseReport(report,parseMap)

        //对比配置文件的值，和解析报告得到的值，不一样则以报告为准
        if (form[key] != parseMap[key]){
            form[key] = parseMap[key];
        }
#endif

        /* for test  */
        auto parseMapIter = parseMap.constBegin();
        while (parseMapIter != parseMap.constEnd()){
            // not find key
            if (form.find(parseMapIter.key()) == form.constEnd()){
                form[parseMapIter.key()] = parseMapIter.value();
            }else{
                // value is empty
                if (form[parseMapIter.key()] == ""){
                    qDebug() << "empty value found in " << parseMapIter.key() << "=" << form[parseMapIter.key()] << " ,use the default value";
                    form[parseMapIter.key()] = parseMapIter.value();
                }
            }
            ++parseMapIter;
        }
        /* for test */

    }

    ui->lineEdit1->setText(form[ui->label1->text()]);
    ui->lineEdit2->setText(form[ui->label2->text()]);
    ui->lineEdit3->setText(form[ui->label3->text()]);
    QImage image(form[ui->label4->text()]);
    showPicture(ui->graphicsView,image);
    QImage image2(form[ui->label5->text()]);
    showPicture(ui->graphicsView_2, image2);
    form.clear();

}

