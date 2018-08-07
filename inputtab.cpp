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

#include "inputtab.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QFileInfo>
#include <QScrollBar>
#include <QMessageBox>

#include "mainwindow.h"

InputTab::InputTab(QMainWindow *mainWindow, QTabWidget *parent, QString filename) :
    QGraphicsScene(), _mainWindow(mainWindow), _parent(parent), _filename(filename), _spriteSheet(filename), _initialFitInView(false)
{
    setUI(parent);
    reloadImage();
    _graphicView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _graphicView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    addItem(&_spriteSheetItem);

    _gridManual.resize(_spriteSheetItem.boundingRect());
    addItem(&_gridManual);
    _gridManual.setVisible(false);

    _gridAutomatic.resize(_spriteSheetItem.boundingRect());
    addItem(&_gridAutomatic);

    connect(_pushButtonApplyToAll, SIGNAL(clicked()), static_cast<MainWindow*>(mainWindow), SLOT(applyToAll()));
    connect(_spinBoxOffsetColumn, SIGNAL(valueChanged(int)), this, SLOT(reselectAutomaticGrid()));
    connect(_spinBoxOffsetRow, SIGNAL(valueChanged(int)), this, SLOT(reselectAutomaticGrid()));
    connect(_pushButtonRemove, SIGNAL(clicked()), this, SLOT(deleteSelf()));
}

InputTab::~InputTab()
{
}

void InputTab::setAutomaticSelection(const Grid& grid)
{
    _gridAutomatic.select(grid.getSelection());
}

void InputTab::setManualSelection(const Grid& grid)
{
    _gridManual.select(grid.getSelection());
}

void InputTab::redrawGrid()
{
    _gridManual.modify(_spinBoxRows->value(), _spinBoxColumns->value());
    _gridAutomatic.modify(_spinBoxRows->value(), _spinBoxColumns->value());

    _spinBoxOffsetColumn->setMaximum(_spinBoxColumns->value());
    _spinBoxOffsetRow->setMaximum(_spinBoxRows->value());

    _spinBoxCurrentFrame->setMaximum(_spinBoxColumns->value() * _spinBoxRows->value());

    static_cast<MainWindow>(_mainWindow).setExport(_spinBoxRows->value(), _spinBoxColumns->value());

    if(_radioButtonAutomatic->isChecked())
        reselectAutomaticGrid();
    else
        update();
}

void InputTab::reselectAutomaticGrid()
{
    _gridAutomatic.clear();
    int offsetColumn = _spinBoxOffsetColumn->value();
    int offsetRow = _spinBoxOffsetRow->value();

    for(int r = 0; r < _gridAutomatic.getNbRows(); r++)
    {
        for(int c = 0; c < _gridAutomatic.getNbColumns(); c++)
        {
            _gridAutomatic.select(c % offsetColumn + (r % offsetRow) * offsetColumn, true, r, c);
        }
    }
    update();
}

void InputTab::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(_radioButtonAutomatic->isChecked())
        return;

    if(mouseEvent->buttons() == Qt::LeftButton || mouseEvent->buttons() == Qt::RightButton)
    {
        _gridManual.select(_spinBoxCurrentFrame->value(), _currentSelectSetting, mouseEvent->scenePos().toPoint());
        update();
    }
}

void InputTab::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(_radioButtonAutomatic->isChecked())
        return;

    if(mouseEvent->buttons() == Qt::LeftButton)
        _currentSelectSetting = true;
    else if(mouseEvent->buttons() == Qt::RightButton)
        _currentSelectSetting = false;

    _gridManual.select(_spinBoxCurrentFrame->value(), _currentSelectSetting, mouseEvent->scenePos().toPoint());
    update();
}

void InputTab::drawForeground(QPainter *painter, const QRectF &rect)
{
    if(!_initialFitInView)
    {
        updateImageView();
        _initialFitInView = true;
    }

    QGraphicsScene::drawForeground(painter, rect);
}

