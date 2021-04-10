########################
# Physical Constraints #
########################

# I2C Chain on FMC-IMAGEON
set_property PACKAGE_PIN J20 [get_ports fmc_imageon_iic_scl_io]
set_property IOSTANDARD LVCMOS25 [get_ports fmc_imageon_iic_scl_io]
set_property SLEW SLOW [get_ports fmc_imageon_iic_scl_io]
set_property DRIVE 8 [get_ports fmc_imageon_iic_scl_io]

set_property PACKAGE_PIN K21 [get_ports fmc_imageon_iic_sda_io]
set_property IOSTANDARD LVCMOS25 [get_ports fmc_imageon_iic_sda_io]
set_property SLEW SLOW [get_ports fmc_imageon_iic_sda_io]
set_property DRIVE 8 [get_ports fmc_imageon_iic_sda_io]

set_property PACKAGE_PIN N20 [get_ports {fmc_imageon_iic_rst_n[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {fmc_imageon_iic_rst_n[0]}]
set_property SLEW SLOW [get_ports {fmc_imageon_iic_rst_n[0]}]
set_property DRIVE 8 [get_ports {fmc_imageon_iic_rst_n[0]}]


# I2C Chain on FMC-IPMI-ID-EEPROM
set_property PACKAGE_PIN R7 [get_ports fmc_ipmi_id_eeprom_scl_io]
set_property IOSTANDARD LVCMOS25 [get_ports fmc_ipmi_id_eeprom_scl_io]
set_property SLEW SLOW [get_ports fmc_ipmi_id_eeprom_scl_io]
set_property DRIVE 8 [get_ports fmc_ipmi_id_eeprom_scl_io]

set_property PACKAGE_PIN U7 [get_ports fmc_ipmi_id_eeprom_sda_io]
set_property IOSTANDARD LVCMOS25 [get_ports fmc_ipmi_id_eeprom_sda_io]
set_property SLEW SLOW [get_ports fmc_ipmi_id_eeprom_sda_io]
set_property DRIVE 8 [get_ports fmc_ipmi_id_eeprom_sda_io]

## ----------------------------------------------------------------------------
## User LEDs - Bank 33
## ---------------------------------------------------------------------------- 
#set_property PACKAGE_PIN T22 [get_ports {leds[0]}];  # "LD0"
#set_property PACKAGE_PIN T21 [get_ports {leds[1]}];  # "LD1"
#set_property PACKAGE_PIN U22 [get_ports {leds[2]}];  # "LD2"
#set_property PACKAGE_PIN U21 [get_ports {leds[3]}];  # "LD3"
#set_property PACKAGE_PIN V22 [get_ports {leds[4]}];  # "LD4"
#set_property PACKAGE_PIN W22 [get_ports {leds[5]}];  # "LD5"
#set_property PACKAGE_PIN U19 [get_ports {leds[6]}];  # "LD6"
#set_property PACKAGE_PIN U14 [get_ports {leds[7]}];  # "LD7"
#set_property IOSTANDARD LVCMOS33 [get_ports {leds[0]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {leds[1]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {leds[2]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {leds[3]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {leds[4]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {leds[5]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {leds[6]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {leds[7]}]
#
## ----------------------------------------------------------------------------
## User Push Buttons - Bank 34
## ---------------------------------------------------------------------------- 
#set_property PACKAGE_PIN P16 [get_ports {BTNS[0]}];  # "BTNC"
#set_property PACKAGE_PIN R16 [get_ports {BTNS[1]}];  # "BTND"
#set_property PACKAGE_PIN N15 [get_ports {BTNS[2]}];  # "BTNL"
#set_property PACKAGE_PIN R18 [get_ports {BTNS[3]}];  # "BTNR"
#set_property PACKAGE_PIN T18 [get_ports {BTNS[4]}];  # "BTNU"
#set_property IOSTANDARD LVCMOS25 [get_ports {BTNS_tri_io*}]

# Virtual Buttons and Switches

set_property PACKAGE_PIN AB6 [get_ports {V_SW_tri_i[7]}];  # "JC1_N"  "Virtual Switch 7"
set_property PACKAGE_PIN AB7 [get_ports {V_SW_tri_i[6]}];  # "JC1_P"  "Virtual Switch 6"
set_property PACKAGE_PIN AA4 [get_ports {V_SW_tri_i[5]}];  # "JC2_N"  "Virtual Switch 5"
set_property PACKAGE_PIN Y4  [get_ports {V_SW_tri_i[4]}];  # "JC2_P"  "Virtual Switch 4"
set_property PACKAGE_PIN T6  [get_ports {V_SW_tri_i[3]}];  # "JC3_N"  "Virtual Switch 3"  
set_property PACKAGE_PIN R6  [get_ports {V_SW_tri_i[2]}];  # "JC3_P"  "Virtual Switch 2"
set_property PACKAGE_PIN U4  [get_ports {V_SW_tri_i[1]}];  # "JC4_N"  "Virtual Switch 1"
set_property PACKAGE_PIN T4  [get_ports {V_SW_tri_i[0]}];  # "JC4_P"  "Virtual Switch 0"

set_property PACKAGE_PIN W7 [get_ports {V_BTN_tri_i[0]}];  # "JD1_N"   "Virtual Button Up"
set_property PACKAGE_PIN V7 [get_ports {V_BTN_tri_i[1]}];  # "JD1_P"   "Virtual Button Down"
set_property PACKAGE_PIN V4 [get_ports {V_BTN_tri_i[2]}];  # "JD2_N"   "Virtual Button Right"
set_property PACKAGE_PIN V5 [get_ports {V_BTN_tri_i[3]}];  # "JD2_P"   "Virtual Button Left"
set_property PACKAGE_PIN W5 [get_ports {V_BTN_tri_i[4]}];  # "JD3_N"   "Virtual Button Center"
set_property IOSTANDARD LVCMOS33 [get_ports -of_objects [get_iobanks 13]];

## ----------------------------------------------------------------------------
## User DIP Switches - Bank 35
## ---------------------------------------------------------------------------- 
set_property PACKAGE_PIN F22 [get_ports {SWS[0]}];  # "SW0"
set_property PACKAGE_PIN G22 [get_ports {SWS[1]}];  # "SW1"
set_property PACKAGE_PIN H22 [get_ports {SWS[2]}];  # "SW2"
set_property PACKAGE_PIN F21 [get_ports {SWS[3]}];  # "SW3"
set_property PACKAGE_PIN H19 [get_ports {SWS[4]}];  # "SW4"
set_property PACKAGE_PIN H18 [get_ports {SWS[5]}];  # "SW5"
set_property PACKAGE_PIN H17 [get_ports {SWS[6]}];  # "SW6"
set_property PACKAGE_PIN M15 [get_ports {SWS[7]}];  # "SW7"
set_property IOSTANDARD LVCMOS25 [get_ports {SWS_tri_io*}]


# HDMI Input (ADV7611) on FMC-IMAGEON
# phjones: NOT USED set_property PACKAGE_PIN D18 [get_ports IO_HDMII_clk]
# phjones: NOT USED  set_property PACKAGE_PIN A17  [get_ports {IO_HDMII_data[0]}]
# phjones: NOT USED  set_property PACKAGE_PIN A16  [get_ports {IO_HDMII_data[1]}]
# phjones: NOT USED  set_property PACKAGE_PIN C18 [get_ports {IO_HDMII_data[2]}]
# phjones: NOT USED  set_property PACKAGE_PIN D21  [get_ports {IO_HDMII_data[3]}]
# phjones: NOT USED  set_property PACKAGE_PIN E18  [get_ports {IO_HDMII_data[4]}]
# phjones: NOT USED  set_property PACKAGE_PIN C17 [get_ports {IO_HDMII_data[5]}]
# phjones: NOT USED  set_property PACKAGE_PIN E21  [get_ports {IO_HDMII_data[6]}]
# phjones: NOT USED  set_property PACKAGE_PIN F18  [get_ports {IO_HDMII_data[7]}]
# phjones: NOT USED  set_property PACKAGE_PIN A22  [get_ports {IO_HDMII_data[8]}]
# phjones: NOT USED  set_property PACKAGE_PIN A21  [get_ports {IO_HDMII_data[9]}]
# phjones: NOT USED  set_property PACKAGE_PIN B22  [get_ports {IO_HDMII_data[10]}]
# phjones: NOT USED  set_property PACKAGE_PIN B21  [get_ports {IO_HDMII_data[11]}]
# phjones: NOT USED  set_property PACKAGE_PIN B15  [get_ports {IO_HDMII_data[12]}]
# phjones: NOT USED  set_property PACKAGE_PIN C15  [get_ports {IO_HDMII_data[13]}]
# phjones: NOT USED  set_property PACKAGE_PIN B17  [get_ports {IO_HDMII_data[14]}]
# phjones: NOT USED  set_property PACKAGE_PIN B16 [get_ports {IO_HDMII_data[15]}]
set_property PACKAGE_PIN A19 [get_ports {IO_HDMII_spdif}]

# phjones: NOT USED  set_property IOSTANDARD LVCMOS25 [get_ports IO_HDMII_clk]
# phjones: NOT USED  set_property IOSTANDARD LVCMOS25 [get_ports {IO_HDMII_data*}]
set_property IOSTANDARD LVCMOS25 [get_ports {IO_HDMII_spdif}]

# HDMI Output (ADV7511) on FMC-IMAGEON
set_property PACKAGE_PIN C19 [get_ports IO_HDMIO_clk]
set_property PACKAGE_PIN C22 [get_ports {IO_HDMIO_data[0]}]
set_property PACKAGE_PIN D22 [get_ports {IO_HDMIO_data[1]}]
set_property PACKAGE_PIN E20 [get_ports {IO_HDMIO_data[2]}]
set_property PACKAGE_PIN D15 [get_ports {IO_HDMIO_data[3]}]
set_property PACKAGE_PIN E19 [get_ports {IO_HDMIO_data[4]}]
set_property PACKAGE_PIN F19 [get_ports {IO_HDMIO_data[5]}]
set_property PACKAGE_PIN C20 [get_ports {IO_HDMIO_data[6]}]
set_property PACKAGE_PIN E15 [get_ports {IO_HDMIO_data[7]}]
set_property PACKAGE_PIN G19 [get_ports {IO_HDMIO_data[8]}]
set_property PACKAGE_PIN G16 [get_ports {IO_HDMIO_data[9]}]
set_property PACKAGE_PIN D20 [get_ports {IO_HDMIO_data[10]}]
set_property PACKAGE_PIN B20 [get_ports {IO_HDMIO_data[11]}]
set_property PACKAGE_PIN G15 [get_ports {IO_HDMIO_data[12]}]
set_property PACKAGE_PIN G21 [get_ports {IO_HDMIO_data[13]}]
set_property PACKAGE_PIN B19 [get_ports {IO_HDMIO_data[14]}]
set_property PACKAGE_PIN G20 [get_ports {IO_HDMIO_data[15]}]
set_property PACKAGE_PIN A18 [get_ports IO_HDMIO_spdif]

set_property IOSTANDARD LVCMOS25 [get_ports IO_HDMIO_clk]
set_property IOSTANDARD LVCMOS25 [get_ports IO_HDMIO_data*]
set_property IOB TRUE [get_ports IO_HDMIO_data*]
set_property IOSTANDARD LVCMOS25 [get_ports IO_HDMIO_spdif]


# VITA Camera Sensor interface

   # SPI for Camera configuration
set_property PACKAGE_PIN P20 [get_ports IO_VITA_SPI_spi_sclk]
set_property PACKAGE_PIN P21 [get_ports IO_VITA_SPI_spi_ssel_n]
set_property PACKAGE_PIN N17 [get_ports IO_VITA_SPI_spi_mosi]
set_property PACKAGE_PIN N18 [get_ports IO_VITA_SPI_spi_miso]


   # Camera data, clocking, reset, trigger, and sync   
set_property PACKAGE_PIN L17 [get_ports IO_VITA_CAM_clk_pll]
set_property PACKAGE_PIN L19 [get_ports IO_VITA_CAM_reset_n]
set_property PACKAGE_PIN M17 [get_ports {IO_VITA_CAM_trigger[2]}]
set_property PACKAGE_PIN K19 [get_ports {IO_VITA_CAM_trigger[1]}]
set_property PACKAGE_PIN K20 [get_ports {IO_VITA_CAM_trigger[0]}]
set_property PACKAGE_PIN J16 [get_ports {IO_VITA_CAM_monitor[0]}]
set_property PACKAGE_PIN J17 [get_ports {IO_VITA_CAM_monitor[1]}]

set_property PACKAGE_PIN M19 [get_ports IO_VITA_CAM_clk_out_p]
set_property PACKAGE_PIN M20 [get_ports IO_VITA_CAM_clk_out_n]
set_property PACKAGE_PIN R19 [get_ports IO_VITA_CAM_sync_p]
set_property PACKAGE_PIN T19 [get_ports IO_VITA_CAM_sync_n]
set_property PACKAGE_PIN R20 [get_ports {IO_VITA_CAM_data_p[0]}]
set_property PACKAGE_PIN R21 [get_ports {IO_VITA_CAM_data_n[0]}]
set_property PACKAGE_PIN T16 [get_ports {IO_VITA_CAM_data_p[1]}]
set_property PACKAGE_PIN T17 [get_ports {IO_VITA_CAM_data_n[1]}]
set_property PACKAGE_PIN J21 [get_ports {IO_VITA_CAM_data_p[2]}]
set_property PACKAGE_PIN J22 [get_ports {IO_VITA_CAM_data_n[2]}]
set_property PACKAGE_PIN J18 [get_ports {IO_VITA_CAM_data_p[3]}]
set_property PACKAGE_PIN K18 [get_ports {IO_VITA_CAM_data_n[3]}]
# phjones: NOT USED  set_property PACKAGE_PIN M21 [get_ports {IO_VITA_CAM_data_p[4]}];
# phjones: NOT USED  set_property PACKAGE_PIN M22 [get_ports {IO_VITA_CAM_data_n[4]}];
# phjones: NOT USED  set_property PACKAGE_PIN L21 [get_ports {IO_VITA_CAM_data_p[5]}];
# phjones: NOT USED  set_property PACKAGE_PIN L22 [get_ports {IO_VITA_CAM_data_n[5]}];
# phjones: NOT USED  set_property PACKAGE_PIN N22 [get_ports {IO_VITA_CAM_data_p[6]}];
# phjones: NOT USED  set_property PACKAGE_PIN P22 [get_ports {IO_VITA_CAM_data_n[6]}];
# phjones: NOT USED  set_property PACKAGE_PIN P17 [get_ports {IO_VITA_CAM_data_p[7]}];
# phjones: NOT USED  set_property PACKAGE_PIN P18 [get_ports {IO_VITA_CAM_data_n[7]}];

set_property IOSTANDARD LVCMOS25 [get_ports IO_VITA_CAM_clk_pll]
set_property IOSTANDARD LVCMOS25 [get_ports IO_VITA_CAM_reset_n]
set_property IOSTANDARD LVCMOS25 [get_ports IO_VITA_CAM_trigger*]
set_property IOSTANDARD LVCMOS25 [get_ports IO_VITA_CAM_monitor*]
set_property IOSTANDARD LVCMOS25 [get_ports IO_VITA_SPI_spi_*]

set_property IOSTANDARD LVDS_25 [get_ports IO_VITA_CAM_clk_out_*]
set_property IOSTANDARD LVDS_25 [get_ports IO_VITA_CAM_sync_*]
set_property IOSTANDARD LVDS_25 [get_ports IO_VITA_CAM_data_*]

set_property DIFF_TERM true [get_ports IO_VITA_CAM_clk_out_*]
set_property DIFF_TERM true [get_ports IO_VITA_CAM_sync_*]
set_property DIFF_TERM true [get_ports IO_VITA_CAM_data_*]

# Note that the bank voltage for IO Bank 33 is fixed to 3.3V on ZedBoard. 
#set_property IOSTANDARD LVCMOS33 [get_ports -of_objects [get_iobanks 33]]

# Set the bank voltage for IO Bank 34 to 1.8V by default.
# set_property IOSTANDARD LVCMOS33 [get_ports -of_objects [get_iobanks 34]];
#set_property IOSTANDARD LVCMOS18 [get_ports -of_objects [get_iobanks 34]]

# Set the bank voltage for IO Bank 35 to 1.8V by default.
# set_property IOSTANDARD LVCMOS33 [get_ports -of_objects [get_iobanks 35]];
# set_property IOSTANDARD LVCMOS25 [get_ports -of_objects [get_iobanks 35]];

# Video Clock Synthesizer
set_property PACKAGE_PIN L18 [get_ports fmc_imageon_vclk]
set_property IOSTANDARD LVCMOS25 [get_ports fmc_imageon_vclk]

######################
#  Clock Constraints #
######################

# The following constraints are already created by the "ZYNQ7 Processing System" core
# phjones: UNUSED create_clock -period 13.333 -name clk_fpga_0 [get_nets -hierarchical FCLK_CLK0]
# phjones: UNUSED create_clock -period  6.667 -name clk_fpga_1 [get_nets -hierarchical FCLK_CLK1]
# phjones: UNUSED create_clock -period  5.000 -name clk_fpga_2 [get_nets -hierarchical FCLK_CLK2]

create_clock -period 6.734 -name video_clk [get_ports fmc_imageon_vclk]

# phjones: UNUSED create_clock -period 6.730 -name hdmii_clk [get_ports IO_HDMII_clk]

create_clock -period 2.692 -name vita_ser_clk [get_ports IO_VITA_CAM_clk_out_p]


# Define asynchronous clock domains
set_clock_groups -asynchronous  -group [get_clocks clk_fpga_0] \
                                -group [get_clocks clk_fpga_1] \
                                -group [get_clocks video_clk]
                                # phjones: UNUSED -group [get_clocks hdmii_clk] \
                                # phjones: UNUSED -group [get_clocks CLKDIV_c_0] \
                                # phjones: UNUSED -group [get_clocks vita_clk_div4_l_n_0]
