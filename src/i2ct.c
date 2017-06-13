#include <stdio.h>
#include <string.h>

#include "si7021.h"

int main()
{
    float f, c;
    int status;

    status = si7021_read_temperature(&c);
    if (status) {
      printf("Error %d reading temperature\n", status);
      return -1;
    }
    // convert to farenheight
    f = 9.0 * c / 5.0 + 32.0;
    printf("Si7021 returns %fc = %ff\n", c, f);
}
