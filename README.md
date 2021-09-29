# HW-SW_Image_Processing_Pipeline

This project features a hardware and a software implementation of an image-processing pipeline.

In this pipeline, images are collected using a FMC IMAGEON video camera. The FMC IMAGEON is connected to a Xilinx Zedboard through an RTL HDMI interface.

The Image processing in this lab involves demosaicing the bayer filter integrated into the FMC IMAGEON. This demosaicing is implemented both in hardware and in software to assess the performance differences.

## Folder Descriptions

```Vivado/``` - contains the full Vivado project archive of the RTL image processing pipeline
```Vitis_HW_pipeline``` - source code for firmware used to control the FMC IMAGEON. The firmware is directly integrated with the RTL pipeline implemented on the Zedboard.
```Vitis_SW_pipeline``` - source code for firmware implementation of image processing pipeline
