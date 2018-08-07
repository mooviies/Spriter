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
