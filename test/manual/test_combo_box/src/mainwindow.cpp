#include "mainwindow.h"
#include <iostream>

MainWindow::MainWindow(int width, int height) :
    TgMainWindow(width, height, "TestApp1", width - 300, height - 300),
    m_buttonClose(this, 20, 20, 200, 50, "Close button"),
    m_buttonAddComboBoxItem(this, 20, 200, 200, 50, "Add item"),
    m_buttonRemoveComboBoxItem(this, 20, 300, 200, 50, "Remove item"),
    m_combobox(this, 20, 100, 100, 50)
{
    m_buttonClose.connectOnMouseClicked( std::bind(&MainWindow::onButtonCloseClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    m_combobox.addItemText("Text 0");
    m_combobox.addItemText("Text 2");
    m_combobox.addItemText("Text 3");
    m_combobox.addItemText("Text 4");
    m_combobox.addItemText("Text 5");
    m_combobox.addItemText("Text 1", 1);
    m_combobox.connectOnSelectedIndexChanged(std::bind(&MainWindow::onComboBoxItemChanged, this, std::placeholders::_1));
    m_buttonAddComboBoxItem.connectOnMouseClicked( std::bind(&MainWindow::onButtonAddClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    m_buttonRemoveComboBoxItem.connectOnMouseClicked( std::bind(&MainWindow::onButtonRemoveClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
}

MainWindow::~MainWindow()
{
    if (m_comboboxSecondary) {
        delete m_comboboxSecondary;
        m_comboboxSecondary = nullptr;
    }
}

/*!
 * \brief MainWindow::onButtonCloseClick
 *
 * callback when button close is clicked
 */
void MainWindow::onButtonCloseClick(TgMouseType type, float x, float y)
{
    std::cout << "Closing the application\n";
    exit();
}

/*!
 * \brief MainWindow::onComboBoxItemChanged
 *
 * callback when combobox item
 * \param index
 */
void MainWindow::onComboBoxItemChanged(size_t index)
{
    std::cout << "on Menu Item changed " << index << "\n";
}

void MainWindow::onButtonAddClick(TgMouseType type, float x, float y)
{
    std::string tmp = std::string("Added ") + std::to_string(m_currentItemIndex);
    m_combobox.addItemText(tmp.c_str());
    m_currentItemIndex++;
    if (!m_comboboxSecondary) {
        m_comboboxSecondary = new TgComboBox(this, 20, 400, 100, 50);
        m_comboboxSecondary->addItemText("First");
        m_comboboxSecondary->addItemText("Second");
        m_comboboxSecondary->addItemText("Third");
    }
}

void MainWindow::onButtonRemoveClick(TgMouseType type, float x, float y)
{
    if (m_combobox.getItemCount() > 7) {
        m_combobox.removeItemText(5);
    } else if (m_combobox.getItemCount() > 0) {
        m_combobox.removeItemText(0);
    } else {
        m_combobox.removeItemText(0);
    }
    if (m_comboboxSecondary) {
        delete m_comboboxSecondary;
        m_comboboxSecondary = nullptr;
    }
}
