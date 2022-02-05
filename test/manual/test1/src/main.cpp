/*!
 * \file
 * \brief file main.cpp
 *
 * Main of opengl example via glfw
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <application/tg_application.h>
#include "mainwindow.h"

/*!
 * \brief main
 * \param argc
 * \param argv
 * \return
 */
int main(int argc , char *argv[])
{
    static TgApplication m_application;
    static MainWindow m_mainwindow(800,600);
    return m_application.exec();
}
