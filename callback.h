
#ifndef CALLBACK_H_
#define CALLBACK_H_

#include <QPointer>
#include <QMetaObject>
#include <QMetaMethod>

/*!
 * \brief A copyable bound signal
 */
template <typename T>
class Callback {
public:
    Callback() : m_method(-1) {}
    
    Callback(QObject *o, const char *methodSignature)
        : m_method(o->metaObject()->indexOfMethod(QMetaObject::normalizedSignature(methodSignature).constData()))
        , m_recv(o) {
        qDebug("%s : %i", methodSignature, m_method);
    }
    
    Callback(const Callback& c) : m_method(c.m_method), m_recv(c.m_recv) {}
    
    Callback& operator = (const Callback& c) {
        m_recv = c.m_recv;
        m_method = c.m_method;
        return *this;
    }
    
    void call(const T& t) {
        if (!m_recv || m_method == -1) {
            qWarning("invalid callback: %p %i", (void*)m_recv, m_method);
            return;
        }
        m_recv->metaObject()->method(m_method).invoke(m_recv, Q_ARG(T, t));
    }
    
private:
    int m_method;
    QPointer<QObject> m_recv;
};

#endif  // CALLBACK_H_
