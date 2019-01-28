#include "pt_fp.h"
#include "ui_fp.h"


#include <QString>
#include <QDebug>
#include <QFileDialog>
#include <iostream>
#include <string>

using namespace std;

pt_fp::pt_fp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fp)
{
    ui->setupUi(this);

    unsigned long i;
    zynq = new fpga_zynq();
    //virt = new virtex();
    //virt_5 = new virtex_5();

    generate_xdc = new generate_fp_zynq(zynq);

    init_gui();

    //scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(&scene);

    //fp::brush_background.setColor(Qt::black);
    //paint_zynq();

    //paint_virtex();                                                                                                                                                                                                                                                                  paint_virtex();
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setRowCount(10);

    connect(ui->set_button, SIGNAL(released()), this, SLOT(set_pressed()));
    connect(ui->FPGA, SIGNAL(released()), this, SLOT(fpga_pressed()));
    connect(ui->enter_button, SIGNAL(released()),this, SLOT(enter_pressed()));
    connect(ui->start_fp, SIGNAL(released()), this, SLOT(start_pressed()));
    connect(ui->browse_button, SIGNAL(released()), this, SLOT(set_browse()));
    connect(ui->set_util, SIGNAL(released()), this, SLOT(set_util()));
/*
    for(i = 0; i < virt->num_forbidden_slots; i++)
        forbidden_region_virtex[i] = virt->forbidden_pos[i];

    for(i = 0; i < virt_5->num_forbidden_slots; i++)
        forbidden_region_virtex_5[i] = virt_5->forbidden_pos[i];
*/
    for(i = 0; i < zynq->num_forbidden_slots; i++) {
        forbidden_region_zynq[i] = zynq->fbdn_pos[i];
        //cout<< " fbdn" << forbidden_region_zynq[i].x << endl;
    }
}

void pt_fp::init_gui()
{
    ui->comboBox->addItem("Zynq");
    ui->comboBox->addItem("Virtex");
    ui->comboBox->addItem("virtex_5");
}

