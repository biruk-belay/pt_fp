#include "pt_fp.h"
#include "ui_pt_fp.h"

pt_fp::pt_fp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fp)
{
    ui->setupUi(this);
}

pt_fp::~pt_fp()
{
    delete ui;
}
