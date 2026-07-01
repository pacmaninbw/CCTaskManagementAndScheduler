#ifndef BASEOBJECTEDITORDIALOG_H_
#define BASEOBJECTEDITORDIALOG_H_

/*
 * Provide a common set of widgets and actions for all editors of model items.
 *
 * Data fields specific to a model item will be defined in the sub class
 * editor for that model object.
 */
class ModelDBInterface;

// Project Header Files

// QT Header Files
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
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
/*
 * The initEditorFieldsFromDataBase() method is called after the editor has been
 * instantiated. This method retrieve the model object being edited from the
 * database and initializes the field values in the editor. Each model editor
 * will include the proper database model object header file. This function is
 * pure because this base class has no way of knowing what the object is or
 * what fields are necessary.
 *
 * The database retrieval is performed in this function to keep any database
 * access out of the constructors for the editor.
 */
    virtual void initEditorFieldsFromDataBase() = 0;

public Q_SLOTS:
    void accept() override;

protected Q_SLOTS:
    void handleDeleteButton_Clicked();

protected:
    virtual QGroupBox* setUpEditorDialogForm() { return nullptr; };
/*
 * The createSharedPtrDBModelForAddObject(), transferEditorValuesToDBModel() and
 * transferDBModelDataToEditorFields() functions all affect specific model objects
 * and data fields, there is no way to provide definitions for them in the super
 * class. Each editor sub class will include different model item header files.
 */
    virtual void createSharedPtrDBModelForAddObject() = 0;
    virtual void transferEditorValuesToDBModel() = 0;
    virtual void transferDBModelDataToEditorFields() = 0;
    
    virtual void setUpEditorUI();
    QDialogButtonBox* setUpEditorButtonBox();
    virtual void limitDialogGrowth();
    QPushButton* createDeleteButton(QWidget* buttonBox);
    int getFormLayoutMaxWidth(QFormLayout* formToSize);
    int getRowMaximumWidth(QFormLayout* layout, int row);

    std::size_t m_UserID;
    std::size_t m_DBModelID;
    std::shared_ptr<ModelDBInterface> m_DBObjectModel = nullptr;
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

#endif // BASEOBJECTEDITORDIALOG_H_
