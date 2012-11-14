/*
tm_profile - program for calculation of cylinder-bevel transmissions
Copyright (C) 2011 Sergey Lopatin

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <QtGui/QApplication>
#include <QtDeclarative>
#include <QMetaType>
#include "profile.h"
#include "imagegenerator.h"
#include "qmlapplicationviewer.h"
#include "mainwindow.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    QmlApplicationViewer viewer;
    QDeclarativeEngine engine;

    QCoreApplication::setApplicationName("tm_profile");
    QCoreApplication::setOrganizationName("zb-susu");

    QDir datadir(QDesktopServices::storageLocation(QDesktopServices::DataLocation));
    if (!datadir.exists())
        datadir.mkpath(datadir.path());

    Profile *profile = new Profile();
    ImageGenerator *generator = new ImageGenerator();
    QObject::connect(profile, SIGNAL(calculateFinished(QMap<double,QMap<double,double> >&,double,double,double,double,QString)),
                     generator, SLOT(paint(QMap<double,QMap<double,double> >&,double,double,double,double,QString)));

 //   engine.addImageProvider("images", generator);
    viewer.rootContext()->setContextProperty("imageGenerator", generator);
    viewer.rootContext()->setContextProperty("profile", profile);


 //   qmlRegisterType<Profile>("org.tm_profile.profile", 1, 0, "Profile");
    //qmlRegisterType<QThread>("org.tm_profile.thread", 1, 0, "Worker");


    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/tm_profile/main.qml"));
    viewer.showExpanded();

//    MainWindow w;
//    w.show();
    return app->exec();
}
