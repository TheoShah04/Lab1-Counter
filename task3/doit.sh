rm -rf obj_dir
rm -f counter.vcd

verilator -Wall --cc --trace counter.sv --exe counter_tb.cpp
make -j -C obj_dir/ -f Vcounter.mk Vcounter

obj_dir/Vcounter

cd
cd ~/Documents/iac/lab0-devtools/tools
source ./attach_usb.sh