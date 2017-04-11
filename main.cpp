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

// * sort out the starting view. get min/max values into QML somehow and zoom to relevant timespan
// * make sliders dynamic width
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
// *



int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    //TimeCollection tc;
    //tc.append(new DataObject(1.0, 2.5));
    //tc.times()->append(new DataObject(1.0, 2.5));

    QList<QObject*> dataList;
    /*
    dataList.append(new DataObject(2.1, 3.0));
    dataList.append(new DataObject(0.4, 3.5));
    dataList.append(new DataObject(1.5, 3.7));
    dataList.append(new DataObject(1.5, 4.7));
    dataList.append(new DataObject(1.5, 2.7));
    dataList.append(new DataObject(1.6, 2.5));
    dataList.append(new DataObject(1.5, 1.7));
    dataList.append(new DataObject(1.6, 1.9));
    dataList.append(new DataObject(1.5, 1.9));
    */

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
        dataList.append(new DataObject(1.5, 1.9));
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
            if (i < 5) {
                qInfo() << line;
                qInfo() << " " << j << " " << pid;
                qInfo() << " " << j << " " << time;
                qInfo() << " " << j << " " << rest;
                qInfo() << " " << j << " " << rx.cap(0) ;
            }
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
                qInfo() << exits << " exit " << pid;

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
                qInfo() << execs << " " << pid << " " << time << " " << rest << " " << execRx.cap(1);

                startTimes.insert(pid, time.toDouble());
                startNames.insert(pid, rest);
            }

            i++;
        }
        inputFile.close();
    } else {
     //   printf("%s\n", inputFile.errorString().toLocal8Bit());
    }


    QQmlApplicationEngine engine;

    QQmlContext * ctx = engine.rootContext();

    QVariant kalle = QVariant::fromValue(QList<QObject*>(dataList));

    //apa.setProperty("min", 7.8);
    ctx->setContextProperty("dataList", QVariant::fromValue(QList<QObject*>(dataList)));

    //setProperty("min", QVariant::fromValue(QString("kalle")));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
