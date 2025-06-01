#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QAxObject>
// #include <QAxSelect>
// #include <QAxWidget>
class MyMainWindow : public QMainWindow {

public slots:
    MyMainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("My Main Window");
        resize(800, 600);
    }
};

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    MyMainWindow mainWindow;
    
    QAxObject com("word.application");
    mainWindow.show();
    return app.exec();
}