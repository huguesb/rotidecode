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

#include "buffers.h"

#include <document.h>
#include <editor.h>
#include <syntaxhighlighter.h>

#include <QFileInfo>
#include <QSet>
#include <QDebug>

struct Buffer {
    qce::Document *document;
    qce::Editor* released;
    QSet<qce::Editor*> acquired;
    
    // must duplicate to handle update properly
    QString fileName;
    
    inline Buffer(qce::Document *d)
    : document(d), released(0), fileName(d->fileName()) {}
};

namespace {

template <typename T, typename K>
const K& KeyExtractor(const T& t) {
    return (K)t;
}

template<>
const QString& KeyExtractor<Buffer*, QString>(Buffer* const& buffer) {
    return buffer->fileName;
}

template <typename T, typename K>
int lowerBound(const QList<T>& l, const K& k) {
    int min = 0, max = l.count();
    while (min < max) {
        int mid = (min + max) / 2;
        const T& t = l.at(mid);
        if (KeyExtractor<T, K>(t) < k) {
            min = mid + 1;
        } else {
            max = mid;
        }
    }
    return min;
}

template <typename T, typename K>
const T& find(QList<T>& l, const K& k, const T& valueIfAbsent = T()) {
    int idx = lowerBound(l, k);
    if (idx == l.count()) return valueIfAbsent;
    const T& v = l.at(idx);
    return KeyExtractor<T, K>(v) == k ? v : valueIfAbsent;
}

template <typename T, typename K>
void insert(QList<T>& l, const T& t, const K& k) {
    l.insert(lowerBound(l, k), t);
}

}  // namespace

Buffers::Buffers(QObject *p)
    : QAbstractListModel(p)
    , m_untitled(0) {
    m_panelLayout[qce::West]
            << "qce.panels.LineMark"
            << "qce.panels.LineNumber"
            << "qce.panels.Fold";
}

Buffers::~Buffers() {
    qDeleteAll(m_buffers);
}

Buffer* Buffers::createBuffer(QString name) {
    Buffer *b = new Buffer(new qce::Document(name, this));
    b->document->setHighlighter(qce::SyntaxHighlighter::fromFilename(name));
    connect(b->document, SIGNAL( cleanChanged(bool) ),
            this       , SLOT  ( cleanChanged(bool) ));
    connect(b->document, SIGNAL( fileNameChanged(QString, QString) ),
            this       , SLOT  ( fileNameChanged(QString, QString) ));
    
    int idx = lowerBound(m_buffers, name);
    beginInsertRows(QModelIndex(), idx, idx);
    m_buffers.insert(idx, b);
    endInsertRows();
    return b;
}

QString Buffers::create() {
    QString name = tr("untitled %1").arg(++m_untitled);
    createBuffer(name);
    return name;
}

void Buffers::load(const QString& fileName) {
    Buffer* b = find(m_buffers, fileName);
    
    if (!b) {
        b = createBuffer(fileName);
        b->document->load(fileName);
    }
}

qce::Editor* Buffers::acquire(const QString& fileName) {
    Buffer* b = find(m_buffers, fileName);
    Q_ASSERT(b);
    
    qce::Editor *e = b->released;
    if (!e) {
        e = new qce::Editor(b->document);
        e->addPanels(m_panelLayout);
    }
    b->acquired.insert(e);
    b->released = 0;
    return e;
}

void Buffers::release(qce::Editor *e) {
    if (!e) return;
    
    QString fileName = e->fileName();
    Buffer *b = find(m_buffers, fileName);
    Q_ASSERT(b && b->acquired.contains(e));
    b->acquired.remove(e);
    if (b->released) b->released->deleteLater();
    b->released = e;
}

bool Buffers::isUntitled(qce::Editor *e) const {
    return e->fileName().startsWith(tr("untitled "));
}

void Buffers::close(const QString& fileName) {
    int idx = lowerBound(m_buffers, fileName);
    if (idx < m_buffers.count() &&
        m_buffers.at(idx)->fileName == fileName) {
        close(idx);
    }
}

