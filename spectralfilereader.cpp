#include "spectralfilereader.h"
#include "dropbox.h"

zwr::SpectralFileReader::SpectralFileReader()
{
    fileType = "Spectral";
    addUploadButton();
    addInterpOutputButton();
    addMathematicaOutputButton();
    addMathematicaPlotButton();
    addSmoothOutputButton();
    addTFCalcOutputButton();
    addHideButton();

    QAction* action1 = new QAction(tr("Format Reflectance"), this);
    QAction* action2 = new QAction(tr("Format Transmittance"), this);
    particularOptions->addAction(action1);
    particularOptions->addAction(action2);
    connect(action1,SIGNAL(triggered()),textForm,SLOT(condenseReflectanceFiles()));
    connect(action2,SIGNAL(triggered()),textForm,SLOT(condenseTransmittanceFiles()));
}

void zwr::SpectralFileReader::setFilePath()
{
    woFile.setFileName( woFileName + "-Spectral.txt");
}

void zwr::SpectralFileReader::writeTextToFile()
{
    woFile.open(QFile::WriteOnly | QFile::Truncate | QFile::Text);
    woFile.resize(0);
    QTextStream fileEraser(&woFile);
    fileEraser << textForm->toPlainText();
    woFile.close();
    readData(woFileName,WODB);
}

void zwr::SpectralFileReader::trimTop(QString& input, QTextStream& streamer)
{
    input = streamer.readLine();
    depParams = dependentParametersUsed(input);
    if (depParams.size() != 0)
        depParams.pop_front();
    return;
}

void zwr::SpectralFileReader::formatXAxis()
{
    graphForm->xAxis->setLabel( "Wavelength(nm)");
    graphForm->yAxis->setLabel("Reflectance (%)");
}
