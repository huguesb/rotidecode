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

#include "buffers.h"

#include <document.h>
#include <editor.h>
#include <syntaxhighlighter.h>

#include <QSet>

struct Buffer {
    qce::Document *document;
    qce::Editor* released;
    QSet<qce::Editor*> acquired;
    
    inline Buffer(qce::Document *d)
    : document(d), released(0) {}
};

Buffers::Buffers(QObject *p)
    : QObject(p)
    , m_untitled(0) {
}

Buffers::~Buffers() {
    qDeleteAll(m_buffers);
}

Buffer* Buffers::createBuffer(QString name) {
    Buffer *b = new Buffer(new qce::Document(name, this));
    b->document->setHighlighter(qce::SyntaxHighlighter::fromFilename(name));
    connect(b->document, SIGNAL( fileNameChanged(QString, QString) ),
            this       , SLOT  ( fileNameChanged(QString, QString) ));
    m_buffers.insert(name, b);
    return b;
}

QString Buffers::create() {
    QString name = tr("untitled %1").arg(++m_untitled);
    createBuffer(name);
    return name;
}

void Buffers::load(const QString& fileName) {
    Buffer* b = m_buffers.value(fileName);
    
    if (!b) {
        b = createBuffer(fileName);
        b->document->load(fileName);
    }
}

qce::Editor* Buffers::acquire(const QString& fileName) {
    Buffer* b = m_buffers.value(fileName);
    Q_ASSERT(b);
    
    qce::Editor *e = b->released;
    if (!e) e = new qce::Editor(b->document);
    b->acquired.insert(e);
    b->released = 0;
    return e;
}

void Buffers::release(qce::Editor *e) {
    QString fileName = e->fileName();
    Buffer *b = m_buffers.value(fileName);
    Q_ASSERT(b && b->acquired.contains(e));
    b->acquired.remove(e);
    if (b->released) b->released->deleteLater();
    b->released = e;
}

bool Buffers::isUntitled(qce::Editor *e) const {
    return e->fileName().startsWith(tr("untitled "));
}

void Buffers::fileNameChanged(QString oldFileName, QString newFileName) {
    Buffer *b = m_buffers.value(oldFileName);
    Q_ASSERT(b);
    m_buffers.remove(oldFileName);
    
    Buffer *c = m_buffers.value(newFileName);
    if (c) {
        // 2 documents pointing to same file...
        // TODO: offer the opportunity to rename?
        if (c->released) c->released->deleteLater();
        if (!c->document->isClean()) {
            // TODO: offer the opportunity to save
        }
        foreach (qce::Editor *e, c->acquired) {
            emit closeRequested(e);
        }
        c->document->deleteLater();
        delete c;
    }
    
    m_buffers.insert(newFileName, b);
}

