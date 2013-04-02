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

#ifndef EDITOR_H_
#define EDITOR_H_

#include <codeeditor.h>

/*!
 * \class Editor
 * \brief A wrapper around qce::CodeEditor
 */
class Editor : public qce::CodeEditor {
    Q_OBJECT
public:
    Editor(qce::Editor *e, QWidget *p = 0);
    ~Editor();
    
private:
};

#endif  // EDITOR_H_
