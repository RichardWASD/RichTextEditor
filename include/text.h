#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // read(), STDIN_FILENO
#include <termios.h> // for echoing
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
// #include <sys/ioctl.h>
#define CTRL_KEY(k) ((k) & 0x1f)