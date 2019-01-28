#include "zynq_model.h"
#include "fpga.h"
#include <string>

#ifndef GENERATE_FP_H
#define GENERATE_FP_H
#ifndef ZYNQ_FINE_GRAINED_H
#define ZYNQ_FINE_GRAINED_H

#ifndef CLB
#define CLB          0
#define BRAM         1
#define DSP          2
#endif

#ifndef CENTRAL_CLK
#define FBDN         3
#define CENTRAL_CLK  4
#define ZYNQ_WIDTH   29
#endif

typedef struct {
    unsigned long int type_of_res;
    unsigned long slice_1;
    unsigned long slice_2;
}finegrained_res_description;


class zynq_fine_grained{

public:
    finegrained_res_description zynq_fg[ZYNQ_WIDTH] =   {{CLB, 0, 1},
                                                        {CLB,  2, 3},
                                                        {CLB,  4, 5},
                                                        {BRAM, 0, 0},
                                                        {CLB,  6, 7},
                                                        {CLB, 8, 9},
                                                        {DSP, 0, 0},
                                                        {CLB, 10, 11},
                                                        {CLB, 12, 13},
                                                        {FBDN, 1, 0},
                                                        {CLB, 14, 15},
                                                        {CLB, 16, 17},
                                                        {CLB, 18, 19},
                                                        {CLB, 20, 21},
                                                        {CENTRAL_CLK, 0 ,1},
                                                        {CLB, 22, 23},
                                                        {CLB, 24, 25},
                                                        {BRAM, 1, 0},
                                                        {CLB, 26, 27},
                                                        {CLB, 28, 29},
                                                        {CLB, 30, 31},
                                                        {DSP, 1, 0},
                                                        {CLB, 32, 33},
                                                        {CLB, 34, 35},
                                                        {BRAM, 2, 0},
                                                        {CLB, 36, 37},
                                                        {CLB, 38, 39},
                                                        {CLB, 40, 41},
                                                        {CLB, 42, 43}};

    void init_fine_grained();
    zynq_fine_grained();
};

#endif // ZYNQ_FINE_GRAINED_H

//typedef std::vector<std::vector<unsigned long>> vec_2d;
typedef  struct {
    unsigned long slice_x1;
    unsigned long slice_x2;
    unsigned long slice_y1;
    unsigned long slice_y2;
}slice_addres;

#define COPY_TO_INPUT_BUFFER(bram_vector, clb_vector, dsp_vector, num_slots, \
                             connections, connection_matrix, num_forbidden_slots, \
                             num_rows, width, forbidden_regions) \
                             to_solver.bram = bram_vector; \
                             to_solver.clb  = clb_vector;\
                             to_solver.dsp  = dsp_vector;\
                             to_solver.num_slots = num_slots;\
                             to_solver.num_connected_slots = connections;\
                             to_solver.conn_vector = connection_matrix;\
                             to_solver.forbidden_slots = num_forbidden_slots;\
                             to_solver.num_rows = num_rows;\
                             to_solver.width = width;\
                             to_solver.fbdn_slot = forbidden_regions;

class generate_fp_zynq
{
public:

    vector <slice_addres> slice_clb     = vector <slice_addres>(MAX_SLOTS);
    vector <slice_addres> slice18_bram  = vector <slice_addres>(MAX_SLOTS);
    vector <slice_addres> slice36_bram  = vector <slice_addres>(MAX_SLOTS);
    vector <slice_addres> slice_dsp     = vector <slice_addres>(MAX_SLOTS);

    //string slot_names[] = {"freq_div", "rot_reg", "shift", "name_4"};

    fpga_zynq *zynq;
    zynq_fine_grained *zynq_fg;

    generate_fp_zynq(fpga_zynq *zynq_ptr);
    void generate_floor_plan(param_from_solver *from_solver);

private:
    void sort_output(param_from_solver *param, int res_type, vector <slice_addres> *output_vec);

};

#endif // GENERATE_FP_H
