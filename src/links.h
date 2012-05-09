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

#ifndef LINKS_H
#define LINKS_H

#include <QtCore/QStringList>
#include <QtCore/QObject>

class Links
{
public:
    Links();

public slots:
    static QStringList linkTypes();
};

#endif // LINKS_H
