// Project Header Files
#include "BaseObjectEditorDialog.h"
#include "ModelDBInterface.h"

// QT Header Files
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

// Standard C++ Header Files
#include <stdexcept>


BaseObjectEditorDialog::BaseObjectEditorDialog(const char* objectType, std::size_t userId, std::size_t dbModelTableIndex, QWidget* parent)
    : QDialog(parent),
    m_userID{userId},
    m_dbModelId{dbModelTableIndex},
    m_editorObjectTypeString{objectType}
{
    m_editorTitleString = dbModelTableIndex? "Edit " : "Add ";
    m_editorTitleString += objectType;
    setWindowTitle(m_editorTitleString + " Dialog");
}

void BaseObjectEditorDialog::initEditorFieldsFromDataBase()
{
    throw std::logic_error("initEditorFieldsFromDataBase not implemented!");
}

void BaseObjectEditorDialog::accept()
{
    if (!m_dbObjectModel)
    {
        createSharedPtrDBModelForAddObject();
    }

    transferEditorValuesToDBModel();

    if (m_dbObjectModel->save())
    {
        QDialog::accept();
    }
    else
    {
        QString errorReport = m_editorObjectTypeString + " edit failed.\n";
        errorReport += QString::fromStdString(m_dbObjectModel->getAllErrorMessages());
        QMessageBox::critical(nullptr, "Critical Error", errorReport, QMessageBox::Ok);
    }
}

void BaseObjectEditorDialog::handleDeleteButton_Clicked()
{
    if (m_dbObjectModel)
    {
        m_dbObjectModel->hide(m_userID);
    }

    done(QDialog::Accepted);
}

void BaseObjectEditorDialog::setUpEditorUI()
{
    m_qt_editorLayout = new QVBoxLayout(this);
    m_qt_editorLayout->setObjectName("m_qt_editorLayout");

    m_qt_editorDialogFormGB = setUpEditorDialogForm();
    m_qt_editorDialogFormGB->setObjectName("m_qt_editorDialogFormGB");

    m_qt_editorLayout->addWidget(m_qt_editorDialogFormGB);

    m_qt_buttonBox = setUpEditorButtonBox();
    m_qt_buttonBox->setObjectName("m_qt_buttonBox");
    m_qt_editorLayout->addWidget(m_qt_buttonBox);

    setLayout(m_qt_editorLayout);

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
    if (m_dbModelId)
    {
        m_qt_deleteButton = createDeleteButton(buttonBox);

        buttonBox->addButton(m_qt_deleteButton, QDialogButtonBox::DestructiveRole);
        connect(m_qt_deleteButton, &QPushButton::clicked, this, &BaseObjectEditorDialog::handleDeleteButton_Clicked);
    }

    m_maxButtonBoxHeight = buttonBox->height() + MarginAndSpacing;

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    return buttonBox;
}

void BaseObjectEditorDialog::limitDialogGrowth()
{
    if (!m_qt_editorDialogFormGB || !m_qt_buttonBox)
    {
        return;
    }

    m_qt_editorDialogFormGB->setMaximumHeight(m_maxGroupBoxHeight);

    m_qt_buttonBox->setMaximumHeight(m_maxButtonBoxHeight);

    int maxDialogWidth = getFormLayoutMaxWidth(m_qt_editorFormLayout) + MarginAndSpacing;

    setMaximumWidth(maxDialogWidth);
    setMaximumHeight(m_maxGroupBoxHeight + m_maxButtonBoxHeight + MarginAndSpacing);
}

QPushButton* BaseObjectEditorDialog::createDeleteButton(QWidget *buttonBox)
{
    QPushButton* deleteButton = new QPushButton(buttonBox);
    QString buttonText = "Delete " + m_editorObjectTypeString;
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
        if (maxLabelWidth >= MaxWidthUndefined) {
            maxLabelWidth = labelItem->sizeHint().width();
        }
    }

    if (fieldItem) {
        maxFieldWidth = fieldItem->maximumSize().width();
        if (maxFieldWidth >= MaxWidthUndefined) {
            maxFieldWidth = fieldItem->sizeHint().width();
        }
    }

    int horizontalSpacing = layout->horizontalSpacing();
    if (horizontalSpacing < 0) {
        horizontalSpacing = 15;
    }

    return maxLabelWidth + maxFieldWidth + horizontalSpacing;
}

