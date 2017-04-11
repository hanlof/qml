#ifndef DATAOBJECT_H
#define DATAOBJECT_H

#include <QObject>


class DataObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(qreal s READ s WRITE setS NOTIFY sChanged)
    Q_PROPERTY(qreal e READ e WRITE setE NOTIFY eChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    DataObject(QObject *parent=0);
    DataObject(const qreal &s, const qreal &e, QObject *parent = 0);
    DataObject(const qreal &s, const qreal &e, const QString &name, QObject *parent = 0);

    qreal s() const;
    void setS(const qreal &s);

    qreal e() const;
    void setE(const qreal &e);

    QString name() const;
    void setName(const QString &name);

signals:
    void sChanged();
    void eChanged();
    void nameChanged();

private:
    qreal m_s;
    qreal m_e;
    QString m_name;
};

Q_DECLARE_METATYPE( DataObject * );

#endif // DATAOBJECT_H
