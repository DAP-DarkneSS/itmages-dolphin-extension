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

#include <QtGui/QMessageBox>
#include <QtGui/QDesktopServices>
#include <QtCore/QProcess>
#include <QtCore/QSettings>
#include <QtCore/QUrl>

#include "itmagesuploader.h"

ITmagesUploader::ITmagesUploader(QStringList images, QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    cmbBoxLinks->addItems(Links::linkTypes());
    loadSettings();

    btnUpload->setIcon(  QIcon::fromTheme("go-up"));
    btnSettings->setIcon(QIcon::fromTheme("application-x-desktop"));

    for (int i = 0; i < images.count(); ++i) {
        itemList.append(new ITmagesItem(i,images.at(i)));

        QListWidgetItem *item = new QListWidgetItem(listWidget);
        item->setSizeHint(QSize(80,80));
        listWidget->setItemWidget(item,itemList.at(i));

        connect(itemList.at(i), SIGNAL(deleted(int)),     this, SLOT(deleteItem(int)));
        connect(itemList.at(i), SIGNAL(getLinkType(int)), this, SLOT(linkType(int)));
        connect(itemList.at(i), SIGNAL(newLinkType(int)), this, SLOT(setLinkType(int)));
    }

    lblSumm->setContentsMargins(5, 0, 0, 0);
    summary();

    // create interface for signals
    QDBusConnection bus = QDBusConnection::sessionBus();
    QDBusInterface *interface =
        new QDBusInterface("org.freedesktop.ITmagesEngine",
                          "/org/freedesktop/ITmagesEngine",
                           "org.freedesktop.ITmagesEngine",
                           bus,this);

    // start daemon if it's disabled
    if (!interface->isValid()) {
        QProcess daemon;
        daemon.start("itmagesd",QStringList("-d"));
        daemon.waitForFinished();
    }

    // connecting to signal, which return information about uploaded files
    connect(interface, SIGNAL(action_complete_string_line(QString)),
            this,        SLOT(uploadComplete(QString)));

    connect(interface, SIGNAL(progress_event(int,QString,double,double,double,double)),
            this,        SLOT(uploadProgress(int,QString,double,double,double,double)));

    setWindowIcon(QIcon::fromTheme("itmages"));
    stackedWidget->setCurrentIndex(0);
//    resize(500,400);
    adjustSize();
}

void ITmagesUploader::loadSettings()
{
    QSettings settings(QSettings::NativeFormat, QSettings::UserScope,
                       "ITmages", "config");

    groupBoxProfile->setChecked     (settings.value("account", false).toBool());
    lineEditAccLogin->setText       (settings.value("accLogin").toString());
    lineEditAccPass->setText        (settings.value("accPass" ).toString());
    groupBoxProxy->setChecked       (settings.value("proxy", false).toBool());
    cmbBoxProxyType->setCurrentIndex(settings.value("proxyType", 0).toInt());
    lineEditPUser->setText          (settings.value("proxyUser").toString());
    lineEditPPass->setText          (settings.value("proxyPass").toString());
    spinBoxPort->setValue           (settings.value("proxyPort", 8080).toInt());
    lineEditPNode->setText          (settings.value("proxyNode").toString());
    cmbBoxLinks->setCurrentIndex    (settings.value("linkType", 0).toInt());
}

void ITmagesUploader::closeEvent(QCloseEvent *)
{
    QSettings settings(QSettings::NativeFormat, QSettings::UserScope,
                       "ITmages", "config");

    settings.setValue("account",   groupBoxProfile->isChecked());
    settings.setValue("accLogin",  lineEditAccLogin->text());
    settings.setValue("accPass",   lineEditAccPass->text());
    settings.setValue("proxy",     groupBoxProxy->isChecked());
    settings.setValue("proxyType", cmbBoxProxyType->currentIndex());
    settings.setValue("proxyNode", lineEditPNode->text());
    settings.setValue("proxyUser", lineEditPUser->text());
    settings.setValue("proxyPass", lineEditPPass->text());
    settings.setValue("proxyPort", spinBoxPort->value());
    settings.setValue("linkType",  cmbBoxLinks->currentIndex());
}

void ITmagesUploader::on_btnUpload_clicked()
{
    QDBusConnection connection =
        QDBusConnection::connectToBus(QDBusConnection::SessionBus,
                                      "org.freedesktop.ITmagesEngine");
    QDBusMessage msg =
        QDBusMessage::createMethodCall("org.freedesktop.ITmagesEngine",
                                       "/org/freedesktop/ITmagesEngine",
                                       "org.freedesktop.ITmagesEngine",
                                       "Shared");
    QList<QVariant> args;
    QMap<QString, QVariant> methodMap;

    // uploading
    for (int i = 0; i < itemList.count(); ++i) {
        args.clear();
        methodMap.clear();

        methodMap.insert("method","upload");
        methodMap.insert("verbose",true);
        methodMap.insert("async",false);
        methodMap.insert("filename",itemList.at(i)->imgPath());

        // accaunt settings
        if (groupBoxProfile->isChecked()) {
            methodMap.insert("autoauth",true);
            methodMap.insert("user",lineEditAccLogin->text());
            methodMap.insert("passwd",lineEditAccPass->text());
        }

        // proxy settings
        if (groupBoxProxy->isChecked()) {
            methodMap.insert("proxyuser",lineEditPUser->text());
            methodMap.insert("proxypass",lineEditPPass->text());
            methodMap.insert("proxyport",spinBoxPort->value());
            methodMap.insert("proxynode",lineEditPNode->text());

            if      (cmbBoxProxyType->currentIndex() == 0)
                methodMap.insert("proxytype","0");
            else if (cmbBoxProxyType->currentIndex() == 1)
                methodMap.insert("proxytype","4");
            else if (cmbBoxProxyType->currentIndex() == 2)
                methodMap.insert("proxytype","5");
        }
        args.append(methodMap);
        msg.setArguments(args);
        connection.call(msg,QDBus::NoBlock); // to faster calls
    }
    btnUpload->setEnabled(false);
}

