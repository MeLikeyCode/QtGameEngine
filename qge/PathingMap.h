#pragma once

#include "Vendor.h"

#include "PathGrid.h"

namespace qge{

/// Represents a rectangular region of space divided into square cells where
/// each cell can either be filled or unfilled.
/// @author Abdullah Aghazadah
/// @date 4-28-15
///
/// To find the shortest path between any two cells (or points), use
/// PathingMap::shortestPath().
///
/// Several functions help with filling the PathingMap in various ways, such as
/// "adding" another PathingMap, or setting a certain region of the PathingMap
/// to be filled in the pattern of another PathingMap, etc...
///
/// To set the filling of the PathingMap based on a 2d int vector, use
/// PathingMap::setFilling(const std::vector<std::vector<int>>&).
///
/// Several functions are provided for retrieving a subset of cells.
/// PathingMap::cells() and its overloads returns a subset of cells represented
/// by Nodes. PathingMap::cellsAsRects() and its overloads returns a subset
/// of cells represented by QRectFs.
class PathingMap{
public:
    // constructors
    PathingMap();
    PathingMap(int numCellsWide, int numCellsLong, int cellSize);
    PathingMap(const QPixmap &pixmap, int cellSize);
    PathingMap(const PathingMap& copy) = default;   // make compiler generate default copy ctor
                                                    // (memberwise copy).
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
    bool canFit(const PathingMap& specifiedPathingMap, const QPointF& specifiedPos) const;
    bool contains(const QPointF& pos) const;

    // modifiers ("setters")
    void fill(const Node& cell);
    void fill(const QPointF& point);
    void fill(const Node& topLeft, const Node& bottomRight);
    void fill(const QPointF& topLeft, const QPointF& bottomRight);
    void fill(const QRectF& region);
    void fill();
    void unfill(const Node& cell);
    void unfill(const QPointF& point);
    void unfill(const Node& topLeft, const Node& bottomRight);
    void unfill(const QPointF& topLeft, const QPointF& bottomRight);
    void unfill(const QRectF& region);
    void unfill();
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

}

Q_DECLARE_METATYPE(qge::PathingMap);
