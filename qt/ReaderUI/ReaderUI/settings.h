#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();


private slots:
    void on_btnApply_clicked();

    void on_cbImshowGray_clicked(bool checked);

    void on_cbImshowGrayBlur_clicked(bool checked);

    void on_cbImshowEdged_clicked(bool checked);

    void on_cbImshowAllContours_clicked(bool checked);

    void on_cbImshowOutLineCnts_clicked(bool checked);

    void on_cbImshowFourLines_clicked(bool checked);

    void on_cbImshowFindFourPoints_clicked(bool checked);

    void on_cbImshowFourPointThresh_clicked(bool checked);

    void on_cbImshowCnts_clicked(bool checked);

    void on_cbImshowMask_clicked(bool checked);

private:
    Ui::Settings *ui;


};

#endif // SETTINGS_H
