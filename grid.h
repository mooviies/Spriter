#ifndef GRID_H
#define GRID_H

#include <QGraphicsItem>
#include <QPainter>
#include <QSet>

class Grid : public QGraphicsItem
{
public:
    Grid(int rows = 1, int columns = 1);
    Grid(const QRectF& rect, int rows = 1, int columns = 1);
    ~Grid();

    int getCellWidth() const { return _cellWidth; }
    int getCellHeight() const { return _cellHeight; }
    int getNbRows() const { return _rows; }
    int getNbColumns() const { return _columns; }

    void resize(const QRectF& rect);
    void modify(int rows, int columns);

    const QMap<int, QSet<QPoint>>& getSelection() const;
    bool selected(int id, int r, int c) const;
    bool selected(int id, const QPoint& point) const;

    void select(const QMap<int, QSet<QPoint>>& selection);
    void select(int id, bool isSelected, int r, int c);
    QPoint select(int id, bool isSelected, const QPoint& point);

    QPoint toggle(int id, const QPoint& point);

    void clear();
    void clear(int id);

    void setCurrentId(int id){ _currentId = id; }

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public:
    QPoint mousePosToGridPos(const QPoint& point) const;

private:
    QRectF _boundingRect;
    int _cellWidth;
    int _cellHeight;
    int _columns;
    int _rows;
    int _currentId;

    QMap<int, QSet<QPoint>> _selection;
};

inline uint qHash(const QPoint& point)
{
    return static_cast<uint>(point.x()) + static_cast<uint>(point.y()) * 11;
}

#endif // GRID_H
