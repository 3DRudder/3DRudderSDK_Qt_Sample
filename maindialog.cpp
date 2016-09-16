#include "maindialog.h"
#include "ui_maindialog.h"
#include <QMessageBox>

#define _3DRUDDER_SDK_STATIC
#include "3DRudderSDK.h"


MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);
    setWindowTitle("3DRudder SDK Sample");

    pTimer = new QTimer(this);
    connect(pTimer, SIGNAL(timeout()), this, SLOT(timer()));

    pTimer->start(1000 / 30);

    ns3DRudder::CSdk* pSdk=ns3DRudder::GetSDK();

    if(pSdk->GetSDKVersion()!=_3DRUDDER_SDK_VERSION)
    {
        QMessageBox msgBox;
        msgBox.setText("Wrong 3DRudder SDK Version Validation");
        msgBox.exec();
    }

    m_bConnected=true;
    UpdateCombo();
}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::UpdateCombo()
{
    ns3DRudder::CSdk* pSdk=ns3DRudder::GetSDK();
	QString sSelected=ui->comboBox->currentText();

	ui->comboBox->clear();

	for(int i=0;i<4;i++)
    {
        if(pSdk->IsDeviceConnected(i))
            ui->comboBox->addItem(QString::asprintf("3DRudder %d",i),QVariant(i));
    }
	ui->comboBox->setCurrentText(sSelected);
	m_nNb3DRudderConnected = pSdk->GetNumberOfConnectedDevice();
}


void MainDialog::timer()
{
    ns3DRudder::CSdk* pSdk=ns3DRudder::GetSDK();
	if (m_nNb3DRudderConnected != pSdk->GetNumberOfConnectedDevice())
		UpdateCombo();

    if(pSdk->IsDeviceConnected(ui->comboBox->currentData().toInt()))
    {
        unsigned short nVersion = pSdk->GetFirmwareVersion(ui->comboBox->currentData().toInt());
        if(nVersion!=0xffff)
            ui->FwVersion->setText(QString::asprintf("FwVersion :%04x",nVersion));
        else
            ui->FwVersion->setText("Fw Version : Error");

        ns3DRudder::State state;
        if (pSdk->Get3DRudderState(ui->comboBox->currentData().toInt(), &state) == ns3DRudder::Success)
        {
            // Display joystick state to dialog
            ui->Roll->setText(QString::asprintf("%d", state.aX));
            ui->Pitch->setText(QString::asprintf("%d", (state.aY*(-1))));
            ui->UpDown->setText(QString::asprintf("%d", state.aZ));
            ui->Yaw->setText(QString::asprintf("%d", state.rZ));

            switch (state.status)
            {
                case  ns3DRudder::State::NoFootStayStill:
                    ui->Status->setText("Status : Don't put your Feet !!! Stay still 5s");
                break;
                case ns3DRudder::State::Initialisation:
                    ui->Status->setText("Status : Initialisation");
                break;
                case ns3DRudder::State::PutYourFeet:
                    ui->Status->setText("Status : Please put your feet");
                break;
                case ns3DRudder::State::PutSecondFoot:
                    ui->Status->setText("Status :  Put your second foot");
                break;
                case ns3DRudder::State::StayStill:
                    ui->Status->setText("Status : Stay still");
                break;
                case ns3DRudder::State::InUse:
                    ui->Status->setText("Status : 3DRudder in use");
                break;
                case ns3DRudder::State::ExtendedMode:
                    ui->Status->setText("Status : Extended functions activated");
                break;
            }

            ui->Sensor1->setText(QString::asprintf("%ld", state.s0));
            ui->Sensor2->setText(QString::asprintf("%ld", state.s1));
            ui->Sensor3->setText(QString::asprintf("%ld", state.s2));
            ui->Sensor4->setText(QString::asprintf("%ld", state.s3));
            ui->Sensor5->setText(QString::asprintf("%ld", state.s4));
            ui->Sensor6->setText(QString::asprintf("%ld", state.s5));

        }
        if(!m_bConnected)
        {
            UpdateCombo();
        }
        m_bConnected=true;
    }
    else
    {
        if(m_bConnected)
        {
            UpdateCombo();
            ui->FwVersion->setText("");
            ui->Roll->setText("");
            ui->Pitch->setText("");
            ui->UpDown->setText("");
            ui->Yaw->setText("");
            ui->Status->setText("Status : Not connected");
            ui->Sensor1->setText("");
            ui->Sensor2->setText("");
            ui->Sensor3->setText("");
            ui->Sensor4->setText("");
            ui->Sensor5->setText("");
            ui->Sensor6->setText("");
        }
        m_bConnected=false;
    }
}



void MainDialog::on_comboBox_activated(int index)
{
    UpdateCombo();
	ui->comboBox->setCurrentIndex(index);
}

void MainDialog::on_pushButtonPLaySound_clicked()
{
    ns3DRudder::CSdk* pSdk=ns3DRudder::GetSDK();
    pSdk->PlaySnd(ui->comboBox->currentData().toInt(),440,1000);
}
