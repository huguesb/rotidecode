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

#include "window.h"

#include "buffers.h"
#include "commandbar.h"

#include <editor.h>
#include <spliteditor.h>

#include <QAction>
#include <QCloseEvent>
#include <QDataStream>
#include <QDockWidget>
#include <QListView>
#include <QMenu>
#include <QMenuBar>
#include <QMetaMethod>
#include <QFileDialog>
#include <QApplication>
#include <QSettings>
#include <QDebug>
#include <QTimer>

namespace {

enum {
    BufferSerializationVersion = 0
};

/*!
 * \brief Helper for Buffer-centric Editor serialization
 */
class Serializer : public qce::EditorSerializer {
public:
    Serializer(Buffers *buffers) : m_buffers(buffers) {}
    virtual ~Serializer() {}
    
    virtual void writeHeader(QDataStream& stream) const {
        stream << qint32(BufferSerializationVersion);
    }
    
    virtual void serialize(QDataStream& stream, const qce::Editor *const editor) const {
        if (m_buffers->isUntitled(editor)) {
            stream << QString();
        } else {
            stream << editor->fileName();
        }
    }
    
private:
    Buffers *m_buffers;
};

/*!
 * \brief Helper for Buffer-centric Editor deserialization
 */
class Deserializer : public qce::EditorDeserializer {
public:
    Deserializer(Buffers *buffers) : m_buffers(buffers) {}
    virtual ~Deserializer() {}
    
    virtual bool readHeader(QDataStream& stream) const {
        qint32 version;
        stream >> version;
        return version == BufferSerializationVersion;
    }
    
    virtual qce::Editor* deserialize(QDataStream& stream) const {
        QString fileName;
        stream >> fileName;
        if (fileName.isEmpty()) fileName = m_buffers->create();
        return m_buffers->acquire(fileName);
    }
    
private:
    Buffers *m_buffers;
};

}  // namespace



Window::Window() {
    m_buffers = new Buffers(this);
    
    m_editor = new qce::SplitEditor(this);
    setCentralWidget(m_editor);
    
    m_commandBar = new CommandBar;
    
    connect(m_buffers, SIGNAL( closeRequested(qce::Editor*) ),
            m_editor , SLOT  ( closeEditor(qce::Editor*) ));
    
    connect(m_editor , SIGNAL( editorOpened(qce::Editor*) ),
            m_buffers, SLOT  ( editorOpened(qce::Editor*) ));
    
    connect(m_editor , SIGNAL( editorClosed(qce::Editor*) ),
            m_buffers, SLOT  ( release(qce::Editor*) ));
    
    connect(m_editor, SIGNAL( activeEditorChanged(qce::Editor*) ),
            this    , SLOT  ( activeEditorChanged(qce::Editor*) ));
    
    connect(m_editor, SIGNAL( cleanChanged(bool) ),
            this    , SLOT  ( cleanChanged(bool) ));
    
    connect(m_editor, SIGNAL( fileNameChanged(QString, QString) ),
            this    , SLOT  ( fileNameChanged(QString, QString) ));
    
    createMenu();
    createBuffersDock();
    
    setWindowTitle("RotiDeCode");
}

