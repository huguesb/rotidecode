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

#include <QAbstractListModel>
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
class Buffers : public QAbstractListModel {
    Q_OBJECT
public:
    // extra data roles offered through the model API
    enum Roles {
        PathRole = Qt::UserRole,        //!< string: absolute path
        ModificationRole,               //!< bool: is the buffer modified?
        OpenViewsRole,                  //!< int: number of views
    };
    
    Buffers(QObject *p = 0);
    ~Buffers();
    
    QString create();
    void load(const QString& fileName);
    
    qce::Editor* acquire(const QString& fileName);
    void release(qce::Editor *editor);
    
    bool isUntitled(qce::Editor *e) const;
    
    // model api
    virtual int rowCount(const QModelIndex& parent) const;
    virtual QVariant data(const QModelIndex& index, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex& index) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    
public slots:
    void close(const QString& fileName);
    
signals:
    void closeRequested(qce::Editor *e);
    
private slots:
    void cleanChanged(bool clean);
    void fileNameChanged(QString oldFileName, QString newFileName);
    
private:
    Buffer* createBuffer(QString name);
    void close(int idx);
    
    void emitDataChanged(int idx);
    
    quint64 m_untitled;
    // alphabetically sorted by fileName
    QList<Buffer*> m_buffers;
};

#endif  // BUFFERS_H_
