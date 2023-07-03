//
// Created by Runze on 30/06/2023.
//
#include "mainwindow.h"
//#include "RayTracer.h"
MainWindow::~MainWindow() {
}

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent) {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout *mainHorizontalLayout = new QHBoxLayout(centralWidget);

    QVBoxLayout *leftLayout = new QVBoxLayout();
//    leftLayout->setSizeConstraint(QLayout::SetFixedSize); // 设置左侧布局大小固定
    mainHorizontalLayout->addLayout(leftLayout, 0); // 设置左侧布局的 stretch 为 0
    createRendererUI(leftLayout);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    mainHorizontalLayout->addLayout(rightLayout, 1); // 设置右侧布局的 stretch 为 1
    imageLabel = new QLabel(this);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored); // 添加这行代码设置 imageLabel 的尺寸策略
    rightLayout->addWidget(imageLabel);

    setWindowTitle("Ray Tracer Renderer");
    resize(1200, 800); // 修改初始窗口大小
    setMinimumSize(300, 200);
    useOpenMP = true; // 默认使用 OpenMP
    renderingInProgress = false; // 默认渲染未开始
    // 连接信号和槽
    connect(this, SIGNAL(renderTimeUpdated(qint64)), this, SLOT(updateRenderTime(qint64)), Qt::QueuedConnection);
    connect(this, SIGNAL(progressUpdated(int)), this, SLOT(handleProgressUpdate(int)), Qt::QueuedConnection);
}

void MainWindow::createRendererUI(QVBoxLayout *leftLayout) {

    sceneLayout = new QHBoxLayout();
    sceneLabel = new QLabel("Scene:", this);
    sceneComboBox = new QComboBox(this);
    sceneComboBox->addItem("Cornell Box");
    sceneComboBox->addItem("Mirror");
//    sceneComboBox->addItem("Glass Ball");
    sceneComboBox->addItem("Glass");
    sceneComboBox->addItem("Smoke");
    sceneComboBox->addItem("Mitsuba");
    sceneComboBox->addItem("Zoom");
    sceneLayout->addWidget(sceneLabel);
    sceneLayout->addWidget(sceneComboBox);

    methodLayout = new QHBoxLayout();
    methodLabel = new QLabel("Method:", this);
    methodComboBox = new QComboBox(this);
    methodComboBox->addItem("BRDF");
    methodComboBox->addItem("Light");
    methodComboBox->addItem("Mixture");
    methodComboBox->addItem("NEE");
    methodComboBox->addItem("MIS");
    methodLayout->addWidget(methodLabel);
    methodLayout->addWidget(methodComboBox);

    samplesLayout = new QHBoxLayout();
    samplesLabel = new QLabel("Samples:", this);
    samplesSpinBox = new QSpinBox(this);
    samplesSpinBox->setMinimum(1);
    samplesSpinBox->setMaximum(1000);
    samplesSpinBox->setValue(8);
    samplesLayout->addWidget(samplesLabel);
    samplesLayout->addWidget(samplesSpinBox);

    // 获取三个 combobox 中最大的宽度
    int maxWidth = std::max({sceneComboBox->sizeHint().width(),
                             methodComboBox->sizeHint().width(),
                             samplesSpinBox->sizeHint().width()});

    // 设置每个 combobox 的最大宽度
    sceneComboBox->setMaximumWidth(maxWidth);
    methodComboBox->setMaximumWidth(maxWidth);
    samplesSpinBox->setMaximumWidth(maxWidth);
    sceneComboBox->setMinimumWidth(maxWidth);
    methodComboBox->setMinimumWidth(maxWidth);
    samplesSpinBox->setMinimumWidth(maxWidth);

    // 在复选框和渲染按钮之间添加进度条
    progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100); // 设置进度条的范围为0到100
    progressBar->setTextVisible(true); // 显示进度文本
    progressBar->setFormat("%v%"); // 设置进度文本格式
    progressBar->setValue(0); // 设置初始值为0


    renderButton = new QPushButton("Render", this);
    connect(renderButton, &QPushButton::clicked, this, &MainWindow::startRendering);

    clearOutputButton = new QPushButton("Clear Output", this);
    connect(clearOutputButton, &QPushButton::clicked, this, &MainWindow::clearOutput);

    outputTextEdit = new QTextEdit(this);
    outputTextEdit->setReadOnly(true);

    // 添加复选框并连接到 toggleOpenMP() 槽
    auto *openMPCheckbox = new QCheckBox("Use OpenMP", this);
    openMPCheckbox->setChecked(true); // 添加这一行，设置复选框默认选中
    connect(openMPCheckbox, &QCheckBox::toggled, this, &MainWindow::toggleOpenMP);

    // 将所有的布局添加到 leftLayout 中
    leftLayout->addLayout(sceneLayout);
    leftLayout->addLayout(methodLayout);
    leftLayout->addLayout(samplesLayout);
    leftLayout->addWidget(openMPCheckbox);
    leftLayout->addWidget(progressBar);
    leftLayout->addWidget(renderButton);
    leftLayout->addWidget(outputTextEdit);
    leftLayout->addWidget(clearOutputButton);
}

