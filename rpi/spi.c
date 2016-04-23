#include <fcntl.h>               
#include <sys/ioctl.h>            
#include <linux/spi/spidev.h>    

#define ZIGBEE_SPI_NUM    0            // SPI port number used (by default 0)
#define ZIGBEE_SPI_FREQ   1000000      // example with 1MHz CLK line
#define ZIGBEE_SPI_BYTE_PER_WORD 8     // 8 Bits per word

int demoSPI();

demoSPI() {
    int mySPI;  // spidev file descriptor
    // Assuming spi_bcm2708 module has already been loaded
    if ( ZIGBEE_SPI_NUM == 0 ) {
        mySPI = open("/dev/spidev0.0", O_RDWR);
    } else {
        mySPI = open("/dev/spidev0.1", O_RDWR);
    }
    if ( mySPI < 0 ) {
        return -1;
    }

    //SPI_MODE_0 (0,0)     CPOL=0 (Clock Idle low level), CPHA=0 (SDO transmit/change edge active to idle)
    int spiWRMode = SPI_MODE_0;
    int spiRDMode = SPI_MODE_0;
    int spiBitsPerWord = ZIGBEE_SPI_BYTE_PER_WORD;
    int spiSpeed = ZIGBEE_SPI_FREQ;


    if (   ( ioctl(mySPI, SPI_IOC_WR_MODE, &spiWRMode) < 0 )
        || ( ioctl(mySPI, SPI_IOC_WR_BITS_PER_WORD, &spiBitsPerWord) < 0 )
        || ( ioctl(mySPI, SPI_IOC_WR_MAX_SPEED_HZ, &spiSpeed)  < 0 )
        || ( ioctl(mySPI, SPI_IOC_RD_BITS_PER_WORD, &spiBitsPerWord) < 0 )
        || ( ioctl(mySPI, SPI_IOC_RD_MODE, &spiRDMode) < 0 )
        || ( ioctl(mySPI, SPI_IOC_RD_MAX_SPEED_HZ, &spiSpeed)  < 0 )
        ) {
        return -1;
    }

    // Read data @ address 0x12
    unsigned char buff[2];
    buff[0] = (0x0 << 7) | (0x12 << 1 ) | 0x0;
    buff[1] = 0x0;

    struct spi_ioc_transfer spi[1];
    spi[0].tx_buf        = (unsigned long)(buff) ;  // transmit from "data"
    spi[0].rx_buf        = (unsigned long)(buff) ;  // receive into "data"
    spi[0].len           = 2;
    spi[0].speed_hz      = ZIGBEE_SPI_FREQ;
    spi[0].bits_per_word = ZIGBEE_SPI_BYTE_PER_WORD;
    spi[0].cs_change     = 0;                       // this keep CS active between the different transfers
    spi[0].delay_usecs   = 0;                       // delay between two transfer

    if ( ioctl(mySPI, SPI_IOC_MESSAGE(1), &spi) < 0 ) {
        return -1;
    }
    printf(" # Read value %d \n",buff[1]);
    return 0;
}
