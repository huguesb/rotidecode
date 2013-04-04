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

#include "commandbar.h"

#include "callback.h"
#include "searchproxymodel.h"

#include <QDebug>
#include <QKeyEvent>
#include <QLineEdit>
#include <QListView>
#include <QHBoxLayout>

CommandBar::CommandBar()
    : QWidget(0, Qt::Popup | Qt::FramelessWindowHint) {
    
    QBoxLayout *l = new QHBoxLayout;
    l->setContentsMargins(0, 0, 0, 0);
    m_command = new QLineEdit(this);
    l->addWidget(m_command);
    setLayout(l);
    
    m_proxy = new SearchProxyModel(this);
    
    m_results = new QListView(0);
    m_results->setBatchSize(10);
    m_results->setMovement(QListView::Static);
    m_results->setFlow(QListView::TopToBottom);
    m_results->setLayoutMode(QListView::Batched);
    m_results->setUniformItemSizes(true);
    m_results->setEditTriggers(QListView::NoEditTriggers);
    m_results->setSelectionMode(QListView::SingleSelection);
    m_results->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    m_results->setModel(m_proxy);
    
    m_results->installEventFilter(this);
    
    m_command->installEventFilter(this);
    connect(m_command, SIGNAL( textChanged(QString) ),
            this,      SLOT  ( textChanged(QString) ));
    
    hide();
}

CommandBar::~CommandBar() {
}

void CommandBar::search(QWidget *p, QAbstractItemModel *model, int role,
                        Callback<QString> callback) {
    m_callback = callback;
    
    m_proxy->setSourceModel(model);
    m_proxy->setSortRole(role);
    m_proxy->setFilterRole(role);
    
    // adjust geom
    QSize s = p->size();
    QPoint o = p->mapToGlobal(QPoint(0, 0));
    
    int w = qBound(100, s.width() / 2, qMin(400, s.width()));
    int h = fontMetrics().lineSpacing();
    int x = o.x() + (s.width() - w) / 2;
    int y = o.y() + (s.height() - h) / 2;
    
    setGeometry(x, y, w, h);
    
    show();
    m_command->setFocus();
}

void CommandBar::done() {
    m_results->hide();
    hide();
}

void CommandBar::textChanged(const QString& text) {
    // TODO: update search model and
    m_proxy->setSearch(text);
    
    int rows = m_proxy->rowCount(QModelIndex());
    if (rows > 0) {
        // TODO: adapt size to available screen space
        QRect r = frameGeometry();
        int w = r.width();
        int h = qMin((rows + 1) * fontMetrics().lineSpacing(), 500);
        int x = r.bottomLeft().x();
        int y = r.bottomLeft().y();
        m_results->setGeometry(x, y, w, h);
        m_results->setCurrentIndex(m_proxy->index(0, 0, QModelIndex()));
        m_results->show();
        m_command->setFocus();
    } else {
        m_results->hide();
    }
}

bool CommandBar::eventFilter(QObject *o, QEvent *e) {
    if (o == m_command) {
        if (e->type() == QEvent::FocusIn) {
            
        } else if (e->type() == QEvent::FocusOut) {
            if (!m_results->hasFocus()) done();
        } else if (e->type() == QEvent::KeyPress) {
            QKeyEvent *ke = static_cast<QKeyEvent*>(e);
            if (ke->key() == Qt::Key_Escape) {
                done();
                return true;
            } else if (ke->key() == Qt::Key_Up) {
                // TODO: change active completion
                QModelIndex idx = m_results->currentIndex();
                if (idx.row() > 0) {
                    m_results->setCurrentIndex(m_proxy->index(idx.row() - 1, 0, idx.parent()));
                }
                return true;
            } else if (ke->key() == Qt::Key_Down) {
                // TODO: change active completion
                QModelIndex idx = m_results->currentIndex();
                if (idx.row() + 1 < m_proxy->rowCount(QModelIndex())) {
                    m_results->setCurrentIndex(m_proxy->index(idx.row() + 1, 0, idx.parent()));
                }
                return true;
            } else if (ke->key() == Qt::Key_Return || ke->key() == Qt::Key_Enter) {
                QModelIndex idx = m_results->currentIndex();
                if (idx.isValid()) {
                    QString result = m_proxy->data(idx, m_proxy->sortRole()).toString();
                    qDebug("res: %i %s", idx.row(), qPrintable(result));
                    m_callback.call(result);
                }
                done();
                return true;
            }
        }
    } else if (o == m_results) {
        if (e->type() == QEvent::KeyPress) {
            if (!eventFilter(m_command, e)) m_command->event(e);
            return true;
        }
    }
    return QWidget::eventFilter(o, e);
}
