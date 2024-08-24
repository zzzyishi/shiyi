#ifndef __KALMAN_H
#define __KALMAN_H

void Kalman_Init(void);
void Kalman_Update(int16_t *roll, int16_t *pitch);

#endif
