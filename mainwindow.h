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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include "inputtab.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setExport(int rows, int columns);

public slots:
    void addImages();
    void applyToAll();
    void clearAll();
    void tabChanged(int index);
    void generateOutput();
    void exportOutput();

private slots:
    void showLicense();
    void showSource();

protected:
    void resizeEvent (QResizeEvent * event);

private:
    void loadSettings();
    void saveSettings();

private:
    Ui::MainWindow *ui;
    QString _currentPath;
    QStringList _filenames;
    QList<InputTab*> _inputTabs;
    bool _manualExport;

    QVector<QImage*> _images;
    QVector<QGraphicsScene*> _scenes;
    QVector<QGraphicsPixmapItem*> _pixmap;
    QVector<int> _currentId;

    const QString APPLICATION_IMAGE_PATH;
    const QString ALL_FILES;
    const QString PNG_FILE;
    const QString TIFF_FILE;
    const QString JPG_FILE;
    const QString BMP_FILE;
    const QString IMAGE_FILE;
};

#endif // MAINWINDOW_H
