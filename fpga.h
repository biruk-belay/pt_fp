#ifndef FPGA_H
#define FPGA_H

#define MAX_SLOTS 100
#define CLB          0
#define BRAM         1
#define DSP          2
#define FBDN         3
#define CENTRAL_CLK  4
#define BRAM36       5

//parameters for zynq
#define ZYNQ_CLB_TOT 2200
#define ZYNQ_BRAM_TOT 60
#define ZYNQ_DSP_TOT 80
#define ZYNQ_CLB_MIN 10
#define ZYNQ_BRAM_MIN 0
#define ZYNQ_DSP_MIN 0
#define ZYNQ_FORBIDDEN 4
#define ZYNQ_NUM_ROWS 10
#define ZYNQ_WIDTH 29
#define ZYNQ_CLK_REG 4
#define ZYNQ_CLK00_BRAM 1
#define ZYNQ_CLK01_BRAM 1
#define ZYNQ_CLK10_BRAM 2
#define ZYNQ_CLK11_BRAM 2
#define ZYNQ_CLK00_DSP 1
#define ZYNQ_CLK01_DSP 1
#define ZYNQ_CLK10_DSP 1
#define ZYNQ_CLK11_DSP 1

typedef struct {
    int x;
    int y;
    int w;
    int h;
}pos;

typedef struct {
    pos clk_reg_pos;
    int clb_per_column;
    int bram_per_column;
    int dsp_per_column;
    int clb_num;
    int bram_num;
    int dsp_num;
    int forbidden_num;
    int *bram_pos;
    int *dsp_pos;
    pos forbidden_pos;
    pos bram_pos_new;
}fpga_clk_reg;

#define init_clk_reg(id, pos, clb, bram, dsp, num_bram, num_dsp, num_forbidden, \
                               pos_bram, pos_dsp, forbidden) \
                               clk_reg[id].clk_reg_pos = pos;    \
                               clk_reg[id].clb_per_column = clb;\
                               clk_reg[id].bram_per_column = bram; \
                               clk_reg[id].dsp_per_column = dsp;  \
                               clk_reg[id].bram_num = num_bram;  \
                               clk_reg[id].dsp_num = num_dsp;\
                               clk_reg[id].forbidden_num = num_forbidden;\
                               clk_reg[id].bram_pos = pos_bram;\
                               clk_reg[id].dsp_pos = pos_dsp; \
                               clk_reg[id].forbidden_pos = forbidden


class fpga_zynq
{
public:
    int num_clk_reg = ZYNQ_CLK_REG;
    fpga_clk_reg clk_reg[ZYNQ_CLK_REG];
    pos clk_reg_pos [ZYNQ_CLK_REG] = {{0,  0,  14, 50},
                                      {0,  50, 14, 50},
                                      {15, 0,  14, 50},
                                      {15, 50, 14, 50}};

    unsigned int clb_per_col  = 50;
    unsigned int bram_per_col = 10;
    unsigned int dsp_per_col  = 20;

    unsigned long num_rows = ZYNQ_NUM_ROWS;
    unsigned long width = ZYNQ_WIDTH;

    int bram_in_reg[ZYNQ_CLK_REG] = {1, 1, 2, 2};
    int bram_pos[ZYNQ_CLK_REG][10] = {{3, 0, 0},  {3, 0, 0},
                                     {17, 24, 0}, {17, 24, 0}};

    int dsp_in_reg[ZYNQ_CLK_REG] = {1, 1, 1, 1};
    int dsp_pos[ZYNQ_CLK_REG][3] = {{6, 0, 0},
                                    {6, 0, 0},
                                    {21, 0, 0},
                                    {21, 0, 0}};

    unsigned long num_forbidden_slots = ZYNQ_FORBIDDEN;
    pos fbdn_pos[ZYNQ_FORBIDDEN] =   {{9, 0,  1, 50},
                                      {9,  50, 1, 50},
                                      {14, 0,  1, 50},
                                      {14, 50, 1, 50}};

    int forbidden_regs[ZYNQ_CLK_REG] = {1, 1, 0, 0};

    void initialize_clk_reg();
    fpga_zynq();
};

#endif // FPGA_H
