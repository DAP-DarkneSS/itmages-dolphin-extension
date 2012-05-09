#ifndef ITMAGESITEM_H
#define ITMAGESITEM_H

#include <QAction>
#include <QActionGroup>
#include <QWidget>

#include "src/links.h"
#include "ui_itmagesitem.h"

class Links;

class ITmagesItem : public QWidget, private Ui::ITmagesItem
{
  Q_OBJECT

public:
  explicit ITmagesItem(int i,QString imgPath,QWidget *parent = 0);
  ~ITmagesItem();

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
  Links *links;

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
  void btnBrowser_clicked();
  void btnClose_clicked();
  void btnCopy_clicked();
  void btnCopy_menu(QPoint point);
  void on_btnImage_clicked();

protected:
  void enterEvent(QEvent *);
  void leaveEvent(QEvent *);
};

#endif // ITMAGESITEM_H