void pt_fp::paint_zynq()
{
    int i, j, k;
    int clk_reg_width, clk_reg_height;

    int pos_ptr_x, pos_ptr_y, bram_pos_ptr, dsp_pos_ptr, forbdn_ptr;
    int brams = 0, dsps = 0, forbdns;
    int bound, rec_height;
    bool is_bram = false, is_dsp = false, is_forbidden = false;

    QPen clk_reg_pen, rec_pen;
    QBrush br(Qt::black);

    pt_fp::clb_height = 8;
    pt_fp::bram_height = 40;
    pt_fp::dsp_height = 20;

    Qt::GlobalColor colors [] = {Qt::GlobalColor::red, Qt::GlobalColor::magenta,
                                 Qt::GlobalColor::blue, Qt::GlobalColor::cyan,
                                 Qt::GlobalColor::yellow};

    total_height = zynq->num_rows * bram_height * zynq->num_clk_reg / 2;

    for(i = 0; i < zynq->num_clk_reg ; i++) {
        clk_reg_width = zynq->clk_reg[i].clk_reg_pos.w * clb_width;
        clk_reg_height = zynq->clk_reg[i].clb_per_column * clb_height;

        bram_pos_ptr =  0; //zynq.clk_reg[i].bram_num;
        dsp_pos_ptr = 0;
        forbdn_ptr = 0;

        pos_ptr_x = zynq->clk_reg[i].clk_reg_pos.x;
        pos_ptr_y = zynq->clk_reg[i].clk_reg_pos.y;
        brams = zynq->clk_reg[i].bram_num;
        dsps = zynq->clk_reg[i].dsp_num;
        forbdns = zynq->clk_reg[i].forbidden_num;

        if(brams > 0)
            is_bram = true;

        if(dsps > 0)
            is_dsp = true;

        if(forbdns > 0)
            is_forbidden = true;

        QPen clk_reg_pen(colors[i]);
        clk_reg_pen.setWidth(2);

        fp_rect = scene.addRect(zynq->clk_reg[i].clk_reg_pos.x * clb_width,
                zynq->clk_reg[i].clk_reg_pos.y * clb_height,
                clk_reg_width, clk_reg_height, clk_reg_pen, brush);

        for(j = 0; j < zynq->clk_reg[i].clk_reg_pos.w; j++) {
            rec_height = clb_height;
            bound = zynq->clk_reg[i].clb_per_column;
            rec_pen.setColor(Qt::blue);
            rec_pen.setWidth(1);

            if(is_bram) {
                if(pos_ptr_x + j == zynq->clk_reg[i].bram_pos[bram_pos_ptr]) {
                    rec_pen.setColor(Qt::red);
                    bound = zynq->clk_reg[i].bram_per_column;
                    rec_height = bram_height;

                if(brams - 1 !=  0)
                    bram_pos_ptr += 1;
                else
                    is_bram = false;
                }
            }

            if(is_dsp) {
                if(pos_ptr_x + j == zynq->clk_reg[i].dsp_pos[dsp_pos_ptr]) {
                    rec_pen.setColor(Qt::green);
                    bound = zynq->clk_reg[i].dsp_per_column;
                    rec_height = dsp_height;

                if(dsps - 1 !=  0)
                    dsp_pos_ptr += 1;
                else
                    is_dsp = false;
                }
            }

            for(k = 0; k < bound; k++) {
                fp_rect = scene.addRect(pos_ptr_x * clb_width +  j * clb_width,
                                         pos_ptr_y * clb_height + k * rec_height,
                                        clb_width, rec_height, rec_pen, brush);
            }
        }
    }


    for(i = 0; i < zynq->num_forbidden_slots; i++) {
        //rec_pen.setColor(Qt::black);
        fp_rect = scene.addRect((zynq->fbdn_pos[i].x * clb_width),
                                (zynq->fbdn_pos[i].y * clb_height) ,
                                (zynq->fbdn_pos[i].w * clb_width) ,
                                (zynq->fbdn_pos[i].h * clb_height), rec_pen, br);

    }
}
void pt_fp::fpga_pressed()
{
    if(ui->comboBox->currentText() == "Zynq") {
        type = ZYNQ;
        pt_fp::clb_height = 8;
        pt_fp::bram_height = 40;
        pt_fp::dsp_height = 20;
        scene.clear();
        paint_zynq();

    }
    /*
    else if(ui->comboBox->currentText() == "Virtex") {
        type = VIRTEX;
        pt_fp::clb_height = 6;
        pt_fp::bram_height = 30;
        pt_fp::dsp_height = 15;
        scene.clear();
        qDebug() << "selected virtex" << endl;
        paint_virtex();
    }

    else if(ui->comboBox->currentText() == "virtex_5") {
        type = VIRTEX_5;
        pt_fp::clb_height = 8;
        pt_fp::bram_height = 40;
        pt_fp::dsp_height = 20;
        scene.clear();
        qDebug() << "selected virtex 5" << endl;
        paint_virtex_5();
    }
*/
}

void pt_fp::set_pressed()
{
     unsigned long int i;

     str = ui->num_slotsLineEdit_2->text();
     num_rm_modules = (unsigned long) str.toInt();

     //TODO: permananely replace num_slots with num_rm_modules
     num_slots = (unsigned long) str.toInt();
     ui->comboBox_2->clear();

     for(i = 0; i < num_rm_modules; i++) {
        ui->comboBox_2->addItem(QString::number(i));
     }

     if(num_rm_modules > 0) {
         clb_vector.clear();
         bram_vector.clear();
         dsp_vector.clear();

         eng_x.clear();
         eng_y.clear();
         eng_w.clear();
         eng_h.clear();

         x_vector.clear();
         y_vector.clear();
         w_vector.clear();
         h_vector.clear();

         platform = new Platform(3);
         task_set = new Taskset(num_rm_modules, num_rm_modules, *platform);
     }

     scene.clear();
     if(type == ZYNQ){
        paint_zynq();
        platform->maxFPGAResources[CLB]  = ZYNQ_CLB_TOT;
        platform->maxFPGAResources[BRAM] = ZYNQ_BRAM_TOT;
        platform->maxFPGAResources[DSP]  = ZYNQ_DSP_TOT;

        platform->recTimePerUnit[CLB] = 1.0/4500.0;
        platform->recTimePerUnit[BRAM] = 1.0/4500.0;
        platform->recTimePerUnit[DSP] = 1.0/4000.0;
     }
/*
     else if(type == VIRTEX)
        paint_virtex();

     else if(type == VIRTEX_5)
         paint_virtex_5();
*/
}

