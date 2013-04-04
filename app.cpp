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

#include "app.h"

#include <qce-version.h>

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
