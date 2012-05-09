/*
 *   This file is part of itmages-dolphin-extension.
 *
 *   itmages-dolphin-extension is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   itmages-dolphin-extension is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ITMAGESITEM_H
#define ITMAGESITEM_H

#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include <QtGui/QWidget>

#include "src/links.h"
#include "ui_itmagesitem.h"

class Links;

class ITmagesItem : public QWidget, private Ui::ITmagesItem
{
    Q_OBJECT

public:
    explicit ITmagesItem(int i, QString imgPath, QWidget *parent = 0);

private:
    bool menuVisible;
    bool uploaded;
    float fSize;
    int itemNum;
    int linkType;
    QActionGroup *group;
    QString imgPathStr;
    QStringList linkTypesList;
    QStringList linkList;
    Links links;

signals:
    void deleted(int item);
    void getLinkType(int item);
    void newLinkType(int type);

public slots:
    float fileSize();
    QString imgPath();
    void createLinks(QString id, QString key, QString server,
                     QString shrt, QString small, QString full);
    void setItemRow(int row);
    void setLinkToolTip(QString link);
    void setLinkType(int type);
    void setProgressText(QString text);
    void setProgressValue(int value);
    void setProgressVisible(bool visible);

private slots:
    void on_btnBrowser_clicked();
    void on_btnClose_clicked();
    void on_btnCopy_clicked();
    void on_btnImage_clicked();
    void on_btnCopy_customContextMenuRequested(const QPoint &pos);

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
};

#endif // ITMAGESITEM_H
