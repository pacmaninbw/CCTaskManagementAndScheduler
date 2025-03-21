#include "taskTodoListUI.h"
#include "scheduleUI.h"
#include "notesUI.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    taskTodoListDialog t;
    t.show();
    return a.exec();
}
