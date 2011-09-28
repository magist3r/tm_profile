#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "profile.h"
#include "math.h"
#include "paint_area.h"
#include <QSettings>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label_dx->setText(QString::number(ui->dx->value() / 100.0,'f',2)); // Установка значения модификации в label_dx
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::load_vars()
{
    m = ui->m->value();
    z1 = ui->z1->value();
    z2 = ui->z2->value();
    x2 = ui->x2->value();
    E = ui->E->value() * M_PI / 180;
    W0 = ui->W0->value();
    alpha = ui->alpha->value() * M_PI / 180;
    ha = ui->ha->value();
    c = ui->c->value();
    z0 = ui->z0->value();
    x0 = ui->x0->value();
    da0 = ui->da0->value();
    i21 = z1 / z2;
    rb2 = 0.5 * m * z2 * cos(alpha);
    delta2 = atan(sin(E) / (cos(E) - i21));
    psi_b2 = M_PI / (2 * z2) + 2 * x2 * tan(alpha) / z2 + tan(alpha) - alpha;
    d0 = ui->d0->value();
    bw = ui->bw->value();
    ra2 = ui->ra2->value();
    rf2 = ui->rf2->value();
    dx = ui->dx->value() / 100.0;
    dx_0 = ui->dx_0->value();
    dx_bw = ui->dx_bw->value();
}

void MainWindow::on_pushButton_clicked()
{
    ui->m->setValue(2);
    ui->z1->setValue(34);
    ui->z2->setValue(40);
    ui->x2->setValue(2.21);
    ui->alpha->setValue(20);
    ui->c->setValue(0.25);
    ui->z0->setValue(19);
    ui->x0->setValue(0.105);
    ui->da0->setValue(43.42);
    ui->ha->setValue(1);
    ui->E->setValue(9);
    ui->W0->setValue(40.777);
    ui->d0->setValue(27);
    ui->bw->setValue(5);

    load_vars();
    radius();
    ui->ra2->setValue(ra2);
    ui->rf2->setValue(rf2);
    //ui->W1_lim->setValue(out_list["W1_lim"]);
    connect(ui->MainButton, SIGNAL(clicked()), ui->PaintContactArea, SLOT(update()));
    // ui->widget->paintEvent(this);
}

void MainWindow::on_MainButton_clicked()
{
    ui->ra2->setValue(42.435);
    load_vars();



}

/*void QWidget::paintEvent(QPaintEvent *)
{
     QPainter painter(this);
     painter.setPen(Qt::blue);
     painter.setFont(QFont("Arial", 30));
     painter.drawText(rect(), Qt::AlignCenter, "Qt");
}*/

void MainWindow::on_dx_valueChanged(int value)
{
    ui->label_dx->setText(QString::number(value / 100.0,'f',2)); // Установка значения модификации в label_dx
}

void MainWindow::saveProperties()
{
    QSettings settings("tm_profile", "zb-susu");
    settings.beginGroup("properties");
    const QString name = "m_" + QString::number(ui->m->value()) + "|z1_" + QString::number(ui->z1->value()) + "|z2_" + QString::number(ui->z2->value());
    settings.beginGroup(name);
    settings.setValue("MainWindow", saveState());
    settings.setValue("z1", ui->z1->value());
    settings.setValue("x2", ui->x2->value());
    settings.endGroup();
    settings.endGroup();

}

void MainWindow::loadProperties()
{
   QSettings settings("tm_profile", "zb-susu");
    settings.beginGroup("properties");
    const QString name = "m_" + QString::number(ui->m->value()) + "|z1_" + QString::number(ui->z1->value()) + "|z2_" + QString::number(ui->z2->value());
    settings.beginGroup(name);
    qDebug() << name;
    restoreState(settings.value("MainWindow").toByteArray());
    ui->z1->setValue(settings.value("z1").toInt());
    ui->x2->setValue(settings.value("x2").toDouble());
    settings.endGroup();
    settings.endGroup();

}



void MainWindow::on_pushButton_2_clicked()
{
    saveProperties();
}

void MainWindow::on_pushButton_3_clicked()
{
    loadProperties();
}
