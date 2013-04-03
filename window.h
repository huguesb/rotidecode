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

#ifndef WINDOW_H_
#define WINDOW_H_

#include <QMainWindow>

class Editor;
class Buffers;

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
    
private slots:
    void fileNew();
    void fileOpen();
    void fileReload();
    void fileSave();
    void fileClose();
    void fileQuit();
    
    void cleanChanged(bool clean);
    void fileNameChanged(QString oldFileName, QString newFileName);
    
    void currentBufferChanged(const QModelIndex& current, const QModelIndex& previous);
    
private:
    void createMenu();
    void createBuffersDock();
    
    Editor *m_editor;
    Buffers *m_buffers;
};

#endif  // WINDOW_H_
