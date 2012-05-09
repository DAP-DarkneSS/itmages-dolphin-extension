#include <QClipboard>
#include <QDesktopServices>
#include <QFile>
#include <QFileInfo>
#include <QMenu>
#include <QUrl>

#include "src/itmagesitem.h"

ITmagesItem::ITmagesItem(int row,QString imgPath,QWidget *parent) :
  QWidget(parent)
{
  setupUi(this);

  itemNum = row;
  imgPathStr = imgPath;
  uploaded = false;

  links = new Links;
  linkTypesList.append(links->linkTypes());
  linkType = 0;

  QPixmap pixmap = QPixmap(imgPath);

  fSize = (float)QFile(imgPath).size()/1024;
  lblSizes->setText(tr("<b>Size:</b> ")+QString::number((int)fSize)
                   +tr("KiB, <b>Res:</b> ")
                   +QString::number(pixmap.width())+"x"+QString::number(pixmap.height())
                   +tr("pix."));

  btnImage->setIcon(pixmap.scaled(80,80,Qt::KeepAspectRatio,Qt::SmoothTransformation));

  lblImgPath->setText(tr("<b>Name:</b> ")+QFileInfo(imgPath).fileName());

  btnCopy->setIcon(   QIcon::fromTheme("edit-copy"));
  btnBrowser->setIcon(QIcon::fromTheme("applications-internet"));
  btnClose->setIcon(  QIcon::fromTheme("dialog-close"));

  btnCopy->setContextMenuPolicy(Qt::CustomContextMenu);

  connect(btnCopy,   SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(btnCopy_menu(QPoint)));
  connect(btnCopy,   SIGNAL(clicked()),this,SLOT(btnCopy_clicked()));
  connect(btnBrowser,SIGNAL(clicked()),this,SLOT(btnBrowser_clicked()));
  connect(btnClose,  SIGNAL(clicked()),this,SLOT(btnClose_clicked()));

  bar->hide();
  btnCopy->hide();
  btnBrowser->hide();
  btnClose->hide();
}

ITmagesItem::~ITmagesItem()
{
}

void ITmagesItem::btnClose_clicked()
{
  deleted(itemNum);
  deleteLater();
}

void ITmagesItem::setItemRow(int row)
{
  itemNum = row;
}

QString ITmagesItem::imgPath()
{
  return imgPathStr;
}

float ITmagesItem::fileSize()
{
  return fSize;
}

void ITmagesItem::setProgressVisible(bool visible)
{
  bar->setVisible(visible);
  btnClose->setEnabled(!visible);
}

void ITmagesItem::setProgressValue(int value)
{
  if (value < 100) {
    bar->setValue(value);
  } else if (value == 101) {
    bar->setValue(100);
    btnClose->hide();
    getLinkType(itemNum);
    uploaded = true;
  }
}

void ITmagesItem::btnBrowser_clicked()
{
  QDesktopServices::openUrl(QUrl(linkList.at(1)));
}

void ITmagesItem::btnCopy_clicked()
{
  QClipboard *clipboard = QApplication::clipboard();
  clipboard->setText(linkList.at(linkType));
}

void ITmagesItem::enterEvent(QEvent *)
{
  if (uploaded) {
    btnCopy->show();
    btnBrowser->show();
  } else {
    btnClose->show();
  }
}

void ITmagesItem::leaveEvent(QEvent *)
{
  if (!menuVisible) {
    btnCopy->hide();
    btnBrowser->hide();
  }
  btnClose->hide();
}

void ITmagesItem::btnCopy_menu(QPoint point)
{
  menuVisible = true;

  getLinkType(itemNum);

  QMenu *menu = new QMenu;

  group = new QActionGroup(this);

  for (int i = 0; i < linkTypesList.count(); ++i) {
    group->addAction(new QAction(this));
    group->actions().at(i)->setCheckable(true);
    if (linkType == i)
      group->actions().at(i)->setChecked(true);
    group->actions().at(i)->setText(linkTypesList.at(i));
  }

  menu->addActions(group->actions());
  menu->exec(btnCopy->mapToGlobal(point));

  for (int i = 0; i < group->actions().count(); ++i) {
    if (group->actions().at(i)->text() == group->checkedAction()->text()) {
      linkType = i;
      break;
    }
  }

  newLinkType(linkType);

  setLinkToolTip(linkList.at(linkType));

  menuVisible = false;
}

void ITmagesItem::setLinkType(int type)
{
  linkType = type;
  if (!linkList.isEmpty())
    setLinkToolTip(linkList.at(linkType));
}

void ITmagesItem::setLinkToolTip(QString link)
{
  btnCopy->setToolTip("<b>"+QString(linkTypesList.at(linkType)).replace(" ","&nbsp;")
                      +":</b>&nbsp;<a href=\""
                      +link.replace("<","&lt;").replace(">","&gt;")
                      +"\"><span style=\" text-decoration: underline; color:#0057ae;\">"
                      +link.replace("<","&lt;").replace(">","&gt;")
                      +"</span></a><br />"+tr("Right click to select type of link."));
}

void ITmagesItem::setProgressText(QString text)
{
  bar->setFormat(text);
}

void ITmagesItem::createLinks(QString id, QString key, QString server,
                              QString shrt, QString small, QString full)
{
  Q_UNUSED(small);

  QString link("http://itmages.ru/image/view/"+id+"/"+key);
  QString linkFull("http://"+server+".static.itmages.ru/"+full);

  //! Link to image
  linkList.append(link);

  //! Page with all links
  linkList.append(QString(link).replace("view","preview"));

  //! Short link
  linkList.append("http://itmag.es/"+shrt);

  //! HTML preview for forum
  linkList.append("<a target=\"_blank\"; href=\""+link+"\">"
                   "<img src=\""+QString(linkFull).replace("/h_","/s_")+"\" /></a>");

  //! HTML full size image
  linkList.append("<a target=\"_blank\"; href=\""+link+"\">"
                   "<img src=\""+linkFull+"\" /></a>");

  //! BB preview for forum
  linkList.append("[url="+link+"][img]"+QString(linkFull).replace("/h_","/s_")+"[/img][/url]");

  //! BB full size image
  linkList.append("[url="+link+"][img]"+linkFull+"[/img][/url]");

  //! Link to full size image
  linkList.append(linkFull);

  if (linkType < linkTypesList.count())
    setLinkToolTip(linkList.at(linkType));

  setProgressValue(101);
}

void ITmagesItem::on_btnImage_clicked()
{
  QDesktopServices::openUrl(QUrl(imgPathStr, QUrl::TolerantMode));
}
