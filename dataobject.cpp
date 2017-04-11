#include "dataobject.h"

DataObject::DataObject(QObject *parent) : QObject(parent)
{

}

DataObject::DataObject(const qreal &s, const qreal &e, QObject *parent)
    : QObject(parent), m_s(s), m_e(e)
{
}

DataObject::DataObject(const qreal &s, const qreal &e, const QString &name, QObject *parent)
    : QObject(parent), m_s(s), m_e(e), m_name(name)
{
}

qreal DataObject::s() const
{
    return m_s;
}

void DataObject::setS(const qreal &s)
{
    if (s != m_s) {
        m_s = s;
        emit sChanged();
    }
}

qreal DataObject::e() const
{
    return m_e;
}

void DataObject::setE(const qreal &e)
{
    if (e != m_e) {
        m_e = e;
        eChanged();
    }
}

QString DataObject::name() const
{
    return m_name;
}

void DataObject::setName(const QString &name)
{
    m_name = name;
    nameChanged();
}
