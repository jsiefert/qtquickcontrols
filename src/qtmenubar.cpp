/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qtmenubar.h"
#include "qwindowitem.h"

#if QT_VERSION < 0x050000
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#else
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#endif

#if QT_VERSION < 0x050000
QtMenuBar::QtMenuBar(QDeclarativeItem *parent)
    : QDeclarativeItem(parent), _menuBar(0) /*, _menuBar(new QMenuBar)*/
{
    connect(this, SIGNAL(parentChanged()), this, SLOT(updateParent()));
    setFlag(QGraphicsItem::ItemHasNoContents, true);
}
#else
QtMenuBar::QtMenuBar(QQuickItem *parent)
    : QQuickItem(parent), _menuBar(0) /*, _menuBar(new QMenuBar)*/
{
    connect(this, SIGNAL(parentChanged(QQuickItem *)), this, SLOT(updateParent(QQuickItem *)));
    setFlag(QQuickItem::ItemHasContents, false);
}
#endif

QtMenuBar::~QtMenuBar()
{
}

#if QT_VERSION < 0x050000
QDeclarativeListProperty<QtMenu> QtMenuBar::menus()
{
    return QDeclarativeListProperty<QtMenu>(this, 0, &QtMenuBar::append_menu);
}
#else
QQmlListProperty<QtMenu> QtMenuBar::menus()
{
    return QQmlListProperty<QtMenu>(this, 0, &QtMenuBar::append_menu);
}
#endif

void QtMenuBar::updateParent(QQuickItem *newParent)
{
    if (QWindowItem* window = qobject_cast<QWindowItem*>(parent()))
        _menuBar = window->window()->menuBar();

    //THIS IS WRONG... WE NEED TO DO THAT DIFFERENT!
    _menuBar->clear();

    foreach (QtMenu *menu, m_menus) {
        _menuBar->addMenu(menu->qmenu());
    }
    //THIS IS WRONG... WE NEED TO DO THAT DIFFERENT!

}

#if QT_VERSION < 0x050000
void QtMenuBar::append_menu(QDeclarativeListProperty<QtMenu> *list, QtMenu *menu)
#else
void QtMenuBar::append_menu(QQmlListProperty<QtMenu> *list, QtMenu *menu)
#endif
{
    QtMenuBar *menuBar = qobject_cast<QtMenuBar *>(list->object);
    if (menuBar) {
        menu->setParent(menuBar);
        menuBar->m_menus.append(menu);
        if (menuBar->_menuBar)
            menuBar->_menuBar->addMenu(menu->qmenu());
    }
}

#