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

#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "GipsiController.h"
using namespace std;

//void *myMessage(void *);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}






void MainWindow::on_OpenAsSrc_clicked()
{

    cout<<"The button *OpenAsSrc* is clicked............"<<endl;
    string filePath;
    int port;
    filePath = ui->filePath->text().toAscii().data();
    port = ui->PortNr->text().toInt();

    GipsiController gipsi;
    string output = gipsi.openAsSrc(filePath);

    ui->DebugOutput->setText(output.c_str());


}



void MainWindow::on_ConnectAsReceiver_clicked()
{
    cout<<"The button *ConnectAsReceiver* is clicked............"<<endl;
    GipsiController gipsi;
    string IPAddr;
    int port;
    if (ui->PortNr->text()==NULL || ui->IPAddress->text()== NULL )
    {
        cout<<"Please input IP Address AND Port!!!"<<endl;
    }
    else
    {
        port = ui->PortNr->text().toInt();

        IPAddr = ui->IPAddress->text().toAscii().data();

        gipsi.connect(IPAddr,port);
    }

}



void MainWindow::on_browse_button_clicked()
{
    QString path;

    path = QFileDialog::getOpenFileName(this,tr("Choose a Media File"), QString::null,tr("Media Files (*.avi *.mp4 *.dvd)"));

    ui->filePath->setText(path);
}
