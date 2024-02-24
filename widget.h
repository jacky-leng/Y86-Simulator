#ifndef WIDGET_H
#define WIDGET_H

#include "CPU.h"
#include "fstream"
#include <QWidget>
#include <iostream>
#include <vector>
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget {
  Q_OBJECT

public:
  Widget(QWidget *parent = nullptr);
  ~Widget();

private slots:
  void on_pushButton_2_clicked();

  void on_pushButton_clicked();

private:
  Ui::Widget *ui;
  CPU cpu;
  int m_num = 0;
};
#endif // WIDGET_H
