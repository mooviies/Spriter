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

    const QString APPLICATION_IMAGE_PATH;
};

#endif // MAINWINDOW_H
