#include "generate_fp.h"
#include <iostream>
#include <fstream>

string slot_names[] = {"freq_div", "rot_reg", "shift", "name_4"};
zynq_fine_grained::zynq_fine_grained()
{

}

generate_fp_zynq::generate_fp_zynq(fpga_zynq *zynq_ptr)
{
    zynq_fg = new zynq_fine_grained();
    zynq = zynq_ptr;
}

void generate_fp_zynq::generate_floor_plan(param_from_solver *from_solver)
{
    unsigned i;
    ofstream write_xdc;

    sort_output(from_solver, CLB,    &slice_clb);
    sort_output(from_solver, BRAM,   &slice18_bram);
    sort_output(from_solver, BRAM36, &slice36_bram);
    sort_output(from_solver, DSP,    &slice_dsp);

    cout << "inside sort output clb " << endl;
    for (i = 0; i < from_solver->num_partition; i++){
        cout << "x1 " << slice_clb[i].slice_x1 << " x2 " << slice_clb[i].slice_x2 <<
                " y1 " << slice_clb[i].slice_y1 << " y2 " << slice_clb[i].slice_y2 << endl;
    }

    cout << "inside sort output bram 18" << endl;
    for (i = 0; i < from_solver->num_partition; i++){
        cout << "x1 " << slice18_bram[i].slice_x1 << " x2 " << slice18_bram[i].slice_x2 <<
                " y1 " << slice18_bram[i].slice_y1 << " y2 " << slice18_bram[i].slice_y2 << endl;
    }

    cout << "inside sort output bram 36" << endl;
    for (i = 0; i < from_solver->num_partition; i++){
        cout << "x1 " << slice36_bram[i].slice_x1 << " x2 " << slice36_bram[i].slice_x2 <<
                " y1 " << slice36_bram[i].slice_y1 << " y2 " << slice36_bram[i].slice_y2 << endl;
    }

    cout << "inside sort output dsp " << endl;
    for (i = 0; i < from_solver->num_partition; i++){
        cout << "x1 " << slice_dsp[i].slice_x1 << " x2 " << slice_dsp[i].slice_x2 <<
                " y1 " << slice_dsp[i].slice_y1 << " y2 " << slice_dsp[i].slice_y2 << endl;
    }

    write_xdc.open("/usr/local/src/floorplan/simulator/pt_fp/fplan.xdc");
    write_xdc<< "# User Generated miscellaneous constraints" << endl <<endl <<endl;

    for(i = 0; i < from_solver->num_partition; i++){
        write_xdc << "set_property HD.RECONFIGURABLE true [get_cells "<< slot_names[i] <<"]" <<endl;
        write_xdc <<"create_pblock pblock_"<<slot_names[i] <<endl;
        write_xdc <<"add_cells_to_pblock [get_pblocks pblock_"<<slot_names[i] <<"] [get_cells -quiet [list "<<slot_names[i] <<"]]" <<endl;

        write_xdc << "resize_pblock [get_pblocks pblock_"<<slot_names[i]<< "] -add {SLICE_X" <<slice_clb[i].slice_x1<<"Y" <<
                     slice_clb[i].slice_y1 <<":" <<"SLICE_X"<<slice_clb[i].slice_x2<<"Y"<<slice_clb[i].slice_y2 << "}" <<endl;

        write_xdc << "resize_pblock [get_pblocks pblock_"<<slot_names[i]<< "] -add {RAMB18_X" <<slice18_bram[i].slice_x1<<"Y" <<
                     slice18_bram[i].slice_y1 <<":" <<"RAMB18_X"<<slice18_bram[i].slice_x2<<"Y"<<slice18_bram[i].slice_y2 << "}" <<endl;

        write_xdc << "resize_pblock [get_pblocks pblock_"<<slot_names[i]<< "] -add {RAMB36_X" <<slice36_bram[i].slice_x1<<"Y" <<
                    slice36_bram[i].slice_y1 <<":" <<"RAMB36_X"<<slice36_bram[i].slice_x2<<"Y"<<slice36_bram[i].slice_y2 << "}" <<endl;

        write_xdc << "resize_pblock [get_pblocks pblock_"<<slot_names[i]<< "] -add {DSP48_X" <<slice_dsp[i].slice_x1<<"Y" <<
                    slice_dsp[i].slice_y1 <<":" <<"DSP48_X"<<slice_dsp[i].slice_x2<<"Y"<<slice_dsp[i].slice_y2 << "}" <<endl;
        write_xdc << "set_property RESET_AFTER_RECONFIG true [get_pblocks pblock_"<< slot_names[i] <<"]" <<endl;
        write_xdc << "set_property SNAPPING_MODE ON [get_pblocks pblock_"<< slot_names[i] <<"]" <<endl;

        write_xdc <<endl <<endl;
    }

    write_xdc.close();

}

void generate_fp_zynq::sort_output(param_from_solver *param, int res_type, vector <slice_addres> *output_vec)
{
    unsigned long i, k;
    unsigned long  flag = 0;
    unsigned long  x, y, w, h;
    unsigned long  col;

    // sort the slices
    for(i = 0; i < param->num_partition; i++){
        x = (*param->x)[i];
        y = (*param->y)[i];
        w = (*param->w)[i];
        h = (*param->h)[i];

        k = x;
        flag = 0;

        while(flag == 0){
            if(k < x + w){
                if((zynq_fg->zynq_fg[k].type_of_res) == res_type){
                    (*output_vec)[i].slice_x1 = zynq_fg->zynq_fg[k].slice_1;
                    flag = 1;
                }
                else
                    k += 1;
            }
            else
                flag = 1;
        }

        flag = 0;
        k = x + w - 1;

        while(flag == 0){
            if (k >= x) {
                if(zynq_fg->zynq_fg[k].type_of_res == res_type){
                    (*output_vec)[i].slice_x2 = zynq_fg->zynq_fg[k].slice_2;
                    flag = 1;
                }
                else
                    k -= 1;
            }
            else
                flag = 1;
        }

        if(res_type == CLB)
            col = zynq->clb_per_col;
        else if (res_type == BRAM)
            col = zynq->bram_per_col * 2;
        else if(res_type == BRAM36)
            col = zynq->bram_per_col;
        else
            col = zynq->dsp_per_col;

       (*output_vec)[i].slice_y1 = y /10 * col;
       (*output_vec)[i].slice_y2 = (((y + h)/10) * col) - 1;
    }
}
