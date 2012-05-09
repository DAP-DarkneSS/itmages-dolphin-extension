#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QtDBus>

#include "src/itmagesitem.h"
#include "src/links.h"
#include "ui_itmages-uploader.h"

class ITmagesItem;
class Links;

class ITmagesUploader : public QMainWindow, private Ui::MainWindow
{
  Q_OBJECT

public:
  ITmagesUploader(QStringList images, QWidget *parent = 0);

private:
  QList<ITmagesItem* > itemList;
  QSettings *settings;
  Links *links;

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
  void uploadProgress(int,QString,double,double,double,double);

protected:
  void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H

