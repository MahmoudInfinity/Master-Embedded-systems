#include "stm32f103x8.h"
#include "stm32f103x8_SPI_driver.h"
#include "stm32f103x8_gpio_driver.h"

SPI_Config spi_cfg;

int main(void)
{
	// ==============================
	// 1. SPI Configuration
	// ==============================
	spi_cfg.Device_mode = SPI_DEVICE_MODE_MASTER;
	spi_cfg.Communication_mode = SPI_DIRECTION_2LINES;
	spi_cfg.Frame_format = SPI_FRAME_FORMAT_MSB_FIRST;
	spi_cfg.Data_Size = SPI_DATASIZE_8BIT;
	spi_cfg.ClckPolarity = SPI_CLK_POLARITY_LOW;
	spi_cfg.ClckPhase = SPI_CLK_PHASE_1EDGE;
	spi_cfg.SPI_BAUDRATEPRESCALER = SPI_BAUDRATEPRESCALER_8;
	spi_cfg.NSS = SPI_NSS_SOFT_SET;
	spi_cfg.IRQ_Enable = SPI_IRQ_NONE;
	spi_cfg.p_IRQ_callback = NULL;

	// ==============================
	// 2. Init SPI
	// ==============================
	MCAL_SPI_Init(SPI1, &spi_cfg);

	// ==============================
	// 3. Init GPIO Pins
	// ==============================
	MCAL_SPI_GPIO_Set_Pins(SPI1);

	// ==============================
	// 4. Test Data
	// ==============================
	uint16_t tx_data = 0xAA;   // 10101010 pattern
	uint16_t rx_data = 0;

	while (1)
	{
		// ==============================
		// 5. Send Data
		// ==============================
		MCAL_SPI_Send_Data(SPI1, &tx_data, Enable);

		// ==============================
		// 6. Receive Data (loopback)
		// ==============================
		MCAL_SPI_Receive_Data(SPI1, &rx_data, Enable);

		// Small delay (for scope clarity)
		for (volatile int i = 0; i < 50000; i++);
	}
}
