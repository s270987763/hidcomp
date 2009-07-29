#include "stdafx.h"
#include "devicewidget.h"


const int LABEL_SPACING = 5;

DeviceWidget::DeviceWidget(QWidget *parent, HIDDevice *pDevice )
: QGroupBox(tr("Device Search Criteria"),parent)
, m_Logger( QCoreApplication::applicationName(), "DeviceWidget" )
, m_pDevice(pDevice)
{
    QVBoxLayout *mainlayout = new QVBoxLayout();
    setLayout( mainlayout );
    setSizePolicy( QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed) );

    QFormLayout *flayout = new QFormLayout();
    flayout->addRow( tr("Device:"), new QLabel("path") );
    mainlayout->addLayout( flayout );

    QHBoxLayout *layout = new QHBoxLayout();
    m_pchkPID = new QCheckBox(tr("PID:"));
    layout->addWidget( m_pchkPID );
    layout->addWidget( new QLabel(tr("%1").arg(FormatPID(pDevice->PID()))) );
    m_pchkVID = new QCheckBox(tr("VID:"));
    layout->addWidget( m_pchkVID );
    layout->addWidget( new QLabel(tr("%1").arg(FormatVID(pDevice->VID()))) );
    m_pchkManufacturer = new QCheckBox(tr("Manufacturer:"));
    layout->addWidget( m_pchkManufacturer );
    layout->addWidget( new QLabel(pDevice->Manufacturer()) );
    m_pchkProduct = new QCheckBox(tr("Product:"));
    layout->addWidget( m_pchkProduct );
    layout->addWidget( new QLabel(pDevice->Product()) );
    m_pchkSN = new QCheckBox(tr("S/N:"));
    layout->addWidget( m_pchkSN );
    layout->addWidget( new QLabel(pDevice->SerialNumber()) );
    m_pchkSystemId = new QCheckBox(tr("System Id:"));
    layout->addWidget( m_pchkSystemId  );
    layout->addWidget( new QLabel(pDevice->SystemId()) );

    QPushButton *button = new QPushButton( "Test Criteria" );
    layout->addWidget( button );
    connect( button, SIGNAL(clicked()), this, SLOT(onTestClicked()) );

    mainlayout->addLayout( layout );
}

DeviceWidget::~DeviceWidget()
{
}

void DeviceWidget::setCriteria( HIDDeviceCriteria *pCriteria )
{
    m_pchkPID->setCheckState( pCriteria->bPID ? Qt::Checked : Qt::Unchecked );
    m_pchkVID->setCheckState( pCriteria->bVID ? Qt::Checked : Qt::Unchecked );
    m_pchkManufacturer->setCheckState( pCriteria->bManufacturer ? Qt::Checked : Qt::Unchecked );
    m_pchkProduct->setCheckState( pCriteria->bProduct ? Qt::Checked : Qt::Unchecked );
    m_pchkSN->setCheckState( pCriteria->bSerialNumber ? Qt::Checked : Qt::Unchecked );
    m_pchkSystemId->setCheckState( pCriteria->bSystemId ? Qt::Checked : Qt::Unchecked );
}


void DeviceWidget::getCriteria( HIDDeviceCriteria *pCriteria )
{
    pCriteria->bPID = m_pchkPID->checkState() == Qt::Checked;
    pCriteria->bVID = m_pchkVID->checkState() == Qt::Checked;
    pCriteria->bManufacturer = m_pchkManufacturer->checkState() == Qt::Checked;
    pCriteria->bProduct = m_pchkProduct->checkState() == Qt::Checked;
    pCriteria->bSerialNumber = m_pchkSN->checkState() == Qt::Checked;
    pCriteria->bSystemId = m_pchkSystemId->checkState() == Qt::Checked;

    pCriteria->nPID = m_pDevice->PID();
    pCriteria->nVID = m_pDevice->VID();
    pCriteria->sManufacturer = m_pDevice->Manufacturer();
    pCriteria->sProduct = m_pDevice->Product();
    pCriteria->sSerialNumber = m_pDevice->SerialNumber();
    pCriteria->sSystemId = m_pDevice->SystemId();
}


void DeviceWidget::onTestClicked()
{
    HIDDeviceCriteria criteria;
    getCriteria( &criteria );

    unsigned short nPID = m_pDevice->PID();
    unsigned short nVID = m_pDevice->VID();
    QString sManufacturer = m_pDevice->Manufacturer();
    QString sProduct = m_pDevice->Product();
    QString sSerialNumber = m_pDevice->SerialNumber();
    QString sSystemId = m_pDevice->SystemId();

    HIDDevices hidDevices;
    std::vector<HIDDevice *> matches = hidDevices.SearchHIDDevices( criteria.bPID, nPID, criteria.bVID, nVID, criteria.bManufacturer, sManufacturer, criteria.bProduct, sProduct, criteria.bSerialNumber, sSerialNumber, criteria.bSystemId, sSystemId );
    QString sMatches;
    for ( int i = 0; i < (int)matches.size(); i++ )
    {
	sMatches += QString( tr("PID:%1 VID:%2 Manufacturer:'%3' Product:'%4' Serial No.:'%5' System Id:'%6'\n" ).arg(matches[i]->PID()).arg(matches[i]->VID()).arg(matches[i]->Manufacturer()).arg(matches[i]->Product()).arg(matches[i]->SerialNumber()).arg(matches[i]->SystemId()) );
    }
    QMessageBox msg(this);
    msg.setWindowTitle( tr("Test Match Criteria") );
    if ( matches.size() == 0 )
    {
	msg.setText( tr("Failed to match any device with the given criteria") );
	msg.setIcon( QMessageBox::Critical );
	msg.exec();
    }
    else if ( matches.size() == 1 )
    {
	msg.setText( tr("Matched one device...") );
	msg.setInformativeText( sMatches );
	msg.setIcon( QMessageBox::Information );
	msg.exec();
    }
    else
    {
	msg.setText( tr("Matched %1 devices...").arg(matches.size()) );
	msg.setInformativeText( sMatches );
	msg.setIcon( QMessageBox::Critical );
	msg.exec();
    }
}