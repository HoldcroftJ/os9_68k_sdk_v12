# Makefile include file for making Rom Libraries from source code included
#   only in the OEM Packages

OEM_MAKERS	=	\
				bp_tk360.make \
				rom_common.make \
				rom_serial.make \
				rom_port.make \
				rom_image.make 