void InputTab::updateImageView()
{
    QRectF rect = _spriteSheet.rect();
    rect.setHeight(16);
    _graphicView->fitInView(rect, Qt::KeepAspectRatio);
    _graphicView->centerOn(0, 0);
    update();
}

void InputTab::setCurrentId(int id)
{
    _gridManual.setCurrentId(id);
    _gridAutomatic.setCurrentId(id);
    update();
}

void InputTab::setAutomaticButton(bool automatic)
{
    if(automatic)
    {
        _gridAutomatic.setVisible(true);
        _gridManual.setVisible(false);
    }
    else
    {
        _gridAutomatic.setVisible(false);
        _gridManual.setVisible(true);
    }
}

void InputTab::setManualButton(bool manual)
{
    if(manual && _gridAutomatic.getSelection().count() > 0)
    {
        QMessageBox::StandardButton reply = QMessageBox::question(_mainWindow, tr("Manual Mode"),
                                                                  "Do you want to keep the automatic selection for editing?",
                                                                  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        if(reply == QMessageBox::Yes)
        {
            _gridManual.select(_gridAutomatic.getSelection());
        }
        else if(reply == QMessageBox::Cancel)
        {
            return;
        }
    }

    setAutomaticButton(!manual);
}

void InputTab::clear()
{
    _gridManual.clear(_spinBoxCurrentFrame->value());
    update();
}

void InputTab::clearAll()
{
    _gridManual.clear();
    update();
}

void InputTab::deleteSelf()
{
    _parent->removeTab(_parent->currentIndex());
}

void InputTab::reloadImage()
{
    _spriteSheet.load(_filename);
    _spriteSheetItem.setPixmap(QPixmap::fromImage(_spriteSheet));
    updateImageView();
}

void InputTab::setUI(QTabWidget *parent)
{
    QWidget* widget = new QWidget(parent);
    parent->addTab(widget, QFileInfo(_filename).fileName());

    _pushButtonReload = new QPushButton(tr("Refresh"));
    _pushButtonRemove = new QPushButton(tr("Remove"));
    _pushButtonClear = new QPushButton(tr("Clear"));
    _pushButtonClearAll = new QPushButton(tr("ClearAll"));
    _pushButtonApplyToAll = new QPushButton(tr("Apply To All"));
    _graphicView = new QGraphicsView(this);
    _radioButtonAutomatic = new QRadioButton(tr("Automatic"));
    _radioButtonAutomatic->setChecked(true);
    _radioButtonManual = new QRadioButton(tr("Manual"));

    _spinBoxRows = new QSpinBox();
    _spinBoxRows->setMinimum(1);

    _spinBoxColumns = new QSpinBox();
    _spinBoxColumns->setMinimum(1);

    _spinBoxOffsetColumn = new QSpinBox();
    _spinBoxOffsetColumn->setMinimum(1);

    _spinBoxOffsetRow = new QSpinBox();
    _spinBoxOffsetRow->setMinimum(1);

    _spinBoxCurrentFrame = new QSpinBox();

    QHBoxLayout* tabLayout = new QHBoxLayout();
    QVBoxLayout* graphicLayout = new QVBoxLayout();
    QHBoxLayout* graphicButtonLayout = new QHBoxLayout();

    tabLayout->addLayout(graphicLayout);
    widget->setLayout(tabLayout);

    graphicButtonLayout->addWidget(_pushButtonReload);
    graphicButtonLayout->addWidget(_pushButtonRemove);
    graphicButtonLayout->addStretch();

    graphicLayout->addLayout(graphicButtonLayout);
    graphicLayout->addWidget(_graphicView);

    QVBoxLayout* settings = new QVBoxLayout();
    QGroupBox* spriteSheet = new QGroupBox(tr("SpriteSheet"));
    QGroupBox* selection = new QGroupBox(tr("Selection"));
    QVBoxLayout* spriteSheetLayout = new QVBoxLayout();
    QVBoxLayout* selectionLayout = new QVBoxLayout();
    QVBoxLayout* automaticLayout = new QVBoxLayout();
    QVBoxLayout* manualLayout = new QVBoxLayout();
    QHBoxLayout* clearLayout = new QHBoxLayout();
    QHBoxLayout* rows = new QHBoxLayout();
    QHBoxLayout* columns = new QHBoxLayout();
    QHBoxLayout* currentFrame = new QHBoxLayout();
    QHBoxLayout* offsetRow = new QHBoxLayout();
    QHBoxLayout* offsetColumn = new QHBoxLayout();
    QHBoxLayout* applyToAll = new QHBoxLayout();
    QFrame* automaticFrame = new QFrame();
    QFrame* manualFrame = new QFrame();

    tabLayout->addLayout(settings);
    settings->addWidget(spriteSheet);
    settings->addWidget(selection);
    settings->addStretch();
    spriteSheet->setLayout(spriteSheetLayout);
    selection->setLayout(selectionLayout);
    automaticFrame->setLayout(automaticLayout);
    manualFrame->setLayout(manualLayout);

    rows->addWidget(new QLabel(tr("Rows")));
    rows->addWidget(_spinBoxRows);
    columns->addWidget(new QLabel(tr("Columns")));
    columns->addWidget(_spinBoxColumns);
    currentFrame->addWidget(new QLabel(tr("Current Output")));
    currentFrame->addWidget(_spinBoxCurrentFrame);
    offsetRow->addWidget(new QLabel(tr("Row Offset")));
    offsetRow->addWidget(_spinBoxOffsetRow);
    offsetColumn->addWidget(new QLabel(tr("Column Offset")));
    offsetColumn->addWidget(_spinBoxOffsetColumn);
    applyToAll->addStretch();
    applyToAll->addWidget(_pushButtonApplyToAll);
    clearLayout->addStretch();
    clearLayout->addWidget(_pushButtonClear);
    clearLayout->addWidget(_pushButtonClearAll);

    spriteSheetLayout->addLayout(rows);
    spriteSheetLayout->addLayout(columns);
    selectionLayout->addWidget(_radioButtonAutomatic);
    selectionLayout->addWidget(_radioButtonManual);
    selectionLayout->addLayout(currentFrame);
    manualLayout->addLayout(clearLayout);
    automaticLayout->addLayout(offsetRow);
    automaticLayout->addLayout(offsetColumn);
    selectionLayout->addWidget(automaticFrame);
    selectionLayout->addWidget(manualFrame);
    selectionLayout->addLayout(applyToAll);
    manualFrame->setVisible(false);

    connect(_spinBoxColumns, SIGNAL(valueChanged(int)), this, SLOT(redrawGrid()));
    connect(_spinBoxRows, SIGNAL(valueChanged(int)), this, SLOT(redrawGrid()));

    connect(_radioButtonAutomatic, SIGNAL(toggled(bool)), automaticFrame, SLOT(setVisible(bool)));
    connect(_radioButtonAutomatic, SIGNAL(toggled(bool)), manualFrame, SLOT(setHidden(bool)));
    connect(_radioButtonAutomatic, SIGNAL(toggled(bool)), this, SLOT(setAutomaticButton(bool)));

    connect(_radioButtonManual, SIGNAL(toggled(bool)), manualFrame, SLOT(setVisible(bool)));
    connect(_radioButtonManual, SIGNAL(toggled(bool)), automaticFrame, SLOT(setHidden(bool)));
    connect(_radioButtonManual, SIGNAL(clicked(bool)), this, SLOT(setManualButton(bool)));

    connect(_spinBoxCurrentFrame, SIGNAL(valueChanged(int)), this, SLOT(setCurrentId(int)));

    connect(_pushButtonClear, SIGNAL(clicked()), this, SLOT(clear()));
    connect(_pushButtonClearAll, SIGNAL(clicked()), this, SLOT(clearAll()));

    connect(_pushButtonReload, SIGNAL(clicked()), this, SLOT(reloadImage()));
}
