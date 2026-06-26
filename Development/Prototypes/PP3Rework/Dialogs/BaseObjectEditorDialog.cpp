
// Project Header Files
#include "BaseObjectEditorDialog.h"
#include "ModelDBInterface.h"

// QT Header Files
#include <QVariant>
#include <QAbstractButton>
#include <QApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QVBoxLayout>

// Standard C++ Header Files
#include <stdexcept>


BaseObjectEditorDialog::BaseObjectEditorDialog(const char* objectType, std::size_t userId, std::size_t dbModelTableIndex, QWidget* parent)
    : QDialog(parent),
    m_userID{userId},
    m_DBModelID{dbModelTableIndex},
    m_EditorObjectTypeString{objectType}
{
    m_EditorTitleString = dbModelTableIndex? "Edit " : "Add ";
    m_EditorTitleString += objectType;
    setWindowTitle(m_EditorTitleString + " Dialog");
}

void BaseObjectEditorDialog::initEditorFieldsFromDataBase()
{
    throw std::logic_error("initEditorFieldsFromDataBase not implemented!");
}

void BaseObjectEditorDialog::accept()
{
    if (!m_DBObjectMode)
    {
        createSharedPtrDBModelForAddObject();
    }

    transferEditorValuesToDBModel();

    if (m_DBObjectMode->save())
    {
        QDialog::accept();
    }
    else
    {
        QString errorReport = m_EditorObjectTypeString + " edit failed.\n";
        errorReport += QString::fromStdString(m_DBObjectMode->getAllErrorMessages());
        QMessageBox::critical(nullptr, "Critical Error", errorReport, QMessageBox::Ok);
    }
}

void BaseObjectEditorDialog::handleDeleteButton_Clicked()
{
    if (m_DBObjectMode)
    {
        m_DBObjectMode->hide(m_userID);
    }

    done(QDialog::Accepted);
}

void BaseObjectEditorDialog::setUpEditorUI()
{
    m_Qt_EditorLayout = new QVBoxLayout(this);
    m_Qt_EditorLayout->setObjectName("m_Qt_EditorLayout");

    m_Qt_EditorDialogFormGB = setUpEditorDialogForm();
    m_Qt_EditorDialogFormGB->setObjectName("m_Qt_EditorDialogFormGB");

    m_Qt_EditorLayout->addWidget(m_Qt_EditorDialogFormGB);

    m_Qt_ButtonBox = setUpEditorButtonBox();
    m_Qt_ButtonBox->setObjectName("m_Qt_ButtonBox");
    m_Qt_EditorLayout->addWidget(m_Qt_ButtonBox);

    setLayout(m_Qt_EditorLayout);

    limitDialogGrowth();

    adjustSize();
}

QDialogButtonBox* BaseObjectEditorDialog::setUpEditorButtonBox()
{
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName("buttonBox");
    buttonBox->setOrientation(Qt::Horizontal);

    QPushButton *saveButton = buttonBox->button(QDialogButtonBox::Ok);
    if (saveButton) {
        saveButton->setText(tr("Save"));
    }

/*
 * If this is an edit rather than add action then allow the user to delete the object.
 */
    if (m_DBModelID)
    {
        m_Qt_DeleteButton = createDeleteButton(buttonBox);

        buttonBox->addButton(m_Qt_DeleteButton, QDialogButtonBox::DestructiveRole);
        connect(m_Qt_DeleteButton, &QPushButton::clicked, this, &BaseObjectEditorDialog::handleDeleteButton_Clicked);
    }

    maxButtonBoxHeight = buttonBox->height() + marginAndSpacing;

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    return buttonBox;
}

void BaseObjectEditorDialog::limitDialogGrowth()
{
    if (!m_Qt_EditorDialogFormGB || !m_Qt_ButtonBox)
    {
        return;
    }

    m_Qt_EditorDialogFormGB->setMaximumHeight(maxGroupBoxHeight);

    m_Qt_ButtonBox->setMaximumHeight(maxButtonBoxHeight);

    int maxDialogWidth = getFormLayoutMaxWidth(m_Qt_EditorFormLayout) + marginAndSpacing;

    setMaximumWidth(maxDialogWidth);
    setMaximumHeight(maxGroupBoxHeight + maxButtonBoxHeight + marginAndSpacing);
}

QPushButton* BaseObjectEditorDialog::createDeleteButton(QWidget *buttonBox)
{
    QPushButton* deleteButton = new QPushButton(buttonBox);
    QString buttonText = "Delete " + m_EditorObjectTypeString;
    deleteButton->setText(buttonText);
    deleteButton->setAutoFillBackground(true);
    QPalette palette = deleteButton->palette();
    palette.setColor(QPalette::ButtonText, Qt::black);
    palette.setColor(QPalette::Button, Qt::red);
    deleteButton->setPalette(palette);

    return deleteButton;
}

int BaseObjectEditorDialog::getFormLayoutMaxWidth(QFormLayout *formToSize)
{
    int maxWidth = 0;

    for (int i = 0; i < formToSize->rowCount(); ++i)
    {
        int tempMaxWidth = getRowMaximumWidth(formToSize, i);
        if (tempMaxWidth > maxWidth)
        {
            maxWidth = tempMaxWidth;
        }
    }

    return maxWidth;
}

int BaseObjectEditorDialog::getRowMaximumWidth(QFormLayout *layout, int row)
{
    int maxLabelWidth = 0;
    int maxFieldWidth = 0;

    QLayoutItem* labelItem = layout->itemAt(row, QFormLayout::LabelRole);
    QLayoutItem* fieldItem = layout->itemAt(row, QFormLayout::FieldRole);

    if (labelItem) {
        maxLabelWidth = labelItem->maximumSize().width();
        if (maxLabelWidth >= maxWidthUndefined) {
            maxLabelWidth = labelItem->sizeHint().width();
        }
    }

    if (fieldItem) {
        maxFieldWidth = fieldItem->maximumSize().width();
        if (maxFieldWidth >= maxWidthUndefined) {
            maxFieldWidth = fieldItem->sizeHint().width();
        }
    }

    int horizontalSpacing = layout->horizontalSpacing();
    if (horizontalSpacing < 0) {
        horizontalSpacing = 15;
    }

    return maxLabelWidth + maxFieldWidth + horizontalSpacing;
}