Window::~Window() {
    delete m_commandBar;
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
    a = m->addAction(tr("Close Active Vie&w"),
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
    
    
    m = menuBar()->addMenu(tr("&Command"));
    a = m->addAction(tr("&Jump to file"),
                     this, SLOT( commandJump() ), tr("Alt+Shift+J"));
    
}

void Window::createBuffersDock() {
    m_bufferList = new QListView;
    m_bufferList->setModel(m_buffers);
    
    connect(m_bufferList->selectionModel(),
            SIGNAL( currentChanged(QModelIndex, QModelIndex) ),
            this,
            SLOT( currentBufferChanged(QModelIndex, QModelIndex) ) );
    
    QDockWidget *dock = new QDockWidget(tr("Open Files"), this);
    dock->setObjectName("openfiles");
    dock->setWidget(m_bufferList);
    dock->setFeatures(QDockWidget::DockWidgetVerticalTitleBar);
    
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    
    addDockWidget(Qt::LeftDockWidgetArea, dock);
}

void Window::readSettings() {
    QSettings s;
    s.beginGroup("window");
    QMainWindow::restoreGeometry(s.value("geometry").toByteArray());
    QMainWindow::restoreState(s.value("state").toByteArray());
    
    QStringList fileNames = s.value("buffers").toStringList();
    foreach (QString fileName, fileNames) {
        m_buffers->load(fileName);
    }
    
    m_editor->restoreState(s.value("editors").toByteArray(),
                           Deserializer(m_buffers));
    
    // for some weird reason, dock widget geometry cannot be restored properly
    // until the central widget is resized and displayed...
    QTimer::singleShot(10, this, SLOT( restoreDockState() ));
}

void Window::writeSettings() {
    QSettings s;
    s.beginGroup("window");
    s.setValue("geometry", QMainWindow::saveGeometry());
    s.setValue("state", QMainWindow::saveState());
    s.setValue("buffers", m_buffers->fileNames());
    s.setValue("editors", m_editor->saveState(Serializer(m_buffers)));
}

void Window::restoreDockState() {
    QMainWindow::restoreState(QSettings().value("window/state").toByteArray());
}

void Window::closeEvent(QCloseEvent *e) {
    // TODO: offer to save modified files
    qDebug("closing...");
    
    writeSettings();
    QMainWindow::closeEvent(e);
}

void Window::fileNew() {
    setActiveBuffer(m_buffers->create());
}

void Window::fileOpen() {
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open file(s)..."));
    if (fileNames.empty()) return;
    
    for (int i = 0; i < fileNames.count(); ++i) {
        QString path = QFileInfo(fileNames.at(i)).absoluteFilePath();
        m_buffers->load(path);
        if (i == fileNames.count() - 1) {
            setActiveBuffer(path);
        }
    }
}

void Window::fileReload() {
    if (!m_editor->isClean()) {
        // TODO: warn if unsaved file
    }
    
    m_editor->load(m_editor->activeFileName());
}

void Window::fileSave() {
    m_editor->save(m_editor->activeFileName());
}

void Window::fileClose() {
    if (!m_editor->isClean()) {
        // TODO: warn if unsaved file
    }
    
    m_buffers->close(m_editor->activeFileName());
}

void Window::fileQuit() {
    // TODO: warn if unsaved buffers
    close();
}

void Window::commandJump() {
    Callback<QString> callback = Callback<QString>(this, "setActiveBuffer(QString)");
    m_commandBar->search(this, m_buffers, Buffers::PathRole, callback);
}

void Window::activeEditorChanged(qce::Editor *editor) {
    QModelIndex idx;
    if (editor == 0) {
        idx = m_buffers->index(0, 0, QModelIndex());
    } else {
        idx = m_buffers->indexForName(editor->fileName());
    }
    m_bufferList->selectionModel()->setCurrentIndex(idx,
            QItemSelectionModel::Clear | QItemSelectionModel::Select);
}

void Window::cleanChanged(bool clean) {
    setWindowModified(!clean);
}

void Window::fileNameChanged(QString oldFileName, QString newFileName) {
    Q_UNUSED(oldFileName);
    QString base;
    if (!newFileName.isEmpty()) base = tr("%1[*] - ").arg(newFileName);
    setWindowTitle(base + "RotiDeCode");
}

void Window::currentBufferChanged(const QModelIndex& current, const QModelIndex& previous) {
    Q_UNUSED(previous);
    if (!current.isValid()) return;
    setActiveBuffer(m_buffers->data(current, Buffers::PathRole).toString());
}

void Window::setActiveBuffer(const QString& path) {
    qDebug("set active: %s", qPrintable(path));
    if (path == m_editor->activeFileName()) return;
    m_buffers->release(m_editor->setActiveEditor(m_buffers->acquire(path)));
}
