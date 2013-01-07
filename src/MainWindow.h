/*
 ******************************************************************************
 * Copyright (C) Haoyun Shen, Junaid Ali-Shah 2011
 *
 * This file is part of GIPSI - GPAC Interactive Peer-to-peer Streaming plug-In.
 *
 *    GIPSI is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    GIPSI is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with GIPSI.  If not, see <http://www.gnu.org/licenses/>
 ******************************************************************************
 */


/*
    \file MainWindow.h

    \ This header file describes the class which start the GUI of Gipsi and
    is called at the starting of both source and client side.
*/



#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;

private slots:

    /* initialization on_OpenAsSrc_clicked
    * This function is used if the Source provide the path of media file and click the button Open As source
    * OR in Command Line the argument provided is "src"
    * @param
    * @return void
    */
    void on_OpenAsSrc_clicked();

    /* initialization on_ConnectAsReceiver_clicked
    * This function is used if the Peer provide the source address and click the button Open As peer
    * OR in Command Line the argument provided is "peer"
    * @param
    * @return void
    */
    void on_ConnectAsReceiver_clicked();


    /* initialization on_browse_button_clicked
    * This function is used to browse the media file on disk
    * @param
    * @return void
    */
    void on_browse_button_clicked();
};

#endif // MAINWINDOW_H