void ITmagesUploader::deleteItem(int in)
{
    listWidget->removeItemWidget(listWidget->takeItem(in));
    itemList.removeAt(in);
    for (int i = 0; i < itemList.count(); ++i)
        itemList.at(i)->setItemRow(i);
    summary();
}

void ITmagesUploader::summary()
{
    float fSize = 0;
    for (int i = 0; i < itemList.count(); ++i)
        fSize += itemList.at(i)->fileSize();

    QString sizeStr;
    if (fSize > 1024)
        sizeStr = QString::number((fSize/1024), 'f', 2) + tr("MiB");
    else
        sizeStr = QString::number((fSize), 'f', 2) + tr("KiB");

    lblSumm->setText(tr("<b>Images:</b> ")+QString::number(itemList.count())+"<br />"
                    +tr("<b>Size:</b> ")+sizeStr);

    if (itemList.count() == 0)
        btnUpload->setEnabled(false);
}

void ITmagesUploader::on_btnSettings_clicked()
{
    stackedWidget->setCurrentIndex(!stackedWidget->currentIndex());
}

void ITmagesUploader::uploadComplete(const QString &reply)
{
    QStringList args = reply.split(";;");
    if (reply.contains(QRegExp("action:upload;;status:True;;full.*filename"))) {
        for (int i = 0; i < itemList.count(); ++i) {
            if (itemList.at(i)->imgPath() == args.filter("filename:").first().remove("filename:")){
                if (reply.contains(QRegExp("id|key|server|short|small|full"))) {
                    itemList.at(i)->createLinks(args.filter("id:").first().remove("id:"),
                                                args.filter("key").first().remove("key:"),
                                                args.filter("server").first().remove("server:"),
                                                args.filter("short").first().remove("short:"),
                                                args.filter("small").first().remove("small:"),
                                                args.filter("full").first().remove("full:"));
                } else {
                    QMessageBox::warning(this,"Warning - ITmages Uploader",
                                         tr("Server returned wrong image info.\n"
                                         "Please, try again."),
                                         QMessageBox::Ok);
                }
            }
        }
    } else if (reply.contains("action:upload;;status:True;;reason:Done")) {

        for (int i = 0; i < itemList.count(); ++i) {
            if (args.filter("filename:").first().remove("filename:") == itemList.at(i)->imgPath()){
                itemList.at(i)->setProgressVisible(true);
                itemList.at(i)->setProgressText(tr("This image already uploaded"));
                itemList.at(i)->setEnabled(false);
            }
        }

    } else if (reply.contains("action:upload;;status:False;;reason:;;")) {

        QMessageBox::warning(this,"Warning - ITmages Uploader",
                             tr("Can't connect to server.\n"
                             "Please, try again later.\n"
                             "Or try to reboot your PC."),
                             QMessageBox::Ok);

    } else if (reply.contains("action:upload;;status:False;;reason:Could not resolve host: "
                              "itmages.ru; Resource temporarily unavailable;;")) {

        QMessageBox::warning(this,"Warning - ITmages Uploader",
                             tr("Can't connect to itmages.ru.\n"
                             "Maybe the site is down."),
                             QMessageBox::Ok);

    } else if (reply.contains("action:login;;status:False;;reason:auth failed")) {

        QMessageBox::warning(this,"Warning - ITmages Uploader",
                             tr("Unable to login.\n"
                             "Check your login and password."),
                             QMessageBox::Ok);
    }
}

void ITmagesUploader::uploadProgress(int action, QString name,
                                     double, double, double ut, double ur)
{
    if (action == 0) {
        for (int i = 0; i < itemList.count(); ++i) {
            if (itemList.at(i)->imgPath() == name) {
                itemList.at(i)->setProgressVisible(true);
                itemList.at(i)->setProgressValue((int)((ur/ut)*100));
                listWidget->scrollTo(listWidget->model()->index(i,0));
            }
        }
    }
}

void ITmagesUploader::on_lblInfo_linkActivated(const QString &)
{
    QDesktopServices::openUrl(QUrl("http://itmages.ru/user/register"));
}

void ITmagesUploader::linkType(int item)
{
    itemList.at(item)->setLinkType(cmbBoxLinks->currentIndex());
    lblSumm->setText("<b>Link type:</b><br />"+cmbBoxLinks->currentText());
}

void ITmagesUploader::setLinkType(int type)
{
    cmbBoxLinks->setCurrentIndex(type);
}

void ITmagesUploader::on_cmbBoxLinks_currentIndexChanged(int index)
{
    for (int i = 0; i < itemList.count(); ++i)
        itemList.at(i)->setLinkType(index);
    if (!btnUpload->isEnabled())
        lblSumm->setText("<b>Link type:</b><br />"+cmbBoxLinks->currentText());
}
