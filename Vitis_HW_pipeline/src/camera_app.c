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

// GLOBALS
/*****************************************************************************/
volatile Xuint16 *pS2MM_Mem;
volatile Xuint16 *pMM2S_Mem;


#define BTN_OFST 0
#define SW_OFST 1
//#define BTN_CENTER 0x1
//#define BTN_UP 0x10
//#define BTN_DOWN 0x2
//#define BTN_RIGHT 0x4
//#define BTN_LEFT 0x8
#define BTN_CENTER 0x4
#define BTN_UP 0x40
#define BTN_DOWN 0x80
#define BTN_RIGHT 0x10
#define BTN_LEFT 0x20

enum ButtonState { PRESSED, OPEN };
enum ButtonState btn_UP_state = OPEN;
enum ButtonState prev_btn_UP_state = OPEN;
enum ButtonState btn_DOWN_state = OPEN;
enum ButtonState prev_btn_DOWN_state = OPEN;
enum ButtonState btn_LEFT_state = OPEN;
enum ButtonState prev_btn_LEFT_state = OPEN;
enum ButtonState btn_RIGHT_state = OPEN;
enum ButtonState prev_btn_RIGHT_state = OPEN;
enum ButtonState btn_CENTER_state = OPEN;
enum ButtonState prev_btn_CENTER_state = OPEN;
enum ButtonState prev_RESET_CAPTURE_INDEX = OPEN;
enum ButtonState RESET_CAPTURE_INDEX = OPEN;

u32 btn_center_hold = 0;

u32* sw_p = (u32*) XPAR_AXI_GPIO_1_BASEADDR;
u32* btn_p = (u32*) XPAR_AXI_GPIO_0_BASEADDR;
u8 sw_state = 0x00;
u8 btn_state = 0x00;

#define RES 1920*1080
#define MAX_IMAGES 32
#define MAX_VIDEO_FRAMES 32

enum ReplayMode { NONE, IMAGE, VIDEO };
enum ReplayMode REPLAY_MODE = NONE;
enum ReplayMode PREV_REPLAY_MODE = NONE;
enum ReplayMode VIDEO_MODE = NONE;

bool exit_condition = false;

const size_t IMG_SIZE = RES * sizeof( u16 );
uint16_t stored_images[ MAX_IMAGES ][ RES ];
uint32_t store_image_index = 0;
int32_t replay_image_index = 0;

//uint16_t video_clip[ MAX_VIDEO_FRAMES ][ RES ];
uint32_t video_end_index = 0;

//u32* GPIO_DEVICE = (u32*)XPAR_LED_SETTER_0_S00_AXI_BASEADDR;
const Xuint8 SW[8] = {0x04, 0x08, 0x01, 0x02, 16, 32, 0x10, 128};
//const Xuint8 SW[8] = {1, 2, 4, 8, 16, 32, 64, 128};

/*****************************************************************************/


// Main function. Initializes the devices and configures VDMA
int main() {

	printf( "Reading inputs...\r\n" );

	camera_config_init(&camera_config);
	fmc_imageon_enable(&camera_config);

	xil_printf( "running camera...\r\n" );
	while( !exit_condition )
	{
		camera_interface( &camera_config );
	}

//	EnableDisableDemo();

	xil_printf( "Exiting... \r\n" );

	cleanup_platform();
	return 0;
}

