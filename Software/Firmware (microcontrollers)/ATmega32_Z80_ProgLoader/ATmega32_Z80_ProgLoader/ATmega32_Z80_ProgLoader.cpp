#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000
#include <util/delay.h>
#include "lib/spi/SPI_M45PE16.h"
#include "lib/bit_handler/bit_handler.h"

SPI_M45PE16 spi;
uint8_t page_buffer[PAGE_SIZE];

#define N_RESET 0
#define N_WR 1
#define N_RD 2
#define CONTROL_P PORTB
#define CONTROL_PD DDRB

#define ADDRESS_LOW PORTC
#define ADDRESS_HIGH PORTD
#define DATA_BUS PORTA

#define RESET_HIGH() bit_set(CONTROL_P,N_RESET);
#define RESET_LOW() bit_clear(CONTROL_P,N_RESET);
#define WR_HIGH() bit_set(CONTROL_P,N_WR);
#define WR_LOW() bit_clear(CONTROL_P,N_WR);
#define RD_HIGH() bit_set(CONTROL_P,N_RD);
#define RD_LOW() bit_clear(CONTROL_P,N_RD);

#define SRAM_SIZE pow(2,16)-1;

void init(){
	bit_set(CONTROL_PD,N_RESET); // Set reset as output
	bit_set(CONTROL_PD,N_WR); // Set write as output
	bit_set(CONTROL_PD,N_RD); // Set read as output
	
	// Set reset low:
	RESET_LOW()
	// Set write high:
	WR_HIGH()
	// Set read high:
	RD_HIGH()
}

void write_SRAM(uint16_t address,uint8_t data){
	// TODO: Write 'data' into address 'address':
	
	// Put address data out:
	ADDRESS_LOW = address & 0x00FF;
	ADDRESS_HIGH = (address & 0xFF00)>>8;
	
	DATA_BUS = data;
	
	WR_LOW();
	//_delay_ms(1);
	WR_HIGH();
}

int main(void) {
	init(); // Initialize inputs
	
	// Set data and address buses as output:
	DDRA = DDRC = DDRD = 0xFF;

	uint16_t sram_current_address = 0;
	
	for(int sector=0;sector<NUM_OF_SECTORS;sector++){
		// Read current sector:
		spi.spi_page_read(sector,page_buffer,PAGE_SIZE); 
		// And write into into SRAM:
		
		for(uint16_t i=0;i<PAGE_SIZE;i++)
			write_SRAM(i+sram_current_address,page_buffer[i]);
		
		sram_current_address+=PAGE_SIZE;
	}
	
	// SRAM is full with the program. Allow Z80 to execute it:
	PORTA = PORTC = PORTD = 0x0;
	DDRA  = DDRC  = DDRD  = 0x0;
	
	WR_LOW()
	RD_LOW()
	RESET_HIGH() // Start Z80 CPU!
	
	while(1){
		//TODO: Do event handling here
	}
}