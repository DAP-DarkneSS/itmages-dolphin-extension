#include "links.h"

Links::Links()
{
}

QStringList Links::linkTypes()
{
  QStringList tempList;
  tempList.append(QObject::tr("Link to image"));
  tempList.append(QObject::tr("Page with all links"));
  tempList.append(QObject::tr("Short link"));
  tempList.append(QObject::tr("HTML preview for forum"));
  tempList.append(QObject::tr("HTML full size image"));
  tempList.append(QObject::tr("BB preview for forum"));
  tempList.append(QObject::tr("BB full size image"));
  tempList.append(QObject::tr("Link to full size image"));

  return tempList;
}
