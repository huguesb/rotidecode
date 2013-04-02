/****************************************************************************
** Copyright (c) 2013 Hugues Luc Bruant <hugues@qcodeedit.org>
** All rights reserved.
**
** This file is part of RotiDeCode
**
** This file may be used under the terms of the GNU General Public License
** version 3 as published by the Free Software Foundation and appearing in the
** file GPL.txt included in the packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
****************************************************************************/

#ifndef BUFFERS_H_
#define BUFFERS_H_

#include <QObject>
#include <QHash>
#include <QString>

class Buffer;

namespace qce {
class Editor;
class Document;
};

/*!
 * \class Buffers
 * \brief Manages open buffers
 */
class Buffers : public QObject {
    Q_OBJECT
public:
    Buffers(QObject *p = 0);
    ~Buffers();
    
    QString create();
    void load(const QString& fileName);
    
    qce::Editor* acquire(const QString& fileName);
    void release(qce::Editor *editor);
    
    bool isUntitled(qce::Editor *e) const;
    
signals:
    void closeRequested(qce::Editor *e);
    
private slots:
    void fileNameChanged(QString oldFileName, QString newFileName);
    
private:
    Buffer* createBuffer(QString name);
    
    quint64 m_untitled;
    QHash<QString, Buffer*> m_buffers;
};

#endif  // BUFFERS_H_
