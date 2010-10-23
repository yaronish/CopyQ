/*
    Copyright (c) 2009, Lukas Holecek <hluk@email.cz>

    This file is part of CopyQ.

    CopyQ is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CopyQ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CopyQ.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CLIPBOARDBROWSER_H
#define CLIPBOARDBROWSER_H

#include <QListView>
#include <QTimer>
#include <QClipboard>
#include "qeditor.h"
#include "configurationmanager.h"

class ClipboardModel;
class ItemDelegate;
class ClipboardMonitor;
class QMimeData;

class ClipboardBrowser : public QListView
{
    Q_OBJECT

    public:
        ClipboardBrowser(QWidget *parent = 0);
        ~ClipboardBrowser();
        void loadSettings();
        bool add(const QString &txt, bool ignore_empty = true);
        bool add(QMimeData *item, bool ignore_empty = true);
        void remove();
        int length() const { return model()->rowCount(); }
        QString itemText(int i = -1) const;
        QString itemText(QModelIndex ind) const;
        void sync(bool list_to_clipboard = true, QClipboard::Mode mode = QClipboard::Clipboard);
        QModelIndex index(int i) const {
            return model()->index(i,0);
        }
        void setCurrent(int row, bool cycle = false, bool selection = false);

        // if items selected: return concatenation of selected items
        // else: return text of first item
        const QString selectedText() const;

        void runCallback() const;

        void setMenu(QMenu *menu);

        void startMonitoring();
        void updateClipboard();

    private:
        int m_maxitems;
        QString m_callback;
        QStringList m_callback_args;
        QString m_editor;
        ClipboardMonitor *m_monitor;
        ClipboardModel *m;
        ItemDelegate *d;
        QTimer timer_save;

        QMenu *menu;
        ConfigurationManager::Commands commands;

    protected:
        void keyPressEvent(QKeyEvent *event);
        void contextMenuEvent(QContextMenuEvent *);
        void selectionChanged ( const QItemSelection & selected,
                                const QItemSelection & deselected );

    signals:
        void requestSearch(const QString &txt);
        void requestActionDialog(int row, const ConfigurationManager::Command *cmd = NULL);
        void hideSearch();
        void escapePressed();
        void closeAllEditors();

    public slots:
        void keyEvent(QKeyEvent *event) { keyPressEvent(event); }
        void moveToClipboard(const QModelIndex &ind);
        void moveToClipboard(int i);
        void filterItems(const QString &str);
        void clearFilter() { filterItems( QString() ); }
        void itemModified(const QString &str);
        void closeEditor(QEditor *editor);
        void openEditor();
        void addItems(const QStringList &items);

        void loadItems();
        void saveItems(int msec=0);

        void contextMenuAction(QAction *act);
        void updateMenuItems();
        void newItem();

        void checkClipboard(QClipboard::Mode mode, QMimeData *data);
};

#endif // CLIPBOARDBROWSER_H
