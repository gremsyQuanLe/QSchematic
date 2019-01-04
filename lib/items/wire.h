#pragma once

#include "item.h"
#include "wirepoint.h"
#include "line.h"

class QVector2D;

namespace QSchematic {

    class Line;

    /**
     * IMPORTANT NOTE: The points coordinates are RELATIVE and in SCENE COORDINATES.
     *                 Wires must be movable so we can move entire groups of stuff.
     */
    class Wire : public Item
    {
        Q_OBJECT

    public:
        Wire(int type = Item::WireType, QGraphicsItem* parent = nullptr);
        virtual ~Wire() override = default;
        virtual void update() override;

        virtual QJsonObject toJson() const override;
        virtual bool fromJson(const QJsonObject& object) override;
        virtual std::unique_ptr<Item> deepCopy() const override;
        virtual QRectF boundingRect() const override;
        virtual QPainterPath shape() const override;

        void prependPoint(const QPoint& point);
        void appendPoint(const QPoint& point);
        void insertPoint(int index, const QPoint& point);        // Index of new point
        void removeFirstPoint();
        void removeLastPoint();
        void removePoint(const QPoint& point);
        QVector<WirePoint> wirePointsRelative() const;
        QVector<WirePoint> wirePointsAbsolute() const;
        QVector<QPoint> pointsRelative() const;
        QVector<QPoint> pointsAbsolute() const;
        void simplify();
        void movePointBy(int index, const QVector2D& moveBy);
        void movePointTo(int index, const QPoint& moveTo);
        void moveLineSegmentBy(int index, const QVector2D& moveBy);
        void setPointIsJunction(int index, bool isJunction);
        bool pointIsOnWire(const QPoint& point) const;

        QList<QSchematic::Line> lineSegments() const;

    signals:
        void pointMoved(Wire& wire, WirePoint& point);

    protected:
        void copyAttributes(Wire& dest) const;
        static void simplify(QVector<WirePoint>& points);
        void calculateBoundingRect();

        virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
        virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
        virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
        virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    private:
        Q_DISABLE_COPY(Wire)

        static void removeDuplicatePoints(QVector<WirePoint>& pointsRelative);
        static void removeObsoletePoints(QVector<WirePoint>& pointsRelative);

        QVector<WirePoint> _points;
        QRectF _rect;
        int _pointToMoveIndex;
        int _lineSegmentToMoveIndex;
        QPoint _prevMousePos;
    };

}
