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
  //  ui->label_dx->setText(QString::number(ui->dx->value() / 100.0,'f',2)); // Установка значения модификации в label_dx
    connect(ui->MainButton, SIGNAL(clicked()), ui->PaintContactArea, SLOT(update()));
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
    dx = ui->dx->value();
    dx_0 = ui->dx_0->value();
    dx_bw = ui->dx_bw->value();
}

void MainWindow::on_pushButton_clicked()
{
    load_vars();
    radius();
    ui->ra2->setValue(ra2);
    ui->rf2->setValue(rf2);
}

void MainWindow::on_MainButton_clicked()
{
    load_vars();
}

void MainWindow::saveProperties()
{
    QSettings settings("tm_profile", "zb-susu");
    settings.beginGroup("properties");
    const QString name = "m_" + QString::number(ui->m->value()) +
                         "|z1_" + QString::number(ui->z1->value()) +
                         "|z2_" + QString::number(ui->z2->value()) +
                         "|bw_" + QString::number(ui->bw->value());
    settings.beginGroup(name);
    settings.setValue("MainWindow", saveState());
    settings.setValue("m", ui->m->value());
    settings.setValue("z1", ui->z1->value());
    settings.setValue("z2", ui->z2->value());
    settings.setValue("x0", ui->x0->value());
    settings.setValue("x2", ui->x2->value());
    settings.setValue("alpha", ui->alpha->value());
    settings.setValue("c", ui->c->value());
    settings.setValue("z0", ui->z0->value());
    settings.setValue("x0", ui->x0->value());
    settings.setValue("da0", ui->da0->value());
    settings.setValue("ha", ui->ha->value());
    settings.setValue("E", ui->E->value());
    settings.setValue("W0", ui->W0->value());
    settings.setValue("d0", ui->d0->value());
    settings.setValue("bw", ui->bw->value());
    settings.setValue("ra2", ui->ra2->value());
    settings.setValue("rf2", ui->rf2->value());
    settings.setValue("dx", ui->dx->value());
    settings.setValue("dx_0", ui->dx_0->value());
    settings.setValue("dx_bw", ui->dx_bw->value());
    settings.endGroup();
    settings.endGroup();

}

void MainWindow::loadProperties()
{
   QSettings settings("tm_profile", "zb-susu");
    settings.beginGroup("properties");
    QStringList groups = settings.childGroups();
    for (int i = 0; i < groups.count(); i++)
    {
        qDebug() << groups[i];
    }
    const QString name = "m_" + QString::number(ui->m->value()) +
                         "|z1_" + QString::number(ui->z1->value()) +
                         "|z2_" + QString::number(ui->z2->value()) +
                         "|bw_" + QString::number(ui->bw->value());
    settings.beginGroup(name);
    qDebug() << name;
    restoreState(settings.value("MainWindow").toByteArray());
    ui->m->setValue(settings.value("m").toDouble());
    ui->z1->setValue(settings.value("z1").toDouble());
    ui->z2->setValue(settings.value("z2").toDouble());
    ui->x0->setValue(settings.value("x0").toDouble());
    ui->x2->setValue(settings.value("x2").toDouble());
    ui->alpha->setValue(settings.value("alpha").toDouble());
    ui->c->setValue(settings.value("c").toDouble());
    ui->z0->setValue(settings.value("z0").toDouble());
    ui->da0->setValue(settings.value("da0").toDouble());
    ui->ha->setValue(settings.value("ha").toDouble());
    ui->E->setValue(settings.value("E").toDouble());
    ui->W0->setValue(settings.value("W0").toDouble());
    ui->d0->setValue(settings.value("d0").toDouble());
    ui->bw->setValue(settings.value("bw").toDouble());
    ui->ra2->setValue(settings.value("ra2").toDouble());
    ui->rf2->setValue(settings.value("rf2").toDouble());
    ui->dx->setValue(settings.value("dx").toDouble());
    ui->dx_0->setValue(settings.value("dx_0").toDouble());
    ui->dx_bw->setValue(settings.value("dx_bw").toDouble());
    settings.endGroup();
    settings.endGroup();

}

void MainWindow::on_pushButton_2_clicked()
{
    if (ui->m->value() == 0 || ui->z1->value() == 0 || ui->z2->value() == 0 || ui->bw->value() == 0)
    {

    }
    else
    {
        saveProperties();
    }

}

void MainWindow::on_pushButton_3_clicked()
{
    if (ui->m->value() == 0 || ui->z1->value() == 0 || ui->z2->value() == 0 || ui->bw->value() == 0)
    {

    }
    else
    {
        loadProperties();
    }
}