void pt_fp::enter_pressed()
{
    int clb = 0, bram = 0 , dsp = 0;
    QString st, temp;
    unsigned int num;

    st = ui->comboBox_2->currentText();
    num = (unsigned int) st.toInt();

    temp = ui->cLBLineEdit->text();
    clb = temp.toInt();
    clb_vector[num] = clb;

    temp = ui->bRAMLineEdit->text();
    bram = temp.toInt();
    bram_vector[num] = bram;

    temp = ui->dSPLineEdit->text();
    dsp =  temp.toInt();
    dsp_vector[num] = dsp;
}

void pt_fp::start_pressed()
{
    unsigned long m, scale;
    unsigned int i;

    str = ui->utilLineEdit->text();
    utilization = str.toDouble();

    if(utilization == 0.0)
        utilization = 2.0;

    cout <<"utilization is " <<utilization <<endl;

    //send description of FPGA parameters and application parameters to optimizer
    param.bram = &bram_vector;
    param.clb  = &clb_vector;
    param.dsp  = &dsp_vector;
    param.num_rm_modules = num_rm_modules;
    param.num_connected_slots = connections;
    param.conn_vector = &connection_matrix;


    for(i = 0; i < num_rm_modules; i++){
        task_set->HW_Tasks[i].resDemand[CLB]  = clb_vector[i];
        task_set->HW_Tasks[i].resDemand[BRAM] = bram_vector[i];
        task_set->HW_Tasks[i].resDemand[DSP]  = dsp_vector[i];
        task_set->HW_Tasks[i].WCET = HW_WCET[i];
        task_set->SW_Tasks[i].H.push_back(i);
        task_set->HW_Tasks[i].SW_Task_ID = i;
    }


    double alpha = 2.0;
    //double tmp2[] = { 1.8, 1.8, 1.8, 1.8 };
    double tmp2[] = { alpha, alpha, alpha};
    std::vector<double> res_usage(tmp2, tmp2+3 );

//    for(i = 0; i < 55; i++) {
        //*task_set = pt_fp::generate_taskset_one_HW_task_per_SW_task(num_rm_modules, *platform, res_usage, 2, 0.25);
        task_set->print();

        //slacks = generate_slacks(*task_set, *platform, utilization);
        param.task_set = task_set;
        param.platform = platform;
        param.slacks = &slacks;

        scene.clear();
        if(type == ZYNQ)
        paint_zynq();
    /*
    else if(type == VIRTEX)
       paint_virtex();

    else if(type == VIRTEX_5)
       paint_virtex_5();
    */

    /*
    if(type == VIRTEX) {
        param.forbidden_slots = virt->num_forbidden_slots;
        param.num_rows = virt->num_rows;
        param.width = virt->width;
        param.fbdn_slot = &forbidden_region_virtex;

        virtex_start_optimizer(&param, &from_solver);
        scale = 3;
    }
*/
        if(type ==ZYNQ) {
            param.forbidden_slots = zynq->num_forbidden_slots;
            param.num_rows = zynq->num_rows;
            param.width = zynq->width;
            param.fbdn_slot = &forbidden_region_zynq;

            scale = 1;
            zynq_start_optimizer(&param, &from_solver);
        }
/*
    if(type ==VIRTEX_5) {
        param.forbidden_slots = virt_5->num_forbidden_slots;
        param.num_rows = virt_5->num_rows;
        param.width = virt_5->width;
        param.fbdn_slot = &forbidden_region_virtex_5;

         virtex_start_optimizer_v5(&param, &from_solver);
        scale = 2;
    }
*/
    //calibrate the data returned from optimizer for visualization
        plot_rects(&from_solver);

    //Plot slots
        QPen slot_pen(Qt::red);
        slot_pen.setWidth(4);

        for(m = 0; m < num_slots; m++) {
            scene.addRect(x_vector[m] / scale, y_vector[m] / scale, w_vector[m] / scale, h_vector[m] / scale, slot_pen, brush);
        }

        generate_xdc->generate_floor_plan(&from_solver);
   // }
}

void pt_fp::plot_rects(param_from_solver *fs)
{
    unsigned long int i;

    /*The information about the total height must be integral part of the FPGA description*/
    for(i = 0; i < num_slots; i++) {
        x_vector[i] = (*fs->x)[i] * clb_width;
        y_vector[i] = total_height - (((*fs->y)[i] + (*fs->h)[i]) * bram_height);
        h_vector[i] = (*fs->h)[i] * bram_height;
        w_vector[i] = (*fs->w)[i] * clb_width;
    }
}

