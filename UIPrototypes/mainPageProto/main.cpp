#include "taskTodoListUI.h"
#include "scheduleUI.h"
#include "notesUI.h"
#include "pacswBaseDialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    taskTodoListDialog t;
    t.show();
    PACSWBaseDialog p;
    return a.exec();
}
