#include "reportfilereader.h"

zwr::ReportFileReader::ReportFileReader()
{
    addUploadButton();
    textForm->setWordWrapMode(QTextOption::WordWrap);
}

void zwr::ReportFileReader::setFilePath()
{
    woFile.setFileName( woFileName + "-Report" + ".txt");
}

void zwr::ReportFileReader::writeTextToFile()
{
    woFile.open(QFile::WriteOnly | QFile::Truncate | QFile::Text);
    woFile.resize(0);
    QTextStream fileEraser(&woFile);
    fileEraser << textForm->toPlainText();
    woFile.close();
    readReportFile(woFileName,WODB);
}

void zwr::ReportFileReader::readReportFile(QString woName, DBStructure* wodb)
{
    WODB = wodb;
    textForm->clear();
    woFileName = woName;
    setFilePath();

    if ( woFile.open(QFile::ReadOnly | QFile::Text) )
        textForm->setText( QTextStream( &woFile ).readAll() );
    woFile.close();
}
