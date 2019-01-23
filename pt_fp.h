#ifndef PT_FP_H
#define PT_FP_H

#include <QDialog>
#include "ui_fp.h"

namespace Ui {
class pt_fp;
}

class pt_fp : public QDialog
{
    Q_OBJECT

public:
    explicit pt_fp(QWidget *parent = 0);
    ~pt_fp();

private:
    Ui::fp *ui;
};

#endif // PT_FP_H