// Initialize the camera configuration data structure
void camera_config_init(camera_config_t *config) {

    config->uBaseAddr_IIC_FmcIpmi =  XPAR_FMC_IPMI_ID_EEPROM_0_BASEADDR;   // Device for reading HDMI board IPMI EEPROM information
    config->uBaseAddr_IIC_FmcImageon = XPAR_FMC_IMAGEON_IIC_0_BASEADDR;    // Device for configuring the HDMI board

    // Uncomment when using VITA Camera for Video input
    config->uBaseAddr_VITA_SPI = XPAR_ONSEMI_VITA_SPI_0_0;  // Device for configuring the Camera sensor
    config->uBaseAddr_VITA_CAM = XPAR_ONSEMI_VITA_CAM_0_0;  // Device for receiving Camera sensor data

    config->uDeviceId_VTC_tpg   = XPAR_V_TC_0_DEVICE_ID;                        // Video Timer Controller (VTC) ID
    config->uDeviceId_VDMA_HdmiFrameBuffer = XPAR_AXI_VDMA_0_DEVICE_ID;         // VDMA ID
    config->uBaseAddr_MEM_HdmiFrameBuffer = XPAR_DDR_MEM_BASEADDR + 0x10000000; // VDMA base address for Frame buffers
    config->uNumFrames_HdmiFrameBuffer = XPAR_AXIVDMA_0_NUM_FSTORES;            // NUmber of VDMA Frame buffers

    return;
}

void EnableDisableDemo( camera_config_t *config )
{
	xil_printf( "Turn on live feed for 5 sec...\r\n" );
	EnableLiveFeed( &camera_config );
	sleep(5);

	xil_printf( "Turn on frame sw relay for 50 frames\r\n" );
	DisableLiveFeed( &camera_config );
	RunSWRelay( &camera_config );

	xil_printf( "Resume live feed for 5 sec...\r\n" );
	EnableLiveFeed( &camera_config );
	sleep(5);
}

void RunSWRelay( camera_config_t *config )
{
	// Pointers to the S2MM memory frame and M2SS memory frame
	pS2MM_Mem = (Xuint16 *)XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_S2MM_ADDR_OFFSET+XAXIVDMA_START_ADDR_OFFSET);
	pMM2S_Mem = (Xuint16 *)XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_MM2S_ADDR_OFFSET+XAXIVDMA_START_ADDR_OFFSET+4);

	xil_printf("Start processing 50 frames!\r\n");
	xil_printf("pS2MM_Mem = %X\n\r", pS2MM_Mem);
	xil_printf("pMM2S_Mem = %X\n\r", pMM2S_Mem);

	int i, j;
	// Run for 50 frames before going back to HW mode
	for (j = 0; j < 50; j++) {
		for (i = 0; i < 1920*1080; i++) {
		   pMM2S_Mem[i] = pS2MM_Mem[1920*1080-i-1];
		}
	}
}

void camera_interface( camera_config_t *config )
{
	// Pointers to the S2MM memory frame and M2SS memory frame
	pS2MM_Mem = (Xuint16 *)XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_S2MM_ADDR_OFFSET+XAXIVDMA_START_ADDR_OFFSET);
	pMM2S_Mem = (Xuint16 *)XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_MM2S_ADDR_OFFSET+XAXIVDMA_START_ADDR_OFFSET+4);

	ReadInputs( config );

	CaptureModeHandler( config );
	ReplayModeHandler( config );
	VideoModeHandler( config );
}

void ReadInputs( camera_config_t *config )
{
	sw_state = *sw_p;
	btn_state = *btn_p;

	// SW0 kills the program
	exit_condition = ( sw_state & SW[0] ) ? true : false;
//	xil_printf( "BTNS: 0x%02x | SWS: 0x%02x\r\n", btn_state, sw_state );

	PREV_REPLAY_MODE = REPLAY_MODE;
	REPLAY_MODE = ( sw_state == SW[1] ) ? IMAGE : NONE;
//	VIDEO_MODE = ( sw_state == SW[2] ) ? VIDEO : NONE;

    prev_btn_UP_state = btn_UP_state;
    btn_UP_state = ( btn_state & BTN_UP ) ? PRESSED : OPEN; // need debounce? probably not
    prev_btn_DOWN_state = btn_DOWN_state;
    btn_DOWN_state = ( btn_state & BTN_DOWN ) ? PRESSED : OPEN; // need debounce? probably not
    prev_btn_LEFT_state = btn_LEFT_state;
    btn_LEFT_state = ( btn_state & BTN_LEFT ) ? PRESSED : OPEN; // need debounce? probably not
    prev_btn_RIGHT_state = btn_RIGHT_state;
    btn_RIGHT_state = ( btn_state & BTN_RIGHT ) ? PRESSED : OPEN; // need debounce? probably not
    prev_btn_CENTER_state = btn_CENTER_state;
    btn_CENTER_state = ( btn_state & BTN_CENTER ) ? PRESSED : OPEN; // need debounce? probably not
    prev_RESET_CAPTURE_INDEX = RESET_CAPTURE_INDEX;
    RESET_CAPTURE_INDEX = ( sw_state & SW[3] ) ? PRESSED : OPEN; // need debounce? probably not
}


