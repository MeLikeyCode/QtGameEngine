#ifndef PATHINGMAP_H
#define PATHINGMAP_H

#include "PathGrid.h"
#include <QPointF>
#include <QRectF>
#include <QMetaType>

/// Represents a pathing map in which each cell is either filled or unfilled.
/// @author Abdullah Aghazadah
/// @date 4-28-15
///
/// A PathingMap can be thought of as a region of space that is occupied by
/// cells. These cells can either be filled or unfilled.
///
/// To find the shortest path between any two cells (or points),
/// PathingMap::shortestPath().
///
/// To set a region of this PathingMap to be the same as the filling of another
/// PathingMap use PathingMap::setFilling().
///
/// To add pathing from another PathingMap to a region of this PathingMap, use
/// PathingMap::addPathing().
///
/// To set the filling of the PathingMap based on a 2d int vector, use
/// PathingMap::setFilling(const std::vector<std::vector<int>>&).
///
/// Several functions are provided for retrieving a subregion of cells.
/// PathingMap::cells() and its overloads returns a subregion of cells represented
/// by Nodes. PathingMap::cellsAsRects() and its overloads returns a subregion
/// of cells represented by QRectFs.
class PathingMap{
public:
    // constructors
    PathingMap();
    PathingMap(int numCellsWid, int numCellsLong, int cellSize);
    PathingMap(const PathingMap& copy) = default;

    // readers ("getters")
    std::vector<Node> cells(const Node& topLeft, const Node& bottomRight) const;
    std::vector<Node> cells(const QPointF& topLeft, const QPointF& bottomRight) const;
    std::vector<Node> cells(const QRectF& inRegion) const;
    std::vector<Node> cells() const;
    QRectF cellAsRect(Node cell) const;
    std::vector<QRectF> cellsAsRects(const Node& topLeft, const Node& bottomRight) const;
    std::vector<QRectF> cellsAsRects(const QPointF& topLeft, const QPointF& bottomRight) const;
    std::vector<QRectF> cellsAsRects(const QRectF& inRegion) const;
    std::vector<QRectF> cellsAsRects() const;
    bool filled(const Node& cell) const;
    bool filled(const QPointF& point) const;
    bool filled(const QRectF& region) const;
    bool free(const QRectF& region) const;
    std::vector<QPointF> shortestPath(const Node& fromCell, const Node& toCell) const;
    std::vector<QPointF> shortestPath(const QPointF& fromPt, const QPointF& toPt) const;
    int width() const;
    int height() const;
    int cellSize() const;
    int numCellsLong() const;
    int numCellsWide() const;
    Node pointToCell(const QPointF& point) const;
    QPointF cellToPoint(const Node& cell) const;
    QPointF pointToCellPoint(const QPointF& point);

    // modifiers ("setters")
    void fill(const Node& cell);
    void fill(const QPointF& point);
    void fill(const Node& topLeft, const Node& bottomRight);
    void fill(const QPointF& topLeft, const QPointF& bottomRight);
    void fill(const QRectF& region);
    void unfill(const Node& cell);
    void unfill(const QPointF& point);
    void unfill(const Node& topLeft, const Node& bottomRight);
    void unfill(const QPointF& topLeft, const QPointF& bottomRight);
    void unfill(const QRectF& region);
    void setFilling(const std::vector<std::vector<int>>& vec);
    void setFilling(const Node& pos, const PathingMap& littleMap);
    void setFilling(const QPointF& pos, const PathingMap& littleMap);
    void addFilling(const PathingMap& littleMap, const Node& pos);
    void addFilling(const PathingMap& littleMap, const QPointF& pos);

private:
    // main private attributes
    PathGrid pathGrid_;
    int numCellsWide_;
    int numCellsLong_;
    int cellSize_;
};

Q_DECLARE_METATYPE(PathingMap);

#endif // PATHINGMAP_H
