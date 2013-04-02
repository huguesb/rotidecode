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

#include "app.h"

#include <formatscheme.h>
#include <markscheme.h>
#include <syntaxhighlighter.h>
#include <genericsyntaxhighlighter.h>

App::App(int& argc, char **argv) 
    : QApplication(argc, argv) {
    printf("RotiDeDode, using QCE %s [%s]\n",
           qce::libVersionString(),
           qce::libGitCommitHash()
          );
}

void App::setup() {
    // TODO: do not use hardcoded absolute data path
    // TODO: try multiple data path?
    setupQCE(QDir("/home/prog/playground/qce3/data"));
}

void App::setupQCE(const QDir& dataDir) {
    QString dataPath = dataDir.absolutePath();
    
    // load format scheme
    QFile qbf(dataDir.absoluteFilePath("formats.qbf"));
    if (qbf.exists() && qbf.open(QFile::ReadOnly)) {
        QDataStream d(&qbf);
        qce::FormatScheme::get()->load(d);
    } else {
        qce::FormatScheme::load(dataPath);
        if (qbf.open(QFile::WriteOnly)) {
            QDataStream d(&qbf);
            qce::FormatScheme::get()->save(d);
        }
    }
    
    // load mark scheme
    QFile qbm(dataDir.absoluteFilePath("marks.qbm"));
    if (qbm.exists() && qbm.open(QFile::ReadOnly)) {
        QDataStream d(&qbm);
        qce::MarkScheme::get()->load(d);
    } else {
        qce::MarkScheme::load(dataPath);
        if (qbm.open(QFile::WriteOnly)) {
            QDataStream d(&qbm);
            qce::MarkScheme::get()->save(d);
        }
    }
    
    // load language definitions
    QFile qbs(dataDir.absoluteFilePath("languages.qbs"));
    if (qbs.exists() && qbs.open(QFile::ReadOnly)) {
        QDataStream d(&qbs);
        qce::GenericSyntaxHighlighter::deserialize(d);
    } else {
        qce::SyntaxHighlighter::load(dataPath);
        if (qbs.open(QFile::WriteOnly)) {
            QDataStream d(&qbs);
            qce::GenericSyntaxHighlighter::serialize(d);
        }
    }
}
