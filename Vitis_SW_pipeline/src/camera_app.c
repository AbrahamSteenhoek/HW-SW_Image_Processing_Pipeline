/*****************************************************************************
 * Joseph Zambreno
 * Phillip Jones
 *
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/

/*****************************************************************************
 * camera_app.c - main camera application code. The camera configures the various
 * video in and video out peripherals, and (optionally) performs some
 * image processing on data coming in from the vdma.
 *
 *
 * NOTES:
 * 02/04/14 by JAZ::Design created.
 *****************************************************************************/

#include "camera_app.h"

camera_config_t camera_config;

// Main function. Initializes the devices and configures VDMA
int main() {

	camera_config_init(&camera_config);
	fmc_imageon_enable(&camera_config);
	camera_loop(&camera_config);

	return 0;
}


// Initialize the camera configuration data structure
void camera_config_init(camera_config_t *config) {

    config->uBaseAddr_IIC_FmcIpmi =  XPAR_FMC_IPMI_ID_EEPROM_0_BASEADDR;   // Device for reading HDMI board IPMI EEPROM information
    config->uBaseAddr_IIC_FmcImageon = XPAR_FMC_IMAGEON_IIC_0_BASEADDR;    // Device for configuring the HDMI board

    // Uncomment when using VITA Camera for Video input
//    config->uBaseAddr_VITA_SPI = XPAR_ONSEMI_VITA_SPI_0_S00_AXI_BASEADDR;  // Device for configuring the Camera sensor
//    config->uBaseAddr_VITA_CAM = XPAR_ONSEMI_VITA_CAM_0_S00_AXI_BASEADDR;  // Device for receiving Camera sensor data
    config->uBaseAddr_VITA_SPI = XPAR_ONSEMI_VITA_SPI_0_0;  // Device for configuring the Camera sensor
    config->uBaseAddr_VITA_CAM = XPAR_ONSEMI_VITA_CAM_0_0;  // Device for receiving Camera sensor data

    // Uncomment as Video Hardware pipeline IP cores are added
//    config->uDeviceId_CFA = XPAR_CFA_0_DEVICE_ID;
//    config->uDeviceId_CRES = XPAR_CRESAMPLE_0_DEVICE_ID;
//    config->uDeviceId_RGBYCC = XPAR_RGB2YCRCB_0_DEVICE_ID;

    // Uncomment when using the TPG for Video input
//    config->uBaseAddr_TPG_PatternGenerator = XPAR_V_TPG_0_S_AXI_CTRL_BASEADDR; // TPG Device

    config->uDeviceId_VTC_tpg   = XPAR_V_TC_0_DEVICE_ID;                        // Video Timer Controller (VTC) ID
    config->uDeviceId_VDMA_HdmiFrameBuffer = XPAR_AXI_VDMA_0_DEVICE_ID;         // VDMA ID
    config->uBaseAddr_MEM_HdmiFrameBuffer = XPAR_DDR_MEM_BASEADDR + 0x10000000; // VDMA base address for Frame buffers
    config->uNumFrames_HdmiFrameBuffer = XPAR_AXIVDMA_0_NUM_FSTORES;            // NUmber of VDMA Frame buffers

    return;
}


const int resolution = 1920*1080;
#define VSIZE 1080
#define HSIZE 1920
u8 bayer[VSIZE][HSIZE];

