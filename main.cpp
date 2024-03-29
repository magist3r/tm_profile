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
#include <QDebug>


Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;
    QQmlComponent component(&engine);

    QCoreApplication::setApplicationName("tm_profile");
    QCoreApplication::setOrganizationName("zb-susu");
    QCoreApplication::setApplicationVersion("1.0.0");

    // Store images next to executable to enable portability
    QDir datadir(QCoreApplication::applicationDirPath() + QDir::separator() + "images");
    if (!datadir.exists())
        datadir.mkpath(datadir.path());

    QTranslator myappTranslator;
    myappTranslator.load(app.applicationName() + "_" + QLocale::system().name(), QCoreApplication::applicationDirPath());
    app.installTranslator(&myappTranslator);

    Profile profile(&app);
    profile.setdataLocation(datadir.absolutePath());

    ImageGenerator generator(&app, datadir.absolutePath());
    QObject::connect(&profile, SIGNAL(calculateFinished(QMap<double,QMap<double,double> >&,double,double,double,double,QString)),
                     &generator, SLOT(paint(QMap<double,QMap<double,double> >&,double,double,double,double,QString)));

    engine.rootContext()->setContextProperty("imageGenerator", &generator);
    engine.rootContext()->setContextProperty("profile", &profile);

    const QUrl url(QStringLiteral("qrc:/qml/tm_profile/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
