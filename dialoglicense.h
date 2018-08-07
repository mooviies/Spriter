/*
    Spriter - Spritesheet cutting utility software
    Copyright (C) 2018  mooviies

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    Software Website : http://mooviies.com
    Sources Repository : https://github.com/mooviies/Spriter
 */

#ifndef DIALOGLICENSE_H
#define DIALOGLICENSE_H

#include <QDialog>

namespace Ui {
class DialogLicense;
}

class DialogLicense : public QDialog
{
    Q_OBJECT

public:
    explicit DialogLicense(QWidget *parent = nullptr);
    ~DialogLicense();

private:
    Ui::DialogLicense *ui;
};

#endif // DIALOGLICENSE_H
