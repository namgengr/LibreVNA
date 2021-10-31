#ifndef TRACEXYPLOT_H
#define TRACEXYPLOT_H

#include "traceplot.h"

#include <set>

class TraceXYPlot : public TracePlot
{
    friend class XYplotAxisDialog;
    Q_OBJECT
public:
    TraceXYPlot(TraceModel &model, QWidget *parent = nullptr);

    enum class YAxisType {
        Disabled,
        // S parameter options
        Magnitude,
        Phase,
        VSWR,
        Real,
        Imaginary,
        // derived parameter options
        SeriesR,
        Reactance,
        Capacitance,
        Inductance,
        QualityFactor,
        // TDR options
        ImpulseReal,
        ImpulseMag,
        Step,
        Impedance,
        Last,
    };
    static const std::set<YAxisType> YAxisTypes;
    enum class XAxisType {
        Frequency,
        Time,
        Distance,
        Power,
        Last,
    };
    enum class XAxisMode {
        UseSpan,
        FitTraces,
        Manual,
        Last,
    };

    void setYAxis(int axis, YAxisType type, bool log, bool autorange, double min, double max, double div);
    void setXAxis(XAxisType type, XAxisMode mode, double min, double max, double div);
    void enableTrace(Trace *t, bool enabled) override;
    void updateSpan(double min, double max) override;
    void replot() override;

    virtual Type getType() override { return Type::XYPlot;};
    virtual nlohmann::json toJSON() override;
    virtual void fromJSON(nlohmann::json j) override;

    bool isTDRtype(YAxisType type);

public slots:
    void axisSetupDialog();

protected:
    virtual bool configureForTrace(Trace *t) override;
    virtual void updateContextMenu() override;
    virtual bool dropSupported(Trace *t) override;
    virtual void draw(QPainter &p) override;

private slots:
    void updateAxisTicks();
private:
    static constexpr int AxisLabelSize = 10;
    static QString AxisTypeToName(YAxisType type);
    static QString AxisTypeToName(XAxisType type);
    static QString AxisModeToName(XAxisMode mode);
    XAxisType XAxisTypeFromName(QString name);
    YAxisType YAxisTypeFromName(QString name);
    XAxisMode AxisModeFromName(QString name);
    void enableTraceAxis(Trace *t, int axis, bool enabled);
    bool supported(Trace *t) override;
    bool supported(Trace *t, YAxisType type);
    QPointF traceToCoordinate(Trace *t, unsigned int sample, YAxisType type);
    QPoint plotValueToPixel(QPointF plotValue, int Yaxis);
    QPointF pixelToPlotValue(QPoint pixel, int YAxis);
    QPoint markerToPixel(Marker *m) override;
    double nearestTracePoint(Trace *t, QPoint pixel, double *distance = nullptr) override;
    virtual bool xCoordinateVisible(double x);
    void traceDropped(Trace *t, QPoint position) override;
    QString mouseText(QPoint pos) override;

    static QString AxisUnit(YAxisType type);
    static QString AxisUnit(XAxisType type);

    std::set<Trace*> tracesAxis[2];

    class YAxis {
    public:
        YAxisType type;
        bool log; // not used yet
        bool autorange;
        double rangeMin;
        double rangeMax;
        double rangeDiv;
        std::vector<double> ticks;
    };
    class XAxis {
    public:
        XAxisType type;
        XAxisMode mode;
        bool log; // not used yet
        double rangeMin;
        double rangeMax;
        double rangeDiv;
        std::vector<double> ticks;
    };

    YAxis YAxis[2];
    XAxis XAxis;

    int plotAreaLeft, plotAreaWidth, plotAreaBottom, plotAreaTop;
};

#endif // TRACEXYPLOT_H
