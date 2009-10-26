#ifndef SCENEHERMES_H
#define SCENEHERMES_H

#include "util.h"

class SolverDialog;

class Solution;
class Linearizer;
class Vectorizer;
class Orderizer;
class Mesh;

struct SolutionArray
{
    double time;
    double adaptiveError;
    int adaptiveSteps;

    Solution *sln1;
    Solution *sln2;
    Orderizer *order1;
    Orderizer *order2;

    SolutionArray();
    ~SolutionArray();
};

class SolverThread : public QThread
{
    Q_OBJECT
public:
    SolverThread(QObject *parent = 0);
    ~SolverThread();

    void run();    
    void setFileNameOrig(const QString &fileNameOrig) { m_fileNameOrig = fileNameOrig; }
    void setMode(SolverMode mode) { m_mode = mode; }

    void showMessage(const QString &msg, bool isError);
    void showProgress(int percent) { emit updateProgress(percent); }
    bool isCanceled() { return m_isCanceled; }
    void cancel();

public slots:
    void doMeshTriangleCreated(int exitCode);
    void doStarted();

signals:
    void updateProgress(int percent);
    void meshed();
    void solved();
    void message(const QString &message, bool isError);

private:
    QString m_fileNameOrig;
    SolverMode m_mode;
    bool m_isCanceled;
    QMutex mutex;
    QWaitCondition condition;

    void runMesh();
    void runSolver();

    bool writeToTriangle();
    bool triangleToHermes2D();
};

class SolverDialog : public QDialog
{
    Q_OBJECT

    public slots:
    void doShowMessage(const QString &message, bool isError);
    void doSolved();

signals:
    void solved();

public:
    SolverDialog(QWidget *parent);
    ~SolverDialog();

    void setFileNameOrig(const QString &fileNameOrig) { thread->setFileNameOrig(fileNameOrig); }
    void setMode(SolverMode mode) { thread->setMode(mode); }
    int solve();

private slots:
    void doCancel();

private:
    SolverThread *thread;

    QLabel *lblMessage;
    QProgressBar *progressBar;
    QTextEdit *lstMessage;
    QPushButton *btnCancel;

    void createControls();
};

#endif //SCENEHERMES_H
