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
    APPLICATION_IMAGE_PATH("application/image/path"),
    ALL_FILES(tr("All files (*.*)")),
    PNG_FILE(tr("PNG (*.png)")),
    TIFF_FILE(tr("TIFF (*.tif)")),
    JPG_FILE(tr("JPEG (*.jpg *.jpeg)")),
    BMP_FILE(tr("BMP (*.bmp)")),
    IMAGE_FILE(ALL_FILES + ";;" + PNG_FILE + ";;" + JPG_FILE + ";;" + TIFF_FILE + ";;" + BMP_FILE)
{
    ui->setupUi(this);
    connect(ui->pushButtonAdd, SIGNAL(clicked()), this, SLOT(addImages()));
    connect(ui->pushButtonClear, SIGNAL(clicked()), this, SLOT(clearAll()));
    connect(ui->tabWidgetMain, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
    connect(ui->spinBoxColumns, SIGNAL(valueChanged(int)), this, SLOT(generateOutput()));
    connect(ui->spinBoxRows, SIGNAL(valueChanged(int)), this, SLOT(generateOutput()));
    connect(ui->pushButtonExport, SIGNAL(clicked()), this, SLOT(exportOutput()));

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

    QString selfilter = PNG_FILE;
    QStringList filenames = QFileDialog::getOpenFileNames(this, tr("Add spritesheet(s)"), currentPath, IMAGE_FILE, &selfilter);
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
    if(ui->tabWidgetMain->currentIndex() == 0)
        return;

    ui->tabWidgetOutput->clear();

    foreach(QImage* image, _images)
        delete image;

    _images.clear();
    _scenes.clear();
    _currentId.clear();

    for(int k = 0; k < ui->tabWidgetInput->count(); k++)
    {
        const Grid& grid = _inputTabs[k]->getCurrentGrid();
        const QImage& image = _inputTabs[k]->getImage();
        const QVector<QVector<int>>& selection = grid.getSelection();

        int cellWidth = grid.getCellWidth();
        int cellHeight = grid.getCellHeight();
        int nbColumnsInput = grid.getNbColumns();
        int nbRows = ui->spinBoxRows->value();
        int nbColumns = ui->spinBoxColumns->value();

        for(int outputId = 0; outputId < selection.count(); outputId++)
        {
            if(selection[outputId].count() == 0)
                continue;

            while(_images.count() <= outputId)
            {
                _images.append(nullptr);
                _scenes.append(nullptr);
                _pixmap.append(new QGraphicsPixmapItem());
                _currentId.append(0);
            }
            if(_images[outputId] == nullptr)
            {
                _images[outputId] = new QImage(cellWidth * nbColumns, cellHeight * nbRows, QImage::Format_ARGB32);
                _images[outputId]->fill(Qt::magenta);
                _scenes[outputId] = new QGraphicsScene();
                QWidget* widget = new QWidget();
                QVBoxLayout* layout = new QVBoxLayout();
                widget->setLayout(layout);
                QGraphicsView* graphicView = new QGraphicsView();
                graphicView->setScene(_scenes[outputId]);
                _pixmap[outputId]->setPixmap(QPixmap::fromImage(*_images[outputId]));
                _scenes[outputId]->addItem(_pixmap[outputId]);
                _scenes[outputId]->views()[0]->fitInView(_pixmap[outputId], Qt::KeepAspectRatio);
                _scenes[outputId]->views()[0]->centerOn(0, 0);
                layout->addWidget(graphicView);
                graphicView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                graphicView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

                ui->tabWidgetOutput->addTab(widget, tr("Output ") + QString::number(outputId));
            }

            for(int p = 0; p < selection[outputId].count(); p++)
            {
                int toId = _currentId[outputId]++;
                int toColumn = toId % nbColumns;
                int toRow = toId / nbColumns;
                int fromColumn = selection[outputId][p] % nbColumnsInput;
                int fromRow = selection[outputId][p] / nbColumnsInput;

                if(toId >= nbColumns * nbRows)
                    continue;

                int startOrigX = fromColumn * cellWidth;
                int startOrigY = fromRow * cellHeight;
                int startDestX = toColumn * cellWidth;
                int startDestY = toRow * cellHeight;

                for(int i = 0; i < cellWidth; i++)
                {
                    for(int j = 0; j < cellHeight; j++)
                    {
                        _images[outputId]->setPixel(i + startDestX, j + startDestY, image.pixel(i + startOrigX, j + startOrigY));
                    }
                }

                _pixmap[outputId]->setPixmap(QPixmap::fromImage(*_images[outputId]));
                _scenes[outputId]->update();
            }
        }
    }
}

void MainWindow::exportOutput()
{
    QString selfilter = PNG_FILE;
    QString filename = QFileDialog::getSaveFileName(this, tr("Export spritesheet(s)"), _currentPath, IMAGE_FILE, &selfilter);
    if(filename.isEmpty())
        return;

    QFileInfo info(filename);

    if(info.suffix() == "")
    {
        if(selfilter == TIFF_FILE)
            filename += ".tif";
        else if(selfilter == JPG_FILE)
            filename += ".jpg";
        else if(selfilter == BMP_FILE)
            filename += ".bmp";
        else
            filename += ".png";
    }

    info = QFileInfo(filename);

    for(int i = 0; i < _images.length(); i++)
    {
        _images[i]->save(info.absoluteDir().absolutePath() + "/" + info.completeBaseName() + "_" + QString::number(i) + "." + info.suffix());
    }
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
