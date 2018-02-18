export PC2SW="/upb/departments/pc2/software"
export ALTERAOCLSDKROOT="$PC2SW/altera/aocl-rte"
export AALSDK="$PC2SW/aalsdk/5.0.2"
export AOCL_BOARD_PACKAGE_ROOT="$ALTERAOCLSDKROOT/board/bdw_fpga_pilot_opencl_bsp_v1.0.2"
export PATH=$PATH:$ALTERAOCLSDKROOT/bin
export LD_LIBRARY_PATH="/usr/lib"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ALTERAOCLSDKROOT/host/linux64/lib:
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$AOCL_BOARD_PACKAGE_ROOT/host/linux64/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/user/local/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$AALSDK/lib

