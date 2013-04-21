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

#ifndef WINDOW_H_
#define WINDOW_H_

#include <QMainWindow>

class Editor;
class Buffers;
class CommandBar;

namespace qce{
class Editor;
class SplitEditor;
}

class QListView;
class QModelIndex;

/*!
 * \class Window
 * \brief Main window of RotiDeCode
 * 
 * Manages the central Editor widget, provides a simple menubar and a handful
 * of dock widgets
 */
class Window : public QMainWindow {
    Q_OBJECT
public:
    Window();
    ~Window();
    
public slots:
    void readSettings();
    void writeSettings();
    
protected:
    virtual void closeEvent(QCloseEvent *e);
    
private slots:
    void fileNew();
    void fileOpen();
    void fileReload();
    void fileSave();
    void fileSaveAs();
    void fileClose();
    void fileQuit();
    
    void commandJump();
    
    void activeEditorChanged(qce::Editor *editor);
    void cleanChanged(bool clean);
    void fileNameChanged(QString oldFileName, QString newFileName);
    
    void currentBufferChanged(const QModelIndex& current, const QModelIndex& previous);
    
    void setActiveBuffer(const QString& path);
    
    void restoreDockState();
    
private:
    void createMenu();
    void createBuffersDock();
    
    qce::SplitEditor *m_editor;
    Buffers *m_buffers;
    QListView *m_bufferList;
    CommandBar *m_commandBar;
};

#endif  // WINDOW_H_
