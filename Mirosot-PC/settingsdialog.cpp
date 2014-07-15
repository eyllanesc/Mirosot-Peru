/****************************************************************/
/*Author:                                                       */
/*	Edwin Christian Yllanes Cucho   <e.yllanescucho@ieee.org>   */
/*                                  <e.yllanescucho@gmail.com>  */
/****************************************************************/

#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :QDialog(parent)
{
    setupUi(this);

    intValidator = new QIntValidator(0, 4000000, this);

    baudRateBox->setInsertPolicy(QComboBox::NoInsert);

    connect(serialPortInfoListBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(showPortInfo(int)));
    connect(baudRateBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(checkCustomBaudRatePolicy(int)));

    fillPortsParameters();
    fillPortsInfo();

    updateSettings();
}

Settings SettingsDialog::settings() const
{
    return currentSettings;
}

void SettingsDialog::showPortInfo(int idx)
{
    if (idx != -1) {
        QStringList list = serialPortInfoListBox->itemData(idx).toStringList();
        descriptionLabel->setText(tr("Description: %1").arg(list.at(1)));
        manufacturerLabel->setText(tr("Manufacturer: %1").arg(list.at(2)));
        serialNumberLabel->setText(tr("Serial number: %1").arg(list.at(3)));
        locationLabel->setText(tr("Location: %1").arg(list.at(4)));
        vidLabel->setText(tr("Vendor Identifier: %1").arg(list.at(5)));
        pidLabel->setText(tr("Product Identifier: %1").arg(list.at(6)));
    }
}

void SettingsDialog::apply()
{
    updateSettings();
    hide();
}

void SettingsDialog::checkCustomBaudRatePolicy(int idx)
{
    bool isCustomBaudRate = !baudRateBox->itemData(idx).isValid();
    baudRateBox->setEditable(isCustomBaudRate);
    if (isCustomBaudRate) {
        baudRateBox->clearEditText();
        QLineEdit *edit = baudRateBox->lineEdit();
        edit->setValidator(intValidator);
    }
}

void SettingsDialog::fillPortsParameters()
{
    baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    baudRateBox->addItem(QStringLiteral("Custom"));

    dataBitsBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    dataBitsBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    dataBitsBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    dataBitsBox->setCurrentIndex(3);

    parityBox->addItem(QStringLiteral("None"), QSerialPort::NoParity);
    parityBox->addItem(QStringLiteral("Even"), QSerialPort::EvenParity);
    parityBox->addItem(QStringLiteral("Odd"), QSerialPort::OddParity);
    parityBox->addItem(QStringLiteral("Mark"), QSerialPort::MarkParity);
    parityBox->addItem(QStringLiteral("Space"), QSerialPort::SpaceParity);

    stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    stopBitsBox->addItem(QStringLiteral("1.5"), QSerialPort::OneAndHalfStop);
#endif
    stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    flowControlBox->addItem(QStringLiteral("None"), QSerialPort::NoFlowControl);
    flowControlBox->addItem(QStringLiteral("RTS/CTS"), QSerialPort::HardwareControl);
    flowControlBox->addItem(QStringLiteral("XON/XOFF"), QSerialPort::SoftwareControl);
}

void SettingsDialog::fillPortsInfo()
{
    serialPortInfoListBox->clear();
    static const QString blankString = QObject::tr("N/A");
    QString description;
    QString manufacturer;
    QString serialNumber;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

        serialPortInfoListBox->addItem(list.first(), list);
    }
}

void SettingsDialog::updateSettings()
{
    currentSettings.name = serialPortInfoListBox->currentText();

    if (baudRateBox->currentIndex() == 4) {
        currentSettings.baudRate = baudRateBox->currentText().toInt();
    } else {
        currentSettings.baudRate = static_cast<QSerialPort::BaudRate>(
                    baudRateBox->itemData(baudRateBox->currentIndex()).toInt());
    }
    currentSettings.stringBaudRate = QString::number(currentSettings.baudRate);

    currentSettings.dataBits = static_cast<QSerialPort::DataBits>(
                dataBitsBox->itemData(dataBitsBox->currentIndex()).toInt());
    currentSettings.stringDataBits = dataBitsBox->currentText();

    currentSettings.parity = static_cast<QSerialPort::Parity>(
                parityBox->itemData(parityBox->currentIndex()).toInt());
    currentSettings.stringParity = parityBox->currentText();

    currentSettings.stopBits = static_cast<QSerialPort::StopBits>(
                stopBitsBox->itemData(stopBitsBox->currentIndex()).toInt());
    currentSettings.stringStopBits = stopBitsBox->currentText();

    currentSettings.flowControl = static_cast<QSerialPort::FlowControl>(
                flowControlBox->itemData(flowControlBox->currentIndex()).toInt());
    currentSettings.stringFlowControl = flowControlBox->currentText();

    currentSettings.localEchoEnabled = localEchoCheckBox->isChecked();
}
