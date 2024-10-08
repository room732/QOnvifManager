﻿#include <QDebug>
#include <QMessageBox>

#include "mainwindow.hpp"
#include "qonvifdevice.hpp"
#include "qonvifmanager.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ionvifManager = new QOnvifManager("admin", "", this);
    connect(
        ionvifManager,
        &QOnvifManager::newDeviceFinded,
        this,
        &MainWindow::onNewDeviceFinded);
    on_btnRefresh_clicked();
}

MainWindow::~MainWindow() {
    delete ionvifManager;
    delete ui;
}

void
MainWindow::onNewDeviceFinded(QOnvifDevice* _device) {
    ui->cmbDevicesComboBox->addItem(
        _device->data()
            .probeData.deviceServiceAddress.replace("http://", "")
            .replace("/onvif/device_service", ""),
        _device->data().probeData.endPointAddress);

    _device->loadDefaultPtzConfiguration();
}

void
MainWindow::on_btnRefresh_clicked() {
    ui->btnRefresh->setEnabled(false);
    ui->cmbDevicesComboBox->clear();
    connect(
        ionvifManager,
        &QOnvifManager::deviceSearchingEnded,
        this,
        [this]() { ui->btnRefresh->setEnabled(true); },
        Qt::UniqueConnection);
    ionvifManager->refreshDevicesList();
}

void
MainWindow::on_cmbDevicesComboBox_currentIndexChanged(int index) {
    Q_UNUSED(index)
    //    on_btnRefreshData_clicked();
}

void
MainWindow::on_btnRefreshData_clicked() {
    ui->txtLocation->setText("");
    ui->txtName->setText("");
    ionvifManager->refreshDeviceCapabilities(currentDevice());
    ionvifManager->refreshDeviceInformations(currentDevice());
    ionvifManager->refreshDeviceProfiles(currentDevice());
    ionvifManager->refreshDeviceImageSetting(currentDevice());
    ionvifManager->refreshDeviceImageSettingOptions(currentDevice());
    ionvifManager->refreshDeviceVideoConfigs(currentDevice());
    ionvifManager->refreshDeviceVideoConfigsOptions(currentDevice());
    ionvifManager->refreshDeviceStreamUris(currentDevice());

    ionvifManager->refreshDeviceUsers(currentDevice());
    ionvifManager->refreshDeviceScopes(currentDevice());
    ionvifManager->refreshDeviceInterfaces(currentDevice());
    ionvifManager->refreshDeviceDNS(currentDevice());
    ionvifManager->refreshDeviceDefaultGateway(currentDevice());
    ionvifManager->refreshDeviceDiscoveryMode(currentDevice());
    ionvifManager->refreshDeviceHostname(currentDevice());
    ionvifManager->refreshDeviceNTP(currentDevice());
    ionvifManager->refreshDevicePtzConfigs(currentDevice());

    // setScopes
    ui->txtLocation->setText(
        ionvifManager->device(currentDevice())->data().scopes.location);
    ui->txtName->setText(
        ionvifManager->device(currentDevice())->data().scopes.name);
    ui->txtHardware->setText(
        ionvifManager->device(currentDevice())->data().scopes.hardware);

    on_btnGetDataAndTime_clicked();

    auto device = ionvifManager->device(currentDevice());
    Q_UNUSED(device)

    // <- add a breake point here to see device informations in debuger.
}

void
MainWindow::on_actionAbout_triggered() {
    QMessageBox aboutMessageBox(this);
    aboutMessageBox.setText(
        "developed by: Mehrdad Shobeyri \nemail: mehrdad.shobeyri@yahoo.com");
    aboutMessageBox.exec();
}

void
MainWindow::on_btnGetDataAndTime_clicked() {
    Data::DateTime dateAndTime;
    ionvifManager->deviceDateAndTime(currentDevice(), dateAndTime);
    ui->dateTimeEditLocal->setDateTime(dateAndTime.localTime);
    ui->dateTimeEditUtc->setDateTime(dateAndTime.utcTime);
}

void
MainWindow::on_btsSetDateAndTime_clicked() {
    ionvifManager->setDeviceDateAndTime(
        currentDevice(),
        ui->dateTimeEditSet->dateTime(),
        "GMT-3:30",
        true,
        false);
}

QString
MainWindow::currentDevice() {
    return ui->cmbDevicesComboBox->currentData().toString();
}

void
MainWindow::on_setUsernamePass_clicked() {
    ionvifManager->setDefaulUsernameAndPassword(
        ui->txtUserName->text(), ui->txtPassword->text());
    on_btnRefresh_clicked();
}

void
MainWindow::on_btnSetScopes_clicked() {
    ionvifManager->setDeviceScopes(
        currentDevice(), ui->txtName->text(), ui->txtLocation->text());
}

void
MainWindow::on_btngoHome_clicked() {
    ionvifManager->device(currentDevice())->goHomePosition();
}

void
MainWindow::on_btnsetHome_clicked() {
    ionvifManager->device(currentDevice())->setHomePosition();
}

void
MainWindow::on_btnrefreshPresents_clicked() {
    ionvifManager->device(currentDevice())->refreshPresets();
}

void
MainWindow::on_btnRight_pressed() {
    ionvifManager->device(currentDevice())->continuousMove(0.5, 0, 0);
}

void MainWindow::on_btnLeft_pressed() {
    ionvifManager->device(currentDevice())->continuousMove(-0.5, 0, 0);
}


void MainWindow::on_actionStopMovement_triggered() {
    ionvifManager->device(currentDevice())->stopMovement();
}


void MainWindow::on_btnUp_pressed() {
    ionvifManager->device(currentDevice())->continuousMove(0, 0.5, 0);
}


void MainWindow::on_btnDown_pressed() {
    ionvifManager->device(currentDevice())->continuousMove(0, -0.5, 0);
}


void MainWindow::on_btnzoomIn_pressed() {
    ionvifManager->device(currentDevice())->continuousMove(0, 0, 0.5);
}


void MainWindow::on_btnzoomOut_pressed() {
    ionvifManager->device(currentDevice())->continuousMove(0, 0, -0.5);
}

