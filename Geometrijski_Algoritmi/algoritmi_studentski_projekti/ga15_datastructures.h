#ifndef GA15_DATASTRUCTURES_H
#define GA15_DATASTRUCTURES_H

#include <QPointF>
#include <QLineF>
#include <QPolygon>
#include <algorithm>

#include "pomocnefunkcije.h"

enum class WhichPolygon {
    LEFT,
    RIGHT
};

enum class EventType {
    TOP_VERTEX,
    BOTTOM_VERTEX,
    MIDDLE_VERTEX
};

struct eventPoint {
    eventPoint(QPoint *v,
               const EventType &type,
               const WhichPolygon &which,
               QLineF *e1,
               QLineF *e2)
        : vertex(v), vertexType(type),
          whichPolygon(which),
          edge1(e1), edge2(e2) {}

    QPoint *vertex;
    const EventType vertexType;
    const WhichPolygon whichPolygon;
    QLineF *edge1;
    QLineF *edge2;

};

struct eventComparison {
    bool operator()(const eventPoint &a, const eventPoint &b) const {

        return (a.vertex->y() > b.vertex->y()) ||
               (a.vertex->y() == b.vertex->y() && a.vertex->x() < b.vertex->x());
    }
};

struct edgeComparison {
    const double *const sweepLineY;
    WhichPolygon whichPolygon;

    edgeComparison(double *y, WhichPolygon which)
        : sweepLineY(y), whichPolygon(which)
    {}

    bool operator()(const QLineF *edge1, const QLineF *edge2) const {

        double minY1 = std::min(edge1->y1(), edge1->y2());
        double minY2 = std::min(edge2->y1(), edge2->y2());
        double eps = std::min(*sweepLineY-minY1, *sweepLineY-minY2) / 2;

        QLineF sweepLine( 0, *sweepLineY-eps,
                         1, *sweepLineY-eps);
        QPointF intersection1;
        QPointF intersection2;
        pomocneFunkcije::presekDuzi(*edge1, sweepLine, intersection1);
        pomocneFunkcije::presekDuzi(*edge2, sweepLine, intersection2);

        if (whichPolygon == WhichPolygon::RIGHT)
            return intersection1.x() < intersection2.x();
        /*else if (whichPolygon == WhichPolygon::LEFT)*/
        return intersection1.x() > intersection2.x();
    }
};

#endif // GA15_DATASTRUCTURES_H