void pt_fp::set_browse()
{
    int row, col;
    int i , k;
    int ptr;
    string str;

    file_path = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("CSV (*.csv)" ));
    CSVData csv_data(file_path.toStdString());

    row = csv_data.rows();
    col = csv_data.columns();

    qDebug() << " row " <<row << "col " << col << endl;
    cout <<"in browse" << endl;

    if(row >= num_slots) {
        for(i = 0, ptr = 0, k = 0; i < num_slots; i++, ptr++) {
            str = csv_data.get_value(i, k++);
            clb_vector[ptr] = std::stoi(str);

            str = csv_data.get_value(i, k++);
            bram_vector[ptr] = std::stoi(str);

            str = csv_data.get_value(i, k++);
            dsp_vector[ptr] = std::stoi(str);

            str = csv_data.get_value(i, k++);
            HW_WCET[ptr] = std::stoi(str);

            str = csv_data.get_value(i, k++);
            slacks[ptr] = std::stoi(str);
            k = 0;
            //cout << "clb " << clb_vector[ptr] << " bram " << bram_vector[ptr] << "dsp " << dsp_vector[ptr] << endl;
        }
    }

    if(row > num_slots) {
        for(i = num_slots + 1, ptr = 0; i < row; i++, ptr++){
            for(k = 0; k < 3; k++) {
                str = csv_data.get_value(i, k);
                connection_matrix[ptr][k] = std::stoi(str);
            }
        }
    }

    connections = row - num_slots - 1;

    for(i = num_slots + 1, ptr = 0, k = 0; i < row; i++, ptr++){
        for(k = 0; k < 3; k++) {
            cout << connection_matrix[ptr][k] << " " ;
        }
        cout << endl;
    }

    cout << "connectons " << connections <<endl;
}

void pt_fp::set_util()
{
    unsigned long i;
    unsigned long clb_tot, bram_tot, dsp_tot;
    unsigned long clb_min, bram_min, dsp_min;
    unsigned long clb_max, bram_max, dsp_max;
    unsigned long clb_mod, bram_mod, dsp_mod;

    QString str;
   // int temp_util = 0;
   // float util_temp = 0.0;

    vector <unsigned long> clb_vec (num_slots, 0);
    vector <unsigned long> bram_vec (num_slots, 0);
    vector <unsigned long> dsp_vec (num_slots, 0);

    //slot s1, s2, s3;
    //slot sl_array[num_slots];
     srand((unsigned)time(0));

     str = ui->utilLineEdit->text();
     utilization = str.toFloat();

     //qDebug() << endl << "util is" << utilization << endl;

     if(type == ZYNQ) {
         clb_tot  = ZYNQ_CLB_TOT;
         bram_tot = ZYNQ_BRAM_TOT;
         dsp_tot  = ZYNQ_DSP_TOT;
         clb_min  = ZYNQ_CLB_MIN;
         clb_max  = 400;
         bram_min = ZYNQ_BRAM_MIN;
         bram_max = 12;
         dsp_min  = ZYNQ_DSP_MIN;
         dsp_max  = 20;
         clb_mod = clb_tot * utilization;
         bram_mod = bram_tot * 0.1;
         dsp_mod = dsp_tot * 0.1;
     }
/*
     else if(type == VIRTEX) {
         clb_tot = VIRTEX_CLB_TOT;
         bram_tot = VIRTEX_BRAM_TOT;
         dsp_tot = VIRTEX_DSP_TOT;
         clb_min = VIRTEX_CLB_MIN;
         clb_max = 10000;
         bram_min = VIRTEX_BRAM_MIN;
         bram_max = 50;
         dsp_min = VIRTEX_DSP_MIN;
         dsp_max = 100;
         clb_mod = clb_tot * utilization;
         bram_mod = bram_tot * 0.1;
         dsp_mod = dsp_tot * 0.1;
     }
*/
 /*
     int mod_clb = (clb_tot * 2 * utilization) / num_slots;
     int mod_bram = (bram_tot * 1 * utilization) / num_slots;
     int mod_dsp = (dsp_tot * 1 * utilization) / num_slots;

     do{
         for(i = 0; i < num_slots; i++) {
             sl_array[i].clb = rand()%mod_clb;
         }
     }while(is_compatible(sl_array, num_slots, clb_tot, clb_min, CLB));
/*
     do{
         for(i = 0; i < num_slots; i++)
             sl_array[i].bram = rand()%mod_bram;//bram_tot;
     }while(is_compatible(sl_array, num_slots, bram_tot, bram_min, BRAM));

     do{
         for(i = 0; i < num_slots; i++)
             sl_array[i].dsp = rand()%mod_dsp; //dsp_tot;
     }while(is_compatible(sl_array, num_slots, dsp_tot, dsp_min, DSP));

     for(i = 0; i < num_slots; i++) {
         qDebug() << "in slot " << i << " " << sl_array[i].clb <<" " << sl_array[i].bram <<" "<< sl_array[i].dsp << endl;
         temp_util += sl_array[i].clb;
     }

    util_temp = (float) temp_util / clb_tot;
    qDebug() << "utilization " << util_temp << endl;
    */

    qDebug() << "clbs " <<clb_mod << " bram " << bram_mod << " dsp " << dsp_mod << endl;
    clb_vec = pt_fp::get_units_per_task(num_slots, clb_mod, clb_min, clb_max);
    //bram_vec = fp::get_units_per_task(num_slots, bram_mod , bram_min, bram_max);
    //dsp_vec = fp::get_units_per_task(num_slots, dsp_mod, dsp_min, dsp_max);

 //   for(i = 0; i < num_slots; i++) {
          //cout<< "clb" << i << " " <<clb_vec[i] << " bram" << i << " " << bram_vec[i] << " dsp" << i << " " <<dsp_vec[i] <<endl;
//          cout<<clb_vec[i] << "," << bram_vec[i] << ", " <<dsp_vec[i] <<endl;
 //   }
    if(num_slots != 0) {
        for(i = 0; i < num_slots; i++) {
            clb_vector[i] = clb_vec[i]; //sl_array[i].clb;
            bram_vector[i] = bram_vec[i]; //sl_array[i].bram;
            dsp_vector[i] =  dsp_vec[i]; //sl_array[i].dsp;
        }
    }
}

