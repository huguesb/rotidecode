/*******************************************************************************
 * Copyright (c) 2013, Hugues Luc Bruant <hugues@qcodeedit.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

#ifndef BUFFERS_H_
#define BUFFERS_H_

#include <qce-config.h>

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
    
    QStringList fileNames() const;
    
    QString create();
    
    qce::Editor* acquire(const QString& fileName);
    
    bool isUntitled(const qce::Editor *const e) const;
    
    // model api
    QModelIndex indexForName(const QString& name) const;
    
    virtual int rowCount(const QModelIndex& parent) const;
    virtual QVariant data(const QModelIndex& index, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex& index) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    
public slots:
    void load(const QString& fileName);
    void close(const QString& fileName);
    
    void release(qce::Editor *editor);
    
signals:
    void closeRequested(qce::Editor *e);
    
private slots:
    void editorOpened(qce::Editor *e);
    
    void cleanChanged(bool clean);
    void fileNameChanged(QString oldFileName, QString newFileName);
    
private:
    Buffer* createBuffer(QString name);
    void close(int idx);
    
    void emitDataChanged(int idx);
    
    quint64 m_untitled;
    // alphabetically sorted by fileName
    QList<Buffer*> m_buffers;
    
    QMap<qce::PanelPosition, QStringList> m_panelLayout;
};

#endif  // BUFFERS_H_
