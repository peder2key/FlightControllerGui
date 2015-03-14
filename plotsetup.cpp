#include "plotsetup.h"

PlotSetup::PlotSetup(QObject *parent) : QObject(parent)
{
    plot = new QCustomPlot;

    currentGraphs = 0;

    graphColorList << "black" << "red" << "darkRed" << "green" << "darkGreen"
                      << "blue" << "darkBlue" << "cyan" << "darkCyan" << "magenta"
                         << "darkMagenta" << "yellow" << "darkYellow" << "darkGray";

    connect(plot->yAxis, SIGNAL(rangeChanged(QCPRange)),plot->yAxis, SLOT(setRange(QCPRange)));

}

PlotSetup::~PlotSetup()
{

}

void PlotSetup::Setup(QCustomPlot *plot)
{
    this->plot = plot;

}

void PlotSetup::SetGraphTextList(QStringList graphTextList)
{
    this->graphTextList = graphTextList;
    this->graphTextList.sort();
}

void PlotSetup::CreateGraph(QComboBox *textBox, QComboBox *colorBox)
{
    PlotData tempContainer;
    tempContainer.textBox = textBox;
    tempContainer.colorBox = colorBox;
    plotDataList.append(tempContainer);
    plot->addGraph();
    currentGraphs++;
    SetBoxText();
}

void PlotSetup::clearGraph()
{
    for(short i = 0; i<currentGraphs; i++)
    {



            plot->graph(i)->clearData();

    }
}



void PlotSetup::SetBoxText()
{
    for(short i = 0; i<currentGraphs;i++)
    {
        plotDataList[i].textBox->clear();
        plotDataList[i].textBox->addItems(graphTextList);
        plotDataList[i].textBox->setCurrentIndex(0);
        plotDataList[i].colorBox->clear();
        plotDataList[i].colorBox->addItems(graphColorList);
        plotDataList[i].colorBox->setCurrentIndex(i);
    }

}

int PlotSetup::GetGraphCount()
{
    return this->currentGraphs;
}

void PlotSetup::SetYAxis(int min, int max)
{
    plot->yAxis->setRangeLower(min);
    plot->yAxis->setRangeUpper(max);
}

void PlotSetup::UpdateMinYAxis(int min)
{
    plot->yAxis->setRangeLower(min);
}

void PlotSetup::UpdateMaxYAxis(int max)
{
    plot->yAxis->setRangeUpper(max);
}

void PlotSetup::BeginPlot()
{
    plot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    plot->xAxis->setDateTimeFormat("hh:mm:ss");
    plot->xAxis->setAutoTickStep(false);
    plot->xAxis->setTickStep(2);

    for(short i = 0; i<currentGraphs; i++)
    {
        QColor color;
        color.setNamedColor(plotDataList[i].colorBox->currentText());
        plot->graph(i)->setPen(color);
    }
}


void PlotSetup::UpdatePlot(QMap<QString, double> dataMap)
{
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    static double lastPointKey = 0;
    if (key-lastPointKey > 0.01) // at most add point every 10 ms
    {
        for(short i = 0; i<currentGraphs; i++)
        {
                double data = dataMap.value(plotDataList[i].textBox->currentText());
                plot->graph(i)->addData(key,data);
                plot->graph(i)->removeDataBefore(key-8);

                plot->graph(i)->rescaleValueAxis(true);

        }
      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    plot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);

    plot->replot();
}

void PlotSetup::setRangeY(QCPRange)
{
    qDebug() << "Data change";
}





