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

const QVector<QVector<int>>& Grid::getSelection() const
{
    return _selection;
}

bool Grid::selected(int outputId, int r, int c) const
{
    if(_selection.count() <= outputId)
        return false;

    return _selection[outputId].contains(getId(r, c));
}

bool Grid::selected(int outputId, const QPoint& point) const
{
    QPoint cell = mousePosToGridPos(point);
    return selected(outputId, cell.x(), cell.y());
}

void Grid::select(const QVector<QVector<int>>& selection)
{
    _selection = selection;
}

void Grid::select(int outputId, bool isSelected, int r, int c)
{
    int imageId = getId(r, c);
    if(isSelected)
    {
        if(_selection.count() <= outputId)
            _selection.resize(outputId + 1);

        if(!_selection[outputId].contains(imageId))
            _selection[outputId].append(imageId);
    }
    else
    {
        if(_selection.count() <= outputId)
            return;

        if(_selection[outputId].contains(imageId))
            _selection[outputId].remove(imageId);
    }
}

QPoint Grid::select(int outputId, bool isSelected, const QPoint& point)
{
    QPoint cell = mousePosToGridPos(point);
    select(outputId, isSelected, cell.x(), cell.y());
    return cell;
}

void Grid::clear()
{
    _selection.clear();
}

void Grid::clear(int outputId)
{
    if(_selection.count() > outputId)
        _selection[outputId].clear();
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

            int imageId = getId(i, j);
            for(int outputId = 0; outputId < _selection.length(); outputId++)
            {
                if(_selection[outputId].contains(imageId))
                {
                    painter->setBrush(qtColors[outputId % 15]);

                    if(outputId == _currentId)
                        painter->setOpacity(0.3);
                    else
                        painter->setOpacity(0.9);

                    painter->drawRect(j * _cellWidth, i * _cellHeight, _cellWidth, _cellHeight);

                    painter->setBrush(Qt::black);
                    painter->setOpacity(1);
                    break;
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


