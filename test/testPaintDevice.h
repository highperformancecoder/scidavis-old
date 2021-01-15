#ifndef TESTPAINTDEVICE_H
#define TESTPAINTDEVICE_H
#include <QPaintDevice>
#include <QPaintEngine>
#include <iostream>

/** a QPaintDevice plugin that records the actions to a stream for
    comparison purposes
*/

struct TestPaintDevice : public QPaintDevice
{
    struct PaintEngine : public QPaintEngine
    {
        std::ostream &out;
        PaintEngine(std::ostream &out) : QPaintEngine(QPaintEngine::AllFeatures), out(out) { }
        bool begin(QPaintDevice *) override { return true; }
        void drawEllipse(const QRectF &) override;
        void drawEllipse(const QRect &) override;
        void drawImage(const QRectF &, const QImage &, const QRectF &,
                       Qt::ImageConversionFlags) override;
        //    void drawLines(const QLine*, int) override;
        void drawPath(const QPainterPath &) override;
        void drawPixmap(const QRectF &, const QPixmap &, const QRectF &) override;
        void drawPoints(const QPointF *, int) override;
        // void drawPoints(const QPoint*, int) override;
        void drawPolygon(const QPointF *, int, PolygonDrawMode) override;
        void drawPolygon(const QPoint *, int, PolygonDrawMode) override;
        void drawRects(const QRectF *, int) override;
        //    void drawRects(const QRect*, int) override;
        void drawTextItem(const QPointF &, const QTextItem &) override;
        void drawTiledPixmap(const QRectF &, const QPixmap &, const QPointF &) override;
        bool end() override { return true; }
        Type type() const override { return QPaintEngine::User; }
        void updateState(const QPaintEngineState &) { }
    };

    mutable PaintEngine pe;
    TestPaintDevice(std::ostream &out) : pe(out) { }
    QPaintEngine *paintEngine() const override { return &pe; }
    int metric(PaintDeviceMetric) { return 256; }
};

#endif
