# Comon Makefile for MVME147 ROM BSP makers

BSP_MAKERS	= \
				rom_initext.make \
				rom_booters.make \
				rom_bootfile.make \
				rom_utilities.make

# end of file
