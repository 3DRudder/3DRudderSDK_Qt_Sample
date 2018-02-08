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

    ns3dRudder::CSdk* pSdk=ns3dRudder::GetSDK();
    pSdk->Init();

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
    ns3dRudder::CSdk* pSdk=ns3dRudder::GetSDK();
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
    ns3dRudder::CSdk* pSdk=ns3dRudder::GetSDK();
	if (m_nNb3DRudderConnected != pSdk->GetNumberOfConnectedDevice())
		UpdateCombo();

    uint    nPort=ui->comboBox->currentData().toInt();

    if(pSdk->IsDeviceConnected(nPort))
    {
        uint16_t nVersion = pSdk->GetVersion(nPort);
        if(nVersion!=0xffff)
            ui->FwVersion->setText(QString::asprintf("FwVersion :%04x",nVersion));
        else
            ui->FwVersion->setText("Fw Version : Error");

        ns3dRudder::Axis axis;
        if (pSdk->GetAxis(nPort,ns3dRudder::NormalizedValue, &axis) == ns3dRudder::Success)
        {
            ns3dRudder::Status status=pSdk->GetStatus(nPort);

            // Display joystick state to dialog
            ui->Roll->setText(QString::asprintf("%f", axis.GetXAxis()));
            ui->Pitch->setText(QString::asprintf("%f", (axis.GetYAxis())));
            ui->UpDown->setText(QString::asprintf("%f", axis.GetZAxis()));
            ui->Yaw->setText(QString::asprintf("%f", axis.GetZRotation()));

            switch (status)
            {
                case  ns3dRudder::NoFootStayStill:
                    ui->Status->setText("Status : Don't put your Feet !!! Stay still 5s");
                break;
                case ns3dRudder::Initialisation:
                    ui->Status->setText("Status : Initialisation");
                break;
                case ns3dRudder::PutYourFeet:
                    ui->Status->setText("Status : Please put your feet");
                break;
                case ns3dRudder::PutSecondFoot:
                    ui->Status->setText("Status :  Put your second foot");
                break;
                case ns3dRudder::StayStill:
                    ui->Status->setText("Status : Stay still");
                break;
                case ns3dRudder::InUse:
                    ui->Status->setText("Status : 3DRudder in use");
                break;
                case ns3dRudder::ExtendedMode:
                    ui->Status->setText("Status : Extended functions activated");
                break;
            }

            ui->Sensor1->setText(QString::asprintf("%ld", pSdk->GetSensor(nPort,0)));
            ui->Sensor2->setText(QString::asprintf("%ld", pSdk->GetSensor(nPort,1)));
            ui->Sensor3->setText(QString::asprintf("%ld", pSdk->GetSensor(nPort,2)));
            ui->Sensor4->setText(QString::asprintf("%ld", pSdk->GetSensor(nPort,3)));
            ui->Sensor5->setText(QString::asprintf("%ld", pSdk->GetSensor(nPort,4)));
            ui->Sensor6->setText(QString::asprintf("%ld", pSdk->GetSensor(nPort,5)));

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
    ns3dRudder::CSdk* pSdk= ns3dRudder::GetSDK();
    pSdk->PlaySnd(ui->comboBox->currentData().toInt(),440,1000);
}
