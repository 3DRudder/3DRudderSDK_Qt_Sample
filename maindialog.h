#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QtCore>



namespace Ui {
class MainDialog;
}

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();

    void UpdateCombo();

    QTimer *pTimer;

private slots:
        void timer();


        void on_comboBox_activated(int index);

        void on_pushButtonPLaySound_clicked();

private:
    Ui::MainDialog *ui;
    bool m_bConnected;
	int m_nNb3DRudderConnected;
};
#endif // MAINDIALOG_H