vector<unsigned long> pt_fp::get_units_per_task(unsigned long n, unsigned long n_units, unsigned long n_min, unsigned long n_max)
{
    vector<unsigned long> ret;
    double rand_dbl;

    uint n_units_sum = n_units, n_units_next=0;

    for(uint i=0; i < n-1; i++)
    {
        srand(time(0));
        rand_dbl = pow(MY_RAND(),(1.0 / (double)(n - i - 1)));
        n_units_next = floor((double)n_units_sum * rand_dbl);
        //cout << n_units_next << " " << rand_dbl << endl;
        // --------- LIMIT Task Utilization --------------
        if(n_units_next > (n_units_sum - n_min))
            n_units_next = n_units_sum - n_min;

        if(n_units_next < ((n - i - 1) * n_min))
            n_units_next = (n - i - 1) * n_min;

        if((n_units_sum - n_units_next) > n_max)
            n_units_next = n_units_sum - n_max;
        // ------------------------------------------------

        ret.push_back(n_units_sum - n_units_next);
        n_units_sum = n_units_next;
    }

    ret.push_back(n_units_sum);

    return ret;
}

vector <double> pt_fp::generate_slacks(Taskset& t, Platform& platform, double alpha)
{
    std::vector<double> maxSlotSizePerResource;

      for(uint x=0; x < platform.N_FPGA_RESOURCES; x++)
      {
          double tmp = 0;
          for(uint a=0; a < t.maxHW_Tasks; a++)
              tmp = max(tmp, t.HW_Tasks[a].resDemand[x]);

          maxSlotSizePerResource.push_back(tmp);
          //cout << "MAX_SIZE[" << x << "] = " << tmp << endl;
      }

      std::vector<double> SLACK_BOUND;

      double max_delay = 1;

      for(uint a=0; a < t.maxHW_Tasks; a++)
          for(uint x=0; x < platform.N_FPGA_RESOURCES; x++)
               max_delay += maxSlotSizePerResource[x]*platform.recTimePerUnit[x];

      for(uint b=0; b < t.maxHW_Tasks; b++)
          max_delay += t.HW_Tasks[b].WCET;

      //cout << "MAX DELAY = " << max_delay << endl;

      for(uint a=0; a < t.maxHW_Tasks; a++)
      {
          const double min_delay = t.HW_Tasks[a].WCET + alpha*(max_delay-t.HW_Tasks[a].WCET);
          SLACK_BOUND.push_back(min_delay + (max_delay-min_delay)*MY_RAND());
          cout <<"slack " <<a << " " <<SLACK_BOUND[a] <<endl;
      }

      return SLACK_BOUND;
}

