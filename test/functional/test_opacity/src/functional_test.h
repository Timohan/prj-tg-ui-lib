#ifndef FUNCTIONAL_TEST_H
#define FUNCTIONAL_TEST_H

#include <stdint.h>
#include <cstddef>
#include <string>
#include "functional_test_orders.h"
class MainWindow;
class TgItem2d;

class FunctionalTest
{
public:
    FunctionalTest();
    void setMainWindow(MainWindow *mainWindow);
    void start();
    int getReturnIndex();

private:
    MainWindow *m_mainWindow;
    int m_returnIndex;
    size_t m_latestHoverIndex { 0 };
    FunctionalTestOrders m_testOrders;
};

FunctionalTest *getTest();

#endif
