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

#include <cstdio>
#include <cstdlib>

#include "sort.h"

void initData(int *data, const int N)
{
  if( N != 11 ) {
    return;
  }

  data[ 0] = 'S';
  data[ 1] = 'O';
  data[ 2] = 'R';
  data[ 3] = 'T';
  data[ 4] = 'E';
  data[ 5] = 'X';
  data[ 6] = 'A';
  data[ 7] = 'M';
  data[ 8] = 'P';
  data[ 9] = 'L';
  data[10] = 'E';
}

void print(const int *data, const int N)
{
  for(int i = 0; i < N; i++) {
    printf("%c", data[i]);
  }
  printf("\n");
}

int main(int argc, char **argv)
{
  const int N = 11;
  int data[N];

  printf("Heapsort:\n");
  initData(data, N);
  print(data, N);
  heapsort(data, N);
  print(data, N);
  printf("\n");

  printf("Insertionsort:\n");
  initData(data, N);
  print(data, N);
  insertionsort(data, N);
  print(data, N);
  printf("\n");

  return EXIT_SUCCESS;
}