vector<uint> get_units_per_task(uint n, uint n_units, uint n_min, uint n_max)
{
    vector<uint> ret;

    uint n_units_sum = n_units, n_units_next=0;

    for(uint i=0; i < n; i++)
    {

        n_units_next = floor((double)n_units_sum * pow(MY_RAND(),(1.0 / (double)(n - i - 1))));

        // --------- LIMIT Task Utilization --------------
        if(n_units_next>(n_units_sum-n_min))
            n_units_next = n_units_sum-n_min;

        if(n_units_next<((n-i-1)*n_min))
            n_units_next = (n-i-1)*n_min;

        if((n_units_sum - n_units_next)>n_max)
            n_units_next = n_units_sum-n_max;
        // ------------------------------------------------


        ret.push_back(n_units_sum-n_units_next);
        n_units_sum =  n_units_next;
    }

//    ret.push_back(n_units_sum);

    return ret;
}

vector<HW_Task_t> generate_HW_tasks
                  (uint n, Platform& p,
                   const vector<double>& res_usage,
                   double WCET_area_ratio,
                   double max_area_usage)
{
    const uint n_res = p.maxFPGAResources.size();
    uint temp[5] = { 10,10,0};

    vector<HW_Task_t> HW_Tasks;

        //cout <<"in generate HW task "<< p.maxFPGAResources.size() << " " << res_usage.size() <<endl;
    if(res_usage.size()!=n_res)
        return HW_Tasks;

    // Prepare task set
    for(uint i=0; i < n; i++)
    {
        HW_Task_t new_task(p);
        HW_Tasks.push_back(new_task);
    }

    // For each FPGA resource
    for(uint x=0; x < n_res; x++)
    {
        const uint n_units = floor(res_usage[x]*p.maxFPGAResources[x]);

        vector<uint> res_per_task = get_units_per_task(n, n_units, temp[x],
                                                       max_area_usage*p.maxFPGAResources[x]);

        cout << endl << endl << "pritning res reqmt " <<endl;
        for(uint i=0; i < n; i++){
            HW_Tasks[i].resDemand[x] = res_per_task[i];
            //cout << " Res"<<i<< " " <<res_per_task[i];
        }

        cout <<endl;
    }

    // For each HW-task
    for(uint i=0; i < n; i++)
    {
        uint acc = 0;
        for(uint x=0; x < n_res; x++)
            acc += (uint)HW_Tasks[i].resDemand[x];
        //FIXME
        //const uint lb = (uint)floor(acc/WCET_area_ratio*(1.0-VAR));
        //const uint ub = (uint)floor(acc/WCET_area_ratio*(1.0+VAR));
        const uint lb   = 5;
        const uint ub   = 500;
        HW_Tasks[i].WCET = lb + (ub-lb)*MY_RAND();

        cout <<"WCET " <<i <<  HW_Tasks[i].WCET <<endl;
    }

    return HW_Tasks;
}


Taskset pt_fp::generate_taskset_one_HW_task_per_SW_task(uint n, Platform& p,
                                                        const vector<double>& res_usage,
                                                        double WCET_area_ratio,
                                                        double max_area_usage)

{
    Taskset t(n, n, p);
    vector<HW_Task_t> HW_Tasks = generate_HW_tasks(n, p, res_usage, WCET_area_ratio, max_area_usage);

    if(HW_Tasks.size()!=n)
        return t;

    // Fill the task set with the HW-tasks

    for(uint i=0; i< n; i++){
        t.HW_Tasks[i] = HW_Tasks[i];
        // i=th HW-Task <---> i-th SW-Task
        t.SW_Tasks[i].H.push_back(i);
        t.HW_Tasks[i].SW_Task_ID = i;
    }

    cout <<"in generate HW task" <<endl;
    return t;
}

pt_fp::~pt_fp()
{
    delete ui;
}

