#include "Vtop.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

int main(int argc, char **argv, char **env){
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);
    //init top verilog instance
    Vtop* top = new Vtop;
    //init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("top.vcd");

    //init Vbuddy
    if (vbdOpen()!=1) return(-1);
    vbdHeader("BIN2BCD");

    //initialize simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->en = 0;
    vbdSetMode (1);

    //run simulation for many clock cycles
    for (i=0; i<1000; i++){
        //dump variables into VCD file and toggle clock
        for(clk=0; clk<2; clk++){
            tfp->dump (2*i+clk);
            top->clk = !top->clk;
            top->eval ();
        }

        //++++ Send count value to Vbuddy
        vbdHex(4, (int(top->v) >> 16) & 0xF);
        vbdHex(3, (int(top->v) >> 8) & 0xF);
        vbdHex(2, (int(top->v) >> 4) & 0xF);
        vbdHex(1, int(top->v) & 0xF);
        vbdCycle(i+1);

        top->rst = 0;
        top->en = vbdFlag();
        top->v = vbdValue();
        if (Verilated::gotFinish()) exit(0);
    }
    vbdClose(); //++++
    tfp->close();
    exit(0);
}