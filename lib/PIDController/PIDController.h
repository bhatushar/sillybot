#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H

#define kP 50
#define kI 0.5
#define kD 2

int P, I, D;

int PID(int err, int lastErr) {
  P = kP * err;
  I += err;
  D = kD * (err - lastErr);
  return abs(P + (kI * I) + D);
}

#undef kP
#undef kI
#undef kD

#endif
