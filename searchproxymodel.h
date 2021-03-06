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

#ifndef SEARCH_PROXY_MODEL_H_
#define SEARCH_PROXY_MODEL_H_

#include <QSortFilterProxyModel>

/*!
 * \class SearchProxyModel
 * \brief Custom proxy model for quick search
 */
class SearchProxyModel : public QSortFilterProxyModel {
    Q_OBJECT
public:
    SearchProxyModel(QObject *p = 0);
    ~SearchProxyModel();
    
    void setSearch(const QString& s);
    
protected:
    virtual bool filterAcceptsColumn(int source_column,
                                     const QModelIndex& source_parent) const;
    virtual bool filterAcceptsRow(int source_row,
                                  const QModelIndex& source_parent) const;
    virtual bool lessThan(const QModelIndex& left,
                          const QModelIndex& right) const;
    
private:
    QString m_search;
};

#endif  // SEARCH_PROXY_MODEL_H_
