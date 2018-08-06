#include "grid.h"

Grid::Grid(int rows, int columns)
    : _currentId(0)
{
    modify(columns, rows);
}

void Grid::resize(const QRectF& rect)
{
    _boundingRect = rect;
}

void Grid::modify(int rows, int columns)
{
    _rows = rows;
    _columns = columns;
    _cellHeight = static_cast<int>(_boundingRect.height() / _rows);
    _cellWidth = static_cast<int>(_boundingRect.width() / _columns);
}

Grid::~Grid()
{

}

const QMap<int, QSet<QPoint>>& Grid::getSelection() const
{
    return _selection;
}

bool Grid::selected(int id, int r, int c) const
{
    if(!_selection.contains(id))
        return false;

    return _selection[id].contains(QPoint(r, c));
}

bool Grid::selected(int id, const QPoint& point) const
{
    QPoint cell = mousePosToGridPos(point);
    return selected(id, cell.x(), cell.y());
}

void Grid::select(const QMap<int, QSet<QPoint>>& selection)
{
    _selection = selection;
}

void Grid::select(int id, bool isSelected, int r, int c)
{
    QPoint point = QPoint(r, c);
    if(isSelected)
    {
        if(!_selection.contains(id) || !_selection[id].contains(point))
            _selection[id].insert(point);
    }
    else
    {
        if(!_selection.contains(id))
            return;

        if(_selection[id].contains(point))
            _selection[id].remove(point);

        if(_selection[id].count() == 0)
            _selection.remove(id);
    }
}

QPoint Grid::select(int id, bool isSelected, const QPoint& point)
{
    QPoint cell = mousePosToGridPos(point);
    select(id, isSelected, cell.x(), cell.y());
    return cell;
}

QPoint Grid::toggle(int id, const QPoint& point)
{
    QPoint cell = mousePosToGridPos(point);

    bool isSelected;
    if(!_selection.contains(id))
        isSelected = true;
    else
        isSelected = !_selection[id].contains(cell);

    select(id, isSelected, cell.x(), cell.y());
    return cell;
}

void Grid::clear()
{
    _selection.clear();
}

void Grid::clear(int id)
{
    if(_selection.contains(id))
        _selection[id].clear();
}

QRectF Grid::boundingRect() const
{
    return _boundingRect;
}

void Grid::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Qt::GlobalColor qtColors[] = {Qt::red, Qt::green, Qt::blue, Qt::cyan, Qt::magenta, Qt::yellow,
                                  Qt::darkGray, Qt::darkRed, Qt::darkGreen, Qt::darkBlue, Qt::lightGray,
                                  Qt::darkCyan, Qt::darkMagenta, Qt::darkYellow, Qt::gray};
    QColor color;

    for(int i = 0; i <= _rows; i++)
    {
        painter->drawLine(QLine(0, i * _cellHeight, static_cast<int>(_boundingRect.width()), i * _cellHeight));
        for(int j = 0; j <= _columns; j++)
        {
            if(i == 0)
            {
                painter->drawLine(QLine(j * _cellWidth, 0, j * _cellWidth, static_cast<int>(_boundingRect.height())));
            }

            foreach(int id, _selection.keys())
            {
                if(_selection[id].contains(QPoint(i, j)))
                {
                    painter->setBrush(qtColors[id % 15]);

                    if(id == _currentId)
                        painter->setOpacity(0.3);
                    else
                        painter->setOpacity(0.9);

                    painter->drawRect(j * _cellWidth, i * _cellHeight, _cellWidth, _cellHeight);

                    painter->setBrush(Qt::black);
                    painter->setOpacity(1);
                }
            }
        }
    }

    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QPoint Grid::mousePosToGridPos(const QPoint& point) const
{
    return QPoint(point.y() * _rows / static_cast<int>(_boundingRect.height()), point.x() * _columns / static_cast<int>(_boundingRect.width()));
}


