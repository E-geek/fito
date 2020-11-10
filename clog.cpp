

#define BINF "%c%c%c%c%c%c%c%c"

#define B2BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 
  
#ifdef DEBUG

void clog(char *format, ...) {
  va_list args;
  char buffer[256];
  va_start(args, format);
  vsnprintf(buffer, 255, format, args);
  Serial.println(buffer);
  va_end(args);
}

#else
#define clog(...)
#define printRTCState(...)
#endif
