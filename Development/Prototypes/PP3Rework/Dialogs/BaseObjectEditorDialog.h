#ifndef BASEOBJECTEDITORDIALOG_H
#define BASEOBJECTEDITORDIALOG_H

class ModelDBInterface;

// Project Header Files

// QT Header Files
#include <QVariant>
#include <QAbstractButton>
#include <QApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

// Standard C++ Header Files
#include <memory>

class BaseObjectEditorDialog : public QDialog
{
    Q_OBJECT

public:
    BaseObjectEditorDialog(const char* objectType, std::size_t userId = 0, std::size_t dbModelTableIndex = 0, QWidget* parent = nullptr);
    virtual void initEditorFieldsFromDataBase() = 0;

public Q_SLOTS:
    void accept() override;

protected Q_SLOTS:
    void handleDeleteButton_Clicked();

protected:
    virtual QGroupBox* setUpEditorDialogForm() { return nullptr; };
    virtual void createSharedPtrDBModelForAddObject() = 0;
    virtual void transferEditorValuesToDBModel() = 0;
    virtual void transferDBModelDataToEditorFields() = 0;
    
    virtual void setUpEditorUI();
    QDialogButtonBox* setUpEditorButtonBox();
    virtual void limitDialogGrowth();
    QPushButton* createDeleteButton(QWidget* buttonBox);
    int getFormLayoutMaxWidth(QFormLayout* formToSize);
    int getRowMaximumWidth(QFormLayout* layout, int row);

    std::size_t m_userID;
    std::size_t m_DBModelID;
    std::shared_ptr<ModelDBInterface> m_DBObjectMode = nullptr;
    QString m_EditorObjectTypeString;
    QString m_EditorTitleString;
    int maxGroupBoxHeight = 0;
    int maxButtonBoxHeight = 0;

    QVBoxLayout* m_Qt_EditorLayout = nullptr;
    QDialogButtonBox* m_Qt_ButtonBox = nullptr;
    QPushButton* m_Qt_DeleteButton = nullptr;
    QGroupBox* m_Qt_EditorDialogFormGB = nullptr;
    QFormLayout* m_Qt_EditorFormLayout = nullptr;

    const int marginAndSpacing = 30;
    const int maxWidthUndefined = 16777215;
    const int maxHeightUndefind = 16777215;
};

#endif // BASEOBJECTEDITORDIALOG_H
