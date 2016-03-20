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

#include "animationbase.h"

////// public ////////////////////////////////////////////////////////////////

AnimationBase::AnimationBase(QObject *parent)
  : QObject(parent)
{
}

AnimationBase::~AnimationBase()
{
}

////// protected /////////////////////////////////////////////////////////////

void AnimationBase::exch(QString& data, const int i, const int j)
{
  const QChar tmp = data[i];
  data[i] = data[j];
  data[j] = tmp;
}

void AnimationBase::sink(QString& data, int k, const int N, const int delay)
{
  while( 2*k+1 < N ) {
    int child = 2*k+1;
    if( child+1 < N  &&  less(data, child, child+1) ) {
      child++;
    }
    if( !less(data, k, child) ) {
      if( delay > 0 ) {
        emit updateHeap(data, k, -1, N);
        QThread::msleep(delay);
      }

      break;
    } else {
      if( delay > 0 ) {
        emit updateHeap(data, k, child, N);
        QThread::msleep(delay);
      }

      exch(data, k, child);
      k = child;
    }
  }
}

////// private ///////////////////////////////////////////////////////////////

bool AnimationBase::less(const QString& data, const int i, const int j)
{
  return data[i].unicode() < data[j].unicode();
}
