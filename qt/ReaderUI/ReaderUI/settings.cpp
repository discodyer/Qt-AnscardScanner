#include "settings.h"
#include "ui_settings.h"

#define SET_IF_ARRYLEN 10

extern bool setIf[SET_IF_ARRYLEN];
extern bool tmpSetIf[SET_IF_ARRYLEN];


Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    //init settings
    ui->cbImshowGray->setChecked(tmpSetIf[0]);
    ui->cbImshowGrayBlur->setChecked(tmpSetIf[1]);
    ui->cbImshowEdged->setChecked(tmpSetIf[2]);

}

Settings::~Settings()
{
    delete ui;
}


void Settings::on_btnApply_clicked()
{
    for(int i=0;i<SET_IF_ARRYLEN;i++){
        setIf[i]=tmpSetIf[i];
    }
}


void Settings::on_cbImshowGray_clicked(bool checked)
{
    tmpSetIf[0]=checked;
}


void Settings::on_cbImshowGrayBlur_clicked(bool checked)
{
    tmpSetIf[1]=checked;
}


void Settings::on_cbImshowEdged_clicked(bool checked)
{
    tmpSetIf[2]=checked;
}


void Settings::on_cbImshowAllContours_clicked(bool checked)
{
    tmpSetIf[3]=checked;
}


void Settings::on_cbImshowOutLineCnts_clicked(bool checked)
{
    tmpSetIf[4]=checked;
}


void Settings::on_cbImshowFourLines_clicked(bool checked)
{
    tmpSetIf[5]=checked;
}


void Settings::on_cbImshowFindFourPoints_clicked(bool checked)
{
    tmpSetIf[6]=checked;
}


void Settings::on_cbImshowFourPointThresh_clicked(bool checked)
{
    tmpSetIf[7]=checked;
}


void Settings::on_cbImshowCnts_clicked(bool checked)
{
    tmpSetIf[8]=checked;
}


void Settings::on_cbImshowMask_clicked(bool checked)
{
    tmpSetIf[9]=checked;
}

