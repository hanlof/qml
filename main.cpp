#include <QObject>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "dataobject.h"
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QDebug>
#include <QHash>

// DONE make sliders dynamic width
// DONE sort out the starting view. get min/max values into QML somehow and zoom to relevant timespan
// * make a better interface for changing displayed timespan
// * group rows by parent pid somehow
// * wrap it up in a script that runs strace
// * add parsing for clone() and whatever else is needed to display every process that gets run
// * make the text show up in a nicer way. consider mouseover balloon text boxes
// * add to github :)
// * rename project :D
// * show wait() and maybe other syscalls within the bar for each process
// * do gridlines
// * display times in the gui somehow, perhaps on an user action (mouseover or clicking a process box)

// Try some layouts in pure xml

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    //TimeCollection tc;
    //tc.append(new DataObject(1.0, 2.5));
    //tc.times()->append(new DataObject(1.0, 2.5));

    QList<QObject*> dataList;


    QHash<QString, QString> unfinished;
    QHash<QString, double> startTimes;
    QHash<QString, QString> startNames;
    QString digits("(\\d+)");
    QString whitespace("\\s+");
    QString decimal("(\\d+\\.\\d+)");
    QString letters("(.*$)");
    QRegExp rx(digits + whitespace + decimal + whitespace + letters);
    QFile inputFile("/home/osboxes/log2.strace");
    int execs = 0;
    int exits = 0;
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        int i = 0;
        while (!in.atEnd())
        {
            QString line = in.readLine();
            //"^(\d\d):(\d\d):(\d\d)(?:\.(\d\d\d\d\d\d))?""
            int j = rx.indexIn(line);
            QString pid(rx.cap(1));
            QString time(rx.cap(2));
            QString rest(rx.cap(3));
            /*
            if (i < 5) {
                qInfo() << line;
                qInfo() << " " << j << " " << pid;
                qInfo() << " " << j << " " << time;
                qInfo() << " " << j << " " << rest;
                qInfo() << " " << j << " " << rx.cap(0) ;
            }
            i++;
            */
            if (QRegExp("^(.*) <unfinished \\.\\.\\.>$").indexIn(rest) > -1) {
          //      qInfo() << pid << " unfinished " << rest;
                unfinished.insert(pid, rest);
                unfinished.insert("apa", "kalle");
                continue;
            }
            if (QRegExp("\\.\\.\\. \\S+ resumed> ").indexIn(rest) > -1) {
                if (unfinished.contains(pid)) {
                    QString line( unfinished.value(pid) + rest );
   //                 qInfo() << pid << " resumed " << line;
                    rest = line;
                    unfinished.remove(pid);
                } else {
                    qInfo() << "resumed unknown call " << unfinished.value(pid);
                    continue;
                }
            }


            if (QRegExp("^\\+\\+\\+ killed by SIG(\\S+) \\+\\+\\+$").indexIn(rest) > -1) {
                // for later: handle_killed(pid, time);
                continue;
            }

            if (QRegExp("^\\+\\+\\+ exited with (\\d+) \\+\\+\\+$").indexIn(rest) > -1) {
                exits++;
          //      qInfo() << exits << " exit " << pid;

                if (startTimes.contains(pid)) {
                    dataList.append(new DataObject( startTimes.value(pid), time.toDouble(), startNames.value(pid) ) );
                }
                //qInfo() << pid << "exited with " << rest;
                // No need to handle(?)
                continue;
            }

            QRegExp execRx("execve.*= 0 <(.*)>$");
            if (execRx.indexIn(rest) > -1) {
                execs++;
           //     qInfo() << execs << " " << pid << " " << time << " " << rest << " " << execRx.cap(1);

                startTimes.insert(pid, time.toDouble());
                startNames.insert(pid, rest);
            }

        }
        inputFile.close();
    } else {
     //   printf("%s\n", inputFile.errorString().toLocal8Bit());
    }


    QQmlApplicationEngine engine;

    QQmlContext * ctx = engine.rootContext();

    QVariant kalle = QVariant::fromValue(QList<QObject*>(dataList));

    qreal min = dataList.at(0)->property("s").toDouble();
    qreal max = dataList.at(0)->property("e").toDouble();
    foreach(QObject *o, dataList) {
        if (o->property("s").toDouble() < min) {
            min = o->property("s").toDouble();
        }
        if (o->property("s").toDouble() > max) {
            max = o->property("e").toDouble();
        }
       // qInfo() << "min" << min;
       // qInfo() << "max" << max;
    }

    /*
    min -= (max - min) * 0.02;
    max += (max - min) * 0.02;
    */
    DataObject minMax(min,max,"apa");
    //std::sort(dataList.begin(), dataList.end());
    ctx->setContextProperty("dataList", QVariant::fromValue(QList<QObject*>(dataList)));
    ctx->setContextProperty("minMax", &minMax);

    //setProperty("min", QVariant::fromValue(QString("kalle")));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
