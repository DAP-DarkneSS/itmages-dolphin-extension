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
 *   along with itmages-dolphin-extension.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ITMAGESUPLOADER_H
#define ITMAGESUPLOADER_H

#include <QMainWindow>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>

#include "src/itmagesitem.h"
#include "src/links.h"
#include "ui_itmagesuploader.h"

class ITmagesItem;
class Links;

class ITmagesUploader : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    ITmagesUploader(QStringList images, QWidget *parent = 0);

private:
    QList<ITmagesItem* > itemList;

private slots:
    void deleteItem(int i);
    void linkType(int item);

void loadSettings();
    void on_btnSettings_clicked();
    void on_btnUpload_clicked();
    void on_cmbBoxLinks_currentIndexChanged(int index);
    void on_lblInfo_linkActivated(const QString &link);
    void setLinkType(int type);
    void summary();
    void uploadComplete(const QString &reply);
    void uploadProgress(int, QString, double, double, double, double);

protected:
    void closeEvent(QCloseEvent *);
};

#endif // ITMAGESUPLOADER_H

