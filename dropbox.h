#ifndef DROPBOX_H
#define DROPBOX_H

#include <QTextEdit>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QMimeData>

class dropBox : public QTextEdit
{
    Q_OBJECT

public:
    dropBox()
    {
        setAcceptDrops(true);
    }

    void dragEnterEvent(QDragEnterEvent *event)
    {
        if ( event->mimeData()->hasUrls())
            event->acceptProposedAction();
        else
            dragEnterEvent(event);
    }

    void dragMoveEvent(QDragMoveEvent *event)
    {
        if ( event->mimeData()->hasUrls())
            event->acceptProposedAction();
        else
            dragMoveEvent(event);
    }

public slots:
    QStringList getFileNameList();

private slots:
    void condenseFiles(QStringList ,char RorT);
    void condenseReflectanceFiles();
    void condenseTransmittanceFiles();


private:
    QStringList fileNameList;
    QStringList headerList;

};

#endif // DROPBOX_H