// Main (SW) processing loop. Recommended to have an explicit exit condition
void camera_loop(camera_config_t *config) {

	Xuint32 parkptr;
	Xuint32 vdma_S2MM_DMACR, vdma_MM2S_DMACR;


	xil_printf("Entering main SW processing loop\r\n");


	// Grab the DMA parkptr, and update it to ensure that when parked, the S2MM side is on frame 0, and the MM2S side on frame 1
	parkptr = XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_PARKPTR_OFFSET);
	parkptr &= ~XAXIVDMA_PARKPTR_READREF_MASK;
	parkptr &= ~XAXIVDMA_PARKPTR_WRTREF_MASK;
	parkptr |= 0x1;
	XAxiVdma_WriteReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_PARKPTR_OFFSET, parkptr);


	// Grab the DMA Control Registers, and clear circular park mode.
	vdma_MM2S_DMACR = XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_TX_OFFSET+XAXIVDMA_CR_OFFSET);
	XAxiVdma_WriteReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_TX_OFFSET+XAXIVDMA_CR_OFFSET, vdma_MM2S_DMACR & ~XAXIVDMA_CR_TAIL_EN_MASK);
	vdma_S2MM_DMACR = XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_RX_OFFSET+XAXIVDMA_CR_OFFSET);
	XAxiVdma_WriteReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_RX_OFFSET+XAXIVDMA_CR_OFFSET, vdma_S2MM_DMACR & ~XAXIVDMA_CR_TAIL_EN_MASK);


	// Pointers to the S2MM memory frame and M2SS memory frame
	volatile Xuint16 *pS2MM_Mem = (Xuint16 *)XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_S2MM_ADDR_OFFSET+XAXIVDMA_START_ADDR_OFFSET); // VDMA mem being written to by camera
	volatile Xuint16 *pMM2S_Mem = (Xuint16 *)XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_MM2S_ADDR_OFFSET+XAXIVDMA_START_ADDR_OFFSET+4); // VDMA mem being read from

	xil_printf("pS2MM_Mem = %X\n\r", pS2MM_Mem);
	xil_printf("pMM2S_Mem = %X\n\r", pMM2S_Mem);
	// pS2MM is 1 frame ahead of pMM2S = 1920*1080*2

	u16 R = 0;
	u16 G = 0;
	u16 B = 0;
	u8 Y = 0;
	u8 Cb = 0;
	u8 Cr = 0;
	u8 prev_Cb = 0;
	u8 prev_Cr = 0;

	xil_printf( "Start recording...\r\n" );

	XTime start, end;
	XTime_GetTime( &start );

	const int num_frames = 30;

	int j;
	// Run for 1000 frames before going back to HW mode
	for (j = 0; j < num_frames; j++) {
	        // pMM2S_Mem[i] = pS2MM_Mem[resolution-i-1];

		// copy frame in pS2MM to bayer array in 8 bit form
		// TODO: find a better way to copy this, maybe use the original somehow instead?
		for( u32 row = 1; row < VSIZE - 1; row++ )
			for( u32 col = 1; col < HSIZE - 1; col++ )
				bayer[row][col] = (u8)(pS2MM_Mem[ HSIZE * row + col ]); // which byte of the pS2MM is the camera output, the 1st or 2nd?

//		xil_printf( "bayer to RGB\r\n" );
		// 1. bayer -> RGB
		// 2. RGB -> YCbCr (4:2:2)
		for( u32 row = 1; row < VSIZE - 1; row++ )
		{
			for( u32 col = 1; col < HSIZE - 1; col++ )
			{
				// bayer -> RGB
				if ( row % 2 == 0 ) // even row
				{
					if ( col % 2 == 0 ) // even col
					{
						// red pixel
						R = bayer[row][col];
						G = ( bayer[row-1][col] + bayer[row+1][col] + bayer[row][col-1] + bayer[row][col+1] ) / 4;
						B = ( bayer[row-1][col-1] + bayer[row-1][col+1] + bayer[row+1][col-1] + bayer[row+1][col+1] ) / 4;
					}
					else // odd col
					{
						// green pixel
						R = ( bayer[row][col-1] + bayer[row][col+1] ) / 2;
						G = bayer[row][col];
						B = ( bayer[row-1][col] + bayer[row+1][col] ) / 2;
					}
				}
				else // odd row
				{
					if ( col % 2 == 0 ) // even col
					{
						// green pixel
                        R = ( bayer[row-1][col] + bayer[row+1][col] ) / 2;
						G = bayer[row][col];
						B = ( bayer[row][col-1] + bayer[row][col+1] ) / 2;
					}
					else // odd col
					{
						// blue pixel
						R = ( bayer[row-1][col-1] + bayer[row-1][col+1] + bayer[row+1][col-1] + bayer[row+1][col+1] ) / 4;
						G = ( bayer[row-1][col] + bayer[row+1][col] + bayer[row][col-1] + bayer[row][col+1] ) / 4;
						B = bayer[row][col];
					}
				}
//				xil_printf( "R: %02X | G: %02X | B: %02X\r\n", R, G, B);

				// RGB -> 4:2:2
				Y =  (u8)( 16 + (( 66*R + 129*G + 25*B + 128) >> 8 ));
				Cb = (u8)( 128 + (( -38*R - 74*G + 112*B + 128) >> 8 ));
				Cr = (u8)( 128 + (( 112*R - 94*G - 18*B + 128) >> 8 ));

				// write this data to pMM2S memory
				if ( col % 2 == 0 )
				{
					pMM2S_Mem[ row*HSIZE + col ] = ( ((u16) Cb) << 8 ) | ( (u16)Y );
				}
				else
				{
					pMM2S_Mem[ row*HSIZE + col ] = ( ((u16) prev_Cr) << 8 ) | ( (u16)Y );
				}

				prev_Cb = Cb;
				prev_Cr = Cr;
			}
		}
	}

	XTime_GetTime( &end );

	double n_frames = (double)num_frames;
	double time_elapsed = ((end - start)/COUNTS_PER_SECOND );
	printf( "Num frames processed: %lf \r\n", n_frames );
	printf( "Time Elapsed: %lf \r\n", time_elapsed );
	printf( "FPS: %lf \r\n", num_frames/time_elapsed );

	// Grab the DMA Control Registers, and re-enable circular park mode.
	vdma_MM2S_DMACR = XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_TX_OFFSET+XAXIVDMA_CR_OFFSET);
	XAxiVdma_WriteReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_TX_OFFSET+XAXIVDMA_CR_OFFSET, vdma_MM2S_DMACR | XAXIVDMA_CR_TAIL_EN_MASK);
	vdma_S2MM_DMACR = XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_RX_OFFSET+XAXIVDMA_CR_OFFSET);
	XAxiVdma_WriteReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_RX_OFFSET+XAXIVDMA_CR_OFFSET, vdma_S2MM_DMACR | XAXIVDMA_CR_TAIL_EN_MASK);


	xil_printf("Main SW processing loop complete!\r\n");

	sleep(5);

	// Uncomment when using TPG for Video input
	fmc_imageon_disable_tpg(config);

	sleep(1);


	return;
}
