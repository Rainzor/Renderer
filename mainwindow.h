//
// Created by Runze on 30/06/2023.
//
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QScrollArea>
#include <QPixmap>
#include <QResizeEvent>
#include <QFrame>
#include <QCheckBox>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QProgressBar>
#include "RenderEngine.h"
//#include "RayTracer.h"
//#include "Scene.h"
class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    void createRendererUI(QVBoxLayout *leftLayout);

    virtual ~MainWindow();

protected:
    void clearOutput();

    void renderInBackground(const std::string &filename);

    void resizeEvent(QResizeEvent* event) override;

    void updateImageLabel();


private slots:

    void startRendering();

    void toggleOpenMP(bool checked);

    void updateRenderTime(qint64 elapsedTime);

    void handleProgressUpdate(int progress);

signals:
    void renderTimeUpdated(qint64 elapsedTime);
    void progressUpdated(int progress);

private:
    QVBoxLayout *mainLayout;
    QHBoxLayout *sceneLayout;
    QHBoxLayout *samplesLayout;
    QHBoxLayout *methodLayout;
    QHBoxLayout *buttonLayout;
    QLabel *sceneLabel;
    QLabel *samplesLabel;
    QLabel *methodLabel;
    QComboBox *sceneComboBox;
    QSpinBox *samplesSpinBox;
    QComboBox *methodComboBox;
    QPushButton *renderButton;
    QPushButton *clearOutputButton;
    QProgressBar *progressBar;
    QTextEdit *outputTextEdit;
    QLabel *imageLabel;
    QPixmap originalPixmap;
private:
    bool useOpenMP;
    bool renderingInProgress;
    RenderEngine myRender;
};

#endif