void DisableLiveFeed( camera_config_t *config )
{
//	xil_printf("Disable Camera\r\n");

	Xuint32 parkptr;
	Xuint32 vdma_S2MM_DMACR, vdma_MM2S_DMACR;
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
}

void EnableLiveFeed( camera_config_t *config )
{
//	xil_printf("Enable Camera\r\n");

	Xuint32 vdma_S2MM_DMACR, vdma_MM2S_DMACR;
	// Grab the DMA Control Registers, and re-enable circular park mode.
	vdma_MM2S_DMACR = XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_TX_OFFSET+XAXIVDMA_CR_OFFSET);
	XAxiVdma_WriteReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_TX_OFFSET+XAXIVDMA_CR_OFFSET, vdma_MM2S_DMACR | XAXIVDMA_CR_TAIL_EN_MASK);
	vdma_S2MM_DMACR = XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_RX_OFFSET+XAXIVDMA_CR_OFFSET);
	XAxiVdma_WriteReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_RX_OFFSET+XAXIVDMA_CR_OFFSET, vdma_S2MM_DMACR | XAXIVDMA_CR_TAIL_EN_MASK);
}

void StoreImage( u32 index )
{
//	xil_printf( "Storing image at Index: %d\r\n", index );
	// memcpy is probably the fastest way to do this tbh
	memcpy( stored_images[ index ], pS2MM_Mem, IMG_SIZE );
}

void DisplayImage( u32 index )
{
//	xil_printf( "Displaying image at Index: %d\r\n", index );
	// memcpy is probably the fastest way to do this tbh
	memcpy( pMM2S_Mem, stored_images[ index ], IMG_SIZE );
}

void ImageToScreen( u32 index, camera_config_t *config )
{
	DisableLiveFeed( config );
	DisplayImage( index );
	sleep( 2 );
	EnableLiveFeed( config );
}

void CaptureModeHandler( camera_config_t *config )
{
	if ( RESET_CAPTURE_INDEX == PRESSED && prev_RESET_CAPTURE_INDEX != PRESSED )
	{
		xil_printf( "Reset capture mode storage index\r\n" );
		store_image_index = 0;
	}
	if ( ( btn_CENTER_state == PRESSED ) && ( btn_CENTER_state != prev_btn_UP_state ) )
	{
		if ( REPLAY_MODE != NONE )
		{
			xil_printf( "Image Replay mode is also enabled. Turn off Image Replay mode to Capture Images" );
		}
		else if ( VIDEO_MODE != NONE )
		{
			xil_printf( "Video mode is also enabled. Turn off Video mode to Capture Images" );
		}
		else
		{
			xil_printf( "Pressed capture button\r\n" );
			xil_printf( "Storing image at Index: %d\r\n", store_image_index );
			StoreImage( store_image_index );
			xil_printf( "Displaying image at Index: %d\r\n", store_image_index );
			ImageToScreen( store_image_index, config );
			xil_printf( "Display done\r\n" );
			if ( store_image_index + 1 < MAX_IMAGES )
			{
				store_image_index++;
				// storage has changed, so reset replay
				replay_image_index = 0;
			}
		}
	}

}

