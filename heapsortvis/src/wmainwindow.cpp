/****************************************************************************
** Copyright (c) 2016, Carsten Schmidt. All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
**
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
**
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
**
** 3. Neither the name of the copyright holder nor the names of its
**    contributors may be used to endorse or promote products derived from
**    this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#include <QtCore/QThread>
#include <QtCore/QVector>
#include <QtWidgets/QGraphicsScene>

#include "wmainwindow.h"
#include "ui_wmainwindow.h"

#include "datainputvalidator.h"
#include "heapifyanimation.h"
#include "sortanimation.h"
#include "visutil.h"

////// public ////////////////////////////////////////////////////////////////

WMainWindow::WMainWindow(QWidget *parent, Qt::WindowFlags flags)
  : QMainWindow(parent, flags)
  , ui(new Ui::WMainWindow)
  , _heapScene()
  , _workerThread()
{
  ui->setupUi(this);

  // Input Validator /////////////////////////////////////////////////////////

  DataInputValidator *validator = new DataInputValidator(ui->inputEdit);
  ui->inputEdit->setValidator(validator);

  // Heap View ///////////////////////////////////////////////////////////////

  ui->heapView->setRenderHint(QPainter::Antialiasing, true);
  ui->heapView->setRenderHint(QPainter::TextAntialiasing, true);

  _heapScene = new QGraphicsScene(ui->heapView);
  _heapScene->setBackgroundBrush(Qt::gray);
  ui->heapView->setScene(_heapScene);

  // Worker Thread ///////////////////////////////////////////////////////////

  _workerThread = new QThread(this);

  // Signals & Slots /////////////////////////////////////////////////////////

  connect(ui->inputEdit, SIGNAL(textEdited(const QString&)),
          this, SLOT(drawHeap(const QString&)));

  connect(ui->animateHeapifyButton, &QPushButton::clicked,
          this, &WMainWindow::startAnimation);
  connect(ui->animateSortButton, &QPushButton::clicked,
          this, &WMainWindow::startAnimation);

  connect(ui->quitAction, &QAction::triggered, this, &WMainWindow::close);
}

WMainWindow::~WMainWindow()
{
  _workerThread->quit();
  _workerThread->wait();

  delete ui;
}

////// private slots /////////////////////////////////////////////////////////

void WMainWindow::drawHeap(const QString& input, const int mark1, const int mark2,
                           const int Nsort)
{
  QVector<QGraphicsItem*> items = newHeapItems(_heapScene, input, mark1, mark2, Nsort);
  placeItems(items, 0);
  connectItems(_heapScene, items, 0, Nsort);
}

void WMainWindow::finishAnimation()
{
  _workerThread->quit();
  _workerThread->wait();

  delete sender();

  ui->inputGroup->setEnabled(true);
  ui->animationGroup->setEnabled(true);
}

void WMainWindow::startAnimation()
{
  if( ui->inputEdit->text().isEmpty() ) {
    return;
  }

  QObject *animation(0);
  if(        sender() == ui->animateHeapifyButton ) {
    animation = new HeapifyAnimation(0);
  } else if( sender() == ui->animateSortButton ) {
    animation = new SortAnimation(0);
  }

  if( animation != 0 ) {
    animation->moveToThread(_workerThread);
    connect(animation, SIGNAL(finished()), this, SLOT(finishAnimation()));
    connect(animation, SIGNAL(updateHeap(const QString&,const int,const int,const int)),
            this, SLOT(drawHeap(const QString&,const int,const int,const int)));

    _workerThread->start();

    QMetaObject::invokeMethod(animation, "animate", Qt::QueuedConnection,
                              Q_ARG(QString, ui->inputEdit->text()),
                              Q_ARG(int, ui->animationDelaySpin->value()));

    ui->inputGroup->setEnabled(false);
    ui->animationGroup->setEnabled(false);
  }
}
