#include <QtGui/QApplication>
#include <QStringList>
#include <QMessageBox>
#include <QTranslator>

#include "src/itmages-uploader.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  if (QLocale::system().name().contains(QRegExp("ru|ua"))) {
    QTranslator *translator = new QTranslator;
    translator->load("/usr/share/itmages/itmages-dolphin-extension_ru");
    qApp->installTranslator(translator);
  }

  QStringList tempList = QApplication::arguments();
  tempList.removeFirst();
  tempList.replaceInStrings("file://","");
  tempList.removeDuplicates();

  QStringList images;
  for (int i = 0; i < tempList.count(); ++i)
    if (tempList.at(i).contains(QRegExp("png$|jpeg$|jpg$|gif$")))
//      if (!tempList.at(i).contains(QRegExp("%")))
        images.append(tempList.at(i));

  if (!images.isEmpty()) {
    ITmagesUploader *w = new ITmagesUploader(images);
    w->show();
  } else {
    QMessageBox::critical(0,"Error - ITmages Uploader",
                          QObject::tr("No images selected.\n"
                          "Program will be closed."),
                          QMessageBox::Ok);
    return 0;
  }

  return a.exec();
}

