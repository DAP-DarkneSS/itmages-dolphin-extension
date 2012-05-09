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

#include "src/links.h"

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
