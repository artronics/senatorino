#include <stdio.h>
#include <stdint.h>

#include "spi.h"

int main(){
    int8_t er=0;

    uint8_t addr =0x12;
    addr = (0x0 << 7) | (addr << 1 ) | 0x0;
    uint8_t byte;

    er=spi_init();
    if (er<0) {
        printf("kir");
    }

    er=spi_read(addr,&byte);
    if (er<0) {
        printf("kos");
    }

    //printf("value %d",*byte);

    return 0;
}
