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

#ifndef COMMAND_BAR_H_
#define COMMAND_BAR_H_

#include <QWidget>

#include "callback.h"

class QLineEdit;
class QListView;
class QMetaMethod;
class QAbstractItemModel;

class SearchProxyModel;

/*!
 * \class CommandBar
 * \brief A wrapper around qce::CodeEditor
 */
class CommandBar : public QWidget {
    Q_OBJECT
public:
    CommandBar();
    ~CommandBar();
    
    void search(QWidget *w, QAbstractItemModel *model, int role,
                Callback<QString> cb);
    
public slots:
    
    
private slots:
    void done();
    void textChanged(const QString& text);
    
protected:
    bool eventFilter(QObject *o, QEvent *e);
    
private:
    QLineEdit *m_command;
    QListView *m_results;
    SearchProxyModel *m_proxy;
    Callback<QString> m_callback;
};

#endif  // COMMAND_BAR_H_
