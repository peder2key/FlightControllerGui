#ifndef PLOTSETUP_H
#define PLOTSETUP_H

#include <QObject>
#include <QDebug>

#include "qcustomplot.h"

class PlotSetup : public QObject
{
    Q_OBJECT
public:
    explicit PlotSetup(QObject *parent = 0);
    ~PlotSetup();
    void Setup(QCustomPlot* plot);
    void SetYAxis(int min, int max);
    int GetGraphCount(void);
    void BeginPlot(void);
    void SetGraphTextList(QStringList graphTextList);
    void CreateGraph(QComboBox* textBox, QComboBox* colorBox);
    void clearGraph(void);

signals:

public slots:
    void UpdateMinYAxis(int min);
    void UpdateMaxYAxis(int max);
    void UpdatePlot(QMap<QString, double> dataMap);

private slots:
    //void reScaleYAxis(void);
    void setRangeY(QCPRange);

private:
    void SetBoxText(void);
    int currentGraphs;
    int minAxis, maxAxis;
    QCustomPlot *plot;
    struct PlotData {QComboBox* textBox; QComboBox* colorBox;};
    QList <PlotData> plotDataList;
    QStringList graphTextList;
    QStringList graphColorList;
};

#endif // PLOTSETUP_H
