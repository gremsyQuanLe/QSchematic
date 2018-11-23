#include <QPainter>
#include <QtMath>
#include "node.h"
#include "connector.h"
#include "../scene.h"

const QColor COLOR_HIGHLIGHTED = QColor(Qt::blue).lighter();
const QColor COLOR_BODY_FILL   = QColor(Qt::green);
const QColor COLOR_BODY_BORDER = QColor(Qt::black);
const qreal PEN_WIDTH          = 1.5;

using namespace QSchematic;

const int DEFAULT_WIDTH     = 8;
const int DEFAULT_HEIGHT    = 12;

Node::Node(QGraphicsItem* parent) :
    Item(ItemType::NodeType, parent),
    _size(DEFAULT_WIDTH, DEFAULT_HEIGHT),
    _connectorsMovable(false)
{
}

QSize Node::size() const
{
    return _size;
}

QRect Node::bodyRect() const
{
    return QRect(0, 0, _size.width(), _size.height());
}

bool Node::addConnector(const QPoint& point, const QString& text)
{
    // Create connector
    auto connector = new Connector(this);
    connector->setGridPoint(point);
    connector->setText(text);
    connector->setMovable(_connectorsMovable);
    _connectors << connector;

    return true;
}

QList<QPoint> Node::connectionPoints() const
{
    QList<QPoint> list;

    for (const auto& connector : _connectors) {
        list << connector->connectionPoint();
    }

    return list;
}

bool Node::isConnectionPoint(const QPoint& gridPoint) const
{
    return connectionPoints().contains(gridPoint);
}

void Node::setConnectorsMovable(bool enabled)
{
    // Update connectors
    for (auto connector : _connectors) {
        connector->setMovable(enabled);
    }

    // Update local
    _connectorsMovable = enabled;
}

bool Node::connectorsMovable() const
{
    return _connectorsMovable;
}

QRectF Node::boundingRect() const
{
    qreal adj = qCeil(PEN_WIDTH / 2.0);
    if (highlighted()) {
        adj += _settings.highlightRectPadding;
    }

    return QRectF(QPoint(0, 0), _size*_settings.gridSize).adjusted(-adj, -adj, adj, adj);
}

void Node::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    // Draw the bounding rect if debug mode is enabled
    if (_settings.debug) {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QBrush(Qt::red));
        painter->drawRect(boundingRect());
    }

    // Highlight rectangle
    if (highlighted()) {
        // Highlight pen
        QPen highlightPen;
        highlightPen.setStyle(Qt::NoPen);

        // Highlight brush
        QBrush highlightBrush;
        highlightBrush.setStyle(Qt::SolidPattern);
        highlightBrush.setColor(COLOR_HIGHLIGHTED);

        // Highlight rectangle
        painter->setPen(highlightPen);
        painter->setBrush(highlightBrush);
        painter->setOpacity(0.5);
        int adj = _settings.highlightRectPadding;
        painter->drawRoundRect(QRect(QPoint(0, 0), _size*_settings.gridSize).adjusted(-adj, -adj, adj, adj), _settings.gridSize/2, _settings.gridSize/2);
    }

    painter->setOpacity(1.0);

    // Body pen
    QPen bodyPen;
    bodyPen.setWidthF(PEN_WIDTH);
    bodyPen.setStyle(Qt::SolidLine);
    bodyPen.setColor(COLOR_BODY_BORDER);

    // Body brush
    QBrush bodyBrush;
    bodyBrush.setStyle(Qt::SolidPattern);
    bodyBrush.setColor(COLOR_BODY_FILL);

    // Draw the component body
    painter->setPen(bodyPen);
    painter->setBrush(bodyBrush);
    painter->drawRoundedRect(QRect(QPoint(0, 0), _size*_settings.gridSize), _settings.gridSize/2, _settings.gridSize/2);
}