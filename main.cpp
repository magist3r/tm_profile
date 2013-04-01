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

#include <QtWidgets/QApplication>
#include <QtQml>
#include <QtQuick/QQuickView>
#include "profile.h"
#include "imagegenerator.h"


Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QQmlEngine engine;
    QQmlComponent component(&engine);

    QCoreApplication::setApplicationName("tm_profile");
    QCoreApplication::setOrganizationName("zb-susu");

    QDir datadir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    if (!datadir.exists())
        datadir.mkpath(datadir.path());

    QTranslator myappTranslator;
    myappTranslator.load(app.applicationName() + "_" + QLocale::system().name());
    qDebug() << app.applicationName() + "_" + QLocale::system().name();
    app.installTranslator(&myappTranslator);

    Profile *profile = new Profile();
    profile->setdataLocation(datadir.absolutePath());

    ImageGenerator *generator = new ImageGenerator();
    QObject::connect(profile, SIGNAL(calculateFinished(QMap<double,QMap<double,double> >&,double,double,double,double,QString)),
                     generator, SLOT(paint(QMap<double,QMap<double,double> >&,double,double,double,double,QString)));

    engine.rootContext()->setContextProperty("imageGenerator", generator);
    engine.rootContext()->setContextProperty("profile", profile);

    component.loadUrl(QUrl("qml/tm_profile/main.qml"));
    if ( !component.isReady() ) {
        qWarning("%s", qPrintable(component.errorString()));
        return -1;
    }

    QObject *topLevel = component.create();
    QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);

    if ( !window ) {
        qWarning("Error: Your root item has to be a Window.");
        return -1;
    }

    window->show();

    return app.exec();
}
