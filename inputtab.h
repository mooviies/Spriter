#ifndef INPUTTAB_H
#define INPUTTAB_H

#include <QObject>
#include <QTableWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QSpinBox>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QMainWindow>

#include "grid.h"

class InputTab : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit InputTab(QMainWindow *mainWindow, QTabWidget *parent, QString filename);
    ~InputTab();

    const QImage& getImage() const { return _spriteSheet; }

    const Grid& getCurrentGrid() const { return _radioButtonAutomatic->isChecked() ? _gridAutomatic : _gridManual; }
    const Grid& getAutomaticGrid() const { return _gridAutomatic; }
    const Grid& getManualGrid() const { return _gridManual; }

    bool getAutomatic() const { return _radioButtonAutomatic->isChecked(); }
    int getRows() const { return _spinBoxRows->value(); }
    int getColumns() const { return _spinBoxColumns->value(); }
    int getOffsetColumn() const { return _spinBoxOffsetColumn->value(); }
    int getOffsetRow() const { return _spinBoxOffsetRow->value(); }

    void setAutomatic(bool automatic) { _radioButtonAutomatic->setChecked(automatic); _radioButtonManual->setChecked(!automatic); }
    void setRows(int rows) { _spinBoxRows->setValue(rows); }
    void setColumns(int columns) { _spinBoxColumns->setValue(columns); }
    void setOffsetColumn(int offsetColumn) { _spinBoxOffsetColumn->setValue(offsetColumn); }
    void setOffsetRow(int offsetRow) { _spinBoxOffsetRow->setValue(offsetRow); }

    void setAutomaticSelection(const Grid& grid);
    void setManualSelection(const Grid& grid);

signals:

public slots:
    void redrawGrid();
    void reselectAutomaticGrid();

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);

private slots:
    void setCurrentId(int id);
    void setAutomaticButton(bool automatic);
    void clear();
    void clearAll();
    void deleteSelf();
    void reloadImage();

private:
    void setUI(QTabWidget *parent);

private:
    QMainWindow *_mainWindow;
    QTabWidget* _parent;

    QPushButton* _pushButtonReload;
    QPushButton* _pushButtonRemove;
    QPushButton* _pushButtonClear;
    QPushButton* _pushButtonClearAll;
    QPushButton* _pushButtonApplyToAll;

    QGraphicsView* _graphicView;
    QRadioButton* _radioButtonAutomatic;
    QRadioButton* _radioButtonManual;

    QSpinBox* _spinBoxRows;
    QSpinBox* _spinBoxColumns;
    QSpinBox* _spinBoxOffsetColumn;
    QSpinBox* _spinBoxOffsetRow;
    QSpinBox* _spinBoxCurrentFrame;

    QString _filename;
    QImage _spriteSheet;
    QGraphicsPixmapItem _spriteSheetItem;

    Grid _gridManual;
    Grid _gridAutomatic;

    bool _currentSelectSetting;
};

#endif // INPUTTAB_H