void Buffers::editorOpened(qce::Editor *e) {
    QString fileName = e->fileName();
    Buffer *b = find(m_buffers, fileName);
    Q_ASSERT(b && !b->acquired.contains(e));
    Q_ASSERT(e->document() == b->document);
    b->acquired.insert(e);
}

void Buffers::cleanChanged(bool clean) {
    Q_UNUSED(clean);
    const qce::Document *d = qobject_cast<const qce::Document*>(sender());
    int idx = lowerBound(m_buffers, d->fileName());
    Q_ASSERT(idx < m_buffers.count() && m_buffers.at(idx)->document == d);
    emitDataChanged(idx);
}

void Buffers::fileNameChanged(QString oldFileName, QString newFileName) {
    int idx = lowerBound(m_buffers, oldFileName);
    Q_ASSERT(idx < m_buffers.count());
    Buffer *b = m_buffers.at(idx);
    Q_ASSERT(b->fileName == oldFileName);
    
    beginRemoveRows(QModelIndex(), idx, idx);
    m_buffers.removeAt(idx);
    endRemoveRows();
    
    int newIdx = lowerBound(m_buffers, newFileName);
    
    if (newIdx < m_buffers.count() &&
        m_buffers.at(newIdx)->fileName == newFileName) {
        // 2 documents pointing to same file...
        // TODO: rename to non-conflicting name instead of forcefully closing
        close(newIdx);
    }
    
    beginInsertRows(QModelIndex(), idx, idx);
    b->fileName = newFileName;
    m_buffers.insert(newIdx, b);
    endInsertRows();
}

void Buffers::close(int idx) {
    Buffer *b = m_buffers.at(idx);
    qDebug("close: %i %s", idx, qPrintable(b->fileName));
    
    if (b->released) {
        b->released->deleteLater();
        b->released = 0;
    }
    
    if (!b->document->isClean()) {
        // TODO: offer the opportunity to save
    }
    
    // need to iterate on a copy:
    // closeRequested -> editor is closed -> release called
    QSet<qce::Editor*> acquired = b->acquired;
    foreach (qce::Editor *e, acquired) {
        emit closeRequested(e);
        b->released = 0;
    }
    b->document->deleteLater();
    
    // must remove from model last to allow release to work
    beginRemoveRows(QModelIndex(), idx, idx);
    m_buffers.removeAt(idx);
    endRemoveRows();
    
    delete b;
}

QModelIndex Buffers::indexForName(const QString& name) const {
    int idx = lowerBound(m_buffers, name);
    Buffer *b = idx < m_buffers.count() ? m_buffers.at(idx) : 0;
    return b && b->fileName == name ? index(idx, 0, QModelIndex()) : QModelIndex();
}

int Buffers::rowCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : m_buffers.count();
}

QVariant Buffers::data(const QModelIndex& index, int role) const {
    int row = index.row();
    if (!index.isValid() || row < 0 || row >= m_buffers.count()) {
        return QVariant();
    }
    
    const Buffer *buffer = m_buffers.at(row);
    switch (role) {
    case Qt::DisplayRole:
        return QFileInfo(buffer->fileName).fileName()
        + (buffer->document->isClean() ? "" : "*");
    case Qt::ToolTipRole:
        return buffer->fileName;
    case PathRole:
        return buffer->fileName;
    case ModificationRole:
        return !buffer->document->isClean();
    case OpenViewsRole:
        return buffer->acquired.count();
    default:
        return QVariant();
    }
}

Qt::ItemFlags Buffers::flags(const QModelIndex& index) const {
    return index.isValid() ? Qt::ItemIsSelectable | Qt::ItemIsEnabled : Qt::NoItemFlags;
}

QVariant Buffers::headerData(int section, Qt::Orientation orientation, int role) const {
    Q_UNUSED(section);
    Q_UNUSED(orientation);
    Q_UNUSED(role);
    return QVariant();
}

void Buffers::emitDataChanged(int idx) {
    QModelIndex midx = index(idx, 0, QModelIndex());
    emit dataChanged(midx, midx);
}

