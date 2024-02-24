#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
  ui->setupUi(this);
  ui->label->lower();
}

Widget::~Widget() { delete ui; }

void Widget::on_pushButton_2_clicked() {
  m_num = 0;
  std::string input_path = "D:\\repo\\ICS_PJ_Y86\\Y86-pipelined\\Y86-pipelined\\frontend\\test\\abs-asum-cmov.txt";
  cpu.MEM.readin(input_path);

  cpu.run();

  on_pushButton_clicked();
}

void Widget::on_pushButton_clicked() {
  std::cout << "size: " << cpu.clock_cycle.size() << std::endl;
  std::cout << "num: " << m_num << std::endl;
  if (m_num >= cpu.clock_cycle.size()) {
    return;
  }

  CPU::clock_cycle_state _temp = cpu.clock_cycle.at(m_num++);
  ui->LE_Stat1->setText(QString::fromStdString(_temp.statW));
  ui->LE_Stat2->setText(QString::fromStdString(_temp.statM));
  ui->LE_Stat3->setText(QString::fromStdString(_temp.statE));
  ui->LE_Stat4->setText(QString::fromStdString(_temp.statD));

  ui->LE_iCode1->setText(QString::fromStdString(_temp.icodeW));
  ui->LE_iCode2->setText(QString::fromStdString(_temp.icodeM));
  ui->LE_iCode3->setText(QString::fromStdString(_temp.icodeE));
  ui->LE_iCode4->setText(QString::fromStdString(_temp.icodeD));

  ui->LE_predPC->setText(QString::number(_temp.predPC));

  ui->LE_ifun1->setText(QString::number(_temp.ifunE));
  ui->LE_ifun2->setText(QString::number(_temp.ifunD));

  ui->LE_rA->setText(QString::fromStdString(_temp.rA));
  ui->LE_rB->setText(QString::fromStdString(_temp.rB));

  ui->LE_valC1->setText(QString::number(_temp.EvalC));
  ui->LE_valC2->setText(QString::number(_temp.DvalC));

  ui->LE_ValE1->setText(QString::number(_temp.WvalE));
  ui->LE_ValE2->setText(QString::number(_temp.MvalE));

  ui->LE_valP->setText(QString::number(_temp.valP));

  ui->LE_valA1->setText(QString::number(_temp.MvalA));
  ui->LE_valA2->setText(QString::number(_temp.EvalA));

  ui->LE_valB->setText(QString::number(_temp.valB));

  ui->LE_dstE1->setText(QString::fromStdString(_temp.WdstE));
  ui->LE_dstE2->setText(QString::fromStdString(_temp.MdstE));
  ui->LE_dstE3->setText(QString::fromStdString(_temp.EdstE));

  ui->LE_dstM1->setText(QString::fromStdString(_temp.WdstM));
  ui->LE_dstM2->setText(QString::fromStdString(_temp.MdstM));
  ui->LE_dstM3->setText(QString::fromStdString(_temp.EdstM));

  ui->LE_srcA->setText(QString::fromStdString(_temp.srcA));
  ui->LE_srcB->setText(QString::fromStdString(_temp.srcB));

  ui->LE_Cnd->setText(QString::number(_temp.Cnd));

  ui->LE_ValM1->setText(QString::number(_temp.valM));
}
