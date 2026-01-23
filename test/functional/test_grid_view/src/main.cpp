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
#include "functional_test.h"
#include <X11/Xlib.h>

/*!
 * \brief main
 * \param argc
 * \param argv
 * \return
 */
int main(int argc , char *argv[])
{
    XInitThreads();
    static TgApplication m_application;
    m_application.setFont("/usr/share/fonts/truetype/samyak-fonts/Samyak-Gujarati.ttf", 1);
    m_application.setFont("/usr/share/fonts/truetype/droid/DroidSansFallbackFull.ttf", 2);
    const std::vector<uint32_t> listCharacters{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '/', ' '};
    m_application.loadCharactersToCache(nullptr, listCharacters, 25);
    static MainWindow m_mainwindow(800,600);
    getTest()->setMainWindow(&m_mainwindow);
    getTest()->start();
    m_application.exec();
    return getTest()->getReturnIndex();
}