// Protected

void MainWindow::startRendering() {

    if (renderingInProgress) {
        return;
    }
    renderingInProgress = true;
    renderButton->setEnabled(false);


    int sceneChoice = sceneComboBox->currentIndex();
    int samples = samplesSpinBox->value();
    int methodChoice = methodComboBox->currentIndex();

    QString sceneName;
    QString methodNam;

    switch (sceneChoice) {
        case 0:
            sceneName = "Cornell Box";
            break;
        case 1:
            sceneName = "Mirror";
            break;
        case 2:
            sceneName = "Triangle and Glass Ball";
            break;
        case 3:
            sceneName = "Clouds";
            break;
        case 4:
            sceneName = "Mitsuba";
            break;
        case 5:
            sceneName = "Zoom";
            break;
        default:
            sceneName = "Cornell Box";
            break;
    }

    switch (methodChoice) {
        case 0:
            methodNam = "BRDF";
            break;
        case 1:
            methodNam = "Light";
            break;
        case 2:
            methodNam = "Mixture";
            break;
        case 3:
            methodNam = "NEE";
            break;
        case 4:
            methodNam = "MIS";
            break;
        default:
            methodNam = "BRDF";
            break;
    }


    // 将渲染结果显示在 outputTextEdit 中
    QString output = QString("Scene: %1\n%2 samples per pixel\n%3 method.").arg(sceneName).arg(samples).arg(
            methodNam);
    outputTextEdit->append(output);

    const char *filename = "../output/img.png";
    QFuture<void> future = QtConcurrent::run(this, &MainWindow::renderInBackground, std::string(filename));
    // 在渲染完成后重新启用按钮
    auto *watcher = new QFutureWatcher<void>();
    connect(watcher, &QFutureWatcher<void>::finished, this, [this]() {
        renderButton->setEnabled(true);
        renderingInProgress = false;
    });
    watcher->setFuture(future);
}

void MainWindow::renderInBackground(const std::string &filename) {
    int sceneChoice = sceneComboBox->currentIndex();
    int samples = samplesSpinBox->value();
    int methodChoice = methodComboBox->currentIndex();
    Scene scene;
    switch (sceneChoice) {
        case 0:
            cornell_box(scene);
            break;
        case 1:
            cornell_specular(scene);
            break;
        case 2:
            cornell_triangle_glass(scene);
            break;
        case 3:
            cornell_smoke(scene);
            break;
        case 4:
            cornell_mitsuba(scene);
            break;
        case 5:
            cornell_zoom(scene);
            break;
        default:
            cornell_box(scene);
            break;
    }

    SampleMethod sm;
    switch (methodChoice) {
        case 0:
            sm = SampleMethod::BRDF;
            break;
        case 1:
            sm = SampleMethod::Light;
            break;
        case 2:
            sm = SampleMethod::Mixture;
            break;
        case 3:
            sm = SampleMethod::NEE;
            break;
        case 4:
            sm = SampleMethod::MIS;
            break;
        default:
            sm = SampleMethod::BRDF;
            break;
    }

//    RayTracer myRender(scene);
    myRender.ChangeScene(scene);
    myRender.setProgressCallback([this](int progress) {
        emit progressUpdated(progress);
    });
    // Connect the RenderEngine's signal to the MainWindow's slot
//    connect(&myRender, SIGNAL(updateProgress(int)), this, SLOT(handleProgressUpdate(int)), Qt::QueuedConnection);
    // 开始计时
    QElapsedTimer timer;
    timer.start();

    //渲染
    myRender.render(samples, sm, filename, useOpenMP);

    // 停止计时并获取所用时间
    qint64 elapsedTime = timer.elapsed();
    emit renderTimeUpdated(elapsedTime);

    // 加载显示图像
    originalPixmap = QPixmap(filename.c_str());
    imageLabel->setAlignment(Qt::AlignCenter);
    updateImageLabel();

}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    updateImageLabel();
}

void MainWindow::updateImageLabel() {
    if (!originalPixmap.isNull()) {
        imageLabel->setPixmap(originalPixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void MainWindow::clearOutput() {
    outputTextEdit->clear();
}
void MainWindow::toggleOpenMP(bool checked) {
    useOpenMP = checked;
}

void MainWindow::updateRenderTime(qint64 elapsedTime) {
    // 将毫秒转换为分钟和秒
    int seconds = static_cast<int>(elapsedTime / 1000); // 转换为整数秒
    int minutes = seconds / 60; // 计算分钟数
    seconds = seconds % 60; // 计算剩余的秒数

    QString timeString = QString("Render time: %1 min %2 s\n").arg(minutes).arg(seconds);
    outputTextEdit->append(timeString);
}

void MainWindow::handleProgressUpdate(int progress) {
    // 在此处处理进度更新，例如更新进度条或在输出中显示进度
    progressBar->setValue(progress); // 更新进度条的值
//    outputTextEdit->append(QString("Progress: %1%").arg(progress));
}