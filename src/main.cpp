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

#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QtCore/QStringList>
#include <QtCore/QTranslator>
#include <QtCore/QLibraryInfo>

#include "src/itmagesuploader.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // load translation for Qt
    QTranslator qtTranslator;
    qtTranslator.load("qt_"+QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    // load itmages-dolphin-extension translation
    if (QLocale::system().name().contains(QRegExp("ru"))) {
        QTranslator *translator = new QTranslator;
        translator->load("/usr/share/itmages/itmages-dolphin-extension_ru");
        qApp->installTranslator(translator);
    }

    QStringList tempList = QApplication::arguments();
    tempList.removeFirst();
    tempList.replaceInStrings("file://", "");
    tempList.removeDuplicates();

    QStringList images;
    for (int i = 0; i < tempList.count(); ++i)
        if (tempList.at(i).contains(QRegExp("png$|jpeg$|jpg$|gif$", Qt::CaseInsensitive)))
            images.append(tempList.at(i));

    if (!images.isEmpty()) {
        ITmagesUploader *w = new ITmagesUploader(images);
        w->show();
    } else {
        QMessageBox::critical(0, "Error - ITmages Uploader",
                              QObject::tr("No images are selected.\n"
                              "Program will be closed."),
                              QMessageBox::Ok);
        return 0;
    }
    return app.exec();
}
