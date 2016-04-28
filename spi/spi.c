#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "spi.h"

#include <stdio.h>

static int spi_fd;

int8_t spi_init(){
    // Assuming spi_bcm2708 module has already been loaded
    if ( SEN_SPI_NUM == 0 ) {
        spi_fd = open("/dev/spidev0.0", O_RDWR);
    } else {
        spi_fd = open("/dev/spidev0.1", O_RDWR);
    }
    if ( spi_fd < 0 ) {
        return -1;
    }

    //SPI_MODE_0 (0,0)     CPOL=0 (Clock Idle low level), CPHA=0 (SDO transmit/change edge active to idle)
    int spiWRMode = SPI_MODE_0;
    int spiRDMode = SPI_MODE_0;
    int spiBitsPerWord = SEN_SPI_BYTE_PER_WORD;
    int spiSpeed = SEN_SPI_FREQ;


    if (   ( ioctl(spi_fd, SPI_IOC_WR_MODE, &spiWRMode) < 0 )
        || ( ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &spiBitsPerWord) < 0 )
        || ( ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spiSpeed)  < 0 )
        || ( ioctl(spi_fd, SPI_IOC_RD_BITS_PER_WORD, &spiBitsPerWord) < 0 )
        || ( ioctl(spi_fd, SPI_IOC_RD_MODE, &spiRDMode) < 0 )
        || ( ioctl(spi_fd, SPI_IOC_RD_MAX_SPEED_HZ, &spiSpeed)  < 0 )
        ) {
        return -1;
    }

    return 0;
}

int8_t spi_read(uint8_t addr, uint8_t *byte){
    uint8_t buff[2];
    buff[0] = addr;
    buff[1] = 0x0;

    struct spi_ioc_transfer spi[1];
    spi[0].tx_buf        = (unsigned long)(buff) ;  // transmit from "data"
    spi[0].rx_buf        = (unsigned long)(buff) ;  // receive into "data"
    spi[0].len           = 2;
    spi[0].speed_hz      = SEN_SPI_FREQ;
    spi[0].bits_per_word = SEN_SPI_BYTE_PER_WORD;
    spi[0].cs_change     = 0;                       // this keep CS active between the different transfers
    spi[0].delay_usecs   = 0;                       // delay between two transfer

    if ( ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi) < 0 ) {
        return -1;
    }

    printf("va %d",buff[1]);
    *byte=buff[1];
    return 0;
}


