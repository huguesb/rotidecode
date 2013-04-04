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

#include "searchproxymodel.h"

SearchProxyModel::SearchProxyModel(QObject *p)
    : QSortFilterProxyModel(p) {
}

SearchProxyModel::~SearchProxyModel() {
}


void SearchProxyModel::setSearch(const QString& search) {
    m_search = search;
    setFilterRegExp(QRegExp(search, Qt::CaseInsensitive, QRegExp::WildcardUnix)),
    invalidate();
}

bool SearchProxyModel::filterAcceptsColumn(
        int source_column, const QModelIndex& source_parent) const {
    return true;
}

bool SearchProxyModel::filterAcceptsRow(
        int source_row, const QModelIndex& source_parent) const {
    //QString s = sourceModel()->data(source_row, 0, source_parent);
    return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}

bool SearchProxyModel::lessThan(
        const QModelIndex& left, const QModelIndex& right) const {
    return QSortFilterProxyModel::lessThan(left, right);
}

