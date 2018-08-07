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

    const QVector<QVector<int>>& getSelection() const;
    bool selected(int id, int r, int c) const;
    bool selected(int id, const QPoint& point) const;

    void select(const QVector<QVector<int>>& selection);
    void select(int id, bool isSelected, int r, int c);
    QPoint select(int id, bool isSelected, const QPoint& point);

    void clear();
    void clear(int id);

    void setCurrentId(int id){ _currentId = id; }

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    int getId(int r, int c) const { return r * _columns + c; }

public:
    QPoint mousePosToGridPos(const QPoint& point) const;

private:
    QRectF _boundingRect;
    int _cellWidth;
    int _cellHeight;
    int _columns;
    int _rows;
    int _currentId;

    QVector<QVector<int>> _selection;
};

inline uint qHash(const QPoint& point)
{
    return static_cast<uint>(point.x()) + static_cast<uint>(point.y()) * 11;
}

#endif // GRID_H
