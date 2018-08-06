#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _manualExport(false),
    APPLICATION_IMAGE_PATH("application/image/path")
{
    ui->setupUi(this);
    connect(ui->pushButtonAdd, SIGNAL(clicked()), this, SLOT(addImages()));
    connect(ui->pushButtonClear, SIGNAL(clicked()), this, SLOT(clearAll()));
    connect(ui->tabWidgetMain, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));

    QCoreApplication::setOrganizationDomain("mooviies.com");
    QCoreApplication::setOrganizationName("mooviies");
    QCoreApplication::setApplicationName("SpriteSheetSplitter");

    loadSettings();
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}

void MainWindow::setExport(int rows, int columns)
{
    if(!_manualExport)
    {
        ui->spinBoxRows->setValue(rows);
        ui->spinBoxColumns->setValue(columns);
    }
}

void MainWindow::addImages()
{
    if(ui->tabWidgetInput->count() == 0)
        _manualExport = false;

    QString currentPath = _currentPath;

    QString selfilter = tr("PNG (*.png)");
    QStringList filenames = QFileDialog::getOpenFileNames(this, "Add spritesheet(s)", currentPath, tr("All files (*.*);;PNG (*.png);;JPEG (*.jpg *.jpeg);;TIFF (*.tif);;BMP (*.bmp)"), &selfilter);
    if(filenames.isEmpty())
        return;

    _currentPath = QFileInfo(filenames[0]).absoluteDir().absolutePath();

    foreach (QString filename, filenames){
        _inputTabs.append(new InputTab(this, ui->tabWidgetInput, filename));
    }

    saveSettings();
}

void MainWindow::applyToAll()
{
    int index = ui->tabWidgetInput->currentIndex();
    InputTab* currentTab = _inputTabs[index];
    for(int i = 0; i < _inputTabs.length(); i++)
    {
        if(i != index)
        {
            InputTab* tab = _inputTabs[i];
            tab->setAutomatic(currentTab->getAutomatic());
            tab->setColumns(currentTab->getColumns());
            tab->setRows(currentTab->getRows());
            tab->setOffsetColumn(currentTab->getOffsetColumn());
            tab->setOffsetRow(currentTab->getOffsetRow());
            tab->setAutomaticSelection(currentTab->getAutomaticGrid());
            tab->setManualSelection(currentTab->getManualGrid());
        }
    }
}

void MainWindow::clearAll()
{
    ui->tabWidgetInput->clear();
    _manualExport = false;
}

void MainWindow::tabChanged(int index)
{
    if(index == 1)
        generateOutput();
}

void MainWindow::generateOutput()
{
    ui->tabWidgetOutput->clear();


}

void MainWindow::exportOutput()
{

}

void MainWindow::loadSettings()
{
    QSettings settings;
    _currentPath = settings.value(APPLICATION_IMAGE_PATH, QDir::currentPath()).toString();
}

void MainWindow::saveSettings()
{
    QSettings settings;
    settings.setValue(APPLICATION_IMAGE_PATH, _currentPath);
}
