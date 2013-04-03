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

#include "window.h"

#include "editor.h"
#include "buffers.h"

#include <QAction>
#include <QDockWidget>
#include <QListView>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QApplication>

Window::Window() {
    m_buffers = new Buffers(this);
    
    m_editor = new Editor(m_buffers->acquire(m_buffers->create()), this);
    setCentralWidget(m_editor);
    
    connect(m_buffers, SIGNAL( closeRequested(qce::Editor*) ),
            m_editor , SLOT  ( closeEditor(qce::Editor*) ));
    
    connect(m_editor, SIGNAL( cleanChanged(bool) ),
            this    , SLOT  ( cleanChanged(bool) ));
    
    connect(m_editor, SIGNAL( fileNameChanged(QString, QString) ),
            this    , SLOT  ( fileNameChanged(QString, QString) ));
    
    createMenu();
    createBuffersDock();
}

Window::~Window() {
}


void Window::createMenu() {
    QAction *a;
    QMenu *m;
    
    m = menuBar()->addMenu(tr("&File"));
    a = m->addAction(tr("&New"),    this, SLOT( fileNew() ), QKeySequence::New);
    m->addSeparator();
    a = m->addAction(tr("&Open"),   this, SLOT( fileOpen() ), QKeySequence::Open);
    a = m->addAction(tr("&Save"),   this, SLOT( fileSave() ), QKeySequence::Save);
    a = m->addAction(tr("Reloa&d"), this, SLOT( fileReload() ), QKeySequence::Refresh);
    a = m->addAction(tr("&Close"),  this, SLOT( fileClose() ), QKeySequence::Close);
    m->addSeparator();
    a = m->addAction(tr("&Quit"),   this, SLOT( fileQuit() ), QKeySequence::Quit);
    
    m = menuBar()->addMenu(tr("&Edit"));
    a = m->addAction(tr("&Undo"),
                     m_editor, SLOT( undo() ), QKeySequence::Undo);
    a->setEnabled(m_editor->canUndo());
    connect(m_editor, SIGNAL(undoAvailable(bool)), a, SLOT(setEnabled(bool)));
    a = m->addAction(tr("&Redo"),
                     m_editor, SLOT( redo() ), QKeySequence::Redo);
    a->setEnabled(m_editor->canRedo());
    connect(m_editor, SIGNAL(redoAvailable(bool)), a, SLOT(setEnabled(bool)));
    m->addSeparator();
    a = m->addAction(tr("Cu&t"),
                     m_editor, SLOT( cut() ), QKeySequence::Cut);
    a->setEnabled(m_editor->hasSelection());
    connect(m_editor, SIGNAL(selectionAvailable(bool)), a, SLOT(setEnabled(bool)));
    a = m->addAction(tr("&Copy"),
                     m_editor, SLOT( copy() ), QKeySequence::Copy);
    a->setEnabled(m_editor->hasSelection());
    connect(m_editor, SIGNAL(selectionAvailable(bool)), a, SLOT(setEnabled(bool)));
    a = m->addAction(tr("&Paste"),
                     m_editor, SLOT( paste() ), QKeySequence::Paste);
    m->addSeparator();
    a = m->addAction(tr("&Select All"),
                     m_editor, SLOT( selectAll() ), QKeySequence::SelectAll);
    
    m = menuBar()->addMenu(tr("&View"));
    a = m->addAction(tr("Split &Horizontally"),
                     m_editor, SLOT( splitHorizontally() ), tr("Alt+Shift+H"));
    a = m->addAction(tr("Split &Vertically"),
                     m_editor, SLOT( splitVertically() ), tr("Alt+Shift+V"));
    a = m->addAction(tr("Close Active View"),
                     m_editor, SLOT( closeActiveEditor() ), tr("Alt+Shift+W"));
    m->addSeparator();
    a = m->addAction(tr("Activate Above"),
                     m_editor, SLOT( activateUp() ), tr("Alt+Shift+Up"));
    a = m->addAction(tr("Activate Below"),
                     m_editor, SLOT( activateDown() ), tr("Alt+Shift+Down"));
    a = m->addAction(tr("Activate Before"),
                     m_editor, SLOT( activateLeft() ), tr("Alt+Shift+Left"));
    a = m->addAction(tr("Activate After"),
                     m_editor, SLOT( activateRight() ), tr("Alt+Shift+Right"));
    
}

void Window::createBuffersDock() {
    QListView *view = new QListView;
    view->setModel(m_buffers);
    
    connect(view->selectionModel(), SIGNAL( currentChanged(QModelIndex, QModelIndex) ),
            this, SLOT( currentBufferChanged(QModelIndex, QModelIndex) ) );
    
    QDockWidget *dock = new QDockWidget(tr("Open Files"), this);
    dock->setWidget(view);
    
    addDockWidget(Qt::LeftDockWidgetArea, dock);
}


void Window::fileNew() {
    m_buffers->release(m_editor->setActiveEditor(m_buffers->acquire(m_buffers->create())));
}

void Window::fileOpen() {
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open file(s)..."));
    if (fileNames.empty()) return;
    
    for (int i = 0; i < fileNames.count(); ++i) {
        QString path = QFileInfo(fileNames.at(i)).absoluteFilePath();
        m_buffers->load(path);
        if (i == fileNames.count() - 1) {
            m_buffers->release(m_editor->setActiveEditor(m_buffers->acquire(path)));
        }
    }
}

void Window::fileReload() {
    // TODO:
}

void Window::fileSave() {
    m_editor->save(m_editor->activeFileName());
}

void Window::fileClose() {
    // TODO: warn if unsaved files
    m_buffers->close(m_editor->activeFileName());
}

void Window::fileQuit() {
    // TODO: warn if unsaved files
    qApp->quit();
}

void Window::cleanChanged(bool clean) {
    setWindowModified(!clean);
}

void Window::fileNameChanged(QString oldFileName, QString newFileName) {
    Q_UNUSED(oldFileName);
    setWindowTitle(tr("%1[*] - RotiDeCode").arg(newFileName));
}

void Window::currentBufferChanged(const QModelIndex& current, const QModelIndex& previous) {
    QString path = m_buffers->data(current, Buffers::PathRole).toString();
    m_buffers->release(m_editor->setActiveEditor(m_buffers->acquire(path)));
}