void ReplayModeHandler( camera_config_t *config )
{
	// SW1 turns on image replay mode
	if ( REPLAY_MODE == IMAGE )
	{
		if ( VIDEO_MODE != NONE )
		{
			xil_printf( "Video mode is also enabled. Turn off Video mode to Capture Images" );
		}
		else
		{
			DisableLiveFeed( config );

			if ( PREV_REPLAY_MODE == NONE )
			{
				xil_printf( "Replay mode enabled. To exit replay mode, turn SW1 OFF\r\n" );
				xil_printf( "Current image index: %d\r\n", replay_image_index );
			}

			DisplayImage( replay_image_index );

			// rewind the replay index
			if ( ( btn_LEFT_state == PRESSED ) && ( btn_LEFT_state != prev_btn_LEFT_state ) )
			{
				if ( replay_image_index - 1 >= 0 )
				{
					replay_image_index--;
					xil_printf( "Replay index decremented. Current Replay index: %d\r\n", replay_image_index );
				}
				else
					xil_printf( "Replay index is at 0\r\n" );
			}

			// rewind the replay index
			if ( ( btn_RIGHT_state == PRESSED ) && ( btn_RIGHT_state != prev_btn_RIGHT_state ) )
			{
				if ( ( replay_image_index < store_image_index-1 ) && ( replay_image_index + 1 <= MAX_IMAGES ) )
				{
					replay_image_index++;
					xil_printf( "Replay index incremeneted. Current Replay index: %d\r\n", replay_image_index );
				}
				else
					xil_printf( "Replay index is at max\r\n" );
			}
		}
	}
	else
	{
		if ( PREV_REPLAY_MODE == IMAGE )
			xil_printf( "Exiting Replay Mode\r\n" );
		EnableLiveFeed( config );
	}

}

void VideoModeHandler( camera_config_t *config )
{
	if ( VIDEO_MODE == VIDEO )
	{
		if ( REPLAY_MODE != NONE )
		{
			xil_printf( "Image Replay mode is also enabled. Turn off Image Replay mode to use Video Mode" );
		}
		else
		{
			if ( btn_UP_state == PRESSED && prev_btn_UP_state != PRESSED )
			{
				CaptureVideo( config );
			}
			else if ( btn_DOWN_state == PRESSED && prev_btn_DOWN_state != PRESSED )
			{
				ReplayVideo( config );
			}
		}
	}

}

void CaptureVideo( camera_config_t *config )
{
	xil_printf( "Recording video for 32 frames\r\n" );
	DisableLiveFeed( config );
//	XTime video_start, curtime;
	double time_elapsed = 0;

//	XTime_GetTime( &video_start );
	xil_printf( "recording...\r\n" );
	int i = 0;
	while( i < MAX_VIDEO_FRAMES );
	{
		xil_printf( "recording video frame %d\r\n", i );
//		XTime_GetTime( &curtime );
//		time_elapsed = ((curtime - video_start)/COUNTS_PER_SECOND );

		xil_printf( "Storing image at Index: %d\r\n", i );
		// memcpy is probably the fastest way to do this tbh
//		memcpy( video_clip[ i ], pS2MM_Mem, IMG_SIZE );

		i++;
	}
	video_end_index = i-1;

	xil_printf( "Finished recording video\r\n" );
	EnableLiveFeed( config );
}

void ReplayVideo( camera_config_t *config )
{
	xil_printf( "Replaying 5 sec video\r\n" );
	DisableLiveFeed( config );

	for ( int i = 0; i < video_end_index; i++ )
	{
		xil_printf( "Displaying image at Index: %d\r\n", i );
		// memcpy is probably the fastest way to do this tbh
//		memcpy( pMM2S_Mem, video_clip[ i ], IMG_SIZE );
	}

	xil_printf( "Finished replaying video\r\n" );
	EnableLiveFeed( config );
}

