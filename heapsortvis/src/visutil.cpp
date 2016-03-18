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

#include <QtGui/QBrush>
#include <QtGui/QFont>
#include <QtGui/QPen>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsLineItem>
#include <QtWidgets/QGraphicsRectItem>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsSimpleTextItem>

#include "visutil.h"

namespace priv {

  inline QPen defaultPen(const qreal width = 1.0, const QColor& color = Qt::black)
  {
    return QPen(QBrush(color, Qt::SolidPattern), width, Qt::SolidLine,
                Qt::RoundCap, Qt::RoundJoin);
  }

  inline QRectF sceneBRect(QGraphicsItem *item)
  {
    return item->mapToScene(item->boundingRect()).boundingRect();
  }

  void moveItem(QGraphicsItem *item, const qreal dx, const qreal dy)
  {
    const QTransform T = item->transform();
    item->setTransform(QTransform::fromTranslate(dx + T.dx(), dy + T.dy()));
  }

  QRectF moveItems(QVector<QGraphicsItem*>& items, const int k,
                   const qreal dx, const qreal dy)
  {
    if( k >= items.size() ) {
      return QRectF();
    }

    const QRectF c1r = moveItems(items, 2*k+1, dx, dy);
    const QRectF c2r = moveItems(items, 2*k+2, dx, dy);

    moveItem(items[k], dx, dy);

    return sceneBRect(items[k]).united(c1r).united(c2r);
  }

  QGraphicsItem *newHeapItem(const int i, const QChar& c,
                             const int mark1, const int mark2,
                             const int Nsort)
  {
    const qreal hSpace(6), vSpace(6);

    QGraphicsRectItem *rect = new QGraphicsRectItem();
    rect->setPen(priv::defaultPen(2.0));
    if(        mark1 >= 0  &&  i == mark1 ) {
      rect->setBrush(QBrush(Qt::red, Qt::SolidPattern));
    } else if( mark2 >= 0  &&  i == mark2 ) {
      rect->setBrush(QBrush(Qt::cyan, Qt::SolidPattern));
    } else if( Nsort >= 0  &&  i >= Nsort ) {
      rect->setBrush(QBrush(Qt::yellow, Qt::SolidPattern));
    } else {
      rect->setBrush(QBrush(Qt::white, Qt::SolidPattern));
    }

    QGraphicsSimpleTextItem *text =
        new QGraphicsSimpleTextItem(QString("%1: %2").arg(i).arg(c), rect);
    text->setPen(QPen());
    text->setBrush(QBrush(Qt::black, Qt::SolidPattern));
    QFont font = qApp->font();
    font.setPointSize(14);
    text->setFont(font);

    rect->setRect(text->boundingRect().adjusted(-hSpace, -vSpace, hSpace, vSpace));

    return rect;
  }

};

QVector<QGraphicsItem*> newHeapItems(QGraphicsScene *scene, const QString& input,
                                     const int mark1, const int mark2,
                                     const int Nsort)
{
  scene->clear();
  if( input.isEmpty() ) {
    return QVector<QGraphicsItem*>();
  }

  QVector<QGraphicsItem*> items(input.size());
  for(int i = 0; i < input.size(); i++) {
    items[i] = priv::newHeapItem(i, input[i], mark1, mark2, Nsort);
    scene->addItem(items[i]);
  }

  return items;
}

QRectF placeItems(QVector<QGraphicsItem*>& items, const int k)
{
  const qreal hSpace(30), vSpace(30);

  if( k >= items.size() ) {
    return QRectF();
  }

  const QRectF r = priv::sceneBRect(items[k]);

  const int c1 = 2*k+1;
  QRectF c1r = placeItems(items, c1);

  const int c2 = 2*k+2;
  QRectF c2r = placeItems(items, c2);

  if(        !c1r.isNull()  &&  !c2r.isNull() ) {
    const qreal dx1 = r.center().x() - hSpace/2.0 - c1r.right();
    const qreal dy1 = r.bottom() + vSpace - c1r.top();
    c1r = priv::moveItems(items, c1, dx1, dy1);

    const qreal dx2 = r.center().x() + hSpace/2.0 - c2r.left();
    const qreal dy2 = r.bottom() + vSpace - c2r.top();
    c2r = priv::moveItems(items, c2, dx2, dy2);
  } else if( !c1r.isNull()  &&   c2r.isNull() ) {
    const qreal dx1 = r.center().x() - c1r.center().x();
    const qreal dy1 = r.bottom() + vSpace - c1r.top();
    c1r = priv::moveItems(items, c1, dx1, dy1);
  } else {
    // NOTE: Nothing to do! Item is leaf!
  }

  return r.united(c1r).united(c2r);
}

void connectItems(QGraphicsScene *scene,
                  const QVector<QGraphicsItem*>& items, const int k,
                  const int Nsort)
{
  const int N = Nsort < 0
      ? items.size()
      : Nsort;

  if( k >= N ) {
    return;
  }

  const QRectF r = priv::sceneBRect(items[k]);

  const int c1 = 2*k + 1;
  const int c2 = 2*k + 2;

  if(        c1 < N  &&  c2 < N ) {
    const QRectF c1r = priv::sceneBRect(items[c1]);
    const QRectF c2r = priv::sceneBRect(items[c2]);

    scene->addLine(r.left(), r.bottom(),
                   c1r.center().x(), c1r.top(), priv::defaultPen(2.0));

    scene->addLine(r.right(), r.bottom(),
                   c2r.center().x(), c2r.top(), priv::defaultPen(2.0));

    connectItems(scene, items, c1, Nsort);
    connectItems(scene, items, c2, Nsort);
  } else if( c1 < N ) {
    const QRectF c1r = priv::sceneBRect(items[c1]);

    scene->addLine(r.center().x(), r.bottom(),
                   c1r.center().x(), c1r.top(), priv::defaultPen(2.0));

    connectItems(scene, items, c1, Nsort);
  }
}
