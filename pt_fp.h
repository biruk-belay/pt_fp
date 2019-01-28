#ifndef PT_FP_H
#define PT_FP_H

#include <QDialog>
#include "ui_fp.h"
#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include <QTableWidget>
#include <QString>

//#include "gurobi_c++.h"
#include "zynq_model.h"
#include "partition.h"
#include "fpga.h"
#include "csv_data_manipulator.hpp"
#include "generate_fp.h"

namespace Ui {
class pt_fp;
}

enum fpga_type {
    ZYNQ,
    VIRTEX,
    VIRTEX_5
};

typedef std::vector<pos> position_vec;
typedef std::vector<std::vector<unsigned long>> vec_2d;
typedef struct{
    unsigned long clb;
    unsigned long bram;
    unsigned long dsp;
}slot;

#define MY_RAND() ((double)((double)rand()/(double)RAND_MAX))

class pt_fp : public QDialog
{
    Q_OBJECT

public:
    explicit pt_fp(QWidget *parent = 0);
    ~pt_fp();
    QGraphicsScene scene;
    QGraphicsRectItem *fp_rect;
    QBrush brush, brush_background;
    QTableWidget *table;
    QString file_path;

    fpga_zynq *zynq;
    //virtex *virt;
    //virtex_5 *virt_5;

    generate_fp_zynq *generate_xdc;

    Taskset *task_set;
    Platform *platform;
    vector<double> slacks = vector<double>(MAX_SLOTS);
    vector <double> HW_WCET = vector<double>(MAX_SLOTS);

    QString str;
    param_to_solver param;

    unsigned long num_slots = 0;
    unsigned int num_rm_modules = 0;
    bool paint_rect = false;
    enum fpga_type type = ZYNQ;
    unsigned long virtex_scale = 3;
    double utilization;
    unsigned long connections;

    std::vector<unsigned long> clb_vector =  std::vector<unsigned long>(MAX_SLOTS);
    std::vector<unsigned long> bram_vector = std::vector<unsigned long>(MAX_SLOTS);
    std::vector<unsigned long> dsp_vector =  std::vector<unsigned long>(MAX_SLOTS);

    std::vector<slot> sl_array = std::vector<slot>(MAX_SLOTS);

    vec_2d connection_matrix = std::vector<std::vector<unsigned long>> (MAX_SLOTS, std::vector<unsigned long> (MAX_SLOTS, 0));

    std::vector<unsigned long> eng_x =  std::vector<unsigned long>(MAX_SLOTS);
    std::vector<unsigned long> eng_y = std::vector<unsigned long>(MAX_SLOTS);
    std::vector<unsigned long> eng_w =  std::vector<unsigned long>(MAX_SLOTS);
    std::vector<unsigned long> eng_h =  std::vector<unsigned long>(MAX_SLOTS);

    std::vector<unsigned long> x_vector =  std::vector<unsigned long>(MAX_SLOTS);
    std::vector<unsigned long> y_vector = std::vector<unsigned long>(MAX_SLOTS);
    std::vector<unsigned long> w_vector =  std::vector<unsigned long>(MAX_SLOTS);
    std::vector<unsigned long> h_vector =  std::vector<unsigned long>(MAX_SLOTS);

    position_vec forbidden_region_zynq = position_vec(MAX_SLOTS);
    position_vec forbidden_region_virtex = position_vec(MAX_SLOTS);
    position_vec forbidden_region_virtex_5 = position_vec(MAX_SLOTS);
    std::vector<unsigned long> get_units_per_task(unsigned long n,
                                                  unsigned long n_units,
                                                  unsigned long n_min,
                                                  unsigned long n_max);

    param_from_solver from_solver = {0, &eng_x, &eng_y,
                                   &eng_w, &eng_h};

    unsigned long clb_width = 10;
    unsigned long clb_height = 8, bram_height = 40, dsp_height = 20;
    unsigned long total_height;

    void init_fpga(enum fpga_type);
    void init_gui();
    void plot_rects(param_from_solver *);
    void paint_zynq();
    void paint_virtex();
    void paint_virtex_5();
    bool is_compatible(std::vector<slot> ptr, unsigned long slot_num, int max, unsigned long min, int type);
    vector <double> generate_slacks(Taskset& t, Platform& platform, double alpha);
    Taskset generate_taskset_one_HW_task_per_SW_task
            (uint n, Platform& p,
            const vector<double>& res_usage,
            double WCET_area_ratio,
            double max_area_usage);


private:
    Ui::fp *ui;

private slots:
    void set_pressed();
    void enter_pressed();
    void start_pressed();
    void set_browse();
    void fpga_pressed();
    void set_util();
};

#endif // PT_FP_H
