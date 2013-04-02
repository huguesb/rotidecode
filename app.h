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

#ifndef APP_H_
#define APP_H_

#include <QApplication>

#include <QDir>

class App : public QApplication {
    Q_OBJECT
public:
    App(int& argc, char **argv);
    
    void setup();
    
private:
    void setupQCE(const QDir& dataPath);
};

#endif  // APP_H_
