#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QGraphicsScene>
#include <QLayout>
#include <QGraphicsPixmapItem>
#include <QPixmap>

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
    QList<QImage*> _images;
    QList<QGraphicsScene*> _scenes;
    QList<QGraphicsPixmapItem*> _pixmap;
    QList<int> _currentId;

    for(int k = 0; k < ui->tabWidgetInput->count(); k++)
    {
        const QImage& image = _inputTabs[k]->getImage();
        const QMap<int, QSet<QPoint>>& selection = _inputTabs[k]->getCurrentGrid().getSelection();

        int cellWidth = _inputTabs[k]->getCurrentGrid().getCellWidth();
        int cellHeight = _inputTabs[k]->getCurrentGrid().getCellHeight();
        int nbRows = ui->spinBoxRows->value();
        int nbColumns = ui->spinBoxColumns->value();

        foreach(int id, selection.keys())
        {
            while(_images.count() <= id)
            {
                _images.append(nullptr);
                _scenes.append(nullptr);
                _pixmap.append(new QGraphicsPixmapItem());
                _currentId.append(0);
            }
            if(_images[id] == nullptr)
            {
                _images[id] = new QImage(cellWidth * nbColumns, cellHeight * nbRows, QImage::Format_ARGB32);
                _scenes[id] = new QGraphicsScene();
                QWidget* widget = new QWidget();
                QVBoxLayout* layout = new QVBoxLayout();
                widget->setLayout(layout);
                QGraphicsView* graphicView = new QGraphicsView();
                graphicView->setScene(_scenes[id]);
                _scenes[id]->addItem(_pixmap[id]);
                layout->addWidget(graphicView);
                graphicView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                graphicView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

                ui->tabWidgetOutput->addTab(widget, tr("Output ") + QString::number(id));
            }

            foreach(QPoint point, selection[id])
            {
                int toId = _currentId[id]++;
                int toColumn = toId % nbColumns;
                int toRow = toId / nbColumns;

                int startOrigX = point.y() * cellWidth;
                int startOrigY = point.x() * cellHeight;
                int startDestX = toColumn * cellWidth;
                int startDestY = toRow * cellHeight;

                for(int i = 0; i < cellWidth; i++)
                {
                    for(int j = 0; j < cellHeight; j++)
                    {
                        _images[id]->setPixel(i + startDestX, j + startDestY, image.pixel(i + startOrigX, j + startOrigY));
                    }
                }

                _pixmap[id]->setPixmap(QPixmap::fromImage(*_images[id]));
                _scenes[id]->views()[0]->fitInView(_pixmap[id], Qt::KeepAspectRatio);
                _scenes[id]->views()[0]->centerOn(0, 0);
                _scenes[id]->update();
            }
        }
    }
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
