#include <stdint.h>

#define SEN_SPI_FREQ          1000000 /* 1MHz freq */
#define SEN_SPI_NUM           0       /* SPI  port number used (by default 0) */
#define SEN_SPI_BYTE_PER_WORD 8       /* 8    Bits per    word */

int8_t spi_init();

int8_t spi_read(uint8_t addr, uint8_t *byte);

int8_t spi_write(uint8_t addr, uint8_t byte);

