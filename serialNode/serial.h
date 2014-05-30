#ifndef SERIAL_H
#define SERIAL_H

#ifdef __cplusplus
extern "C" {
#endif

  #define SERIAL_SUCCESS 0
  #define SERIAL_FAIL -1
    
  int serialOpen(const char* path, int speed);
  void serialClose();
  void serialWrite(const char* out, int length);
  void serialWriteByte(char out);
  char serialRead();
  int serialStatus();
  int serialAvailable();
  void serialWaitForData();
  int serialGetSpeed();

#ifdef __cplusplus
}
#endif

#endif
