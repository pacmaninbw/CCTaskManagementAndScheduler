#ifndef GUINOTEMODEL_H
#define GUINOTEMODEL_H

// Project Header Files

// QT Header Files
#include <QObject>
#include <QString>

// Standard C++ Header Files

class GuiNoteModel : public QObject
{
    Q_OBJECT
public:
    explicit GuiNoteModel(QObject *parent = nullptr);

signals:

};

#endif // GUINOTEMODEL_H
